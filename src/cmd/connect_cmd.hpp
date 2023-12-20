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

#include "basic_command.hpp"
#include "core/ied_object.hpp"
#include "cmd/con_credentials.hpp"

namespace Core::Cmd
{
	/*
	 * This class realizes connecting to IED through pure TCP or TLS
	 * */
	class ConnectCmd : public BasicCommand
	{
		Q_OBJECT
	public:
		ConnectCmd(const ConCredentials &t_cred, ptrIED_Object t_ied)
				: m_cred(t_cred), m_ied(t_ied) {}
		~ConnectCmd() {}

		void	execute(LibInterface &t_con) override;

		static auto create(const ConCredentials &t_cred, ptrIED_Object t_ied) {
			return QSharedPointer<ConnectCmd>::create(t_cred, t_ied);
		}

	private slots:
		void 	slotMsgProgress(const QString &t_msg);

	private:
		ConCredentials	m_cred;
		ptrIED_Object	m_ied;
	};
}