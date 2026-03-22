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
    class DS_SignalsTable : public QAbstractTableModel
    {
        Q_OBJECT
        enum ColumnType {
            DS_INDEX_COLUMN = 0,
            DS_LD_COLUMN,
            DS_REF_COLUMN,
            DS_VALUE_COLUMN,

            COLUMN_COUNT
        };

    public:
        DS_SignalsTable(QObject *parent, Core::IED::ptr ied);

        void    setActiveIED(Core::IED::ptr ied);
        auto    getDataSet() {
            return m_dataSet;
        }

        QVariant headerData(int section, Qt::Orientation orientation,
                            int role = Qt::DisplayRole) const override;
        QHash<int, QByteArray> roleNames() const override;
        int     rowCount(const QModelIndex &parent = QModelIndex()) const override;
        int     columnCount(const QModelIndex &parent = QModelIndex()) const override;

        QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

        Q_INVOKABLE QVariantList getItemDetail(int row) const;

    public slots:
        void    slotDataSetSelected(int ds);
        void    slotDataUpdated(QList<Core::ModelItem::ptr> items);

    private:
        Core::IED::ptr          m_ied;
        Core::DataSet::ptr      m_dataSet;
        QMetaObject::Connection m_updConnection; // signal from LogicalNode
    };
}