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
#include "cmd/con_credentials.hpp"
#include "lib61850_adapter/lib61850.hpp"

namespace App
{
	/*
	 * 
	 * */
	class AppConContainer : public QObject
	{
		Q_OBJECT
	public:
		AppConContainer() {
			newConnection(Core::Cmd::ConCredentials());
		}

		void 	newConnection(const Core::Cmd::ConCredentials &t_cred) {
			m_cmdQueue.clear();
			m_lib.clear();
			m_iedObj.clear();

			m_cred = t_cred;

			auto lib = QSharedPointer<Core::Lib::Lib61850_Adapter>::create();
			connect(lib.get(), &Core::Lib::Lib61850_Adapter::sigConClosed, this, &AppConContainer::slotDisconnected);
			m_lib = lib;

			m_iedObj = QSharedPointer<Core::IED_Object>::create();
			m_cmdQueue = QSharedPointer<Core::Cmd::CmdThread>::create(m_lib);
		}

		bool 	isConnected() const {
			return m_lib->isConnected();
		}

	public slots:
		void 	slotDisconnected() {
			emit sigConnected(false);
		}

	signals:
		void 	sigConnected(bool t_status);

	public:
		Core::Cmd::ConCredentials				m_cred; // Information about ip/port/etc
		QSharedPointer<Core::IED_Object>		m_iedObj;
		QSharedPointer<Core::Cmd::LibInterface>	m_lib;
		QSharedPointer<Core::Cmd::CmdThread>	m_cmdQueue;
	};
}