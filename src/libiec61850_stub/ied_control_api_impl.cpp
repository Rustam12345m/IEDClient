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

extern "C"
{
#include <iec61850_client.h>
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
        ClientReportControlBlock_setTrgOps(rcb, t_trgOps);
        ClientReportControlBlock_setBufTm(rcb, t_bufTm);
        ClientReportControlBlock_setIntgPd(rcb, t_intgPd);
        ClientReportControlBlock_setRptId(rcb, t_rptId.toStdString().data());
        ClientReportControlBlock_setDataSetReference(rcb, t_datSet.toStdString().data());

        uint32_t mask = RCB_ELEMENT_RPT_ENA | RCB_ELEMENT_TRG_OPS
                      | RCB_ELEMENT_BUF_TM | RCB_ELEMENT_INTG_PD
                      | RCB_ELEMENT_RPT_ID | RCB_ELEMENT_DATSET;

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
}
