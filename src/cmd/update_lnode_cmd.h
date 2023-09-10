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
#include "ied_tree.h"

namespace Core::Cmd
{
	class UpdateLNodeCmd : public IED_BaseCommand
	{
		Core::IED_Tree&	m_tree;
		int				m_ldIndex = 0;
		int				m_lnIndex = 0;

	public:
		UpdateLNodeCmd(Core::IED_Tree &t_tree, int t_ldIndex, int t_lnIndex)
			: IED_BaseCommand(IED_CMD::UPDATE_LN),
			  m_tree{t_tree}, m_ldIndex{t_ldIndex}, m_lnIndex{t_lnIndex}
		{
		}

		void	execute(LibInterface &t_con) override;
	};
}
