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

#include <QObject>

#include "core/ied_object.hpp"
#include "cmd/cmd_thread.hpp"
#include "lib61850_adapter/lib61850.hpp"

/*
 * 
 * */
class IED_Connection : public QObject
{
	Q_OBJECT
public:
	IED_Connection() {
		newConnection();
	}

	void 	newConnection() {
		m_cmdQueue.clear();
		m_lib.clear();
		m_iedObj.clear();

		auto lib = QSharedPointer<Core::Lib::Lib61850_Adapter>::create();
		connect(lib.get(), &Core::Lib::Lib61850_Adapter::sigConClosed, this, &IED_Connection::slotConClosed);
		m_lib = lib;

		m_iedObj = QSharedPointer<Core::IED_Object>::create();
		m_cmdQueue = QSharedPointer<Core::Cmd::CmdThread>::create(m_lib);
	}

	bool 	isConnected() const {
		return m_lib->isConnected();
	}

public slots:
	void 	slotConClosed() {
		emit sigConnected(false);
	}

signals:
	void 	sigConnected(bool t_status);

public:
	QSharedPointer<Core::IED_Object>		m_iedObj;
	QSharedPointer<Core::Cmd::LibInterface>	m_lib;
	QSharedPointer<Core::Cmd::CmdThread>	m_cmdQueue;
};