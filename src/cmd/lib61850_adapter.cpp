/*
 *  main.cpp
 *
 *  Copyright 2023-2023 Rustam Mustafin
 *
 *  This file is part of IEDMaster.
 *
 *  IEDMaster is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  IEDMaster is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with IEDMaster.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  See COPYING file for the complete license text.
 * */

#include "lib61850_adapter.h"
#include <string>

#include <QDebug>

extern "C" {
#include "iec61850_client.h"
}

namespace Core::Cmd
{
	namespace
	{
		void getFX_fromDAName(const QString &t_da, QString &t_daName, QString &t_fc)
		{
			// ctlModel[CF]
			int pos = t_da.indexOf("[");
			if (pos != -1) {
				t_fc = t_da.mid(pos + 1, t_da.size() - (pos + 1) - 1);
				t_daName = t_da.mid(0, pos);
			} else {
				t_daName = t_da;
			}
		}
	}

	void Lib61850::printfVersion() const
	{
		char *pv = LibIEC61850_getVersionString();
		printf("Libiec61850: Version %s \r\n", pv);
	}

	bool Lib61850::connect(const QString &t_ip, unsigned int t_port, bool t_checked,
							const QString &t_name, const QString &t_pass)
	{
		IedClientError retval = IED_ERROR_OK;

		m_libConn = IedConnection_create();
		IedConnection_connect(m_libConn, &retval, t_ip.toStdString().c_str(), t_port);
		if (retval == IED_ERROR_OK) {
			// Get complete model from IED
			IedConnection_getDeviceModelFromServer(m_libConn, &retval);
			if (retval != IED_ERROR_OK) {
				qDebug() << "!!! ERROR !!!: Connect, get model with error = " << retval;
			}

		} else {
			IedConnection_destroy(m_libConn);
			m_libConn = nullptr;
			return false;
		}
		return true;
	}

	void Lib61850::disconnect()
	{
		if (m_libConn == nullptr) {
			return;
		}

		IedClientError error = IED_ERROR_OK;

		IedConnection_abort(m_libConn, &error);
		IedConnection_destroy(m_libConn);
		m_libConn = nullptr;
	}

	int Lib61850::getFileList(Core::DirOn &t_dir)
	{
		if (isConnected()) {
			std::string path = t_dir.name().toStdString();

			IedClientError retval = IED_ERROR_OK;
			LinkedList dirRoot = IedConnection_getFileDirectory(m_libConn, &retval, path.c_str());
			if (retval != IED_ERROR_OK) {
				qDebug() << "Adapter: Error getFileDirectory " << path << ", retval = " << retval;
				return -1;
			}

			if (dirRoot) {
				LinkedList dirEntry = LinkedList_getNext(dirRoot);

				while (dirEntry != nullptr) {
					FileDirectoryEntry entry = (FileDirectoryEntry)dirEntry->data;

					QString fname = QString(FileDirectoryEntry_getFileName(entry));
					uint32_t fsize = FileDirectoryEntry_getFileSize(entry);
					uint64_t fmodif = FileDirectoryEntry_getLastModified(entry);

					t_dir.put(Core::FileOn(fname, fsize, fmodif));

					dirEntry = LinkedList_getNext(dirEntry);
				}
			}
			LinkedList_destroyDeep(dirRoot, (LinkedListValueDeleteFunction)FileDirectoryEntry_destroy);
		}
		return 0;
	}

