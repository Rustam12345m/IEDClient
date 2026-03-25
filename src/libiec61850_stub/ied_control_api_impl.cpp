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
    bool IED_ControlAPI_Impl::setRCBValues(const QString &rcbRef, bool enable,
                                           int trgOps, uint32_t bufTm, uint32_t intgPd,
                                           const QString &rptId, const QString &datSet)
    {
        IedClientError error = IED_ERROR_OK;

        ClientReportControlBlock rcb = ClientReportControlBlock_create(
            rcbRef.toStdString().data());

        ClientReportControlBlock_setRptEna(rcb, enable);

        uint32_t mask = RCB_ELEMENT_RPT_ENA;

        if (enable) {
            ClientReportControlBlock_setTrgOps(rcb, trgOps);
            ClientReportControlBlock_setBufTm(rcb, bufTm);
            ClientReportControlBlock_setIntgPd(rcb, intgPd);
            ClientReportControlBlock_setRptId(rcb, rptId.toStdString().data());
            ClientReportControlBlock_setDataSetReference(rcb, datSet.toStdString().data());

            mask |= RCB_ELEMENT_TRG_OPS | RCB_ELEMENT_BUF_TM
                  | RCB_ELEMENT_INTG_PD | RCB_ELEMENT_RPT_ID | RCB_ELEMENT_DATSET;
        }

        IedConnection_setRCBValues(m_api.m_libConn, &error, rcb, mask, true);

        ClientReportControlBlock_destroy(rcb);

        return error == IED_ERROR_OK;
    }

    bool IED_ControlAPI_Impl::refreshRCBValues(Core::ReportBlock::ptr rcb)
    {
        IedClientError error = IED_ERROR_OK;
        QString prefix = rcb->isBuffered() ? "BR" : "RP";
        QString rcbRef = QString("%1.%2.%3").arg(rcb->lnRef(), prefix, rcb->getName());

        ClientReportControlBlock clientRcb = IedConnection_getRCBValues(
            m_api.m_libConn, &error, rcbRef.toStdString().data(), nullptr);

        if (error != IED_ERROR_OK || clientRcb == nullptr) {
            return false;
        }

        rcb->setRptEna(ClientReportControlBlock_getRptEna(clientRcb));
        rcb->setResv(ClientReportControlBlock_getResv(clientRcb));
        rcb->setTrgOps(ClientReportControlBlock_getTrgOps(clientRcb));
        rcb->setConfRev(ClientReportControlBlock_getConfRev(clientRcb));
        rcb->setBufTm(ClientReportControlBlock_getBufTm(clientRcb));
        rcb->setIntgPd(ClientReportControlBlock_getIntgPd(clientRcb));

        const char *rptId = ClientReportControlBlock_getRptId(clientRcb);
        if (rptId) rcb->setRptId(QString::fromLocal8Bit(rptId));

        const char *dsRef = ClientReportControlBlock_getDataSetReference(clientRcb);
        if (dsRef) rcb->setDsRef(QString::fromLocal8Bit(dsRef));

        MmsValue *owner = ClientReportControlBlock_getOwner(clientRcb);
        if (owner != nullptr) {
            int size = MmsValue_getOctetStringSize(owner);

            if (size == 4) {
                // IPv4 address
                rcb->setOwner(QString("%1.%2.%3.%4")
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
                rcb->setOwner(ownerStr);
            } else {
                rcb->setOwner("");
            }
        }

        ClientReportControlBlock_destroy(clientRcb);
        return true;
    }

    // ── GOOSE Control Block ────────────────────────────────────────

    bool IED_ControlAPI_Impl::refreshGOOSEValues(Core::GooseControlBlock::ptr gocb)
    {
        IedClientError error = IED_ERROR_OK;
        QString gocbRef = QString("%1.%2").arg(gocb->lnRef(), gocb->getName());
        QByteArray refUtf8 = gocbRef.toUtf8();

        ClientGooseControlBlock clientGocb = IedConnection_getGoCBValues(
            m_api.m_libConn, &error, refUtf8.constData(), nullptr);

        if (error != IED_ERROR_OK || clientGocb == nullptr) {
            return false;
        }

        gocb->setGoEna(ClientGooseControlBlock_getGoEna(clientGocb));
        gocb->setConfRev(ClientGooseControlBlock_getConfRev(clientGocb));
        gocb->setMinTime(ClientGooseControlBlock_getMinTime(clientGocb));
        gocb->setMaxTime(ClientGooseControlBlock_getMaxTime(clientGocb));

        PhyComAddress dstAddr = ClientGooseControlBlock_getDstAddress(clientGocb);
        gocb->setAppId(dstAddr.appId);
        gocb->setVlanId(dstAddr.vlanId);
        gocb->setVlanPriority(dstAddr.vlanPriority);

        const char *goId = ClientGooseControlBlock_getGoID(clientGocb);
        if (goId && goId[0] != '\0') gocb->setGoId(QString::fromLocal8Bit(goId));

        const char *datSet = ClientGooseControlBlock_getDatSet(clientGocb);
        if (datSet && datSet[0] != '\0') gocb->setDatSet(QString::fromLocal8Bit(datSet));

        ClientGooseControlBlock_destroy(clientGocb);
        return true;
    }

    QString IED_ControlAPI_Impl::setGOOSEEnable(const QString &gocbRef, bool enable)
    {
        if (!m_api.isConnected()) {
            return QString("Not connected");
        }

        IedClientError error = IED_ERROR_OK;
        QByteArray refUtf8 = gocbRef.toUtf8();

        ClientGooseControlBlock clientGocb = IedConnection_getGoCBValues(
            m_api.m_libConn, &error, refUtf8.constData(), nullptr);

        if (error != IED_ERROR_OK) {
            return QString("Failed to read GoCB: %1 (error %2)")
                .arg(IedClientError_toString(error)).arg(static_cast<int>(error));
        }
        if (clientGocb == nullptr) {
            return QString("GoCB object not found: %1").arg(gocbRef);
        }

        ClientGooseControlBlock_setGoEna(clientGocb, enable);

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

    bool IED_ControlAPI_Impl::refreshSVValues(Core::SV_ControlBlock::ptr svcb)
    {
        QString svcbRef = QString("%1.%2").arg(svcb->lnRef(), svcb->getName());

        ClientSVControlBlock clientSvcb = ClientSVControlBlock_create(
            m_api.m_libConn, svcbRef.toStdString().data());

        if (clientSvcb == nullptr) {
            return false;
        }

        svcb->setSvEna(ClientSVControlBlock_getSvEna(clientSvcb));
        svcb->setConfRev(ClientSVControlBlock_getConfRev(clientSvcb));
        svcb->setSmpRate(ClientSVControlBlock_getSmpRate(clientSvcb));
        svcb->setNoASDU(ClientSVControlBlock_getNoASDU(clientSvcb));

        const char *svId = ClientSVControlBlock_getMsvID(clientSvcb);
        if (svId) svcb->setSvId(QString::fromLocal8Bit(svId));

        const char *datSet = ClientSVControlBlock_getDatSet(clientSvcb);
        if (datSet) svcb->setDatSet(QString::fromLocal8Bit(datSet));

        ClientSVControlBlock_destroy(clientSvcb);
        return true;
    }

    QString IED_ControlAPI_Impl::setSVEnable(const QString &svcbRef, bool enable)
    {
        if (!m_api.isConnected()) {
            return QString("Not connected");
        }

        QByteArray refUtf8 = svcbRef.toUtf8();

        ClientSVControlBlock clientSvcb = ClientSVControlBlock_create(
            m_api.m_libConn, refUtf8.constData());

        if (clientSvcb == nullptr) {
            return QString("SVCB object not found: %1").arg(svcbRef);
        }

        bool ok = ClientSVControlBlock_setSvEna(clientSvcb, enable);

        ClientSVControlBlock_destroy(clientSvcb);

        if (!ok) {
            return QString("Failed to set SvEna on %1").arg(svcbRef);
        }
        return {};
    }

    // ── Report handling ──────────────────────────────────────────

    namespace
    {
        void flattenMmsValue(const QString &baseName, MmsValue *value, int reason,
                             QStringList &names, QStringList &values, QList<int> &reasons)
        {
            if (!value) {
                names.append(baseName);
                values.append(QString());
                reasons.append(reason);
                return;
            }

            MmsType type = MmsValue_getType(value);

            if (type == MMS_STRUCTURE) {
                int count = MmsValue_getArraySize(value);
                for (int i = 0; i < count; i++) {
                    MmsValue *child = MmsValue_getElement(value, i);
                    QString childName = QString("%1.%2").arg(baseName).arg(i);
                    flattenMmsValue(childName, child, reason, names, values, reasons);
                }
            } else if (type == MMS_ARRAY) {
                int count = MmsValue_getArraySize(value);
                for (int i = 0; i < count; i++) {
                    MmsValue *child = MmsValue_getElement(value, i);
                    QString childName = QString("%1[%2]").arg(baseName).arg(i);
                    flattenMmsValue(childName, child, reason, names, values, reasons);
                }
            } else {
                char buf[256];
                MmsValue_printToBuffer(value, buf, sizeof(buf));
                names.append(baseName);
                values.append(QString::fromUtf8(buf));
                reasons.append(reason);
            }
        }
    }

    void IED_ControlAPI_Impl::staticReportCallback(void *param, void *report)
    {
        auto *storage = static_cast<Core::ReportStorage*>(param);
        auto  rptData = static_cast<ClientReport>(report);
        if (!storage || !rptData) {
            return;
        }

        auto rpt = QSharedPointer<Core::ReceivedReport>::create();

        char *rcbRef = ClientReport_getRcbReference(rptData);
        if (rcbRef) {
            rpt->rcbRef = QString::fromUtf8(rcbRef);
        }

        const char *dsName = ClientReport_getDataSetName(rptData);
        if (dsName) {
            rpt->dataSetRef = QString::fromUtf8(dsName);
        } else {
            rpt->dataSetRef = storage->dataSetRef();
        }

        if (ClientReport_hasTimestamp(rptData)) {
            rpt->timestamp = ClientReport_getTimestamp(rptData);
        }

        if (ClientReport_hasSeqNum(rptData)) {
            rpt->seqNum = ClientReport_getSeqNum(rptData);
        }

        MmsValue *dataSetValues = ClientReport_getDataSetValues(rptData);
        if (dataSetValues) {
            int numElements = MmsValue_getArraySize(dataSetValues);
            bool hasReasons = ClientReport_hasReasonForInclusion(rptData);

            for (int i = 0; i < numElements; i++) {
                MmsValue *element = MmsValue_getElement(dataSetValues, i);

                const char *dataRef = ClientReport_getDataReference(rptData, i);
                QString entryName = dataRef ? QString::fromUtf8(dataRef)
                                            : storage->memberName(i);

                int reason = 0;
                if (hasReasons) {
                    reason = ClientReport_getReasonForInclusion(rptData, i);
                }
                rpt->reasonCode |= reason;

                flattenMmsValue(entryName, element, reason,
                                rpt->entryNames, rpt->entryValues, rpt->entryReasons);
            }
        }

        storage->addReport(rpt);
    }

    bool IED_ControlAPI_Impl::installReportHandler(const QString &rcbRef,
                                                    const QString &rptId,
                                                    Core::ReportStorage *storage)
    {
        if (!storage || !m_api.m_libConn) {
            return false;
        }

        IedConnection_installReportHandler(
            m_api.m_libConn,
            rcbRef.toStdString().c_str(),
            rptId.toStdString().c_str(),
            reinterpret_cast<ReportCallbackFunction>(&staticReportCallback),
            storage);

        m_activeHandlers.insert(rcbRef);
        return true;
    }

    void IED_ControlAPI_Impl::uninstallReportHandler(const QString &rcbRef)
    {
        if (!m_api.m_libConn) {
            return;
        }

        IedConnection_uninstallReportHandler(m_api.m_libConn,
                                              rcbRef.toStdString().c_str());
        m_activeHandlers.remove(rcbRef);
    }

    void IED_ControlAPI_Impl::uninstallAllHandlers()
    {
        destroyActiveClient();

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

    IED_ControlAPI_Impl::~IED_ControlAPI_Impl()
    {
        destroyActiveClient();
    }

    namespace
    {
        Cmd::Interface::CtlValType mmsTypeToCtlValType(MmsType type)
        {
            switch (type) {
            case MMS_BOOLEAN:   return Cmd::Interface::CtlValType::Boolean;
            case MMS_INTEGER:   return Cmd::Interface::CtlValType::Integer;
            case MMS_UNSIGNED:  return Cmd::Interface::CtlValType::Unsigned;
            case MMS_FLOAT:     return Cmd::Interface::CtlValType::Float;
            case MMS_STRUCTURE: return Cmd::Interface::CtlValType::Float; // APC AnalogueValue
            default:            return Cmd::Interface::CtlValType::Unknown;
            }
        }

        MmsValue *createCtlVal(Cmd::Interface::CtlValType type, const QVariant &value,
                               MmsType origType = MMS_FLOAT)
        {
            using VT = Cmd::Interface::CtlValType;
            switch (type) {
            case VT::Boolean:  return MmsValue_newBoolean(value.toBool());
            case VT::Integer:  return MmsValue_newIntegerFromInt32(value.toInt());
            case VT::Unsigned: return MmsValue_newUnsignedFromUint32(static_cast<uint32_t>(value.toUInt()));
            case VT::Float: {
                // APC: ctlVal is AnalogueValue structure wrapping f or i
                if (origType == MMS_STRUCTURE) {
                    MmsValue *av = MmsValue_createEmptyStructure(1);
                    MmsValue_setElement(av, 0, MmsValue_newFloat(value.toFloat()));
                    return av;
                }
                return MmsValue_newFloat(value.toFloat());
            }
            default:           return nullptr;
            }
        }
    }

    void IED_ControlAPI_Impl::commandTerminationHandler(void *param, ControlObjectClient client)
    {
        auto *self = static_cast<IED_ControlAPI_Impl*>(param);
        if (!self) return;

        LastApplError lastErr = ControlObjectClient_getLastApplError(client);

        // CommandTermination+: error=NO_ERROR and addCause=UNKNOWN
        bool success = (lastErr.error == CONTROL_ERROR_NO_ERROR
                        && lastErr.addCause == ADD_CAUSE_UNKNOWN);

        QString addCauseStr = Cmd::Interface::addCauseToString(static_cast<int>(lastErr.addCause));
        QString objRef = self->m_activeClientRef;

        emit self->m_api.sigCommandTermination(objRef, success, addCauseStr);
    }

    ControlObjectClient IED_ControlAPI_Impl::getOrCreateClient(const QString &objRef)
    {
        if (m_activeClient && m_activeClientRef == objRef) {
            applyControlFlags(m_activeClient);
            return m_activeClient;
        }

        destroyActiveClient();

        if (!m_api.m_libConn) {
            return nullptr;
        }

        auto ref = objRef.toStdString();
        m_activeClient = ControlObjectClient_create(ref.c_str(), m_api.m_libConn);
        if (!m_activeClient) {
            return nullptr;
        }

        m_activeClientRef = objRef;

        ControlObjectClient_setCommandTerminationHandler(
            m_activeClient, commandTerminationHandler, this);

        ControlObjectClient_setOrigin(m_activeClient, nullptr, CONTROL_ORCAT_STATION_CONTROL);

        applyControlFlags(m_activeClient);

        return m_activeClient;
    }

    void IED_ControlAPI_Impl::destroyActiveClient()
    {
        if (m_activeClient) {
            ControlObjectClient_destroy(m_activeClient);
            m_activeClient = nullptr;
        }
        m_activeClientRef.clear();
    }

    void IED_ControlAPI_Impl::applyControlFlags(ControlObjectClient client)
    {
        ControlObjectClient_setTestMode(client, m_testMode);
        ControlObjectClient_setInterlockCheck(client, m_interlockCheck);
        ControlObjectClient_setSynchroCheck(client, m_synchroCheck);
    }

    void IED_ControlAPI_Impl::setTestMode(bool test)
    {
        m_testMode = test;
        if (m_activeClient) {
            ControlObjectClient_setTestMode(m_activeClient, test);
        }
    }

    void IED_ControlAPI_Impl::setInterlockCheck(bool check)
    {
        m_interlockCheck = check;
        if (m_activeClient) {
            ControlObjectClient_setInterlockCheck(m_activeClient, check);
        }
    }

    void IED_ControlAPI_Impl::setSynchroCheck(bool check)
    {
        m_synchroCheck = check;
        if (m_activeClient) {
            ControlObjectClient_setSynchroCheck(m_activeClient, check);
        }
    }

    Cmd::Interface::ControlInfo IED_ControlAPI_Impl::getControlInfo(const QString &objRef)
    {
        using namespace Cmd::Interface;
        ControlInfo info;

        if (!m_api.m_libConn) {
            return info;
        }

        auto ref = objRef.toStdString();
        ControlObjectClient client = ControlObjectClient_create(ref.c_str(), m_api.m_libConn);
        if (!client) {
            return info;
        }

        info.model = static_cast<CtlModel>(ControlObjectClient_getControlModel(client));
        info.valType = mmsTypeToCtlValType(ControlObjectClient_getCtlValType(client));

        ControlObjectClient_destroy(client);
        return info;
    }

    bool IED_ControlAPI_Impl::controlOperate(const QString &objRef, Cmd::Interface::CtlModel model,
                                              Cmd::Interface::CtlValType valType, const QVariant &value)
    {
        ControlObjectClient client = getOrCreateClient(objRef);
        if (!client) {
            return false;
        }

        ControlObjectClient_setControlModel(client, static_cast<ControlModel>(static_cast<int>(model)));

        MmsType origType = ControlObjectClient_getCtlValType(client);
        MmsValue *val = createCtlVal(valType, value, origType);
        if (!val) {
            return false;
        }

        bool ok = ControlObjectClient_operate(client, val, 0);
        MmsValue_delete(val);
        return ok;
    }

    bool IED_ControlAPI_Impl::controlSelect(const QString &objRef, Cmd::Interface::CtlModel model,
                                             Cmd::Interface::CtlValType valType, const QVariant &value)
    {
        ControlObjectClient client = getOrCreateClient(objRef);
        if (!client) {
            return false;
        }

        ControlObjectClient_setControlModel(client, static_cast<ControlModel>(static_cast<int>(model)));

        bool ok = false;
        if (model == Cmd::Interface::CtlModel::SBOEnhanced) {
            MmsType origType = ControlObjectClient_getCtlValType(client);
            MmsValue *val = createCtlVal(valType, value, origType);
            if (val) {
                ok = ControlObjectClient_selectWithValue(client, val);
                MmsValue_delete(val);
            }
        } else {
            ok = ControlObjectClient_select(client);
        }

        return ok;
    }

    bool IED_ControlAPI_Impl::controlCancel(const QString &objRef)
    {
        ControlObjectClient client = getOrCreateClient(objRef);
        if (!client) {
            return false;
        }

        bool ok = ControlObjectClient_cancel(client);
        destroyActiveClient();
        return ok;
    }
}