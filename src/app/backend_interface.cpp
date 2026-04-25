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

#include <QDebug>

namespace App
{
    void BackendInterface::putCmdToQueue(Cmd::CmdInterface::ptr cmd)
    {
        connect(cmd.get(), &Cmd::CmdInterface::sigCmdEvent, this, &BackendInterface::slotCmdEvent);

        m_con.m_cmdThread->putCommand(cmd);
    }

    void BackendInterface::slotCmdEvent(Cmd::CmdEvent ev)
    {
        if (ev.m_ip.isEmpty() || ev.m_ip == "IP?") {
            ev.m_ip = m_con.m_cred.ip();
        }

        m_events.putEventToStorage(ev);

        switch (ev.m_type) {
        case Cmd::PROCESS_EVENT: {
            emit sigCmdProgress(ev.m_perc, ev.m_msg);
            break;
        }
        case Cmd::FINISH_EVENT: {
            emit sigCmdFinished(ev.m_result);
            if (!ev.m_result) {
                qWarning().noquote()
                    << "Command failed:" << ev.m_msg
                    << "(ip:" << ev.m_ip << ")";
                emit sigCmdError(ev.m_msg);
            }
            break;
        }
        case Cmd::START_EVENT:
        case Cmd::UNDEFINED_EVENT: {
            break;
        }
        }
    }

    void BackendInterface::slotConnected(bool done)
    {
    }
}