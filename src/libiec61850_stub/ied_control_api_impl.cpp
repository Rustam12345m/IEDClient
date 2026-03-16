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
                                           int t_trgOps, uint32_t t_bufTm, uint32_t t_intgPd)
    {
        IedClientError error = IED_ERROR_OK;

        ClientReportControlBlock rcb = ClientReportControlBlock_create(
            t_rcbRef.toStdString().data());

        ClientReportControlBlock_setRptEna(rcb, t_enable);
        ClientReportControlBlock_setTrgOps(rcb, t_trgOps);
        ClientReportControlBlock_setBufTm(rcb, t_bufTm);
        ClientReportControlBlock_setIntgPd(rcb, t_intgPd);

        uint32_t mask = RCB_ELEMENT_RPT_ENA | RCB_ELEMENT_TRG_OPS
                      | RCB_ELEMENT_BUF_TM | RCB_ELEMENT_INTG_PD;

        IedConnection_setRCBValues(m_api.m_libConn, &error, rcb, mask, true);

        ClientReportControlBlock_destroy(rcb);

        return error == IED_ERROR_OK;
    }
}
