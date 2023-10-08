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

#include "backend_base.hpp"

void BackendBase::putCmdToQueue(Core::Cmd::ptrCMD t_cmd)
{
	connect(t_cmd.get(), SIGNAL(sigProgress(int,QString)), this, SLOT(slotCmdProcess(int,QString)));
	connect(t_cmd.get(), SIGNAL(sigFinished()), this, SLOT(slotCmdFinished()));

	m_con.m_cmdQueue->putCommand(t_cmd);
}

void BackendBase::slotCmdProcess(int t_proc, QString t_msg)
{
	emit sigProgress(t_proc, t_msg);
}

void BackendBase::slotCmdFinished()
{
	emit sigFinished();
}

void BackendBase::slotNewIED()
{
}