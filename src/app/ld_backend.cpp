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
		m_ldModel = new LD_GridModel(this, m_con.m_ied);
		m_ldPropModel = new LD_PropertyModel(this, m_con.m_ied);
		m_lnModel = new LN_TableModel(this, m_con.m_ied);
		m_doModel = new DO_TableModel(this, m_con.m_ied);

		m_sortDOModel = new SortProxyModel(this);
		m_sortDOModel->setSourceModel(m_doModel);

		// Selection process LD -> LN -> DO
		connect(m_ldModel, &LD_GridModel::sigLDSelected, m_ldPropModel, &LD_PropertyModel::slotLDSelected);		
		connect(m_ldModel, &LD_GridModel::sigLDSelected, m_lnModel, &LN_TableModel::slotLDSelected);
		connect(m_lnModel, &LN_TableModel::sigLNSelected, m_doModel, &DO_TableModel::slotLNSelected);
	}

	void LD_Backend::updateDO_Table()
	{
		int ld = -1, ln = -1;
		m_doModel->getSelectedLN(ld, ln);
		if (ld < 0 || ln < 0) {
			return;
		}

		auto cmd = Core::Cmd::UpdateLNode::create(m_con.m_ied, ld, ln);
		connect(cmd.get(), &Core::Cmd::IED_BaseCommand::sigFinished,
				m_doModel, &DO_TableModel::slotDataUpdated);
		putCmdToQueue(cmd);
	}

	QString LD_Backend::getLD_TextStatus()
	{
		return "LD status text";
	}

	QString LD_Backend::getLN_TextStatus()
	{
		return "LN status text";
	}

	void LD_Backend::slotNewIED(bool t_done)
	{
		m_ldModel->setNewIED(m_con.m_ied);
		m_ldPropModel->setNewIED(m_con.m_ied);

		m_lnModel->setNewIED(m_con.m_ied);
		m_doModel->setNewIED(m_con.m_ied);
	}
}