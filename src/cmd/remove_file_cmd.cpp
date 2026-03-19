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

#include "remove_file_cmd.hpp"

namespace Cmd
{
    void RemoveFileCMD::execute(Cmd::Interface::IEC61850_API::ptr t_api)
    {
        int rc = t_api->fs().remove(m_filename);

        if (rc == 0) {
            emit sigFileRemoved(m_fileIndex);
            emit sigCmdEvent(CmdEvent::FinishEvent("",
                    QString("File deleted: %1").arg(m_filename), true));
        } else {
            emit sigCmdEvent(CmdEvent::FinishEvent("",
                    QString("Failed to delete: %1").arg(m_filename), false));
        }
    }
}