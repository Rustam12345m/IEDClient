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
	DevBackend::DevBackend(AppConContainer &t_con) : BackendBase(t_con)
	{
		m_ldsModel = new Models::LD_CommonGrid(this, m_con.m_iedObj);
		m_ldPropModel = new Models::LD_PropTable(this, m_con.m_iedObj);
		m_lnsModel = new Models::LN_CommonTable(this, m_con.m_iedObj);
		m_lnStateModel = new Models::LN_StateTable(this, m_con.m_iedObj);
		m_lnTreeModel = new Models::LN_SignalTree(this, m_con.m_iedObj);
		m_dsComModel = new Models::DS_CommonLModel(this, m_con.m_iedObj);
		m_dsSigModel = new Models::DS_SignalsTable(this, m_con.m_iedObj);
		m_rcbComModel = new Models::RCB_CommonTable(this, m_con.m_iedObj);
		m_reportsModel = new Models::ReportsTable(this, m_con.m_iedObj);

		m_sortDOModel = new Models::SortProxyModel(this);
		m_sortDOModel->setSourceModel(m_lnStateModel);

		// Selection process LD -> LN -> DO
		connect(m_ldsModel, &Models::LD_CommonGrid::sigLDSelected, m_ldPropModel, &Models::LD_PropTable::slotLDSelected);		
		connect(m_ldsModel, &Models::LD_CommonGrid::sigLDSelected, m_lnsModel, &Models::LN_CommonTable::slotLDSelected);
		connect(m_lnsModel, &Models::LN_CommonTable::sigLNSelected, m_lnStateModel, &Models::LN_StateTable::slotLNSelected);
		connect(m_lnsModel, &Models::LN_CommonTable::sigLNSelected, m_lnTreeModel, &Models::LN_SignalTree::slotLNSelected);

		connect(m_dsComModel, &Models::DS_CommonLModel::sigDSSelected, m_dsSigModel, &Models::DS_SignalsTable::slotDSSelected);
		connect(m_rcbComModel, &Models::RCB_CommonTable::sigRCBSelected, m_reportsModel, &Models::ReportsTable::slotRCBSelected);
	}

	void DevBackend::updateLDs_Status()
	{
		qDebug() << "DevBackend: Update LDs";

		auto cmd = Core::Cmd::UpdateLDs_StatusCmd::create(m_con.m_iedObj);
		connect(cmd.get(), &Core::Cmd::UpdateLDs_StatusCmd::sigFinished,
				m_ldsModel, &Models::LD_CommonGrid::slotDataUpdated);
		putCmdToQueue(cmd);
	}

	void DevBackend::updateLNs_Status()
	{
		qDebug() << "DevBackend: Update LNs";

		auto cmd = Core::Cmd::UpdateLNs_StatusCmd::create(m_con.m_iedObj, m_lnsModel->getCurrentLD());
		connect(cmd.get(), &Core::Cmd::UpdateLNs_StatusCmd::sigFinished,
				m_lnsModel, &Models::LN_CommonTable::slotDataUpdated);
		putCmdToQueue(cmd);
	}

	void DevBackend::updateRCBs_Status()
	{
		qDebug() << "DevBackend: Update RCBs";

		auto cmd = Core::Cmd::UpdateRCBs_Cmd::create(m_con.m_iedObj);
		connect(cmd.get(), &Core::Cmd::UpdateRCBs_Cmd::sigFinished,
				m_rcbComModel, &Models::RCB_CommonTable::slotDataUpdated);
		putCmdToQueue(cmd);
	}

	void DevBackend::updateLN_TreeValues()
	{
		qDebug() << "DevBackend: Update LN";

		int ld = -1, ln = -1;
		m_lnStateModel->getSelectedLN(ld, ln);
		if (ld < 0 || ln < 0) {
			return;
		}

		auto cmd = Core::Cmd::UpdateLNode_Cmd::create(m_con.m_iedObj, ld, ln);
		connect(cmd.get(), &Core::Cmd::BasicCommand::sigFinished, m_lnStateModel, &Models::LN_StateTable::slotDataUpdated);
		connect(cmd.get(), &Core::Cmd::BasicCommand::sigFinished, m_lnTreeModel, &Models::LN_SignalTree::slotDataUpdated);
		putCmdToQueue(cmd);
	}

	void DevBackend::updateDS_Values()
	{
		qDebug() << "DevBackend: Update DataSet";

		auto cmd = Core::Cmd::UpdateDataSet_Cmd::create(m_con.m_iedObj, m_dsComModel->getCurrentDS());
		connect(cmd.get(), &Core::Cmd::UpdateDataSet_Cmd::sigFinished,
				m_dsComModel, &Models::DS_CommonLModel::slotDataUpdated);
		putCmdToQueue(cmd);
	}

	QString DevBackend::ldsPageStatus()
	{
		return "IED: " + m_con.m_iedObj->model().name();
	}

	QString DevBackend::lnsPageStatus()
	{
		auto ld = m_con.m_iedObj->model().getLogicalDevice(m_lnsModel->getCurrentLD());
		if (ld) {
			return "LD: " + ld->name();
		}
		return " - ";
	}

	QString DevBackend::dsPageStatus()
	{
		return "All found DataSets";
	}

	QString DevBackend::rcbPageStatus()
	{
		return "All found RCB";
	}

	void DevBackend::slotConnected(bool t_done)
	{
		m_ldsModel->setNewIED(m_con.m_iedObj);
		m_ldPropModel->setNewIED(m_con.m_iedObj);
		m_lnsModel->setNewIED(m_con.m_iedObj);
		m_lnStateModel->setNewIED(m_con.m_iedObj);
		m_lnTreeModel->setNewIED(m_con.m_iedObj);
		m_dsComModel->setNewIED(m_con.m_iedObj);
		m_dsSigModel->setNewIED(m_con.m_iedObj);
		m_rcbComModel->setNewIED(m_con.m_iedObj);
		m_reportsModel->setNewIED(m_con.m_iedObj);
	}
}