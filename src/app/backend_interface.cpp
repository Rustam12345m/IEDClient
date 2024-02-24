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
	void BackendInterface::putCmdToQueue(Cmd::ptrCMD t_cmd)
	{
		connect(t_cmd.get(), &Cmd::CmdInterface::sigProcessEvent, this, &BackendInterface::slotCmdProcess);
		connect(t_cmd.get(), &Cmd::CmdInterface::sigFinishedEvent, this, &BackendInterface::slotCmdFinished);

		m_con.m_cmdThread->putCommand(t_cmd);
	}

	void BackendInterface::slotCmdStart(Cmd::CmdEventInfo t_ev)
	{
        m_events.slotEventToLog(t_ev);
	}

	void BackendInterface::slotCmdProcess(Cmd::CmdEventInfo t_ev)
	{
        m_events.slotEventToLog(t_ev);

		emit sigCmdProgress(t_ev.m_perc, t_ev.m_msg);
	}

	void BackendInterface::slotCmdFinished(Cmd::CmdEventInfo t_ev)
	{
        m_events.slotEventToLog(t_ev);

		emit sigCmdFinished(t_ev.m_result);
	}

	void BackendInterface::slotConnected(bool t_done)
	{
	}
}