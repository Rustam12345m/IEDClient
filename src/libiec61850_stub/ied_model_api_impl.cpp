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
        QString ref = t_ref.left(t_ref.size() - 4);
        QString fc = t_ref.mid(ref.size() + 1, 2);
        return { ref, fc };
    }

    void    recursiveReadAttributes(IedConnection t_con, QSharedPointer<Core::ModelItem> t_parent,
                                    Core::DataModelBuilder &t_builder)
    {
        IedClientError retval = IED_ERROR_OK;
        LinkedList daList = IedConnection_getDataDirectory(t_con, &retval, t_parent->getReference().toLocal8Bit().data());
        if ((retval == IED_ERROR_OK) && (daList != nullptr)) {
            LinkedList attr = LinkedList_getNext(daList);

            while (attr != nullptr) {
                QString name = QString::fromLocal8Bit((char *)attr->data);

                t_builder.createSDA(t_parent, name);
                auto sda = t_builder.lastSDA();

                QString ref = t_parent->getReference() + "." + name;
                recursiveReadAttributes(t_con, sda, t_builder);

                attr = LinkedList_getNext(attr);
            }
            LinkedList_destroy(daList);
        }
    }
}

namespace Libiec61850
{
    int IED_ModelAPI_Impl::fetchDataModel(Core::DataModelBuilder &t_builder)
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
            t_builder.createLD(QString::fromLocal8Bit((char *)ld->data)); // New LD

            // Fetch all Logical Nodes
            LinkedList lnList = IedConnection_getLogicalDeviceDirectory(m_api.m_libConn, &retval, (char *)ld->data);
            if (retval == IED_ERROR_OK) {

                LinkedList node = LinkedList_getNext(lnList);
                while (node != nullptr) {
                    t_builder.createLN(QString::fromLocal8Bit((char *)node->data)); // New LN

                    emit sigFoundNode(t_builder.lastLN()->getReference());

                    fetchLN_DO(t_builder);
                    auto updVals = m_api.state().getValsForLN(t_builder.lastLN());
                    updVals->update();

                    fetchLN_DS(t_builder);

                    fetchLN_RCB(t_builder);

                    fetchLN_GOCB(t_builder);

                    fetchLN_SVCB(t_builder);

                    m_api.m_state.getValsForLN(t_builder.lastLN());

                    node = LinkedList_getNext(node); // next LN
                }
                LinkedList_destroy(lnList);
            }

