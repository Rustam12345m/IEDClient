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

#include "backend_base.hpp"

namespace App
{
	void BackendBase::putCmdToQueue(Core::Cmd::ptrCMD t_cmd)
	{
		connect(t_cmd.get(), &Core::Cmd::BasicCommand::sigProgress, this, &BackendBase::slotCmdProcess);
		connect(t_cmd.get(), &Core::Cmd::BasicCommand::sigFinished, this, &BackendBase::slotCmdFinished);

		m_con.m_cmdQueue->putCommand(t_cmd);
	}

	void BackendBase::slotCmdProcess(int t_proc, QString t_msg)
	{
		emit sigCmdProgress(t_proc, t_msg);
	}

	void BackendBase::slotCmdFinished(bool t_done)
	{
		emit sigCmdFinished(t_done);
	}

	void BackendBase::slotConnected(bool t_done)
	{
	}
}