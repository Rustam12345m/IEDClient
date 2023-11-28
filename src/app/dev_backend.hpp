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

#pragma once

#include "backend_base.hpp"

#include "models/sort_proxy_model.hpp"
#include "models/lds_grid.hpp"
#include "models/ld_prop_table.hpp"
#include "models/lns_table.hpp"
#include "models/ln_state_table.hpp"
#include "models/ln_signals_tree.hpp"
#include "models/datasets_table.hpp"
#include "models/ds_signals_table.hpp"
#include "models/rcb_general_table.hpp"
#include "models/reports_table.hpp"

namespace App
{
	/*
	* Presenter for LD, LN, DO table in QML
	* */
	class DevBackend : public BackendBase
	{
		Q_OBJECT
	public:
		DevBackend(IED_Connection &t_con);
		~DevBackend() = default;

		Q_PROPERTY(QAbstractListModel* 	ldModel 		READ getLD_Model		CONSTANT)
		Q_PROPERTY(QAbstractTableModel* lnModel 		READ getLN_Model 		CONSTANT)
		Q_PROPERTY(QAbstractItemModel* 	doModel 		READ getSortDO_Model 	CONSTANT)
		Q_PROPERTY(QAbstractItemModel* 	lnTreeModel 	READ getLN_TreeModel 	CONSTANT)
		Q_PROPERTY(QAbstractListModel* 	ldPropModel 	READ getLD_PropModel	CONSTANT)
		//Q_PROPERTY(QAbstractTableModel* 	doModel 		READ getDO_Model 		CONSTANT)
		Q_PROPERTY(QAbstractTableModel* dataSetsModel 	READ getDSInfo_Model 	CONSTANT)
		Q_PROPERTY(QAbstractTableModel* dataSetModel	READ getDS_Model 		CONSTANT)
		Q_PROPERTY(QAbstractTableModel* rcbMainModel	READ getRCB_Model 		CONSTANT)
		Q_PROPERTY(QAbstractTableModel* rcbReportsModel	READ getReports_Model 	CONSTANT)

		QAbstractListModel*		getLD_Model() const { return m_ldModel; }
		QAbstractTableModel*	getLN_Model() const { return m_lnsModel; }
		QAbstractTableModel*	getDO_Model() const { return m_doModel; }
		QAbstractListModel*		getLD_PropModel() const { return m_ldPropModel; }
		QAbstractItemModel* 	getSortDO_Model() const { return m_sortDOModel; }
		QAbstractItemModel* 	getLN_TreeModel() const { return m_lnTreeModel; }
		QAbstractTableModel*	getDSInfo_Model() const { return m_dsInfoModel; }
		QAbstractTableModel*	getDS_Model() const { return m_dsModel; }
		QAbstractTableModel*	getRCB_Model() const { return m_rcbMainModel; }
		QAbstractTableModel*	getReports_Model() const { return m_reportsModel; }

		// Commands
		Q_INVOKABLE void 		updateLDs_Status();
		Q_INVOKABLE void 		updateDO_Table();

		// Status bar
		Q_INVOKABLE QString 	getLD_TextStatus();
		Q_INVOKABLE QString 	getLN_TextStatus();

	public slots:
		void 	slotConnected(bool t_done) override;

	protected:
		// Models for Tables in GUI
		Models::LDs_Grid*			m_ldModel = nullptr;
		Models::LD_PropTable*		m_ldPropModel = nullptr;
		Models::LNs_Table*			m_lnsModel = nullptr;
		Models::LN_StateTable*		m_doModel = nullptr;
		Models::LN_SignalsTree* 	m_lnTreeModel = nullptr;
		Models::SortProxyModel* 	m_sortDOModel = nullptr;
		Models::DataSetsTable*  	m_dsInfoModel = nullptr;
		Models::DS_SignalsTable* 	m_dsModel = nullptr;
		Models::RCB_GeneralTable* 	m_rcbMainModel = nullptr;
		Models::ReportsTable* 		m_reportsModel = nullptr;
	};
}