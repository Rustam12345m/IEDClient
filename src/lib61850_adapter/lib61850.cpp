/*
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
#include <iec61850_client.h>
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

		std::tuple<QString, QString> parseDataSetItemRef(const QString &t_ref)
		{
			// 
			QString ref = t_ref.left(t_ref.size() - 4);
			QString fc = t_ref.mid(ref.size() + 1, 2);
			return { ref, fc };
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
		void	recursiveReadAttributes(IedConnection t_con, QSharedPointer<Item> t_parent, DataModelBuilder &t_builder)
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

		bool 	getFileAsyncHandler(uint32_t invokeId, void* parameter, IedClientError err, uint32_t originalInvokeId,
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

		// Call-back for monitor close event
		void 	callback_ConnectionHandler(void *t_param, sIedConnection *t_con)
		{
			printf("Callback CloseEvent: \r\n");
			Lib61850_Adapter *l = static_cast<Lib61850_Adapter*>(t_param);
			if (l != nullptr) {
				l->callbackOnCloseEvent();
			}
		}

		QString convertTimestampMsToUserString(uint64_t t_ms)
		{
			uint64_t sec = t_ms / 1000;
			QDateTime dt = QDateTime::fromSecsSinceEpoch(sec);
			return dt.toString("HH:mm:ss dd.MM.yyyy"); // ms isn't important
		}
	
		void 	updateItemValueByMmsValue(ptrItem t_item, MmsValue *t_mmsValue)
		{
			if (t_item == nullptr || t_mmsValue == nullptr) {
				return;
			}

			switch (MmsValue_getType(t_mmsValue)) {
			case MMS_ARRAY:
			case MMS_STRUCTURE: {
				// t_item->update("Struct");
				int count = MmsValue_getArraySize(t_mmsValue);
				int items = t_item->getItemCount();
				if (count != items) {
					t_item->update("Error: Number of values");
					break;
				}

				for (int i=0;i<count;i++) {
					MmsValue *subMmsValue = (MmsValue *)MmsValue_getElement(t_mmsValue, i);
					ptrItem subItem = t_item->getItem(i);

					updateItemValueByMmsValue(subItem, subMmsValue);
				}
				break;
			}
			case MMS_BOOLEAN: {
				t_item->update(MmsValue_getBoolean(t_mmsValue) ? "True" : "False");
				break;
			}
			case MMS_BIT_STRING: {
				uint64_t value = 0;
				int size = MmsValue_getBitStringSize(t_mmsValue);
				if (size < sizeof(value)*8) {
					for (int i=0;i<size;i++) {
						value |= MmsValue_getBitStringBit(t_mmsValue, i) ? 1 << i : 0;
					}
				}
				t_item->update(QString::number((qulonglong)value, 2));
				break;
			}
			case MMS_INTEGER: {
				t_item->update(QString::number((long long)MmsValue_toInt64(t_mmsValue)));
				break;
			}
			case MMS_UNSIGNED: {
				t_item->update(QString::number(MmsValue_toUint32(t_mmsValue)));
				break;
			}
			case MMS_FLOAT: {
				t_item->update(QString::number(MmsValue_toFloat(t_mmsValue)));
				break;
			}
			case MMS_OCTET_STRING: {
				QString tmp;
				int size = MmsValue_getOctetStringSize(t_mmsValue);
				tmp.reserve(2 * size);

				for (int i=0;i<size;i++) {
					tmp.push_back(QString::number(MmsValue_getOctetStringOctet(t_mmsValue, i), 16));
				}
				t_item->update(tmp);
				break;
			}
			case MMS_GENERALIZED_TIME: {
				t_item->update("Unsupported");
				break;
			}
			case MMS_BINARY_TIME: {
				uint64_t ms = MmsValue_getBinaryTimeAsUtcMs(t_mmsValue);
				t_item->update(convertTimestampMsToUserString(ms));
				break;
			}
			case MMS_BCD: {
				t_item->update("? BCD");
				break;
			}
			case MMS_OBJ_ID: {
				t_item->update("? OBJ_ID");
				break;
			}
			case MMS_STRING:
			case MMS_VISIBLE_STRING: {
				char tmp[256] = { 0 };
				strncpy(tmp, MmsValue_toString(t_mmsValue), 256);
				tmp[255] = 0;
				t_item->update(QString::fromLocal8Bit(tmp));
				break;
			}
			case MMS_UTC_TIME: {
				t_item->update(convertTimestampMsToUserString(MmsValue_getUtcTimeInMs(t_mmsValue)));
				break;
			}
			default: {
				char tmp[1024] = { 0 };
				MmsValue_printToBuffer(t_mmsValue, tmp, 1024);
				t_item->update(QString::fromLocal8Bit(tmp));
			}
			}
		}

		int 	updateItemByMMS(ptrItem t_item, sIedConnection *t_con, FunctionalConstraint t_fc)
		{
			IedClientError retval = IED_ERROR_OK;
			auto ref = t_item->ref().toStdString();

			MmsValue *value = IedConnection_readObject(t_con, &retval, ref.data(), t_fc);
			if (retval != IED_ERROR_OK) {
				return -1;
			}

			updateItemValueByMmsValue(t_item, value);
			MmsValue_delete(value);
			return 0;
		}

		int 	updateDataObjectItem(Core::ptrDO t_do, sIedConnection *t_con)
		{
			if (t_do == nullptr || t_con == nullptr) {
				return -1;
			}

			for (size_t j=0;j<t_do->getItemCount();j++) {
				auto daNode = t_do->getItem< Core::DataAttribute >(j);
				auto fcNum = (FunctionalConstraint)daNode->fcNum();

				updateItemByMMS(daNode, t_con, fcNum);
			}
			return 0;
		}
	}


	bool Lib61850_Adapter::isConnected() const
	{
		if (m_libConn == nullptr) {
			return false;
		}
		IedConnectionState retval = IedConnection_getState(m_libConn);
		if (retval == IED_STATE_CLOSED) {
			return false;
		}
		return true;
	}

	void Lib61850_Adapter::printfVersion() const
	{
		char *pv = LibIEC61850_getVersionString();
		printf("Libiec61850: Version %s \r\n", pv);
	}

	bool Lib61850_Adapter::connect(const QString &t_ip, unsigned int t_port, bool t_checked,
									const QString &t_name, const QString &t_pass)
	{
		IedClientError retval = IED_ERROR_OK;

		m_libConn = IedConnection_create();
		IedConnection_connect(m_libConn, &retval, t_ip.toStdString().c_str(), t_port);
		if (retval == IED_ERROR_OK) {
			// Callback for close-events
			IedConnection_installConnectionClosedHandler(m_libConn, &callback_ConnectionHandler, this);

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

	void Lib61850_Adapter::disconnect()
	{
		if (m_libConn == nullptr) {
			return;
		}

		IedClientError error = IED_ERROR_OK;

		IedConnection_abort(m_libConn, &error);
		IedConnection_destroy(m_libConn);
		m_libConn = nullptr;
	}

	Core::DevServIdentity Lib61850_Adapter::getServIdentity()
	{
		Core::DevServIdentity ident;
		MmsConnection mmsCon = IedConnection_getMmsConnection(m_libConn);

		MmsError error = MMS_ERROR_NONE;
		MmsServerIdentity *identity = MmsConnection_identify(mmsCon, &error);
		if ((error == MMS_ERROR_NONE) && (identity != nullptr)) {
			ident.m_vendor = QString::fromLocal8Bit(identity->vendorName);
			ident.m_model = QString::fromLocal8Bit(identity->modelName);
			ident.m_revision = QString::fromLocal8Bit(identity->revision);
		}

		MmsConnectionParameters param = MmsConnection_getMmsConnectionParameters(mmsCon);
		ident.m_maxPduSize = param.maxPduSize;
		ident.m_dataStructureNestingLevel = param.dataStructureNestingLevel;
		ident.m_maxServOutstandingCalled = param.maxServOutstandingCalled;
		ident.m_maxServOutstandingCalling = param.maxServOutstandingCalling;
		return ident;
	}

	int Lib61850_Adapter::fetchDataModel(Core::DataModelBuilder &t_builder)
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

					fetchLN_GOCB(t_builder);

					fetchLN_SVCB(t_builder);

					updateLN_Signals(t_builder.lastLN());

					node = LinkedList_getNext(node); // next LN
				}
				LinkedList_destroy(lnList);
			}

			ld = LinkedList_getNext(ld); // next LD
		}
		LinkedList_destroy(ldList);
		return 0;
	}

	int Lib61850_Adapter::fetchLN_DO(Core::DataModelBuilder &t_builder)
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

	int Lib61850_Adapter::fetchLN_DS(Core::DataModelBuilder &t_builder)
	{
		IedClientError retval = IED_ERROR_OK;
		QString lnRef = t_builder.lastLN()->ref();

		LinkedList dsList = IedConnection_getLogicalNodeDirectory(m_libConn, &retval, lnRef.toStdString().data(),
																ACSI_CLASS_DATA_SET);
		LinkedList dataSet = LinkedList_getNext(dsList);
		while (dataSet != nullptr) {
			char *dsName = (char *)dataSet->data;
			bool isDeletable = false;

			char dataSetRef[130] = { 0 };
			sprintf(dataSetRef, "%s.%s", lnRef.toStdString().data(), dsName);

			t_builder.createDataSet(QString::fromLocal8Bit(dsName), lnRef, isDeletable);

			LinkedList dsEntityList = IedConnection_getDataSetDirectory(m_libConn, &retval, dataSetRef,
																		&isDeletable);
			LinkedList dsEntity = LinkedList_getNext(dsEntityList);
			while (dsEntity != nullptr) {
				QString dsElemRef = QString::fromLocal8Bit((char *)dsEntity->data);

				auto [ref, fc] = parseDataSetItemRef(dsElemRef);
				t_builder.createDataSet_Elem(ref, fc);

				dsEntity = LinkedList_getNext(dsEntity);
			}
			LinkedList_destroy(dsEntityList);

			dataSet = LinkedList_getNext(dataSet);
		}
		LinkedList_destroy(dsList);
		return 0;
	}

	int Lib61850_Adapter::fetchLN_RCB(Core::DataModelBuilder &t_builder)
	{
		IedClientError retval = IED_ERROR_OK;
		QString ref = t_builder.lastLN()->ref();

		// Unbuffered RCB
		LinkedList rcbList = IedConnection_getLogicalNodeDirectory(m_libConn, &retval, ref.toStdString().data(),
																ACSI_CLASS_URCB);

		LinkedList rcb = LinkedList_getNext(rcbList);
		while (rcb != nullptr) {
			char* reportName = (char *) rcb->data;

			//printf("    URCB: %s\n", reportName);

			rcb = LinkedList_getNext(rcb);
		}
		LinkedList_destroy(rcbList);

		// Buffered RCB
		rcbList = IedConnection_getLogicalNodeDirectory(m_libConn, &retval, ref.toStdString().data(),
														ACSI_CLASS_BRCB);

		rcb = LinkedList_getNext(rcbList);
		while (rcb != nullptr) {
			char* reportName = (char *) rcb->data;

			//printf("    BRCB: %s\n", reportName);

			rcb = LinkedList_getNext(rcb);
		}
		LinkedList_destroy(rcbList);
		return 0;
	}

	int Lib61850_Adapter::fetchLN_GOCB(Core::DataModelBuilder &t_builder)
	{
		// Goose CB
		IedClientError retval = IED_ERROR_OK;
		QString ref = t_builder.lastLN()->ref();
		LinkedList rcbList = IedConnection_getLogicalNodeDirectory(m_libConn, &retval, ref.toStdString().data(),
																	ACSI_CLASS_GoCB);

		LinkedList rcb = LinkedList_getNext(rcbList);
		while (rcb != nullptr) {
			char* reportName = (char *) rcb->data;

			printf("    GOCB: %s\n", reportName);

			rcb = LinkedList_getNext(rcb);
		}
		LinkedList_destroy(rcbList);
		return 0;
	}

	int Lib61850_Adapter::fetchLN_SVCB(Core::DataModelBuilder &t_builder)
	{
		return 0;
	}

	void Lib61850_Adapter::callbackOnCloseEvent()
	{
		emit sigConClosed();
	}

	int Lib61850_Adapter::updateLDs_Status(Core::ptrDataModel t_model)
	{
		if (!isConnected()) {
			return -1;
		}

		for (size_t i=0;i<t_model->getItemCount();i++) {
			auto ld = t_model->getItem<Core::LogicalDevice>(i);
			if (ld->lln0()) {
				updateDataObjectItem(ld->lln0()->mod(), m_libConn);
				updateDataObjectItem(ld->lln0()->beh(), m_libConn);
				updateDataObjectItem(ld->lln0()->health(), m_libConn);
			}
		}
		return 0;
	}

	int Lib61850_Adapter::updateLNs_Status(Core::ptrLD t_ld)
	{
		if (!isConnected()) {
			return -1;
		}

		for (size_t i=0;i<t_ld->getItemCount();i++) {
			auto ln = t_ld->getItem< Core::LogicalNode >(i);

			for (size_t j=0;j<ln->getItemCount();j++) {
				updateDataObjectItem(ln->mod(), m_libConn);
				updateDataObjectItem(ln->beh(), m_libConn);
				updateDataObjectItem(ln->health(), m_libConn);
			}
		}
		return 0;
	}

	int Lib61850_Adapter::updateLN_Signals(Core::ptrLN t_ln)
	{
		if (!isConnected()) {
			return -1;
		}

		for (size_t i=0;i<t_ln->getItemCount();i++) {
			auto doNode = t_ln->getItem< Core::DataObject >(i);

			updateDataObjectItem(doNode, m_libConn);
		}
		return 0;
	}

	int Lib61850_Adapter::updateDS_Signals(Core::ptrDataSet t_ds)
	{
		if (!isConnected()) {
			return -1;
		}

		//ClientDataSet clientDataSet = IedConnection_readDataSetValues(con, &error, "simpleIOGenericIO/LLN0.AnalogueValues", NULL);
		return 0;
	}

	int Lib61850_Adapter::getFileList(Core::DirOn &t_dir)
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

	int Lib61850_Adapter::removeFile(const QString &t_filename)
	{
		IedClientError retval = IED_ERROR_OK;
		IedConnection_deleteFile(m_libConn, &retval, t_filename.toStdString().c_str());
		return (retval != IED_ERROR_OK);
	}

	void Lib61850_Adapter::downloadFile(const QString &t_filename)
	{
		IedClientError error = IED_ERROR_OK;
		uint32_t id = IedConnection_getFileAsync(m_libConn, &error, t_filename.toStdString().c_str(),
												getFileAsyncHandler, nullptr);
	}
}