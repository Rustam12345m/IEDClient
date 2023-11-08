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

#pragma once

#include "backend_base.hpp"

#include "models/sort_proxy_model.hpp"
#include "models/ld_grid_model.hpp"
#include "models/ld_property_model.hpp"
#include "models/ln_tablemodel.hpp"
#include "models/do_tablemodel.hpp"
#include "models/ds_info_table_model.hpp"
#include "models/ds_table_model.hpp"

namespace App
{
	/*
	* Presenter for LD, LN, DO table in QML
	* */
	class LD_Backend : public BackendBase
	{
		Q_OBJECT
	public:
		LD_Backend(ConnectionObject &t_con);
		~LD_Backend() = default;

		Q_PROPERTY(LD_GridModel* 		ldModel 		READ getLD_Model		CONSTANT)
		Q_PROPERTY(LD_PropertyModel* 	ldPropModel 	READ getLD_PropModel	CONSTANT)
		Q_PROPERTY(LN_TableModel* 		lnModel 		READ getLN_Model 		CONSTANT)
		Q_PROPERTY(QAbstractItemModel* 	doModel 		READ getSortDO_Model 	CONSTANT)
		//Q_PROPERTY(DO_TableModel* 	doModel 		READ getDO_Model 		CONSTANT)
		Q_PROPERTY(DSInfo_TableModel* 	dsInfoModel 	READ getDSInfo_Model 	CONSTANT)
		Q_PROPERTY(DS_TableModel* 		dataSetModel	READ getDS_Model 		CONSTANT)
	
		LD_GridModel*		getLD_Model() const { return m_ldModel; }
		LD_PropertyModel*	getLD_PropModel() const { return m_ldPropModel; }
		LN_TableModel*		getLN_Model() const { return m_lnModel; }
		DO_TableModel*		getDO_Model() const { return m_doModel; }
		QAbstractItemModel* getSortDO_Model() const { return m_sortDOModel; }
		DSInfo_TableModel*	getDSInfo_Model() const { return m_dsInfoModel; }
		DS_TableModel*		getDS_Model() const { return m_dsModel; }

		// Commands
		Q_INVOKABLE void 	updateDO_Table();

		// Status bar
		Q_INVOKABLE QString 	getLD_TextStatus();
		Q_INVOKABLE QString 	getLN_TextStatus();

	public slots:
		void 	slotNewIED(bool t_done) override;

	protected:
		// Models for Tables in GUI
		LD_GridModel*		m_ldModel = nullptr;
		LD_PropertyModel*	m_ldPropModel = nullptr;
		LN_TableModel*		m_lnModel = nullptr;
		DO_TableModel*		m_doModel = nullptr;
		SortProxyModel* 	m_sortDOModel = nullptr;
		DSInfo_TableModel* 	m_dsInfoModel = nullptr;
		DS_TableModel* 		m_dsModel = nullptr;
	};
}