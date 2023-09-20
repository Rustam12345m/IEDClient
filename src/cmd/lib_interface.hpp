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

#include <QString>

#include "core/object_tree.hpp"
#include "core/fs_tree.hpp"

namespace Core::Cmd
{
	class LibInterface
	{
	public:
		virtual ~LibInterface() {}

		virtual bool	isConnected() const = 0;
		virtual void	printfVersion() const = 0;

		virtual bool	connect(const QString &t_ip, unsigned int t_port, bool t_checked,
								const QString &t_name, const QString &t_pass) = 0;
		virtual void	disconnect() = 0;

		virtual int		getLD_List(Core::ObjectTree &t_tree) = 0;
		virtual int		getDO_List(Core::ptrLN t_node) = 0;

		virtual int		getFS_List(Core::DirOn &t_dir) = 0;

		virtual int		updateDO_List(Core::ptrLN t_node) = 0;
	};
}
