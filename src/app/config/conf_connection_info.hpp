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

#include <QString>

namespace App
{
	class ConfConnectionInfo
	{
	public:
		ConfConnectionInfo() = default;
		ConfConnectionInfo(const QString &t_ip, int t_port, bool t_tls, const QString &t_name, const QString &t_date)
			: m_ip(t_ip), m_port(t_port), m_tls(t_tls), m_name(t_name), m_date(t_date) {}

		const auto 	ip() const { return m_ip; }
		const auto 	port() const { return m_port; }
		const auto	tls() const { return m_tls; }
		const auto 	ied() const { return m_name; }
		const auto 	date() const { return m_date; }

		bool operator==(const ConfConnectionInfo &t_right) {
			return (m_ip == t_right.m_ip)
					&& (m_port == t_right.m_port)
					&& (m_tls == t_right.m_tls)
					/* && (m_name == t_right.m_name) */
					/* && (m_date == t_right.m_date) */;
		}

	public:
		QString 	m_ip;
		int			m_port = 0;
		bool 		m_tls = false;
		QString 	m_name;
		QString 	m_date;
	};
}