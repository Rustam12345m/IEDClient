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

#include <QObject>

#include <memory>

#include "lib_interface.h"

namespace Core::Cmd
{
	enum class IED_CMD
	{
		UNDEFINED = 0,
		CONNECT,
		UPDATE_LD,
		UPDATE_LN,
		UPDATE_DIR,
		GET_FILELIST,
		GET_FILE,
		REMOVE_FILE
	};

	/*
	 * This is a basic class for all requests to IED through Lib61850_Adapter
	 * */
	class IED_BaseCommand : public QObject
	{
		Q_OBJECT
	protected:
		IED_CMD		m_type = IED_CMD::UNDEFINED;

	public:
		IED_BaseCommand() = delete;
		IED_BaseCommand(IED_CMD t_type) : m_type(t_type) {}
		virtual ~IED_BaseCommand() {}

		virtual void	execute(LibInterface &t_con) {
		}

	signals:
		void	sigProgress(int t_perc, QString t_msg);
		void	sigFinished();

		/*
		template<typename... Args>
		static QSharedPointer<ConnectCmd> create(Args&&... args) {
			return QSharedPointer<ConnectCmd>::create(std::forward<Args>(args)...);
		}
		*/
	};
	typedef QSharedPointer< IED_BaseCommand >	ptrCMD;
}
