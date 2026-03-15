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

#include "backend_interface.hpp"

#include "models/sort_proxy_model.hpp"
#include "models/ld_overview_grid.hpp"
#include "models/ld_properties_table.hpp"
#include "models/ln_overview_table.hpp"
#include "models/ln_state_table.hpp"
#include "models/ln_common_tree.hpp"
#include "models/ds_overview_table.hpp"
#include "models/ds_signals_table.hpp"
#include "models/rcb_overview_table.hpp"
#include "models/reports_table.hpp"
#include "models/goose_overview_table.hpp"
#include "models/sv_overview_table.hpp"

namespace App
{
    /**
     * @brief Presenter for LD, LN, DO table in QML
     */
    class IED_Backend : public BackendInterface
    {
        Q_OBJECT
    public:
        IED_Backend(IEDConContainer &t_con, EventStorage &t_ev);
        ~IED_Backend() = default;

        Q_INVOKABLE QAbstractListModel*  getLD_GridModel() const { return m_ldsModel; }
        Q_INVOKABLE QAbstractListModel*  getLD_PropModel() const { return m_ldPropModel; }
        Q_INVOKABLE QAbstractTableModel* getLN_ComModel() const { return m_lnsModel; }
        Q_INVOKABLE QAbstractItemModel*  getLN_StateModel() const { return m_sortDOModel; }
        Q_INVOKABLE QAbstractItemModel*  getLN_TreeModel() const { return m_lnTreeModel; }
        Q_INVOKABLE QAbstractListModel*  getDS_ComModel() const { return m_dsComModel; }
        Q_INVOKABLE QAbstractTableModel* getDS_ItemModel() const { return m_dsSigModel; }
        Q_INVOKABLE QAbstractTableModel* getBRCB_ComModel() const { return m_brcbComModel; }
        Q_INVOKABLE QAbstractTableModel* getURCB_ComModel() const { return m_urcbComModel; }
        Q_INVOKABLE QAbstractTableModel* getGOSE_ComModel() const { return m_gooseComModel; }
        Q_INVOKABLE QAbstractTableModel* getSV_ComModel()   const { return m_svComModel; }
        Q_INVOKABLE QAbstractTableModel* getReportsModel()  const { return m_reportsModel; }

        // CMD
        Q_INVOKABLE void updateLDs_Status();
        Q_INVOKABLE void updateLNs_Status();
        Q_INVOKABLE void updateRCBs_Status();
        Q_INVOKABLE void updateLN_TreeValues();
        Q_INVOKABLE void updateDS_Values();

        // Status bar
        Q_INVOKABLE QString ldsPageStatus();
        Q_INVOKABLE QString lnsPageStatus();
        Q_INVOKABLE QString dsPageStatus();
        Q_INVOKABLE QString rcbPageStatus();

    public slots:
        void slotConnected(bool t_done) override;
        void slotUpdateItems(Core::ModelStateUpdater::ptr t_vals);

    protected:
        // Models for Tables in GUI
        Models::LD_OverviewGrid*    m_ldsModel = nullptr;
        Models::LD_PropertiesTable* m_ldPropModel = nullptr;
        Models::LN_OverviewTable*   m_lnsModel = nullptr;
        Models::LN_SignalTable*     m_lnStateModel = nullptr;
        Models::LN_CommonTree*      m_lnTreeModel = nullptr;
        Models::SortProxyModel*     m_sortDOModel = nullptr;
        Models::DS_OverviewTable*    m_dsComModel    = nullptr;
        Models::DS_SignalsTable*     m_dsSigModel    = nullptr;
        Models::RCB_OverviewTable*   m_brcbComModel  = nullptr;
        Models::RCB_OverviewTable*   m_urcbComModel  = nullptr;
        Models::GOOSE_OverviewTable* m_gooseComModel = nullptr;
        Models::SV_OverviewTable*    m_svComModel    = nullptr;
        Models::ReportsTable*        m_reportsModel  = nullptr;
    };
}
