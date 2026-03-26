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
     * @brief This is a model for the selected LogicalNode that contains the following columns:
     * Path to the attirubte, value, quality, timestamp and description
     * 
     */
    class LN_SignalTable : public QAbstractTableModel
    {
        Q_OBJECT
        enum ColumnType {
            DO_NAME_COLUMN = 0,
            DO_FC_COLUMN,
            DO_VALUE_COLUMN,
            DO_QUALITY_COLUMN,
            DO_TS_COLUMN,
            DO_DESC_COLUMN,
            COLUMN_COUNT = 6
        };

        enum ControlColumnType {
            CO_NAME = 0,
            CO_TYPE,
            CO_VALUE,
            CO_MODEL,
            CO_SELD,
            CO_OPOK,
            CO_DESC,
            CO_COLUMN_COUNT = 7
        };

        enum SettingsColumnType {
            SET_NAME = 0,
            SET_FC,
            SET_VALUE,
            SET_DESC,
            SET_COLUMN_COUNT = 4
        };

    public:
        enum class MatrixType { State, Controls, Settings };

        LN_SignalTable(QObject *parent, Core::IED::ptr ied,
                       MatrixType type = MatrixType::State);

        void     setActiveIED(Core::IED::ptr ied);
        auto     getCurrectLN() const {
            return m_lnode;
        }

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
        Core::LN_SignalMatrix::ptr getMatrix() const;

        Core::IED::ptr          m_ied;
        Core::LogicalNode::ptr  m_lnode; // currect logical node of this model
        QMetaObject::Connection m_updConnection; // signal from LogicalNode
        MatrixType              m_type;
    };
}
