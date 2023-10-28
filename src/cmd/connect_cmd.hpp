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

#include "base_command.hpp"
#include "core/ied_object.hpp"

namespace Core::Cmd
{
	/*
	 * This class realizes connecting to IED through pure TCP or TLS
	 * */
	class ConnectCmd : public IED_BaseCommand
	{
		Q_OBJECT
	public:
		ConnectCmd(const QString &t_ip, unsigned t_port, bool t_tls,
					const QString &t_user, const QString &t_pass,
					QSharedPointer<Core::IED_Object> t_ied)
				: IED_BaseCommand(IED_CMD::CONNECT),
				  m_ip(t_ip), m_port(t_port), m_tls(t_tls),
				  m_user(t_user), m_password(t_pass),
				  m_ied(t_ied)
		{
		}
		~ConnectCmd() {}

		void	execute(LibInterface &t_con) override;

		static QSharedPointer<ConnectCmd> create(const QString &t_ip, unsigned t_port, bool t_tls,
												const QString &t_user, const QString &t_pass,
												ptrIED_Object t_ied) {
			return QSharedPointer<ConnectCmd>::create(t_ip, t_port, t_tls, t_user, t_pass, t_ied);
		}

	private slots:
		void 	slotMsgProgress(const QString &t_msg);

	private:
		QString			m_ip;
		unsigned		m_port;
		bool			m_tls;
		QString			m_user, m_password;
		QSharedPointer<Core::IED_Object>	m_ied;
	};
}