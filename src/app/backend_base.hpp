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

#pragma once

#include "app_con_container.hpp"

// All known commands
#include "cmd/all_cmd_header.hpp"

namespace App
{
	/*
	 * Interface for all Backends
	 * */
	class BackendBase : public QObject
	{
		Q_OBJECT

		BackendBase() = delete;
	public:
		BackendBase(AppConContainer &t_con) : m_con(t_con) {};
		virtual ~BackendBase() {}

	protected:
		void	putCmdToQueue(Core::Cmd::ptrCMD t_cmd);

	signals:
		void	sigCmdProgress(int t_perc, QString t_msg);
		void	sigCmdFinished(bool t_done);

	public slots:
		void			slotCmdProcess(int t_proc, QString t_msg);
		void			slotCmdFinished(bool t_done);
		virtual void 	slotConnected(bool t_done);

	protected:
		AppConContainer& 	m_con;
	};
}