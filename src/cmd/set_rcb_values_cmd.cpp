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

#include "set_rcb_values_cmd.hpp"

namespace Cmd
{
    void SetRCBValues_Cmd::execute(Cmd::Interface::IEC61850_API::ptr t_api)
    {
        QString prefix = m_rcb->isBuffered() ? "BR" : "RP";
        QString rcbRef = QString("%1.%2.%3").arg(m_rcb->lnRef(), prefix, m_rcb->getName());
        QString action = m_enable ? "Enable" : "Disable";

        emit sigCmdEvent(CmdEvent::StartEvent(rcbRef,
            QString("%1 RCB: %2").arg(action, rcbRef)));

        bool ok = t_api->control().setRCBValues(rcbRef, m_enable, m_trgOps, m_bufTm, m_intgPd);

        if (ok) {
            m_rcb->setRptEna(m_enable);
            m_rcb->setTrgOps(m_trgOps);
            m_rcb->setBufTm(m_bufTm);
            m_rcb->setIntgPd(m_intgPd);

            emit sigCmdEvent(CmdEvent::FinishEvent(rcbRef,
                QString("RCB %1: %2").arg(action, rcbRef), true));
        } else {
            emit sigCmdEvent(CmdEvent::FinishEvent(rcbRef,
                QString("Failed to %1 RCB: %2").arg(action.toLower(), rcbRef), false));
        }
    }
}