	int Lib61850::getLD_List(Core::IED_Tree &t_tree)
	{
		if (isConnected()) {
			IedClientError retval = IED_ERROR_OK;

			LinkedList ldList = IedConnection_getLogicalDeviceList(m_libConn, &retval);
			if (retval != IED_ERROR_OK) {
				//printf("Failed to read device list (error code: %i)\n", retval);
				return -1;
			}

			LinkedList device = LinkedList_getNext(ldList);
			while (device != nullptr) {
				QString ldName = QString::fromLocal8Bit((char *)device->data);

				// Found new LD
				auto ldev = QSharedPointer<Core::LogicalDevice>::create(&t_tree, ldName);
				t_tree.addChild(ldev);

				LinkedList lnodes = IedConnection_getLogicalDeviceDirectory(m_libConn, &retval,
																			(char *)device->data);
				if (retval == IED_ERROR_OK) {
					LinkedList node = LinkedList_getNext(lnodes);

					while (node != nullptr) {
						QString name = QString::fromLocal8Bit((char *)node->data);

						auto ln = QSharedPointer<Core::LogicalNode>::create(ldev.get(), name);
						ldev->addChild(ln);

						node = LinkedList_getNext(node); // next Logical Node
					}
					LinkedList_destroy(lnodes);
				}

				device = LinkedList_getNext(device); // next Logical Device
			}
			LinkedList_destroy(ldList);
		}
		return 0;
	}

	int Lib61850::getDO_List(Core::ptrLN t_node)
	{
		if (isConnected()) {
			IedClientError retval = IED_ERROR_OK;

			char lnRef[129] = { 0 };
			snprintf(lnRef, 129, "%s/%s",
					t_node->parentName().toLocal8Bit().data(),
					t_node->name().toLocal8Bit().data());

			// Get list of DataObjects for this LogicalNode
			LinkedList doList = IedConnection_getLogicalNodeDirectory(m_libConn, &retval,
																	lnRef, ACSI_CLASS_DATA_OBJECT);
			if ((retval == IED_ERROR_OK) && (doList != nullptr)) {

				LinkedList obj = LinkedList_getNext(doList);
				while (obj != nullptr) {
					QString name = QString::fromLocal8Bit((char *)obj->data);
					QString ref = QString("%1/%2.%3").arg(t_node->parentName(), t_node->name(), name);

					auto fDO = QSharedPointer<Core::DataObject>::create(t_node.get(), name, ref);
					t_node->addChild(fDO);

					// Get list of DA for this DO
					//LinkedList daList = IedConnection_getDataDirectoryFC(m_libConn, &retval, ref.toLocal8Bit().data());
					LinkedList daList = IedConnection_getDataDirectory(m_libConn, &retval, ref.toLocal8Bit().data());
					if ((retval == IED_ERROR_OK) && (daList != nullptr)) {

						LinkedList attr = LinkedList_getNext(daList);
						while (attr != nullptr) {
							QString name, fc;
							getFX_fromDAName(QString::fromLocal8Bit((char *)attr->data), name, fc);
							int fcNum = FunctionalConstraint_fromString(fc.toLocal8Bit().data());
							QString ref = QString("%1/%2.%3.%4").arg(t_node->parentName(), t_node->name(), fDO->name(), name);

							auto fDA = QSharedPointer<Core::DataAttribute>::create(fDO.get(), name, ref, fc, fcNum); // found DA
							fDO->addChild(fDA);

							attr = LinkedList_getNext(attr);
						}
						LinkedList_destroy(daList);
					}

					fDO->flatten();

					obj = LinkedList_getNext(obj); // next Data Object
				}
			}
			LinkedList_destroy(doList);
		}
		return 0;
	}

	int Lib61850::updateDataObjects(Core::ptrLN t_node)
	{
		if (isConnected()) {
			IedClientError retval = IED_ERROR_OK;

			for (size_t i=0;i<t_node->getNodeCount();i++) {
				auto dObj = t_node->getChildPtr< Core::DataObject >(i);

				for (size_t j=0;j<dObj->getNodeCount();j++) {
					auto dAttr = dObj->getChildPtr< Core::DataAttribute >(j);
					QString ref = dAttr->ref();

					MmsValue *val = IedConnection_readObject(m_libConn, &retval, ref.toLocal8Bit().data(),
															(FunctionalConstraint)dAttr->fcNum());
					if (retval == IED_ERROR_OK && val != nullptr) {
						char tmp[1024] = { 0 };
						MmsValue_printToBuffer(val, tmp, 1024);

						dAttr->update(QString::fromLocal8Bit(tmp));
					}
				}
			}
		}
		return 0;
	}
}
