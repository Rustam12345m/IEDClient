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

#include "ied_presenter.hpp"

#include <QCoreApplication>

namespace App
{
	IED_Presenter::IED_Presenter(AppConContainer &t_con) : BackendBase(t_con)
	{
		m_ldsModel = new Models::LD_OverviewGrid(this, m_con.m_iedObj);
		m_ldPropModel = new Models::LD_PropertiesTable(this, m_con.m_iedObj);
		m_lnsModel = new Models::LN_OverviewTable(this, m_con.m_iedObj);
		m_lnStateModel = new Models::LN_SignalTable(this, m_con.m_iedObj);
		m_lnTreeModel = new Models::LN_SignalTree(this, m_con.m_iedObj);
		m_dsComModel = new Models::DS_OverviewTable(this, m_con.m_iedObj);
		m_dsSigModel = new Models::DS_SignalsTable(this, m_con.m_iedObj);
		m_rcbComModel = new Models::RCB_OverviewTable(this, m_con.m_iedObj);
		m_reportsModel = new Models::ReportsTable(this, m_con.m_iedObj);

		m_sortDOModel = new Models::SortProxyModel(this);
		m_sortDOModel->setSourceModel(m_lnStateModel);

		// Selection process LD -> LN -> DO
		connect(m_ldsModel, &Models::LD_OverviewGrid::sigLDSelected, m_ldPropModel, &Models::LD_PropertiesTable::slotLDSelected);		
		connect(m_ldsModel, &Models::LD_OverviewGrid::sigLDSelected, m_lnsModel, &Models::LN_OverviewTable::slotLDSelected);
		connect(m_lnsModel, &Models::LN_OverviewTable::sigLNSelected, m_lnStateModel, &Models::LN_SignalTable::slotLNSelected);
		connect(m_lnsModel, &Models::LN_OverviewTable::sigLNSelected, m_lnTreeModel, &Models::LN_SignalTree::slotLNSelected);

		connect(m_dsComModel, &Models::DS_OverviewTable::sigDSSelected, m_dsSigModel, &Models::DS_SignalsTable::slotDataSetSelected);
		connect(m_rcbComModel, &Models::RCB_OverviewTable::sigRCBSelected, m_reportsModel, &Models::ReportsTable::slotRCBSelected);
	}

	void IED_Presenter::updateLDs_Status()
	{
		qDebug() << "IED_Presenter: Update LDs";

		auto cmd = Core::Cmd::UpdateLDs_StatusCmd::create(m_con.m_iedObj);
		// connect(cmd.get(), &Core::Cmd::UpdateLDs_StatusCmd::sigFinished, m_ldsModel, &Models::LD_OverviewGrid::slotDataUpdated);
		putCmdToQueue(cmd);
	}

	void IED_Presenter::updateLNs_Status()
	{
		qDebug() << "IED_Presenter: Update LNs";

		auto cmd = Core::Cmd::UpdateLNs_StatusCmd::create(m_con.m_iedObj, m_lnsModel->getLogicalDevice());
		// connect(cmd.get(), &Core::Cmd::UpdateLNs_StatusCmd::sigFinished, m_lnsModel, &Models::LN_OverviewTable::slotDataUpdated);
		putCmdToQueue(cmd);
	}

	void IED_Presenter::updateRCBs_Status()
	{
		qDebug() << "IED_Presenter: Update RCBs";

		auto cmd = Core::Cmd::UpdateRCBs_Cmd::create(m_con.m_iedObj);
		// connect(cmd.get(), &Core::Cmd::UpdateRCBs_Cmd::sigFinished, m_rcbComModel, &Models::RCB_OverviewTable::slotDataUpdated);
		putCmdToQueue(cmd);
	}

	void IED_Presenter::updateLN_TreeValues()
	{
		qDebug() << "IED_Presenter: Update LN command";

		auto lnode = m_lnStateModel->getCurrectLN();
		auto cmd = Core::Cmd::UpdateLNode_Cmd::create(m_con.m_iedObj, lnode);
		connect(cmd.get(), &Core::Cmd::UpdateLNode_Cmd::sigNewValues, this, &IED_Presenter::slotUpdateItems, Qt::QueuedConnection);
		putCmdToQueue(cmd);
	}

	void IED_Presenter::updateDS_Values()
	{
		qDebug() << "IED_Presenter: Update DataSet";

		Core::ptrDataSet ds = m_dsSigModel->getDataSet();
		auto cmd = Core::Cmd::UpdateDataSet_Cmd::create(m_con.m_iedObj, ds);
		connect(cmd.get(), &Core::Cmd::UpdateDataSet_Cmd::sigNewValues, this, &IED_Presenter::slotUpdateItems, Qt::QueuedConnection);
		// connect(cmd.get(), &Core::Cmd::UpdateDataSet_Cmd::sigFinished, m_dsComModel, &Models::DS_OverviewTable::slotDataUpdated);
		putCmdToQueue(cmd);
	}

	QString IED_Presenter::ldsPageStatus()
	{
		return "IED: " + m_con.m_iedObj->model().getName();
	}

	QString IED_Presenter::lnsPageStatus()
	{
		auto ld = m_lnsModel->getLogicalDevice();
		if (ld) {
			return "LD: " + ld->getName();
		}
		return " - ";
	}

	QString IED_Presenter::dsPageStatus()
	{
		return "All found DataSets";
	}

	QString IED_Presenter::rcbPageStatus()
	{
		return "All found RCB";
	}

	void IED_Presenter::slotConnected(bool t_done)
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

	void IED_Presenter::slotUpdateItems(Core::ptrValuesUpdater t_vals)
	{
		if (t_vals) {
			t_vals->update();
		}
	}
}