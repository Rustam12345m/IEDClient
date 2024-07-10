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
    /*
     * This is a model for the selected LogicalNode that contains the following columns:
     * Path to the attirubte, value, quality, timestamp and description
     * 
     * */
    class LN_SignalTable : public QAbstractTableModel
    {
        Q_OBJECT
        enum ColumnType {
            DO_NAME_COLUMN = 0,
            DO_FC_COLUMN,
            DO_VALUE_COLUMN,
            DO_QUALITY_COLUMN,
            DO_TS_COLUMN,
            DO_DESC_COLUMN
        };

    public:
        LN_SignalTable(QObject *t_parent, Core::IED::ptr t_ied);

        void     setActiveIED(Core::IED::ptr t_ied);
        auto     getCurrectLN() const {
            return m_lnode;
        }

        QVariant headerData(int t_section, Qt::Orientation t_orientation,
                            int t_role = Qt::DisplayRole) const override;
        QHash<int, QByteArray> roleNames() const override;

        int      rowCount(const QModelIndex &t_parent = QModelIndex()) const override;
        int      columnCount(const QModelIndex &t_parent = QModelIndex()) const override;
        QVariant data(const QModelIndex &t_index, int t_role = Qt::DisplayRole) const override;

    public slots:
        void     slotDataUpdated(Core::ModelItem::ptrList t_nodes);
        void     slotLNSelected(int t_ld, int t_ln);

    private:
        Core::IED::ptr          m_ied;
        Core::LogicalNode::ptr  m_lnode; // currect logical node of this model
        QMetaObject::Connection m_updConnection; // signal from LogicalNode
    };
}