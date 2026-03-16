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

#include "ied_control_api_impl.hpp"
#include "libiec61850_adapter.hpp"

#include <QDebug>

extern "C"
{
#include <iec61850_client.h>
#include <mms_value.h>
}

namespace Libiec61850
{
    bool IED_ControlAPI_Impl::setRCBValues(const QString &t_rcbRef, bool t_enable,
                                           int t_trgOps, uint32_t t_bufTm, uint32_t t_intgPd,
                                           const QString &t_rptId, const QString &t_datSet)
    {
        IedClientError error = IED_ERROR_OK;

        ClientReportControlBlock rcb = ClientReportControlBlock_create(
            t_rcbRef.toStdString().data());

        ClientReportControlBlock_setRptEna(rcb, t_enable);

        uint32_t mask = RCB_ELEMENT_RPT_ENA;

        if (t_enable) {
            ClientReportControlBlock_setTrgOps(rcb, t_trgOps);
            ClientReportControlBlock_setBufTm(rcb, t_bufTm);
            ClientReportControlBlock_setIntgPd(rcb, t_intgPd);
            ClientReportControlBlock_setRptId(rcb, t_rptId.toStdString().data());
            ClientReportControlBlock_setDataSetReference(rcb, t_datSet.toStdString().data());

            mask |= RCB_ELEMENT_TRG_OPS | RCB_ELEMENT_BUF_TM
                  | RCB_ELEMENT_INTG_PD | RCB_ELEMENT_RPT_ID | RCB_ELEMENT_DATSET;
        }

        IedConnection_setRCBValues(m_api.m_libConn, &error, rcb, mask, true);

        ClientReportControlBlock_destroy(rcb);

        return error == IED_ERROR_OK;
    }

    bool IED_ControlAPI_Impl::refreshRCBValues(Core::ReportBlock::ptr t_rcb)
    {
        IedClientError error = IED_ERROR_OK;
        QString prefix = t_rcb->isBuffered() ? "BR" : "RP";
        QString rcbRef = QString("%1.%2.%3").arg(t_rcb->lnRef(), prefix, t_rcb->getName());

        ClientReportControlBlock clientRcb = IedConnection_getRCBValues(
            m_api.m_libConn, &error, rcbRef.toStdString().data(), nullptr);

        if (error != IED_ERROR_OK || clientRcb == nullptr) {
            return false;
        }

        t_rcb->setRptEna(ClientReportControlBlock_getRptEna(clientRcb));
        t_rcb->setResv(ClientReportControlBlock_getResv(clientRcb));
        t_rcb->setTrgOps(ClientReportControlBlock_getTrgOps(clientRcb));
        t_rcb->setConfRev(ClientReportControlBlock_getConfRev(clientRcb));
        t_rcb->setBufTm(ClientReportControlBlock_getBufTm(clientRcb));
        t_rcb->setIntgPd(ClientReportControlBlock_getIntgPd(clientRcb));

        const char *rptId = ClientReportControlBlock_getRptId(clientRcb);
        if (rptId) t_rcb->setRptId(QString::fromLocal8Bit(rptId));

        const char *dsRef = ClientReportControlBlock_getDataSetReference(clientRcb);
        if (dsRef) t_rcb->setDsRef(QString::fromLocal8Bit(dsRef));

        MmsValue *owner = ClientReportControlBlock_getOwner(clientRcb);
        if (owner != nullptr) {
            int size = MmsValue_getOctetStringSize(owner);
            QString ownerStr;
            for (int i = 0; i < size; i++) {
                ownerStr += QString("%1").arg(
                    MmsValue_getOctetStringOctet(owner, i), 2, 16, QChar('0'));
            }
            t_rcb->setOwner(ownerStr);
        }

        ClientReportControlBlock_destroy(clientRcb);
        return true;
    }

