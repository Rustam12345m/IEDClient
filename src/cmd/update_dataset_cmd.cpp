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

#include "update_dataset_cmd.hpp"

namespace Cmd
{
    void UpdateDataSet_Cmd::execute(Cmd::Interface::IEC61850_API::ptr t_api)
    {
        Core::ModelStateUpdater::ptr vals = t_api->state().getValsForDS(m_dataset);
        if (vals) {
            emit sigModelValues(vals);

            emit sigCmdEvent(Cmd::CmdEvent::FinishEvent("", "DataSet values updated", true));
        } else {
            emit sigCmdEvent(Cmd::CmdEvent::FinishEvent("", "Cannot get information about this dataset", false));
        }
    }
}