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

#include "ied_model_api_impl.hpp"
#include "libiec61850_adapter.hpp"

extern "C"
{
#include <iec61850_client.h>
}

namespace
{
    std::tuple<QString, QString, int> getFX_fromName(const char *data)
    {
        // Parse paramets from string like: ctlModel[CF]
        QString tmp = QString::fromLocal8Bit(data);

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

    std::tuple<QString, QString> parseDataSetItemRef(const QString &ref)
    {
        QString refTrimmed = ref.left(ref.size() - 4);
        QString fc = ref.mid(refTrimmed.size() + 1, 2);
        return { refTrimmed, fc };
    }

    void    recursiveReadAttributes(IedConnection con, QSharedPointer<Core::ModelItem> parent,
                                    Core::DataModelBuilder &builder, FunctionalConstraint fc)
    {
        IedClientError retval = IED_ERROR_OK;
        LinkedList daList = IedConnection_getDataDirectoryByFC(con, &retval,
                                parent->getReference().toLocal8Bit().data(), fc);
        if ((retval == IED_ERROR_OK) && (daList != nullptr)) {
            LinkedList attr = LinkedList_getNext(daList);

            while (attr != nullptr) {
                QString name = QString::fromLocal8Bit((char *)attr->data);

                builder.createSDA(parent, name);
                auto sda = builder.lastSDA();

                recursiveReadAttributes(con, sda, builder, fc);

                attr = LinkedList_getNext(attr);
            }
            LinkedList_destroy(daList);
        }
    }
}

namespace Libiec61850
{
    int IED_ModelAPI_Impl::fetchDataModel(Core::DataModelBuilder &builder)
    {
        if (!m_api.isConnected()) {
            return -1;
        }

        IedClientError retval = IED_ERROR_OK;

        LinkedList ldList = IedConnection_getLogicalDeviceList(m_api.m_libConn, &retval);
        if (retval != IED_ERROR_OK) {
            //printf("Failed to read device list (error code: %i)\n", retval);
            return -2;
        }

        // Fetch all Logical Devices
        LinkedList ld = LinkedList_getNext(ldList);
        while (ld != nullptr) {
            builder.createLD(QString::fromLocal8Bit((char *)ld->data)); // New LD

            // Fetch all Logical Nodes
            LinkedList lnList = IedConnection_getLogicalDeviceDirectory(m_api.m_libConn, &retval, (char *)ld->data);
            if (retval == IED_ERROR_OK) {

                LinkedList node = LinkedList_getNext(lnList);
                while (node != nullptr) {
                    builder.createLN(QString::fromLocal8Bit((char *)node->data)); // New LN

                    emit sigFoundNode(builder.lastLN()->getReference());

                    fetchLN_DO(builder);
                    auto updVals = m_api.state().getValsForLN(builder.lastLN());
                    updVals->update();

                    fetchLN_DS(builder);

                    fetchLN_RCB(builder);

                    fetchLN_GOCB(builder);

                    fetchLN_SVCB(builder);

                    fetchLN_SGCB(builder);

                    node = LinkedList_getNext(node); // next LN
                }
                LinkedList_destroy(lnList);
            }

            ld = LinkedList_getNext(ld); // next LD
        }
        LinkedList_destroy(ldList);
        return 0;
    }

