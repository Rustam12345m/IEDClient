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

#include "get_filelist_cmd.hpp"

namespace Cmd
{
	void GetFileList::execute(Cmd::Interface::ptrIEC61850_API t_api)
	{
		if (!t_api->isConnected()) {
            emit sigFinishedEvent(Cmd::CmdEventInfo::FinishEvent("IP?", "Connection is closed", true));
			return;
		}

        emit sigProcessEvent(Cmd::CmdEventInfo::ProcessEvent("IP?", QString("Send query to IED: GetDirectory %1").arg(m_path), 50));

		Core::DirOn dir(m_path);
		int retval = t_api->fs().getFileList(dir);
		if (retval == 0) {
		}

		m_fsModel.put(dir);

        emit sigFinishedEvent(Cmd::CmdEventInfo::FinishEvent("IP?", "File list was received", true));
	}
}