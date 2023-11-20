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

#include <QSettings>

namespace App
{
	class DevConInfo
	{
	public:
		DevConInfo(const QString &t_name, const QString &t_ip, int t_port)
			: m_name(t_name), m_ip(t_ip), m_port(t_port)
		{
		}

		QString 	name() const { return m_name; }
		QString 	ip() const { return m_ip; }
		int 		port() const { return m_port; }

		bool operator==(const DevConInfo &t_right) {
			return (m_name == t_right.name()) && (m_ip == t_right.ip()) && (m_port == t_right.port());
		}

	protected:
		QString 	m_name;
		QString 	m_ip;
		int 		m_port = 102;
	};

	/**
	 * @brief AppSettings is a class which get/set IEDClient's settings
	 */
	class AppSettings
	{
	public:
		AppSettings() = default;
		~AppSettings() = default;

		QList<DevConInfo>	getDevConList();
		void 				saveNewDevCon(const DevConInfo &t_dev);

		const int SaveDevsHistoryLen = 10;
	};
}