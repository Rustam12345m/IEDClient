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

#include "backend_base.hpp"
#include "app/app_settings.hpp"

#include "models/app_events_table.hpp"
#include "models/history_connections.hpp"

namespace App
{
	/*
	* Presenter of other small tables
	* */
	class ComBackend : public BackendBase
	{
		Q_OBJECT
	public:
		ComBackend(IED_Connection &t_con);
		~ComBackend() = default;

		Q_PROPERTY(QAbstractTableModel* eventsModel 	READ getEventsModel 	CONSTANT)
		Q_PROPERTY(QAbstractItemModel* 	lastConnList 	READ getLastConn_Model 	CONSTANT)

		Models::AppEventsTable*	getEventsModel() const { return m_eventsModel; }
		Models::HistConTable* 	getLastConn_Model() const { return m_lastConnModel; }

	protected:
		AppSettings				m_ini;

		Models::AppEventsTable*	m_eventsModel = nullptr;
		Models::HistConTable*	m_lastConnModel = nullptr;
	};
}