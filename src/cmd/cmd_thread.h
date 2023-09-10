/*
 *  main.cpp
 *
 *  Copyright 2023 Rustam Mustafin
 *
 *  This file is part of IEDMaster.
 *
 *  IEDMaster is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  IEDMaster is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with IEDMaster.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  See COPYING file for the complete license text.
 * */

#pragma once

#include <QThread>

#include "command_queue.h"
#include "lib_interface.h"

// All known commands
#include "connect_cmd.h"
#include "get_filelist_cmd.h"
#include "update_lnode_cmd.h"

namespace Core::Cmd
{
	class CmdThread : public QThread
	{
		Q_OBJECT
	private:
		CommandQueue<ptrCMD>	m_queue;
		LibInterface&			m_con;

	public:
		CmdThread(LibInterface &t_lib);
		~CmdThread();

		void	putCommand(ptrCMD t_cmd);

	private:
		void	run();
	};
}
