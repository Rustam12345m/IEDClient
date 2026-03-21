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
#include "core/model_item.hpp"

namespace App::Models
{
    DS_SignalsTable::DS_SignalsTable(QObject *parent, Core::IED::ptr ied)
        : QAbstractTableModel(parent), m_ied(ied)
    {
    }

    void DS_SignalsTable::setActiveIED(Core::IED::ptr ied)
    {
        beginResetModel();
        m_ied = ied;
        m_dataSet.reset();
        endResetModel();
    }

    QVariant DS_SignalsTable::headerData(int column, Qt::Orientation orientation, int role) const
    {
        if (orientation != Qt::Horizontal) {
            return QVariant();
        }

        switch (column) {
        case DS_INDEX_COLUMN: {
            return QVariant("#");
        }
        case DS_LD_COLUMN: {
            return QVariant("LD");
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

    int DS_SignalsTable::rowCount(const QModelIndex &parent) const
    {
        if (m_dataSet) {
            return m_dataSet->getItemCount();
        }
        return 0;
    }

    int DS_SignalsTable::columnCount(const QModelIndex &parent) const
    {
        return COLUMN_COUNT;
    }

    QVariant DS_SignalsTable::data(const QModelIndex &index, int role) const
    {
        int row = index.row(), column = index.column();

        if (column == DS_INDEX_COLUMN) {
            return QVariant(QString::number(row + 1));
        }

        if (m_dataSet) {
            auto dsItem = m_dataSet->getItem<Core::DataSetItem>(row);
            if (dsItem) {
                switch (column) {
                case DS_LD_COLUMN: {
                    QString name = dsItem->getName();
                    int sep = name.indexOf('/');
                    return QVariant(sep >= 0 ? name.left(sep) : "");
                }
                case DS_REF_COLUMN: {
                    QString name = dsItem->getName();
                    int sep = name.indexOf('/');
                    return QVariant(sep >= 0 ? name.mid(sep + 1) : name);
                }
                case DS_FC_COLUMN: {
                    return QVariant(dsItem->fc());
                }
                case DS_VALUE_COLUMN: {
                    auto item = dsItem->item();
                    if (item) {
                        QString val = Core::ModelItemFullValue::get(item);
                        if (val.size() > 256) {
                            val.truncate(256);
                            val += "...";
                        }
                        return QVariant(val);
                    }
                    break;
                }
                }
            }
        }
        return QVariant(" ? ");
    }

    namespace
    {
        void collectLeaves(Core::ModelItem::ptr root, Core::ModelItem::ptr item, QVariantList &out)
        {
            if (item->getItemCount() == 0) {
                QVariantMap entry;
                entry["name"] = item->getReference(root.get());
                entry["value"] = item->getValue();
                out.append(entry);
                return;
            }
            for (auto &sub : item->getItemList()) {
                collectLeaves(root, sub, out);
            }
        }
    }

    QVariantList DS_SignalsTable::getItemDetail(int row) const
    {
        QVariantList result;
        if (!m_dataSet || row < 0 || row >= m_dataSet->getItemCount()) {
            return result;
        }

        auto dsItem = m_dataSet->getItem<Core::DataSetItem>(row);
        if (!dsItem) {
            return result;
        }

        auto item = dsItem->item();
        if (!item) {
            return result;
        }

        if (item->getItemCount() == 0) {
            QVariantMap entry;
            entry["name"] = dsItem->getName();
            entry["value"] = item->getValue();
            result.append(entry);
        } else {
            collectLeaves(item, item, result);
        }

        return result;
    }

    void DS_SignalsTable::slotDataUpdated(QList<Core::ModelItem::ptr> items)
    {
        if (rowCount() > 0) {
            emit dataChanged(index(0, DS_VALUE_COLUMN), index(rowCount() - 1, COLUMN_COUNT - 1));
        }
    }

    void DS_SignalsTable::slotDataSetSelected(int ds)
    {
        auto getDataSetList = m_ied->model().getDataSetList();
        Core::DataSet::ptr newDS;
        if (ds >= 0 && ds < getDataSetList.size()) {
            newDS = getDataSetList[ds];

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