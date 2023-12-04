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

namespace Core::Cmd
{
	/*
	 * This class realizes downloading file from the IED
	 * */
	class UpdateDataSet_Cmd : public BasicCommand
	{
		Q_OBJECT
	public:
		UpdateDataSet_Cmd(ptrIED_Object t_ied, int t_dsInx)
			: m_ied{t_ied}, m_dsIndex{t_dsInx}
		{
		}
		~UpdateDataSet_Cmd() = default;

		void	execute(LibInterface &t_con) override;

		static auto create(ptrIED_Object t_ied, int t_dsInx) {
			return QSharedPointer<UpdateDataSet_Cmd>::create(t_ied, t_dsInx);
		}

	private:
		ptrIED_Object	m_ied;
		int 			m_dsIndex = -1;
	};
}