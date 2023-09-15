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

#include "base_command.h"

namespace Core::Cmd
{
	/*
	 * This class realizes connecting to IED through pure TCP or TLS
	 * */
	class ConnectCmd : public IED_BaseCommand
	{
		Q_OBJECT

		QString			m_ip;
		unsigned		m_port;
		bool			m_tls;
		QString			m_user;
		QString			m_password;
		Core::ObjectTree&	m_tree;

	public:
		ConnectCmd(const QString &t_ip, unsigned t_port, bool t_tls,
					const QString &t_user, const QString &t_pass,
					ObjectTree &t_tree)
				: IED_BaseCommand(IED_CMD::CONNECT),
				  m_ip(t_ip), m_port(t_port), m_tls(t_tls),
				  m_user(t_user), m_password(t_pass),
				  m_tree(t_tree)
		{
		}
		~ConnectCmd() {}

		void	execute(LibInterface &t_con) override;

		// Create new command like Builder pattern
		static QSharedPointer<ConnectCmd> create(const QString &t_ip, unsigned t_port, bool t_tls,
												const QString &t_user, const QString &t_pass,
												ObjectTree &t_tree) {
			return QSharedPointer<ConnectCmd>::create(t_ip, t_port, t_tls, t_user, t_pass, t_tree);
		}
	};
}
