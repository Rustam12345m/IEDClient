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

#include "ds_signals_table.hpp"
#include "core/model_item_value.hpp"

namespace App::Models
{
    DS_SignalsTable::DS_SignalsTable(QObject *t_parent, Core::IED::ptr t_ied)
        : QAbstractTableModel(t_parent), m_ied(t_ied)
    {
    }

    void DS_SignalsTable::setActiveIED(Core::IED::ptr t_ied)
    {
        beginResetModel();
        m_ied = t_ied;
        m_dataSet.reset();
        endResetModel();
    }

    QVariant DS_SignalsTable::headerData(int t_column, Qt::Orientation t_orientation, int t_role) const
    {
        if (t_orientation != Qt::Horizontal) {
            return QVariant();
        }

        switch (t_column) {
        case DS_INDEX_COLUMN: {
            return QVariant("#");
        }
        case DS_REF_COLUMN: {
            return QVariant("Reference");
        }
        case DS_FC_COLUMN: {
            return QVariant("FC");
        }
        case DS_VALUE_COLUMN: {
            return QVariant("Value");
        }
        }
        return QVariant("");
    }

    QHash<int, QByteArray> DS_SignalsTable::roleNames() const
    {
        return { { Qt::DisplayRole, "display" } };
    }

    int DS_SignalsTable::rowCount(const QModelIndex &t_parent) const
    {
        if (m_dataSet) {
            return m_dataSet->getItemCount();
        }
        return 0;
    }

    int DS_SignalsTable::columnCount(const QModelIndex &t_parent) const
    {
        return COLUMN_COUNT;
    }

    QVariant DS_SignalsTable::data(const QModelIndex &t_index, int t_role) const
    {
        int row = t_index.row(), column = t_index.column();

        if (column == DS_INDEX_COLUMN) {
            return QVariant(QString::number(row + 1));
        }

        if (m_dataSet) {
            auto dsItem = m_dataSet->getItem<Core::DataSetItem>(row);
            if (dsItem) {
                switch (column) {
                case DS_REF_COLUMN: {
                    return QVariant(dsItem->getName());
                }
                case DS_FC_COLUMN: {
                    return QVariant(dsItem->fc());
                }
                case DS_VALUE_COLUMN: {
                    auto item = dsItem->item();
                    if (item) {
                        return Core::ModelItemFullValue::get(item);
                    }
                    break;
                }
                }
            }
        }
        return QVariant(" ? ");
    }

    void DS_SignalsTable::slotDataUpdated(QList<Core::ModelItem::ptr> t_items)
    {
        emit dataChanged(index(0, DS_VALUE_COLUMN), index(rowCount() - 1, COLUMN_COUNT));
    }

    void DS_SignalsTable::slotDataSetSelected(int t_ds)
    {
        auto getDataSetList = m_ied->model().getDataSetList();
        Core::DataSet::ptr newDS;
        if (t_ds >= 0 && t_ds < getDataSetList.size()) {
            newDS = getDataSetList[t_ds];

            if (m_dataSet != newDS) {
                if (m_dataSet) {
                    disconnect(m_updConnection);
                }

                beginResetModel();
                m_dataSet = newDS;
                m_updConnection = connect(m_dataSet.get(), &Core::DataSet::sigItemUpdated,
                                          this, &DS_SignalsTable::slotDataUpdated);
                endResetModel();
            }
        }
    }
}