    namespace
    {
        void flattenMmsValue(const QString &t_baseName, MmsValue *t_value, int t_reason,
                             QStringList &t_names, QStringList &t_values, QList<int> &t_reasons)
        {
            if (!t_value) {
                t_names.append(t_baseName);
                t_values.append(QString());
                t_reasons.append(t_reason);
                return;
            }

            MmsType type = MmsValue_getType(t_value);

            if (type == MMS_STRUCTURE) {
                int count = MmsValue_getArraySize(t_value);
                for (int i = 0; i < count; i++) {
                    MmsValue *child = MmsValue_getElement(t_value, i);
                    QString childName = QString("%1.%2").arg(t_baseName).arg(i);
                    flattenMmsValue(childName, child, t_reason, t_names, t_values, t_reasons);
                }
            } else if (type == MMS_ARRAY) {
                int count = MmsValue_getArraySize(t_value);
                for (int i = 0; i < count; i++) {
                    MmsValue *child = MmsValue_getElement(t_value, i);
                    QString childName = QString("%1[%2]").arg(t_baseName).arg(i);
                    flattenMmsValue(childName, child, t_reason, t_names, t_values, t_reasons);
                }
            } else {
                char buf[256];
                MmsValue_printToBuffer(t_value, buf, sizeof(buf));
                t_names.append(t_baseName);
                t_values.append(QString::fromUtf8(buf));
                t_reasons.append(t_reason);
            }
        }
    }

    void IED_ControlAPI_Impl::staticReportCallback(void *t_param, void *t_report)
    {
        auto *storage = static_cast<Core::ReportStorage*>(t_param);
        auto  report  = static_cast<ClientReport>(t_report);
        if (!storage || !report) {
            return;
        }

        auto rpt = QSharedPointer<Core::ReceivedReport>::create();

        char *rcbRef = ClientReport_getRcbReference(report);
        if (rcbRef) {
            rpt->rcbRef = QString::fromUtf8(rcbRef);
        }

        const char *dsName = ClientReport_getDataSetName(report);
        if (dsName) {
            rpt->dataSetRef = QString::fromUtf8(dsName);
        } else {
            rpt->dataSetRef = storage->dataSetRef();
        }

        if (ClientReport_hasTimestamp(report)) {
            rpt->timestamp = ClientReport_getTimestamp(report);
        }

        if (ClientReport_hasSeqNum(report)) {
            rpt->seqNum = ClientReport_getSeqNum(report);
        }

        MmsValue *dataSetValues = ClientReport_getDataSetValues(report);
        if (dataSetValues) {
            int numElements = MmsValue_getArraySize(dataSetValues);
            bool hasReasons = ClientReport_hasReasonForInclusion(report);

            for (int i = 0; i < numElements; i++) {
                MmsValue *element = MmsValue_getElement(dataSetValues, i);

                const char *dataRef = ClientReport_getDataReference(report, i);
                QString entryName = dataRef ? QString::fromUtf8(dataRef)
                                            : storage->memberName(i);

                int reason = 0;
                if (hasReasons) {
                    reason = ClientReport_getReasonForInclusion(report, i);
                }
                rpt->reasonCode |= reason;

                flattenMmsValue(entryName, element, reason,
                                rpt->entryNames, rpt->entryValues, rpt->entryReasons);
            }
        }

        storage->addReport(rpt);
    }

    bool IED_ControlAPI_Impl::installReportHandler(const QString &t_rcbRef,
                                                    const QString &t_rptId,
                                                    Core::ReportStorage *t_storage)
    {
        if (!t_storage || !m_api.m_libConn) {
            return false;
        }

        IedConnection_installReportHandler(
            m_api.m_libConn,
            t_rcbRef.toStdString().c_str(),
            t_rptId.toStdString().c_str(),
            reinterpret_cast<ReportCallbackFunction>(&staticReportCallback),
            t_storage);

        m_activeHandlers.insert(t_rcbRef);
        return true;
    }

    void IED_ControlAPI_Impl::uninstallReportHandler(const QString &t_rcbRef)
    {
        if (!m_api.m_libConn) {
            return;
        }

        IedConnection_uninstallReportHandler(m_api.m_libConn,
                                              t_rcbRef.toStdString().c_str());
        m_activeHandlers.remove(t_rcbRef);
    }

    void IED_ControlAPI_Impl::uninstallAllHandlers()
    {
        if (!m_api.m_libConn) {
            m_activeHandlers.clear();
            return;
        }

        for (const auto &ref : m_activeHandlers) {
            IedConnection_uninstallReportHandler(m_api.m_libConn,
                                                  ref.toStdString().c_str());
        }
        m_activeHandlers.clear();
    }
}
