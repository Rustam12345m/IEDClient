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

#include "get_filelist_cmd.hpp"

namespace Core::Cmd
{
	void GetFileList::execute(LibInterface &t_con)
	{
		if (!t_con.isConnected()) {
			emit sigFinished();
			return;
		}

		emit sigProgress(0, "Send query to device: GetDirectory " + m_path);

		Core::DirOn dir(m_path);
		int retval = t_con.getFS_List(dir);
		if (retval == 0) {
		}

		emit sigProgress(80, "Save received information");

		m_fsTree.put(dir);

		emit sigFinished();
	}
}
