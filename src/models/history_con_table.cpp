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

#include "history_con_table.hpp"

namespace App::Models
{
    HistConTable::HistConTable(QObject *parent, App::AppSettings &conf)
        : QAbstractTableModel(parent), m_settings(conf)
    {
        m_con = m_settings.getConnectionList();

        connect(&m_settings, &AppSettings::sigConfUpdated, this, &HistConTable::slotAppConfigUpdated);
    }

    QVariant HistConTable::headerData(int column, Qt::Orientation orientation, int role) const
    {
        const QString headerNames[] = { "№", "IED", "IP-address", "Port", "Last connection" };
        if (column >= 0 && column < COLUMNS_COUNT) {
            return QVariant(headerNames[column]);
        }
        return QVariant(" ? ");
    }

    QHash<int, QByteArray> HistConTable::roleNames() const
    {
        return { { Qt::DisplayRole, "display" } };
    }

    int HistConTable::rowCount(const QModelIndex &parent) const
    {
        return m_con.size();
    }

    int HistConTable::columnCount(const QModelIndex &parent) const
    {
        return ColumnType::COLUMNS_COUNT;
    }

    QVariant HistConTable::data(const QModelIndex &index, int role) const
    {
        if (index.row() < 0 || index.row() >= m_con.size()) return QVariant();
        switch (index.column()) {
        case ColumnType::NUMBER: {
            return QVariant(index.row() + 1);
        }
        case ColumnType::IED_NAME: {
            return m_con[index.row()].ied();
        }
        case ColumnType::IP_ADDR : {
            return m_con[index.row()].ip();
        }
        case ColumnType::PORT_COLUMN : {
            return m_con[index.row()].port();
        }
        case ColumnType::LAST_CONNECT: {
            return m_con[index.row()].date();
        }
        }
        return QVariant(" ? ");
    }

    void HistConTable::removeFromHistory(int idx)
    {
        // qDebug() << "Remove from history index: " << idx;

        if (idx >= 0 && idx < m_con.size()) {
            beginResetModel();
            m_con.removeAt(idx);
            endResetModel();

            m_settings.saveHistoryList(m_con);
        }
    }

    void HistConTable::slotAppConfigUpdated()
    {
        beginResetModel();
        m_con = m_settings.getConnectionList();
        endResetModel();
    }
}
