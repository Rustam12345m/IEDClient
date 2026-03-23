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

#include "write_value_cmd.hpp"

namespace Cmd
{
    void WriteValue_Cmd::execute(Cmd::Interface::IEC61850_API::ptr api)
    {
        emit sigCmdEvent(CmdEvent::StartEvent(m_ref,
            QString("Write %1 [%2] = %3").arg(m_ref, m_fc, m_value)));

        QString err = api->state().writeValueByRef(m_ref, m_fc, m_value);

        bool ok = err.isEmpty();
        QString msg = ok ? QString("Write OK: %1").arg(m_ref)
                         : QString("Write FAILED: %1 — %2").arg(m_ref, err);

        emit sigWriteResult(m_ref, ok, msg);
        emit sigCmdEvent(CmdEvent::FinishEvent(m_ref, msg, ok));
    }
}
