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

#include "update_goose_cbs_cmd.hpp"
#include <QDebug>

namespace Cmd
{
    void UpdateGooseCBs_Cmd::execute(Cmd::Interface::IEC61850_API::ptr t_api)
    {
        emit sigCmdEvent(CmdEvent::StartEvent("", "Update GOOSE: refreshing all values"));

        const auto &gocbList = m_ied->model().getGO_CBList();
        int updated = 0;

        for (const auto &gocb : gocbList) {
            if (t_api->control().refreshGOOSEValues(gocb)) {
                ++updated;
            }
        }

        bool ok = (gocbList.size() == 0) || (updated > 0);
        emit sigCmdEvent(CmdEvent::FinishEvent("",
            QString("Update GOOSE: refreshed %1 of %2").arg(updated).arg(gocbList.size()),
            ok));
    }
}
