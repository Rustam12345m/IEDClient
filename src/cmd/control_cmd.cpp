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

#include "control_cmd.hpp"

namespace Cmd
{
    void GetControlInfo_Cmd::execute(Cmd::Interface::IEC61850_API::ptr t_api)
    {
        emit sigCmdEvent(CmdEvent::StartEvent(m_objRef, "Query control info: " + m_objRef));

        auto info = t_api->control().getControlInfo(m_objRef);

        emit sigControlInfo(m_objRef,
                            static_cast<int>(info.model),
                            static_cast<int>(info.valType));

        emit sigCmdEvent(CmdEvent::FinishEvent(m_objRef, "Control info: " + m_objRef, true));
    }

    void ControlOperate_Cmd::execute(Cmd::Interface::IEC61850_API::ptr t_api)
    {
        bool ok = false;
        QString actionName;

        switch (m_action) {
        case Action::Operate:
            actionName = "Operate";
            emit sigCmdEvent(CmdEvent::StartEvent(m_objRef, "Operate: " + m_objRef));
            ok = t_api->control().controlOperate(m_objRef, m_model, m_valType, m_value);
            break;

        case Action::Select:
            actionName = "Select";
            emit sigCmdEvent(CmdEvent::StartEvent(m_objRef, "Select: " + m_objRef));
            ok = t_api->control().controlSelect(m_objRef, m_model, m_valType, m_value);
            break;

        case Action::Cancel:
            actionName = "Cancel";
            emit sigCmdEvent(CmdEvent::StartEvent(m_objRef, "Cancel: " + m_objRef));
            ok = t_api->control().controlCancel(m_objRef);
            break;
        }

        QString msg = ok ? (actionName + " OK: " + m_objRef)
                         : (actionName + " FAILED: " + m_objRef);

        emit sigControlResult(m_objRef, ok, msg);
        emit sigCmdEvent(CmdEvent::FinishEvent(m_objRef, msg, ok));
    }
}
