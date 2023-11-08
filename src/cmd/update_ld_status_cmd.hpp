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
	 * This class ...
	 * */
	class UpdateLD_StatusCmd : public IED_BaseCommand
	{
		Q_OBJECT
	public:
		UpdateLD_StatusCmd(int t_inx, QSharedPointer<Core::IED_Object> t_ied)
				: IED_BaseCommand(IED_CMD::UNDEFINED), m_inx(t_inx), m_ied(t_ied)
		{
		}
		~UpdateLD_StatusCmd() override {}

		void	execute(LibInterface &t_con) override;

		static QSharedPointer<UpdateLD_StatusCmd> create(int t_inx, ptrIED_Object t_ied) {
			return QSharedPointer<UpdateLD_StatusCmd>::create(t_inx, t_ied);
		}

	private slots:
		void 	slotMsgProgress(const QString &t_msg);

	private:
		int 								m_inx = -1;
		QSharedPointer<Core::IED_Object>	m_ied;
	};
}