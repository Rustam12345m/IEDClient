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

#include "dev_backend.hpp"

namespace App
{
	DevBackend::DevBackend(IED_Connection &t_con) : BackendBase(t_con)
	{
		m_ldModel = new Models::LDs_Grid(this, m_con.m_iedObj);
		m_ldPropModel = new Models::LD_PropTable(this, m_con.m_iedObj);
		m_lnsModel = new Models::LNs_Table(this, m_con.m_iedObj);
		m_doModel = new Models::LN_StateTable(this, m_con.m_iedObj);
		m_lnTreeModel = new Models::LN_SignalsTree(this, m_con.m_iedObj);
		m_dsInfoModel = new Models::DataSetsTable(this, m_con.m_iedObj);
		m_dsModel = new Models::DS_SignalsTable(this, m_con.m_iedObj);
		m_rcbMainModel = new Models::RCB_GeneralTable(this, m_con.m_iedObj);
		m_reportsModel = new Models::ReportsTable(this, m_con.m_iedObj);

		m_sortDOModel = new Models::SortProxyModel(this);
		m_sortDOModel->setSourceModel(m_doModel);

		// Selection process LD -> LN -> DO
		connect(m_ldModel, &Models::LDs_Grid::sigLDSelected, m_ldPropModel, &Models::LD_PropTable::slotLDSelected);		
		connect(m_ldModel, &Models::LDs_Grid::sigLDSelected, m_lnsModel, &Models::LNs_Table::slotLDSelected);
		connect(m_lnsModel, &Models::LNs_Table::sigLNSelected, m_doModel, &Models::LN_StateTable::slotLNSelected);
		connect(m_lnsModel, &Models::LNs_Table::sigLNSelected, m_lnTreeModel, &Models::LN_SignalsTree::slotLNSelected);
	}

	void DevBackend::updateLDs_Status()
	{
		qDebug() << "DevBackend: Update LDs";
	}

	void DevBackend::updateDO_Table()
	{
		int ld = -1, ln = -1;
		m_doModel->getSelectedLN(ld, ln);
		if (ld < 0 || ln < 0) {
			return;
		}

		auto cmd = Core::Cmd::UpdateLNode::create(m_con.m_iedObj, ld, ln);
		connect(cmd.get(), &Core::Cmd::IED_BaseCommand::sigFinished,
				m_doModel, &Models::LN_StateTable::slotDataUpdated);
		putCmdToQueue(cmd);
	}

	QString DevBackend::getLD_TextStatus()
	{
		return m_con.m_iedObj->model().name();
	}

	QString DevBackend::getLN_TextStatus()
	{
		auto ld = m_con.m_iedObj->model().getLogicalDevice(m_lnsModel->getCurrentLD());
		if (ld) {
			return ld->name();
		}
		return "";
	}

	void DevBackend::slotConnected(bool t_done)
	{
		m_ldModel->setNewIED(m_con.m_iedObj);
		m_ldPropModel->setNewIED(m_con.m_iedObj);
		m_lnsModel->setNewIED(m_con.m_iedObj);
		m_doModel->setNewIED(m_con.m_iedObj);
		m_lnTreeModel->setNewIED(m_con.m_iedObj);
		m_dsInfoModel->setNewIED(m_con.m_iedObj);
		m_dsModel->setNewIED(m_con.m_iedObj);
		m_rcbMainModel->setNewIED(m_con.m_iedObj);
		m_reportsModel->setNewIED(m_con.m_iedObj);
	}
}