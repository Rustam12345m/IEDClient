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

#include "backend_interface.hpp"

namespace App
{
    void BackendInterface::putCmdToQueue(Cmd::CmdInterface::ptr t_cmd)
    {
        connect(t_cmd.get(), &Cmd::CmdInterface::sigCmdEvent, this, &BackendInterface::slotCmdEvent);

        m_con.m_cmdThread->putCommand(t_cmd);
    }

    void BackendInterface::slotCmdEvent(Cmd::CmdEvent t_ev)
    {
        m_events.putEventToStorage(t_ev);

        switch (t_ev.m_type) {
        case Cmd::PROCESS_EVENT: {
            emit sigCmdProgress(t_ev.m_perc, t_ev.m_msg);
            break;
        }
        case Cmd::FINISH_EVENT: {
            emit sigCmdFinished(t_ev.m_result);
            break;
        }
        case Cmd::START_EVENT:
        case Cmd::UNDEFINED_EVENT: {
            break;
        }
        }
    }

    void BackendInterface::slotConnected(bool t_done)
    {
    }
}