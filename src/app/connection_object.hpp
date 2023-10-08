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

#include "core/ied_object.hpp"
#include "cmd/cmd_thread.hpp"
#include "cmd/lib61850_adapter.hpp"

class ConnectionObject
{
public:
	ConnectionObject() {
		reset();
	}

	void 	reset() {
		m_cmdQueue.clear();
		m_lib.clear();
		m_ied.clear();

		m_ied = QSharedPointer<Core::IED_Object>::create();
		m_lib = QSharedPointer<Core::Cmd::Lib61850>::create();
		m_cmdQueue = QSharedPointer<Core::Cmd::CmdThread>::create(m_lib);
	}

	QSharedPointer<Core::IED_Object>		m_ied;
	QSharedPointer<Core::Cmd::LibInterface>	m_lib;
	QSharedPointer<Core::Cmd::CmdThread>	m_cmdQueue;
};