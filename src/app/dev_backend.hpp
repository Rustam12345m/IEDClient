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
#include "models/ld_common_grid.hpp"
#include "models/ld_prop_table.hpp"
#include "models/ln_common_table.hpp"
#include "models/ln_state_table.hpp"
#include "models/ln_signals_tree.hpp"
#include "models/ds_common_table.hpp"
#include "models/ds_signals_table.hpp"
#include "models/rcb_common_table.hpp"
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
		DevBackend(AppConContainer &t_con);
		~DevBackend() = default;

		Q_INVOKABLE QAbstractListModel*		getLD_GridModel() const { return m_ldsModel; }
		Q_INVOKABLE QAbstractListModel*		getLD_PropModel() const { return m_ldPropModel; }
		Q_INVOKABLE QAbstractTableModel*	getLN_ComModel() const { return m_lnsModel; }
		Q_INVOKABLE QAbstractItemModel* 	getLN_StateModel() const { return m_sortDOModel; }
		Q_INVOKABLE QAbstractItemModel* 	getLN_TreeModel() const { return m_lnTreeModel; }
		Q_INVOKABLE QAbstractListModel*		getDS_ComModel() const { return m_dsComModel; }
		Q_INVOKABLE QAbstractTableModel*	getDS_ItemModel() const { return m_dsSigModel; }
		Q_INVOKABLE QAbstractTableModel*	getRCB_ComModel() const { return m_rcbComModel; }
		Q_INVOKABLE QAbstractTableModel*	getReportsModel() const { return m_reportsModel; }

		// CMD
		Q_INVOKABLE void 		updateLDs_Status();
		Q_INVOKABLE void 		updateLNs_Status();
		Q_INVOKABLE void 		updateRCBs_Status();
		Q_INVOKABLE void 		updateLN_TreeValues();
		Q_INVOKABLE void 		updateDS_Values();

		// Status bar
		Q_INVOKABLE QString 	ldsPageStatus();
		Q_INVOKABLE QString 	lnsPageStatus();
		Q_INVOKABLE QString 	dsPageStatus();
		Q_INVOKABLE QString 	rcbPageStatus();

	public slots:
		void 	slotConnected(bool t_done) override;

	protected:
		// Models for Tables in GUI
		Models::LD_CommonGrid*		m_ldsModel = nullptr;
		Models::LD_PropTable*		m_ldPropModel = nullptr;
		Models::LN_CommonTable*		m_lnsModel = nullptr;
		Models::LN_StateTable*		m_lnStateModel = nullptr;
		Models::LN_SignalTree* 		m_lnTreeModel = nullptr;
		Models::SortProxyModel* 	m_sortDOModel = nullptr;
		Models::DS_CommonLModel*	m_dsComModel = nullptr;
		Models::DS_SignalsTable* 	m_dsSigModel = nullptr;
		Models::RCB_CommonTable* 	m_rcbComModel = nullptr;
		Models::ReportsTable* 		m_reportsModel = nullptr;
	};
}