    int IED_ModelAPI_Impl::fetchLN_DO(Core::DataModelBuilder &builder)
    {
        IedClientError retval = IED_ERROR_OK;
        QString ref = builder.lastLN()->getReference();

        LinkedList doList = IedConnection_getLogicalNodeDirectory(m_api.m_libConn, &retval,
                                        ref.toStdString().data(), ACSI_CLASS_DATA_OBJECT);
        if ((retval == IED_ERROR_OK) && (doList != nullptr)) {

            LinkedList dObj = LinkedList_getNext(doList);
            while (dObj != nullptr) {
                QString doName = QString::fromLocal8Bit((char *)dObj->data);
                QString refDO = QString("%1.%2").arg(ref, doName);

                builder.createDO(doName);

                LinkedList daListFC = IedConnection_getDataDirectoryFC(m_api.m_libConn,
                                                    &retval, refDO.toLocal8Bit().data());
                if ((retval == IED_ERROR_OK) && (daListFC != nullptr)) {

                    LinkedList attrFC = LinkedList_getNext(daListFC);
                    while (attrFC != nullptr) {
                        auto [name, fc, fcNum] = getFX_fromName((char *)attrFC->data);

                        builder.createDA(name, fc);

                        // Recursive search SubAttr for DA (filtered by FC)
                        recursiveReadAttributes(m_api.m_libConn, builder.lastDA(), builder,
                                                (FunctionalConstraint)fcNum);

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

    int IED_ModelAPI_Impl::fetchLN_DS(Core::DataModelBuilder &builder)
    {
        IedClientError retval = IED_ERROR_OK;
        QString lnRef = builder.lastLN()->getReference();

        LinkedList dsList = IedConnection_getLogicalNodeDirectory(m_api.m_libConn, &retval,
                                    lnRef.toStdString().data(), ACSI_CLASS_DATA_SET);
        if (retval != IED_ERROR_OK || dsList == nullptr) {
            return 0;
        }
        LinkedList dataSet = LinkedList_getNext(dsList);
        while (dataSet != nullptr) {
            char *dsName = (char *)dataSet->data;
            bool isDeletable = false;

            char dataSetRef[256] = { 0 };
            snprintf(dataSetRef, sizeof(dataSetRef), "%s.%s", lnRef.toStdString().data(), dsName);

            builder.createDataSet(QString::fromLocal8Bit(dsName), lnRef, isDeletable);

            LinkedList dsEntityList = IedConnection_getDataSetDirectory(m_api.m_libConn, &retval,
                                                                        dataSetRef, &isDeletable);
            if (retval != IED_ERROR_OK || dsEntityList == nullptr) {
                dataSet = LinkedList_getNext(dataSet);
                continue;
            }
            LinkedList dsEntity = LinkedList_getNext(dsEntityList);
            while (dsEntity != nullptr) {
                QString dsElemRef = QString::fromLocal8Bit((char *)dsEntity->data);

                auto [ref, fc] = parseDataSetItemRef(dsElemRef);
                builder.createDataSet_Elem(ref, fc);

                dsEntity = LinkedList_getNext(dsEntity);
            }
            LinkedList_destroy(dsEntityList);

            dataSet = LinkedList_getNext(dataSet);
        }
        LinkedList_destroy(dsList);
        return 0;
    }

    int IED_ModelAPI_Impl::fetchLN_RCB(Core::DataModelBuilder &builder)
    {
        IedClientError retval = IED_ERROR_OK;
        QString lnRef = builder.lastLN()->getReference();

        auto fetchRCBList = [&](ACSIClass acsiClass, bool isBuffered, const char *prefix) {
            LinkedList rcbList = IedConnection_getLogicalNodeDirectory(m_api.m_libConn, &retval,
                                     lnRef.toStdString().data(), acsiClass);
            if (retval != IED_ERROR_OK || rcbList == nullptr) {
                return;
            }

            LinkedList rcb = LinkedList_getNext(rcbList);
            while (rcb != nullptr) {
                QString rcbName = QString::fromLocal8Bit((char *)rcb->data);
                QString rcbRef  = QString("%1.%2.%3").arg(lnRef, prefix, rcbName);

                builder.createRCB(rcbName, lnRef, isBuffered);

                ClientReportControlBlock clientRcb = IedConnection_getRCBValues(
                    m_api.m_libConn, &retval, rcbRef.toStdString().data(), nullptr);

                if (retval == IED_ERROR_OK && clientRcb != nullptr) {
                    auto rcbItem = builder.lastRCB();

                    rcbItem->setRptEna(ClientReportControlBlock_getRptEna(clientRcb));
                    rcbItem->setResv(ClientReportControlBlock_getResv(clientRcb));
                    rcbItem->setTrgOps(ClientReportControlBlock_getTrgOps(clientRcb));
                    rcbItem->setConfRev(ClientReportControlBlock_getConfRev(clientRcb));
                    rcbItem->setBufTm(ClientReportControlBlock_getBufTm(clientRcb));
                    rcbItem->setIntgPd(ClientReportControlBlock_getIntgPd(clientRcb));

                    const char *rptId = ClientReportControlBlock_getRptId(clientRcb);
                    if (rptId) rcbItem->setRptId(QString::fromLocal8Bit(rptId));

                    const char *dsRef = ClientReportControlBlock_getDataSetReference(clientRcb);
                    if (dsRef) rcbItem->setDsRef(QString::fromLocal8Bit(dsRef));

                    MmsValue *owner = ClientReportControlBlock_getOwner(clientRcb);
                    if (owner != nullptr) {
                        int size = MmsValue_getOctetStringSize(owner);
                        if (size == 4) {
                            rcbItem->setOwner(QString("%1.%2.%3.%4")
                                .arg(MmsValue_getOctetStringOctet(owner, 0))
                                .arg(MmsValue_getOctetStringOctet(owner, 1))
                                .arg(MmsValue_getOctetStringOctet(owner, 2))
                                .arg(MmsValue_getOctetStringOctet(owner, 3)));
                        } else if (size > 0) {
                            QString ownerStr;
                            for (int i = 0; i < size; i++) {
                                if (i > 0) ownerStr += ":";
                                ownerStr += QString("%1").arg(
                                    MmsValue_getOctetStringOctet(owner, i), 2, 16, QChar('0'));
                            }
                            rcbItem->setOwner(ownerStr);
                        }
                    }

                    ClientReportControlBlock_destroy(clientRcb);
                }

                rcb = LinkedList_getNext(rcb);
            }
            LinkedList_destroy(rcbList);
        };

        fetchRCBList(ACSI_CLASS_URCB, false, "RP");
        fetchRCBList(ACSI_CLASS_BRCB, true,  "BR");
        return 0;
    }

    int IED_ModelAPI_Impl::fetchLN_GOCB(Core::DataModelBuilder &builder)
    {
        IedClientError retval = IED_ERROR_OK;
        QString lnRef = builder.lastLN()->getReference();

        LinkedList gocbList = IedConnection_getLogicalNodeDirectory(m_api.m_libConn, &retval,
                                  lnRef.toStdString().data(), ACSI_CLASS_GoCB);

        if (retval != IED_ERROR_OK || gocbList == nullptr) {
            return 0;
        }

        LinkedList gocb = LinkedList_getNext(gocbList);
        while (gocb != nullptr) {
            QString name    = QString::fromLocal8Bit((char *)gocb->data);
            QString gocbRef = QString("%1.%2").arg(lnRef, name);

            builder.createGOCB(name, lnRef);

            ClientGooseControlBlock clientGocb = IedConnection_getGoCBValues(
                m_api.m_libConn, &retval, gocbRef.toStdString().data(), nullptr);

            if (retval == IED_ERROR_OK && clientGocb != nullptr) {
                auto gocbItem = builder.lastGOCB();

                gocbItem->setGoEna(ClientGooseControlBlock_getGoEna(clientGocb));
                gocbItem->setConfRev(ClientGooseControlBlock_getConfRev(clientGocb));
                gocbItem->setMinTime(ClientGooseControlBlock_getMinTime(clientGocb));
                gocbItem->setMaxTime(ClientGooseControlBlock_getMaxTime(clientGocb));

                PhyComAddress dstAddr = ClientGooseControlBlock_getDstAddress(clientGocb);
                gocbItem->setAppId(dstAddr.appId);
                gocbItem->setVlanId(dstAddr.vlanId);
                gocbItem->setVlanPriority(dstAddr.vlanPriority);

                const char *goId = ClientGooseControlBlock_getGoID(clientGocb);
                if (goId) gocbItem->setGoId(QString::fromLocal8Bit(goId));

                const char *datSet = ClientGooseControlBlock_getDatSet(clientGocb);
                if (datSet) gocbItem->setDatSet(QString::fromLocal8Bit(datSet));

                ClientGooseControlBlock_destroy(clientGocb);
            }

            gocb = LinkedList_getNext(gocb);
        }
        LinkedList_destroy(gocbList);
        return 0;
    }

    int IED_ModelAPI_Impl::fetchLN_SVCB(Core::DataModelBuilder &builder)
    {
        IedClientError retval = IED_ERROR_OK;
        QString lnRef = builder.lastLN()->getReference();

        auto fetchSVCBList = [&](ACSIClass acsiClass, bool isMulticast, const char *prefix) {
            LinkedList svcbList = IedConnection_getLogicalNodeDirectory(m_api.m_libConn, &retval,
                                      lnRef.toStdString().data(), acsiClass);
            if (retval != IED_ERROR_OK || svcbList == nullptr) {
                return;
            }

            LinkedList svcb = LinkedList_getNext(svcbList);
            while (svcb != nullptr) {
                QString name    = QString::fromLocal8Bit((char *)svcb->data);
                QString svcbRef = QString("%1.%2").arg(lnRef, name);

                builder.createSVCB(name, lnRef, isMulticast);

                ClientSVControlBlock clientSvcb = ClientSVControlBlock_create(
                    m_api.m_libConn, svcbRef.toStdString().data());

                if (clientSvcb != nullptr) {
                    auto svcbItem = builder.lastSVCB();

                    svcbItem->setSvEna(ClientSVControlBlock_getSvEna(clientSvcb));
                    svcbItem->setConfRev(ClientSVControlBlock_getConfRev(clientSvcb));
                    svcbItem->setSmpRate(ClientSVControlBlock_getSmpRate(clientSvcb));
                    svcbItem->setNoASDU(ClientSVControlBlock_getNoASDU(clientSvcb));

                    const char *svId = ClientSVControlBlock_getMsvID(clientSvcb);
                    if (svId) svcbItem->setSvId(QString::fromLocal8Bit(svId));

                    const char *datSet = ClientSVControlBlock_getDatSet(clientSvcb);
                    if (datSet) svcbItem->setDatSet(QString::fromLocal8Bit(datSet));

                    ClientSVControlBlock_destroy(clientSvcb);
                }

                svcb = LinkedList_getNext(svcb);
            }
            LinkedList_destroy(svcbList);
        };

        fetchSVCBList(ACSI_CLASS_MSVCB, true,  "MS");
        fetchSVCBList(ACSI_CLASS_USVCB, false, "US");
        return 0;
    }

    int IED_ModelAPI_Impl::fetchLN_SGCB(Core::DataModelBuilder &builder)
    {
        IedClientError retval = IED_ERROR_OK;
        QString lnRef = builder.lastLN()->getReference();

        // SGCB only exists on LLN0
        if (!lnRef.endsWith("/LLN0")) {
            return 0;
        }

        LinkedList sgcbList = IedConnection_getLogicalNodeDirectory(m_api.m_libConn, &retval,
                                  lnRef.toStdString().data(), ACSI_CLASS_SGCB);

        if (retval != IED_ERROR_OK || sgcbList == nullptr) {
            return 0;
        }

        // Usually only one SGCB per LD
        LinkedList sgcb = LinkedList_getNext(sgcbList);
        if (sgcb != nullptr) {
            QString name = QString::fromLocal8Bit((char *)sgcb->data);
            QString sgcbRef = lnRef + "." + name;
            QString ldRef = lnRef.left(lnRef.indexOf('/'));

            // Read SGCB structure as MMS object with FC=SP
            MmsValue *sgcbVal = IedConnection_readObject(m_api.m_libConn, &retval,
                sgcbRef.toStdString().data(), IEC61850_FC_SP);

            if (retval == IED_ERROR_OK && sgcbVal != nullptr
                && MmsValue_getType(sgcbVal) == MMS_STRUCTURE) {

                int count = MmsValue_getArraySize(sgcbVal);
                uint8_t numOfSG = 0, actSG = 0, editSG = 0;
                bool cnfEdit = false;
                uint64_t lActTm = 0;
                uint16_t resvTms = 0;

                // Parse by position: NumOfSG[0], ActSG[1], EditSG[2],
                //                    CnfEdit[3], LActTm[4], ResvTms[5]
                if (count > 0) numOfSG = (uint8_t)MmsValue_toUint32(MmsValue_getElement(sgcbVal, 0));
                if (count > 1) actSG   = (uint8_t)MmsValue_toUint32(MmsValue_getElement(sgcbVal, 1));
                if (count > 2) editSG  = (uint8_t)MmsValue_toUint32(MmsValue_getElement(sgcbVal, 2));
                if (count > 3) cnfEdit = MmsValue_getBoolean(MmsValue_getElement(sgcbVal, 3));
                if (count > 4) lActTm  = MmsValue_getUtcTimeInMs(MmsValue_getElement(sgcbVal, 4));
                if (count > 5) resvTms = (uint16_t)MmsValue_toUint32(MmsValue_getElement(sgcbVal, 5));

                builder.createSGCB(ldRef, numOfSG, actSG);
                auto sgcbItem = builder.lastSGCB();
                sgcbItem->setEditSG(editSG);
                sgcbItem->setCnfEdit(cnfEdit);
                sgcbItem->setLActTm(lActTm);
                sgcbItem->setResvTms(resvTms);

                MmsValue_delete(sgcbVal);
            }
        }

        LinkedList_destroy(sgcbList);
        return 0;
    }
}