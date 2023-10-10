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

#include "ld_backend.hpp"

namespace App
{
	LD_Backend::LD_Backend(ConnectionObject &t_con) : BackendBase(t_con)
	{
		m_ldModel = new LD_ListModel(this, m_con.m_ied);
		m_lnModel = new LN_TableModel(this, m_con.m_ied);
		m_doModel = new DO_TableModel(this, m_con.m_ied);

		m_sortDOModel = new SortProxyModel(this);
		m_sortDOModel->setSourceModel(m_doModel);
	}

	QString LD_Backend::getLN_TextStatus()
	{
		auto ln = m_con.m_ied->tree().getLogicalNode(m_currentLD, m_currentLN);
		if (ln) {
			return QString("%1 / %2").arg(ln->parentName()).arg(ln->name());
		}
		return "";
	}

	void LD_Backend::updateLNodeData(int t_ldIndex, int t_lnIndex)
	{
		if (t_ldIndex < 0 || t_lnIndex < 0) {
			return;
		}

		auto cmd = Core::Cmd::UpdateLNode::create(m_con.m_ied->tree(), t_ldIndex, t_lnIndex);
		putCmdToQueue(cmd);
	}

	void LD_Backend::slotNewIED()
	{
		m_ldModel->setNewIED(m_con.m_ied);
		m_lnModel->setNewIED(m_con.m_ied);
		m_doModel->setNewIED(m_con.m_ied);
	}
}