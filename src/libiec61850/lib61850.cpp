/*
 *  main.cpp
 *
 *  Copyright 2023 Rustam Mustafin
 *
 *  This file is part of IEDClient.
 *
 *  IEDClient is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  IEDClient is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with IEDClient.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  See COPYING file for the complete license text.
 * */

#include "lib61850.hpp"

#include <string>

#include <QDebug>
#include <QDateTime>

extern "C"
{
#include "iec61850_client.h"
}

namespace Core::Lib
{
	// static functions for this file
	namespace
	{
		std::tuple<QString, QString, int> getFX_fromName(const char *t_data)
		{
			// Parse paramets from string like: ctlModel[CF]
			QString tmp = QString::fromLocal8Bit(t_data);

			QString daName, fc;
			int fcNum = 0;

			int pos = tmp.indexOf("[");
			if (pos != -1) {
				fc = tmp.mid(pos + 1, tmp.size() - (pos + 1) - 1);
				fcNum = FunctionalConstraint_fromString(fc.toLocal8Bit().data());
				daName = tmp.mid(0, pos);
			} else {
				daName = tmp;
			}
			return std::make_tuple(daName, fc, fcNum);
		}

		/*
		void recursiveReadAttributes(IedConnection t_con, const QString &t_ref,
									QSharedPointer<Item> t_parent)
		{
			IedClientError retval = IED_ERROR_OK;
			LinkedList daList = IedConnection_getDataDirectory(t_con, &retval, t_ref.toLocal8Bit().data());
			if ((retval == IED_ERROR_OK) && (daList != nullptr)) {
				LinkedList attr = LinkedList_getNext(daList);

				while (attr != nullptr) {
					QString name = QString::fromLocal8Bit((char *)attr->data);

					auto subAttrNode = ItemFactory::createSA(t_parent.get(), name);
					t_parent->push(subAttrNode);

					QString ref = t_ref + "." + name;
					recursiveReadAttributes(t_con, ref, subAttrNode);

					attr = LinkedList_getNext(attr);
				}
				LinkedList_destroy(daList);
			}
		}
		*/
		void recursiveReadAttributes(IedConnection t_con, QSharedPointer<Item> t_parent, DataModelBuilder &t_builder)
		{
			IedClientError retval = IED_ERROR_OK;
			LinkedList daList = IedConnection_getDataDirectory(t_con, &retval, t_parent->ref().toLocal8Bit().data());
			if ((retval == IED_ERROR_OK) && (daList != nullptr)) {
				LinkedList attr = LinkedList_getNext(daList);

				while (attr != nullptr) {
					QString name = QString::fromLocal8Bit((char *)attr->data);

					t_builder.createSDA(t_parent, name);
					auto sda = t_builder.lastSDA();

					QString ref = t_parent->ref() + "." + name;
					recursiveReadAttributes(t_con, sda, t_builder);

					attr = LinkedList_getNext(attr);
				}
				LinkedList_destroy(daList);
			}
		}

		bool getFileAsyncHandler(uint32_t invokeId, void* parameter, IedClientError err, uint32_t originalInvokeId,
								uint8_t* buffer, uint32_t bytesRead, bool moreFollows)
		{
			if ((err != IED_ERROR_OK) || (moreFollows == false)) {
				if (err == IED_ERROR_OK) {
					printf("Received %d bytes\n", bytesRead);
				}
				printf("File transfer complete (err:%d)\n", err);
			}
			else {
				printf("Received %d bytes\n", bytesRead);
			}
			return true;
		}

