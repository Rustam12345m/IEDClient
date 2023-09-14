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

#include <QObject>

#include "cmd/cmd_thread.h"
#include "cmd/lib61850_adapter.h"
#include "core/ied_tree.h"
#include "core/fs_tree.h"

/*
 * AppCore -
 * */
class AppCore : public QObject
{
	Q_OBJECT
protected:
	Core::Cmd::Lib61850		m_con;
	Core::Cmd::CmdThread	m_cmdThread;

	Core::IED_Tree		m_iedTree;
	Core::FS_Tree		m_fsTree;

public:
	Core::IED_Tree&		getIEDTree() {
		return m_iedTree;
	}
	Core::FS_Tree&		getFSTree() {
		return m_fsTree;
	}

public:
	AppCore(QObject *t_parent=nullptr);
	~AppCore();

	void	putCommand(Core::Cmd::ptrCMD t_cmd);

signals:
	void	mySignal(const QString &t_msg);
};
