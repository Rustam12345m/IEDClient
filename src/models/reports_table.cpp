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

#include "reports_table.hpp"

namespace App::Models
{
    ReportsTable::ReportsTable(QObject *t_parent, Core::IED::ptr t_ied)
        : QAbstractTableModel(t_parent), m_ied(t_ied)
    {
    }

    void ReportsTable::setActiveIED(Core::IED::ptr t_ied)
    {
        beginResetModel();
        m_ied = t_ied;
        endResetModel();
    }

    QVariant ReportsTable::headerData(int t_section, Qt::Orientation t_orientation, int t_role) const
    {
        switch (t_orientation) {
        case Qt::Horizontal: {
            const char* labels[] = { "Name", "Mode", "Beh", "Health" };

            return QVariant(labels[t_section % COLUMN_COUNT]);
        }
        case Qt::Vertical: {
            break;
        }
        }
        return QVariant();
    }

    QHash<int, QByteArray> ReportsTable::roleNames() const
    {
        return { { Qt::DisplayRole, "display" } };
    }

    int ReportsTable::rowCount(const QModelIndex &t_parent) const
    {
        return 0;
    }

    int ReportsTable::columnCount(const QModelIndex &t_parent) const
    {
        return COLUMN_COUNT;
    }

    QVariant ReportsTable::data(const QModelIndex &t_index, int t_role) const
    {
        return QVariant(" ? ");
    }

    void ReportsTable::slotDataUpdated()
    {
        emit dataChanged(index(0, ModeColumn), index(rowCount() - 1, HealthColumn));
    }

    void ReportsTable::slotRCBSelected(int t_inx)
    {
        if (m_currentRCB != t_inx) {
            beginResetModel();
            m_currentRCB = t_inx;
            endResetModel();
        }
    }
}