            ld = LinkedList_getNext(ld); // next LD
        }
        LinkedList_destroy(ldList);
        return 0;
    }

    int IED_ModelAPI_Impl::fetchLN_DO(Core::DataModelBuilder &t_builder)
    {
        IedClientError retval = IED_ERROR_OK;
        QString ref = t_builder.lastLN()->getReference();

        LinkedList doList = IedConnection_getLogicalNodeDirectory(m_api.m_libConn, &retval,
                                        ref.toStdString().data(), ACSI_CLASS_DATA_OBJECT);
        if ((retval == IED_ERROR_OK) && (doList != nullptr)) {

            LinkedList dObj = LinkedList_getNext(doList);
            while (dObj != nullptr) {
                QString doName = QString::fromLocal8Bit((char *)dObj->data);
                QString refDO = QString("%1.%2").arg(ref, doName);

                t_builder.createDO(doName);

                LinkedList daListFC = IedConnection_getDataDirectoryFC(m_api.m_libConn,
                                                    &retval, refDO.toLocal8Bit().data());
                if ((retval == IED_ERROR_OK) && (daListFC != nullptr)) {

                    LinkedList attrFC = LinkedList_getNext(daListFC);
                    while (attrFC != nullptr) {
                        auto [name, fc, fcNum] = getFX_fromName((char *)attrFC->data);

                        t_builder.createDA(name, fc);

                        // Recursive search SubAttr for DA
                        recursiveReadAttributes(m_api.m_libConn, t_builder.lastDA(), t_builder);

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

    int IED_ModelAPI_Impl::fetchLN_DS(Core::DataModelBuilder &t_builder)
    {
        IedClientError retval = IED_ERROR_OK;
        QString lnRef = t_builder.lastLN()->getReference();

        LinkedList dsList = IedConnection_getLogicalNodeDirectory(m_api.m_libConn, &retval,
                                    lnRef.toStdString().data(), ACSI_CLASS_DATA_SET);
        LinkedList dataSet = LinkedList_getNext(dsList);
        while (dataSet != nullptr) {
            char *dsName = (char *)dataSet->data;
            bool isDeletable = false;

            char dataSetRef[130] = { 0 };
            sprintf(dataSetRef, "%s.%s", lnRef.toStdString().data(), dsName);

            t_builder.createDataSet(QString::fromLocal8Bit(dsName), lnRef, isDeletable);

            LinkedList dsEntityList = IedConnection_getDataSetDirectory(m_api.m_libConn, &retval,
                                                                        dataSetRef, &isDeletable);
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

    int IED_ModelAPI_Impl::fetchLN_RCB(Core::DataModelBuilder &t_builder)
    {
        IedClientError retval = IED_ERROR_OK;
        QString lnRef = t_builder.lastLN()->getReference();

        auto fetchRCBList = [&](ACSIClass t_acsiClass, bool t_isBuffered, const char *t_prefix) {
            LinkedList rcbList = IedConnection_getLogicalNodeDirectory(m_api.m_libConn, &retval,
                                     lnRef.toStdString().data(), t_acsiClass);
            if (retval != IED_ERROR_OK || rcbList == nullptr) {
                return;
            }

            LinkedList rcb = LinkedList_getNext(rcbList);
            while (rcb != nullptr) {
                QString rcbName = QString::fromLocal8Bit((char *)rcb->data);
                QString rcbRef  = QString("%1.%2.%3").arg(lnRef, t_prefix, rcbName);

                t_builder.createRCB(rcbName, lnRef, t_isBuffered);

                ClientReportControlBlock clientRcb = IedConnection_getRCBValues(
                    m_api.m_libConn, &retval, rcbRef.toStdString().data(), nullptr);

                if (retval == IED_ERROR_OK && clientRcb != nullptr) {
                    auto rcbItem = t_builder.lastRCB();

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
                        QString ownerStr;
                        for (int i = 0; i < size; i++) {
                            ownerStr += QString("%1").arg(
                                MmsValue_getOctetStringOctet(owner, i), 2, 16, QChar('0'));
                        }
                        rcbItem->setOwner(ownerStr);
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

    int IED_ModelAPI_Impl::fetchLN_GOCB(Core::DataModelBuilder &t_builder)
    {
        IedClientError retval = IED_ERROR_OK;
        QString lnRef = t_builder.lastLN()->getReference();

        LinkedList gocbList = IedConnection_getLogicalNodeDirectory(m_api.m_libConn, &retval,
                                  lnRef.toStdString().data(), ACSI_CLASS_GoCB);
        if (retval != IED_ERROR_OK || gocbList == nullptr) {
            return 0;
        }

        LinkedList gocb = LinkedList_getNext(gocbList);
        while (gocb != nullptr) {
            QString name    = QString::fromLocal8Bit((char *)gocb->data);
            QString gocbRef = QString("%1.GO.%2").arg(lnRef, name);

            t_builder.createGOCB(name, lnRef);

            ClientGooseControlBlock clientGocb = IedConnection_getGoCBValues(
                m_api.m_libConn, &retval, gocbRef.toStdString().data(), nullptr);

            if (retval == IED_ERROR_OK && clientGocb != nullptr) {
                auto gocbItem = t_builder.lastGOCB();

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

    int IED_ModelAPI_Impl::fetchLN_SVCB(Core::DataModelBuilder &t_builder)
    {
        IedClientError retval = IED_ERROR_OK;
        QString lnRef = t_builder.lastLN()->getReference();

        auto fetchSVCBList = [&](ACSIClass t_acsiClass, bool t_isMulticast, const char *t_prefix) {
            LinkedList svcbList = IedConnection_getLogicalNodeDirectory(m_api.m_libConn, &retval,
                                      lnRef.toStdString().data(), t_acsiClass);
            if (retval != IED_ERROR_OK || svcbList == nullptr) {
                return;
            }

            LinkedList svcb = LinkedList_getNext(svcbList);
            while (svcb != nullptr) {
                QString name    = QString::fromLocal8Bit((char *)svcb->data);
                QString svcbRef = QString("%1.%2.%3").arg(lnRef, t_prefix, name);

                t_builder.createSVCB(name, lnRef, t_isMulticast);

                ClientSVControlBlock clientSvcb = ClientSVControlBlock_create(
                    m_api.m_libConn, svcbRef.toStdString().data());

                if (clientSvcb != nullptr) {
                    auto svcbItem = t_builder.lastSVCB();

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
}