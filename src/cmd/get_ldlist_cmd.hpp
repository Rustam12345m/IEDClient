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
	 * This request gets information about Logical Devices from IED:
	 * 1. All LD with their working status: Mod, Beh, Health
	 * 2. All LN within each LD with their working status: Mod, Beh, Health
	 *
	 * */
	class UpdateLDList_Cmd : public IED_BaseCommand
	{
	public:
		UpdateLDList_Cmd() : IED_BaseCommand(IED_CMD::UPDATE_LD)
		{
		}
		~UpdateLDList_Cmd()
		{
		}

		void		execute(LibInterface &t_con) override;
	};
}