		QString 	convertTimestampMsToUserString(uint64_t t_ms)
		{
			uint64_t sec = t_ms / 1000;
			QDateTime dt = QDateTime::fromSecsSinceEpoch(sec);
			return dt.toString("HH:mm:ss dd.MM.yyyy"); // ms isn't important
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


	int Lib61850::fetchDataModel(Core::DataModelBuilder &t_builder)
	{
		if (!isConnected()) {
			return -1;
		}

		IedClientError retval = IED_ERROR_OK;

		LinkedList ldList = IedConnection_getLogicalDeviceList(m_libConn, &retval);
		if (retval != IED_ERROR_OK) {
			//printf("Failed to read device list (error code: %i)\n", retval);
			return -2;
		}

		// Fetch all Logical Devices
		LinkedList ld = LinkedList_getNext(ldList);
		while (ld != nullptr) {
			t_builder.createLD(QString::fromLocal8Bit((char *)ld->data)); // New LD

			// Fetch all Logical Nodes
			LinkedList lnList = IedConnection_getLogicalDeviceDirectory(m_libConn, &retval, (char *)ld->data);
			if (retval == IED_ERROR_OK) {

				LinkedList node = LinkedList_getNext(lnList);
				while (node != nullptr) {
					t_builder.createLN(QString::fromLocal8Bit((char *)node->data)); // New LN

					emit sigFoundNode(t_builder.lastLN()->ref());

					fetchLN_DO(t_builder);

					fetchLN_DS(t_builder);

					fetchLN_RCB(t_builder);

					updateLN_PinValues(t_builder.lastLN());

					node = LinkedList_getNext(node); // next LN
				}
				LinkedList_destroy(lnList);
			}

			ld = LinkedList_getNext(ld); // next LD
		}
		LinkedList_destroy(ldList);
		return 0;
	}

	int Lib61850::fetchLN_DO(Core::DataModelBuilder &t_builder)
	{
		IedClientError retval = IED_ERROR_OK;
		QString ref = t_builder.lastLN()->ref();

		LinkedList doList = IedConnection_getLogicalNodeDirectory(m_libConn, &retval, ref.toStdString().data(),
																ACSI_CLASS_DATA_OBJECT);
		if ((retval == IED_ERROR_OK) && (doList != nullptr)) {

			LinkedList dObj = LinkedList_getNext(doList);
			while (dObj != nullptr) {
				QString doName = QString::fromLocal8Bit((char *)dObj->data);
				QString refDO = QString("%1.%2").arg(ref, doName);

				t_builder.createDO(doName);

				LinkedList daListFC = IedConnection_getDataDirectoryFC(m_libConn, &retval, refDO.toLocal8Bit().data());
				if ((retval == IED_ERROR_OK) && (daListFC != nullptr)) {
					
					LinkedList attrFC = LinkedList_getNext(daListFC);
					while (attrFC != nullptr) {
						auto [name, fc, fcNum] = getFX_fromName((char *)attrFC->data);

						t_builder.createDA(name, fc, fcNum);

						// Recursive search SubAttr for DA
						recursiveReadAttributes(m_libConn, t_builder.lastDA(), t_builder);

						attrFC = LinkedList_getNext(attrFC);
					}
					LinkedList_destroy(daListFC);
				}

				dObj = LinkedList_getNext(dObj); // next DO
			}
		}
		LinkedList_destroy(doList);
		return 0;
	}

	int Lib61850::fetchLN_DS(Core::DataModelBuilder &t_builder)
	{
		IedClientError retval = IED_ERROR_OK;
		QString ref = t_builder.lastLN()->ref();

		LinkedList dsList = IedConnection_getLogicalNodeDirectory(m_libConn, &retval, ref.toStdString().data(),
																ACSI_CLASS_DATA_SET);

		LinkedList dataSet = LinkedList_getNext(dsList);
		while (dataSet != nullptr) {
			char* dsName = (char*) dataSet->data;
			bool isDeletable;

			char dataSetRef[130];
			sprintf(dataSetRef, "%s.%s", ref.toStdString().data(), dsName);

			LinkedList doList = IedConnection_getDataSetDirectory(m_libConn, &retval, dataSetRef,
																&isDeletable);

			LinkedList doRef = LinkedList_getNext(doList);
			while (doRef != nullptr) {
				char* memberRef = (char*) doRef->data;

				//printf("      %s\n", memberRef);

				doRef = LinkedList_getNext(doRef);
			}
			LinkedList_destroy(doList);

			dataSet = LinkedList_getNext(dataSet);
		}
		LinkedList_destroy(dsList);
		return 0;
	}

	int Lib61850::fetchLN_RCB(Core::DataModelBuilder &t_builder)
	{
		IedClientError retval = IED_ERROR_OK;
		QString ref = t_builder.lastLN()->ref();

		// Unbuffered RCB
		LinkedList rcbList = IedConnection_getLogicalNodeDirectory(m_libConn, &retval, ref.toStdString().data(),
																ACSI_CLASS_URCB);

		LinkedList rcb = LinkedList_getNext(rcbList);
		while (rcb != nullptr) {
			char* reportName = (char *) rcb->data;

			//printf("    RP: %s\n", reportName);

			rcb = LinkedList_getNext(rcb);
		}
		LinkedList_destroy(rcbList);

		// Buffered RCB
		rcbList = IedConnection_getLogicalNodeDirectory(m_libConn, &retval, ref.toStdString().data(),
														ACSI_CLASS_BRCB);

		rcb = LinkedList_getNext(rcbList);
		while (rcb != nullptr) {
			char* reportName = (char *) rcb->data;

			printf("    BR: %s\n", reportName);

			rcb = LinkedList_getNext(rcb);
		}
		LinkedList_destroy(rcbList);
		return 0;
	}


	int Lib61850::updateLN_PinValues(Core::ptrLN t_ln)
	{
		if (isConnected()) {
			IedClientError retval = IED_ERROR_OK;

			for (size_t i=0;i<t_ln->getItemCount();i++) {
				auto doNode = t_ln->getItem< Core::DataObject >(i);

				for (size_t j=0;j<doNode->getItemCount();j++) {
					auto daNode = doNode->getItem< Core::DataAttribute >(j);

					auto ref = daNode->ref().toStdString();
					auto fcNum = (FunctionalConstraint)daNode->fcNum();

					MmsValue *val = IedConnection_readObject(m_libConn, &retval, ref.data(), fcNum);
					if (retval == IED_ERROR_OK && val != nullptr) {
						switch (MmsValue_getType(val)) {
						case MMS_BOOLEAN: {
							daNode->update(MmsValue_getBoolean(val) ? "True" : "False");
							break;
						}
						case MMS_UTC_TIME: {
							daNode->update(convertTimestampMsToUserString(MmsValue_getUtcTimeInMs(val)));
							break;
						}
						default: {
							//daNode->update(QString("Type: %1").arg(MmsValue_getType(val)));
							//break;
							char tmp[1024] = { 0 };
							MmsValue_printToBuffer(val, tmp, 1024);
							daNode->update(QString::fromLocal8Bit(tmp));
						}
						}
					}
				}
			}
		}
		return 0;
	}

	int Lib61850::updateDS_PinValues(Core::ptrLN t_node)
	{
		return 0;
	}


	int Lib61850::getFS_FileList(Core::DirOn &t_dir)
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

	int Lib61850::removeFile(const QString &t_filename)
	{
		IedClientError retval = IED_ERROR_OK;
		IedConnection_deleteFile(m_libConn, &retval, t_filename.toStdString().c_str());
		return (retval != IED_ERROR_OK);
	}
	
	void Lib61850::downloadFile(const QString &t_filename)
	{
		IedClientError error = IED_ERROR_OK;
		uint32_t id = IedConnection_getFileAsync(m_libConn, &error, t_filename.toStdString().c_str(),
												getFileAsyncHandler, nullptr);
	}
}