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

namespace Core::Cmd
{
	void UpdateDataSet_Cmd::execute(LibInterface &t_con)
	{
		/*
		ptrValuesUpdater vals = t_con.getValuesForLN(m_lnode);
		if (vals) {
			connect(this, &Core::Cmd::UpdateLNode_Cmd::sigNewValues,
					m_lnode.get(), &Core::LogicalNode::slotUpdateValues,
					Qt::QueuedConnection); // Qt::BlockingQueuedConnection

			// Update process must to be finished in the GUI thread
			emit sigNewValues(vals);

			emit sigFinished(true);
		} else {
			emit sigFinished(false);
		}
		*/
	}
}