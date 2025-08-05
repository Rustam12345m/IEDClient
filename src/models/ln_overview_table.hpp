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

#include <QAbstractTableModel>

#include "core/ied.hpp"

namespace App::Models
{
    class LN_OverviewTable : public QAbstractTableModel
    {
        Q_OBJECT
    public:
        enum ColumnType {
            NAME_COLUMN = 0,
            MOD_COLUMN,
            BEH_COLUMN,
            HEALTH_COLUMN,

            COLUMN_COUNT
        };

        LN_OverviewTable(QObject *t_parent, Core::IED::ptr t_ied);

        Q_INVOKABLE void setSelectedLN(int t_inx);

        void     setActiveIED(Core::IED::ptr t_ied);
        auto     getLogicalDevice() const { return m_ldev; }

        QVariant headerData(int t_section, Qt::Orientation t_orientation,
                            int t_role = Qt::DisplayRole) const override;

        QHash<int, QByteArray> roleNames() const override;

        int      rowCount(const QModelIndex &t_parent = QModelIndex()) const override;
        int      columnCount(const QModelIndex &t_parent = QModelIndex()) const override;

        QVariant data(const QModelIndex &t_index, int t_role = Qt::DisplayRole) const override;

    signals:
        void     sigLNSelected(int t_ld, int t_ln);

    public slots:
        void     slotDataUpdated(Core::ModelItem::ptrList t_nodes);
        void     slotLDSelected(int t_ld);

    private:
        Core::IED::ptr              m_ied;
        Core::LogicalDevice::ptr    m_ldev; // selected Logical Device by user
        int                         m_ldevIndex = 0; //
        QMetaObject::Connection     m_updConnection; // signal from LD
    };
}