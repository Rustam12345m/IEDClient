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
#include "core/fs_data.hpp"

namespace Core::Cmd
{
	class GetFileList : public IED_BaseCommand
	{
		Q_OBJECT
		QString			m_path;
		Core::FS_Data&	m_fsTree;

	public:
		GetFileList(Core::FS_Data &t_tree, const QString &t_path)
			: IED_BaseCommand(IED_CMD::GET_FILELIST),
			  m_fsTree(t_tree), m_path{t_path} {}
		~GetFileList() {}

		void		execute(LibInterface &t_con) override;

		static QSharedPointer<GetFileList> create(Core::FS_Data &t_tree, const QString &t_path) {
			return QSharedPointer<GetFileList>::create(t_tree, t_path);
		}
		/*
		template<typename... Args>
		static QSharedPointer<GetFileList> create(Args&&... args) {
			return QSharedPointer<GetFileList>::create(std::forward<Args>(args)...);
		}
		*/
	};
}