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

            if (size == 4) {
                // IPv4 address
                t_rcb->setOwner(QString("%1.%2.%3.%4")
                    .arg(MmsValue_getOctetStringOctet(owner, 0))
                    .arg(MmsValue_getOctetStringOctet(owner, 1))
                    .arg(MmsValue_getOctetStringOctet(owner, 2))
                    .arg(MmsValue_getOctetStringOctet(owner, 3)));
            } else if (size > 0) {
                // Other format — show as hex
                QString ownerStr;
                for (int i = 0; i < size; i++) {
                    if (i > 0) ownerStr += ":";
                    ownerStr += QString("%1").arg(
                        MmsValue_getOctetStringOctet(owner, i), 2, 16, QChar('0'));
                }
                t_rcb->setOwner(ownerStr);
            } else {
                t_rcb->setOwner("");
            }
        }

        ClientReportControlBlock_destroy(clientRcb);
        return true;
    }

    // ── GOOSE Control Block ────────────────────────────────────────

    bool IED_ControlAPI_Impl::refreshGOOSEValues(Core::GooseControlBlock::ptr t_gocb)
    {
        IedClientError error = IED_ERROR_OK;
        QString gocbRef = QString("%1.%2").arg(t_gocb->lnRef(), t_gocb->getName());
        QByteArray refUtf8 = gocbRef.toUtf8();

        ClientGooseControlBlock clientGocb = IedConnection_getGoCBValues(
            m_api.m_libConn, &error, refUtf8.constData(), nullptr);

        if (error != IED_ERROR_OK || clientGocb == nullptr) {
            return false;
        }

        t_gocb->setGoEna(ClientGooseControlBlock_getGoEna(clientGocb));
        t_gocb->setConfRev(ClientGooseControlBlock_getConfRev(clientGocb));
        t_gocb->setMinTime(ClientGooseControlBlock_getMinTime(clientGocb));
        t_gocb->setMaxTime(ClientGooseControlBlock_getMaxTime(clientGocb));

        PhyComAddress dstAddr = ClientGooseControlBlock_getDstAddress(clientGocb);
        t_gocb->setAppId(dstAddr.appId);
        t_gocb->setVlanId(dstAddr.vlanId);
        t_gocb->setVlanPriority(dstAddr.vlanPriority);

        const char *goId = ClientGooseControlBlock_getGoID(clientGocb);
        if (goId && goId[0] != '\0') t_gocb->setGoId(QString::fromLocal8Bit(goId));

        const char *datSet = ClientGooseControlBlock_getDatSet(clientGocb);
        if (datSet && datSet[0] != '\0') t_gocb->setDatSet(QString::fromLocal8Bit(datSet));

        ClientGooseControlBlock_destroy(clientGocb);
        return true;
    }

    QString IED_ControlAPI_Impl::setGOOSEEnable(const QString &t_gocbRef, bool t_enable)
    {
        if (!m_api.isConnected()) {
            return QString("Not connected");
        }

        IedClientError error = IED_ERROR_OK;
        QByteArray refUtf8 = t_gocbRef.toUtf8();

        ClientGooseControlBlock clientGocb = IedConnection_getGoCBValues(
            m_api.m_libConn, &error, refUtf8.constData(), nullptr);

        if (error != IED_ERROR_OK) {
            return QString("Failed to read GoCB: %1 (error %2)")
                .arg(IedClientError_toString(error)).arg(static_cast<int>(error));
        }
        if (clientGocb == nullptr) {
            return QString("GoCB object not found: %1").arg(t_gocbRef);
        }

        ClientGooseControlBlock_setGoEna(clientGocb, t_enable);

        IedConnection_setGoCBValues(m_api.m_libConn, &error, clientGocb,
            GOCB_ELEMENT_GO_ENA, true);

        ClientGooseControlBlock_destroy(clientGocb);

        if (error != IED_ERROR_OK) {
            return QString("Failed to write GoCB: %1 (error %2)")
                .arg(IedClientError_toString(error)).arg(static_cast<int>(error));
        }
        return {};
    }

    // ── SV Control Block ─────────────────────────────────────────

    bool IED_ControlAPI_Impl::refreshSVValues(Core::SV_ControlBlock::ptr t_svcb)
    {
        QString svcbRef = QString("%1.%2").arg(t_svcb->lnRef(), t_svcb->getName());

        ClientSVControlBlock clientSvcb = ClientSVControlBlock_create(
            m_api.m_libConn, svcbRef.toStdString().data());

        if (clientSvcb == nullptr) {
            return false;
        }

        t_svcb->setSvEna(ClientSVControlBlock_getSvEna(clientSvcb));
        t_svcb->setConfRev(ClientSVControlBlock_getConfRev(clientSvcb));
        t_svcb->setSmpRate(ClientSVControlBlock_getSmpRate(clientSvcb));
        t_svcb->setNoASDU(ClientSVControlBlock_getNoASDU(clientSvcb));

        const char *svId = ClientSVControlBlock_getMsvID(clientSvcb);
        if (svId) t_svcb->setSvId(QString::fromLocal8Bit(svId));

        const char *datSet = ClientSVControlBlock_getDatSet(clientSvcb);
        if (datSet) t_svcb->setDatSet(QString::fromLocal8Bit(datSet));

        ClientSVControlBlock_destroy(clientSvcb);
        return true;
    }

    QString IED_ControlAPI_Impl::setSVEnable(const QString &t_svcbRef, bool t_enable)
    {
        if (!m_api.isConnected()) {
            return QString("Not connected");
        }

        QByteArray refUtf8 = t_svcbRef.toUtf8();

        ClientSVControlBlock clientSvcb = ClientSVControlBlock_create(
            m_api.m_libConn, refUtf8.constData());

        if (clientSvcb == nullptr) {
            return QString("SVCB object not found: %1").arg(t_svcbRef);
        }

        bool ok = ClientSVControlBlock_setSvEna(clientSvcb, t_enable);

        ClientSVControlBlock_destroy(clientSvcb);

        if (!ok) {
            return QString("Failed to set SvEna on %1").arg(t_svcbRef);
        }
        return {};
    }

    // ── Report handling ──────────────────────────────────────────

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

    // ── Direct Control / SBO ──────────────────────────────────────────

    namespace
    {
        Cmd::Interface::CtlValType mmsTypeToCtlValType(MmsType t_type)
        {
            switch (t_type) {
            case MMS_BOOLEAN:  return Cmd::Interface::CtlValType::Boolean;
            case MMS_INTEGER:  return Cmd::Interface::CtlValType::Integer;
            case MMS_UNSIGNED: return Cmd::Interface::CtlValType::Unsigned;
            case MMS_FLOAT:    return Cmd::Interface::CtlValType::Float;
            default:           return Cmd::Interface::CtlValType::Unknown;
            }
        }

        MmsValue *createCtlVal(Cmd::Interface::CtlValType t_type, const QVariant &t_value)
        {
            using VT = Cmd::Interface::CtlValType;
            switch (t_type) {
            case VT::Boolean:  return MmsValue_newBoolean(t_value.toBool());
            case VT::Integer:  return MmsValue_newIntegerFromInt32(t_value.toInt());
            case VT::Unsigned: return MmsValue_newUnsignedFromUint32(static_cast<uint32_t>(t_value.toUInt()));
            case VT::Float:    return MmsValue_newFloat(t_value.toFloat());
            default:           return nullptr;
            }
        }
    }

    Cmd::Interface::ControlInfo IED_ControlAPI_Impl::getControlInfo(const QString &t_objRef)
    {
        using namespace Cmd::Interface;
        ControlInfo info;

        if (!m_api.m_libConn) {
            return info;
        }

        auto ref = t_objRef.toStdString();
        ControlObjectClient client = ControlObjectClient_create(ref.c_str(), m_api.m_libConn);
        if (!client) {
            return info;
        }

        info.model = static_cast<CtlModel>(ControlObjectClient_getControlModel(client));
        info.valType = mmsTypeToCtlValType(ControlObjectClient_getCtlValType(client));

        ControlObjectClient_destroy(client);
        return info;
    }

    bool IED_ControlAPI_Impl::controlOperate(const QString &t_objRef, Cmd::Interface::CtlModel t_model,
                                              Cmd::Interface::CtlValType t_valType, const QVariant &t_value)
    {
        if (!m_api.m_libConn) {
            return false;
        }

        auto ref = t_objRef.toStdString();
        ControlObjectClient client = ControlObjectClient_create(ref.c_str(), m_api.m_libConn);
        if (!client) {
            return false;
        }

        ControlObjectClient_setControlModel(client, static_cast<ControlModel>(static_cast<int>(t_model)));

        MmsValue *val = createCtlVal(t_valType, t_value);
        if (!val) {
            ControlObjectClient_destroy(client);
            return false;
        }

        bool ok = ControlObjectClient_operate(client, val, 0);

        MmsValue_delete(val);
        ControlObjectClient_destroy(client);
        return ok;
    }

    bool IED_ControlAPI_Impl::controlSelect(const QString &t_objRef, Cmd::Interface::CtlModel t_model,
                                             Cmd::Interface::CtlValType t_valType, const QVariant &t_value)
    {
        if (!m_api.m_libConn) {
            return false;
        }

        auto ref = t_objRef.toStdString();
        ControlObjectClient client = ControlObjectClient_create(ref.c_str(), m_api.m_libConn);
        if (!client) {
            return false;
        }

        ControlObjectClient_setControlModel(client, static_cast<ControlModel>(static_cast<int>(t_model)));

        bool ok = false;
        if (t_model == Cmd::Interface::CtlModel::SBOEnhanced) {
            MmsValue *val = createCtlVal(t_valType, t_value);
            if (val) {
                ok = ControlObjectClient_selectWithValue(client, val);
                MmsValue_delete(val);
            }
        } else {
            ok = ControlObjectClient_select(client);
        }

        ControlObjectClient_destroy(client);
        return ok;
    }

    bool IED_ControlAPI_Impl::controlCancel(const QString &t_objRef)
    {
        if (!m_api.m_libConn) {
            return false;
        }

        auto ref = t_objRef.toStdString();
        ControlObjectClient client = ControlObjectClient_create(ref.c_str(), m_api.m_libConn);
        if (!client) {
            return false;
        }

        bool ok = ControlObjectClient_cancel(client);
        ControlObjectClient_destroy(client);
        return ok;
    }
}
