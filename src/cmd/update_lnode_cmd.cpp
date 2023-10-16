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

#include "update_lnode_cmd.hpp"
#include <QDebug>

namespace Core::Cmd
{
	void UpdateLNode::execute(LibInterface &t_con)
	{
		qDebug() << "UpdateLNode: ldInx = " << m_ldIndex << ", lnInx " << m_lnIndex;

		if (t_con.isConnected()) {
			auto ln = m_ied->model().getLogicalNode(m_ldIndex, m_lnIndex);
			if (ln) {
				t_con.updateLN_PinValues(ln);
			}
		}
		emit sigFinished();
	}
}