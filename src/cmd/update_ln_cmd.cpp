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

#include "update_ln_cmd.hpp"

namespace Cmd
{
    void UpdateLNode_Cmd::execute(Cmd::Interface::IEC61850_API::ptr t_api)
    {
        Core::ModelStateUpdater::ptr vals = t_api->state().getValsForLN(m_lnode);
        if (vals) {
            // Update process must to be finished in the GUI thread
            emit sigModelValues(vals);

            emit sigCmdEvent(CmdEvent::FinishEvent("IP?",
                    QString("Values of %1 were received").arg(m_lnode->getName()),
                    true));
        } else {
            emit sigCmdEvent(CmdEvent::FinishEvent("IP?",
                    QString("Can't get value of %1").arg(m_lnode->getName()),
                    false));
        }
    }
}