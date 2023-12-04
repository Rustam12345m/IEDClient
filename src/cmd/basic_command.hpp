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

#include "lib_interface.hpp"

namespace Core::Cmd
{
	/*
	 * This is a basic class for all requests to IED through Lib61850_Adapter
	 * */
	class BasicCommand : public QObject
	{
		Q_OBJECT
	public:
		BasicCommand() = default;
		virtual ~BasicCommand() {}

		virtual void	execute(LibInterface &t_con) {
		}

	signals:
		void	sigProgress(int t_perc, QString t_msg);
		void	sigFinished(bool t_done);

		/*
		template<typename... Args>
		static QSharedPointer<ConnectCmd> create(Args&&... args) {
			return QSharedPointer<ConnectCmd>::create(std::forward<Args>(args)...);
		}
		*/
	};
	typedef QSharedPointer< BasicCommand >	ptrCMD;
}