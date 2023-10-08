/*
 *  main.cpp
 *
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

#include "cmd_thread.hpp"

namespace Core::Cmd
{
	CmdThread::CmdThread(QSharedPointer<LibInterface> &t_con) : m_con(t_con)
	{
		setObjectName("CmdThread");
		start();
	}

	CmdThread::~CmdThread()
	{
		m_queue.stop();

		if (isRunning()) {
			wait();
		}
	}

	void CmdThread::putCommand(ptrCMD t_cmd)
	{
		m_queue.push(t_cmd);
	}

	void CmdThread::run()
	{
		while (m_queue.isRunning()) {
			ptrCMD cmd = m_queue.pop();
			if (cmd) {
				cmd->execute(*m_con);
			}
		}
	}
}