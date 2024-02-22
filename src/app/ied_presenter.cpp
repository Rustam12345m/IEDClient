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
	IEDBackend::IEDBackend(IEDConContainer &t_con) : BackendBase(t_con)
	{
		m_ldsModel = new Models::LD_OverviewGrid(this, m_con.m_ied);
		m_ldPropModel = new Models::LD_PropertiesTable(this, m_con.m_ied);
		m_lnsModel = new Models::LN_OverviewTable(this, m_con.m_ied);
		m_lnStateModel = new Models::LN_SignalTable(this, m_con.m_ied);
		m_lnTreeModel = new Models::LN_SignalTree(this, m_con.m_ied);
		m_dsComModel = new Models::DS_OverviewTable(this, m_con.m_ied);
		m_dsSigModel = new Models::DS_SignalsTable(this, m_con.m_ied);
		m_rcbComModel = new Models::RCB_OverviewTable(this, m_con.m_ied);
		m_reportsModel = new Models::ReportsTable(this, m_con.m_ied);

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

	void IEDBackend::updateLDs_Status()
	{
		qDebug() << "IEDBackend: Update LDs";

		auto cmd = Cmd::UpdateLDs_StatusCmd::create(m_con.m_ied);
		// connect(cmd.get(), &Cmd::UpdateLDs_StatusCmd::sigFinished, m_ldsModel, &Models::LD_OverviewGrid::slotDataUpdated);
		putCmdToQueue(cmd);
	}

	void IEDBackend::updateLNs_Status()
	{
		qDebug() << "IEDBackend: Update LNs";

		auto cmd = Cmd::UpdateLNs_StatusCmd::create(m_con.m_ied, m_lnsModel->getLogicalDevice());
		// connect(cmd.get(), &Cmd::UpdateLNs_StatusCmd::sigFinished, m_lnsModel, &Models::LN_OverviewTable::slotDataUpdated);
		putCmdToQueue(cmd);
	}

	void IEDBackend::updateRCBs_Status()
	{
		qDebug() << "IEDBackend: Update RCBs";

		auto cmd = Cmd::UpdateRCBs_Cmd::create(m_con.m_ied);
		// connect(cmd.get(), &Cmd::UpdateRCBs_Cmd::sigFinished, m_rcbComModel, &Models::RCB_OverviewTable::slotDataUpdated);
		putCmdToQueue(cmd);
	}

	void IEDBackend::updateLN_TreeValues()
	{
		qDebug() << "IEDBackend: Update LN command";

		auto lnode = m_lnStateModel->getCurrectLN();
		auto cmd = Cmd::UpdateLNode_Cmd::create(m_con.m_ied, lnode);
		connect(cmd.get(), &Cmd::UpdateLNode_Cmd::sigNewValues,
                this, &IEDBackend::slotUpdateItems,
                Qt::QueuedConnection);
		putCmdToQueue(cmd);
	}

	void IEDBackend::updateDS_Values()
	{
		qDebug() << "IEDBackend: Update DataSet";

		Core::ptrDataSet ds = m_dsSigModel->getDataSet();
		auto cmd = Cmd::UpdateDataSet_Cmd::create(m_con.m_ied, ds);
		connect(cmd.get(), &Cmd::UpdateDataSet_Cmd::sigNewValues, this, &IEDBackend::slotUpdateItems, Qt::QueuedConnection);
		// connect(cmd.get(), &Cmd::UpdateDataSet_Cmd::sigFinished, m_dsComModel, &Models::DS_OverviewTable::slotDataUpdated);
		putCmdToQueue(cmd);
	}

	QString IEDBackend::ldsPageStatus()
	{
		return "IED: " + m_con.m_ied->model().getName();
	}

	QString IEDBackend::lnsPageStatus()
	{
		auto ld = m_lnsModel->getLogicalDevice();
		if (ld) {
			return "LD: " + ld->getName();
		}
		return " - ";
	}

	QString IEDBackend::dsPageStatus()
	{
		return "All found DataSets";
	}

	QString IEDBackend::rcbPageStatus()
	{
		return "All found RCB";
	}

	void IEDBackend::slotConnected(bool t_done)
	{
		m_ldsModel->setActiveIED(m_con.m_ied);
		m_ldPropModel->setActiveIED(m_con.m_ied);
		m_lnsModel->setActiveIED(m_con.m_ied);
		m_lnStateModel->setActiveIED(m_con.m_ied);
		m_lnTreeModel->setActiveIED(m_con.m_ied);
		m_dsComModel->setActiveIED(m_con.m_ied);
		m_dsSigModel->setActiveIED(m_con.m_ied);
		m_rcbComModel->setActiveIED(m_con.m_ied);
		m_reportsModel->setActiveIED(m_con.m_ied);
	}

	void IEDBackend::slotUpdateItems(Core::ptrValuesUpdater t_vals)
	{
		if (t_vals) {
			t_vals->update();
		}
	}
}