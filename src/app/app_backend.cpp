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

#include "app_backend.hpp"

namespace App
{
	AppBackend::AppBackend(AppConContainer &t_con) : BackendBase(t_con)
	{
		m_eventsModel = new Models::AppEventsTable(this);
		m_lastConnModel = new Models::HistConTable(this, m_settings);
	}

	void AppBackend::newConnection(const Core::Cmd::ConCredentials &t_cred)
	{
		m_settings.putConnectionToConfig(t_cred, m_con.m_iedObj->model().name());
	}
}