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

#include "ied_backend.hpp"

#include <QCoreApplication>

namespace App
{
	IED_Backend::IED_Backend(IEDConContainer &t_con, EventStorage &t_ev)
        : BackendInterface(t_con, t_ev)
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

	void IED_Backend::updateLDs_Status()
	{
		qDebug() << "IED_Backend: Update LDs";

		auto cmd = Cmd::UpdateLDs_StatusCmd::create(m_con.m_ied);
        connect(cmd.get(), &Cmd::UpdateLDs_StatusCmd::sigNewModelValues,
                this, &IED_Backend::slotUpdateItems, Qt::QueuedConnection);

		putCmdToQueue(cmd);
	}

	void IED_Backend::updateLNs_Status()
	{
		qDebug() << "IED_Backend: Update LNs";

		auto cmd = Cmd::UpdateLNs_StatusCmd::create(m_con.m_ied, m_lnsModel->getLogicalDevice());
        connect(cmd.get(), &Cmd::UpdateLNs_StatusCmd::sigNewModelValues,
                this, &IED_Backend::slotUpdateItems, Qt::QueuedConnection);

		putCmdToQueue(cmd);
	}

	void IED_Backend::updateRCBs_Status()
	{
		qDebug() << "IED_Backend: Update RCBs";

		auto cmd = Cmd::UpdateRCBs_Cmd::create(m_con.m_ied);

		putCmdToQueue(cmd);
	}

	void IED_Backend::updateLN_TreeValues()
	{
		qDebug() << "IED_Backend: Update LN command";

		auto lnode = m_lnStateModel->getCurrectLN();
        if (lnode == nullptr) {
            qDebug() << "IED_Backend: LNode not found!";
            return;
        }

		auto cmd = Cmd::UpdateLNode_Cmd::create(m_con.m_ied, lnode);
		connect(cmd.get(), &Cmd::UpdateLNode_Cmd::sigNewModelValues,
                this, &IED_Backend::slotUpdateItems, Qt::QueuedConnection);

		putCmdToQueue(cmd);
	}

	void IED_Backend::updateDS_Values()
	{
		qDebug() << "IED_Backend: Update DataSet";

		Core::ptrDataSet ds = m_dsSigModel->getDataSet();
		auto cmd = Cmd::UpdateDataSet_Cmd::create(m_con.m_ied, ds);
		connect(cmd.get(), &Cmd::UpdateDataSet_Cmd::sigNewModelValues, this,
                &IED_Backend::slotUpdateItems, Qt::QueuedConnection);

		putCmdToQueue(cmd);
	}

	QString IED_Backend::ldsPageStatus()
	{
		return "IED: " + m_con.m_ied->model().getName();
	}

	QString IED_Backend::lnsPageStatus()
	{
		auto ld = m_lnsModel->getLogicalDevice();
		if (ld) {
			return "LD: " + ld->getName();
		}
		return " - ";
	}

	QString IED_Backend::dsPageStatus()
	{
		return "All found DataSets";
	}

	QString IED_Backend::rcbPageStatus()
	{
		return "All found RCB";
	}

	void IED_Backend::slotConnected(bool t_done)
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

	void IED_Backend::slotUpdateItems(Core::ptrModelValuesUpd t_vals)
	{
		if (t_vals) {
			t_vals->update();
		}
	}
}