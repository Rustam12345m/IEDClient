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

#include "app_events_table.hpp"

namespace App::Models
{
    AppEventsTable::AppEventsTable(QObject *parent) : QAbstractTableModel{parent}
    {
    }

    int AppEventsTable::rowCount(const QModelIndex &parent) const
    {
        if (parent.isValid()) {
            return 0;
        }
        return m_count;
    }

    int AppEventsTable::columnCount(const QModelIndex &parent) const
    {
        if (parent.isValid()) {
            return 0;
        }
        return COLUMN_COUNT;
    }

    QHash<int, QByteArray> AppEventsTable::roleNames() const
    {
        return { { Qt::DisplayRole, "display" } };
    }

    Qt::ItemFlags AppEventsTable::flags(const QModelIndex &index) const
    {
        return QAbstractTableModel::flags(index) | Qt::ItemIsSelectable;
    }

    QVariant AppEventsTable::headerData(int section, Qt::Orientation orientation, int role) const
    {
        if (role != Qt::DisplayRole || orientation != Qt::Horizontal) {
            return QVariant();
        }

        switch (section) {
        case DATE_COLUMN:   return "Date and Time";
        case SOURCE_COLUMN: return "Source";
        case DESC_COLUMN:   return "Description";
        }
        return QVariant();
    }

    QVariant AppEventsTable::data(const QModelIndex &index, int role) const
    {
        if (role != Qt::DisplayRole || !index.isValid()) {
            return QVariant();
        }

        // Newest events first: row 0 = most recent
        int logicalIndex = m_count - 1 - index.row();
        int pos = (m_head + logicalIndex) % Capacity;
        const auto &ev = m_buffer[pos];

        switch (index.column()) {
        case DATE_COLUMN:
            return ev.m_time.toString("yyyy-MM-dd hh:mm:ss");
        case SOURCE_COLUMN:
            return ev.m_ip;
        case DESC_COLUMN:
            return ev.m_msg;
        }
        return QVariant();
    }

    void AppEventsTable::addEvent(Cmd::CmdEvent event)
    {
        if (m_count < Capacity) {
            beginInsertRows(QModelIndex(), 0, 0);
            int pos = (m_head + m_count) % Capacity;
            m_buffer[pos] = std::move(event);
            ++m_count;
            endInsertRows();
        } else {
            m_buffer[m_head] = std::move(event);
            m_head = (m_head + 1) % Capacity;
            emit dataChanged(index(0, 0), index(m_count - 1, COLUMN_COUNT - 1));
        }
    }
}
