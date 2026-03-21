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

#include "set_sv_enable_cmd.hpp"

namespace Cmd
{
    void SetSVEnable_Cmd::execute(Cmd::Interface::IEC61850_API::ptr api)
    {
        QString svcbRef = QString("%1.%2").arg(m_svcb->lnRef(), m_svcb->getName());
        QString action = m_enable ? "Enable" : "Disable";

        emit sigCmdEvent(CmdEvent::StartEvent(svcbRef,
            QString("%1 SV: %2").arg(action, svcbRef)));

        QString err = api->control().setSVEnable(svcbRef, m_enable);

        if (err.isEmpty()) {
            m_svcb->setSvEna(m_enable);
            emit sigCmdEvent(CmdEvent::FinishEvent(svcbRef,
                QString("SV %1: %2").arg(action, svcbRef), true));
        } else {
            emit sigCmdEvent(CmdEvent::FinishEvent(svcbRef,
                QString("Failed to %1 SV %2: %3").arg(action.toLower(), svcbRef, err), false));
        }
    }
}
