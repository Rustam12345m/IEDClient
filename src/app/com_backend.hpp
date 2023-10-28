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

#include "backend_base.hpp"
#include "app/app_settings.hpp"

#include "models/app_events_model.hpp"
#include "models/last_conn_list.hpp"

namespace App
{
	/*
	* Presenter of other small tables
	* */
	class ComBackend : public BackendBase
	{
		Q_OBJECT
	public:
		ComBackend(ConnectionObject &t_con);
		~ComBackend() = default;

		Q_PROPERTY(AppEventsModel* 		eventsModel 	READ getEventsModel 	CONSTANT)
		Q_PROPERTY(QAbstractItemModel* 	lastConnList 	READ getLastConn_Model 	CONSTANT)

		AppEventsModel*			getEventsModel() const { return m_eventsModel; }
		LastConn_TableModel* 	getLastConn_Model() const { return m_lastConnModel; }

		void 		slotNewIED(bool t_done) override;

	protected:
		AppSettings				m_ini;

		AppEventsModel*			m_eventsModel = nullptr;
		LastConn_TableModel*	m_lastConnModel = nullptr;
	};
}