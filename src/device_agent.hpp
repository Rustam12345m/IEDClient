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

#pragma once

#include <QObject>

#include "cmd/cmd_thread.hpp"
#include "cmd/lib61850_adapter.hpp"
#include "core/ied_tree.hpp"
#include "core/fs_tree.hpp"

class DeviceAgent : public QObject
{
	Q_OBJECT
public:
	DeviceAgent(QObject *t_parent=nullptr);
	~DeviceAgent();

	Core::IED_Tree&		getObjectTree() {
		return m_objTree;
	}
	Core::FS_Tree&		getFileTree() {
		return m_fsTree;
	}

	void		putCommand(Core::Cmd::ptrCMD t_cmd);

signals:
	void		mySignal(const QString &t_msg);

protected:
	Core::IED_Tree			m_objTree;
	Core::FS_Tree			m_fsTree;

	Core::Cmd::Lib61850		m_con;
	Core::Cmd::CmdThread	m_cmdThread;
};