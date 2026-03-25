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

#include "models_stub.hpp"
#include "core/ied.hpp"

namespace App::Models
{
    /**
     * @brief LD-wide settings table shown when LLN0 is selected.
     * Shows LLN0 SP+SG+SE and all other LNs' SG+SE in one table.
     *
     * 7 columns: LN | Reference | FC | Value | Quality | Timestamp | Description
     */
    class LD_SettingsTable : public QAbstractTableModel
    {
        Q_OBJECT
        enum ColumnType {
            COL_LN = 0,
            COL_REF,
            COL_FC,
            COL_VALUE,
            COL_QUALITY,
            COL_TS,
            COL_DESC,
            COLUMN_COUNT = 7
        };

    public:
        LD_SettingsTable(QObject *parent, Core::IED::ptr ied);

        void     setActiveIED(Core::IED::ptr ied);
        Core::LogicalDevice::ptr getLogicalDevice() const { return m_ldev; }
        Core::LN_SignalMatrix::ptr getMatrix() const;

        QVariant headerData(int section, Qt::Orientation orientation,
                            int role = Qt::DisplayRole) const override;
        QHash<int, QByteArray> roleNames() const override;

        int      rowCount(const QModelIndex &parent = QModelIndex()) const override;
        int      columnCount(const QModelIndex &parent = QModelIndex()) const override;
        QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    public slots:
        void     slotDataUpdated(Core::ModelItem::ptrList nodes);
        void     slotLNSelected(int ld, int ln);

    private:
        Core::IED::ptr              m_ied;
        Core::LogicalDevice::ptr    m_ldev;
        QMetaObject::Connection     m_updConnection;
    };
}
