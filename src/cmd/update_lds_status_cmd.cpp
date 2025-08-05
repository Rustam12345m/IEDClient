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

#include "update_lds_status_cmd.hpp"

#include <QDebug>

namespace Cmd
{
    void UpdateLDs_StatusCmd::execute(Cmd::Interface::IEC61850_API::ptr t_api)
    {
        Core::ModelStateUpdater::ptr vals = t_api->state().getStatusForAllLD(m_ied->getDataModel());
        if (vals) {
            // Update process must to be finished in the GUI thread
            emit sigModelValues(vals);

            emit sigCmdEvent(CmdEvent::FinishEvent("IP?",
                    QString("Received values for %1 signals for { LD }").arg(vals->count()),
                    true));
        } else {
            emit sigCmdEvent(CmdEvent::FinishEvent("IP?",
                    QString("Can't get values for { LD }"),
                    false));
        }
    }

    void UpdateLDs_StatusCmd::slotMsgProgress(const QString &t_msg)
    {
    }
}