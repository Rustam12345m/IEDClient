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

namespace Core::Cmd
{
	/*
	 * This class realizes downloading file from the IED
	 * */
	class DownloadFile : public IED_BaseCommand
	{
		Q_OBJECT
	public:
		DownloadFile(const QString &t_name)
			: IED_BaseCommand(IED_CMD::UNDEFINED)
		{
		}
		~DownloadFile() {}

		void	execute(LibInterface &t_con) override;

		// Create new command like Builder pattern
		static QSharedPointer<DownloadFile> create(const QString &t_name) {
			return QSharedPointer<DownloadFile>::create(t_name);
		}
	};
}