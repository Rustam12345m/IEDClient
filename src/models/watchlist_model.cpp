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

#include "watchlist_model.hpp"

namespace App::Models
{
    WatchlistModel::WatchlistModel(QObject *parent)
        : QAbstractTableModel(parent)
    {
    }

    int WatchlistModel::rowCount(const QModelIndex &parent) const
    {
        Q_UNUSED(parent)
        return m_items.size();
    }

    int WatchlistModel::columnCount(const QModelIndex &parent) const
    {
        Q_UNUSED(parent)
        return COLUMN_COUNT;
    }

    QVariant WatchlistModel::data(const QModelIndex &index, int role) const
    {
        if (!index.isValid() || index.row() >= m_items.size()) {
            return {};
        }

        const auto &item = m_items[index.row()];

        if (role == Qt::DisplayRole) {
            int slashPos = item.ref.indexOf('/');
            switch (index.column()) {
            case COL_LD:        return slashPos >= 0 ? item.ref.left(slashPos) : "";
            case COL_REFERENCE: return slashPos >= 0 ? item.ref.mid(slashPos + 1) : item.ref;
            case COL_FC:        return item.fc.isEmpty() ? "" : "[" + item.fc + "]";
            case COL_VALUE:     return item.value;
            }
        }

        if (role == Qt::ToolTipRole && (index.column() == COL_LD || index.column() == COL_REFERENCE)) {
            return item.ref;
        }

        return {};
    }

    QVariant WatchlistModel::headerData(int section, Qt::Orientation orientation, int role) const
    {
        if (orientation != Qt::Horizontal || role != Qt::DisplayRole) {
            return {};
        }

        switch (section) {
        case COL_LD:        return "LD";
        case COL_REFERENCE: return "Reference";
        case COL_FC:        return "FC";
        case COL_VALUE:     return "Value";
        }
        return {};
    }

    bool WatchlistModel::addItem(const QString &ref, const QString &fc, const QString &value)
    {
        for (const auto &item : m_items) {
            if (item.ref == ref && item.fc == fc) {
                return false;
            }
        }

        beginInsertRows(QModelIndex(), m_items.size(), m_items.size());
        m_items.append({ref, fc, value});
        endInsertRows();

        emit sigItemsChanged();
        return true;
    }

    void WatchlistModel::removeItem(int row)
    {
        if (row < 0 || row >= m_items.size()) {
            return;
        }

        beginRemoveRows(QModelIndex(), row, row);
        m_items.removeAt(row);
        endRemoveRows();

        emit sigItemsChanged();
    }

    void WatchlistModel::clear()
    {
        if (m_items.isEmpty()) {
            return;
        }

        beginResetModel();
        m_items.clear();
        endResetModel();

        emit sigItemsChanged();
    }

    void WatchlistModel::setItems(const QList<WatchItem> &items)
    {
        beginResetModel();
        m_items = items;
        endResetModel();
    }

    void WatchlistModel::updateValues(const QVariantList &results)
    {
        for (const auto &entry : results) {
            auto map = entry.toMap();
            QString ref = map.value("ref").toString();
            QString value = map.value("value").toString();

            for (int i = 0; i < m_items.size(); i++) {
                if (m_items[i].ref == ref) {
                    m_items[i].value = value;
                    emit dataChanged(createIndex(i, COL_VALUE), createIndex(i, COL_VALUE));
                    break;
                }
            }
        }
    }
}
