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

#include "update_lns_status_cmd.hpp"
#include <QDebug>

namespace Cmd
{
    void UpdateLNs_StatusCmd::execute(Cmd::Interface::IEC61850_API::ptr t_api)
    {
        Core::ModelStateUpdater::ptr vals = t_api->state().getStatusForAllLN(m_ld);
        if (vals) {
            emit sigModelValues(vals);

            emit sigCmdEvent(Cmd::CmdEvent::FinishEvent("", "All status of LNs were updated", true));
        } else {
            emit sigCmdEvent(Cmd::CmdEvent::FinishEvent("", "Can't get status for LNs", false));
        }
    }
}