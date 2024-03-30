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

#include "ln_overview_table.hpp"
#include "iec61850_enums.hpp"

namespace App::Models
{
    namespace
    {
        inline int getInt(Core::ptrModelItem t_item)
        {
            return t_item ? t_item->getValue().toInt() : -1;
        }
    }

    LN_OverviewTable::LN_OverviewTable(QObject *t_parent, Core::ptrIED t_ied)
        : QAbstractTableModel(t_parent), m_ied(t_ied)
    {
    }

    void LN_OverviewTable::setActiveIED(Core::ptrIED t_ied)
    {
        beginResetModel();
        m_ied = t_ied;
        m_ldev.reset();
        m_ldevIndex = -1;
        endResetModel();
    }

    void LN_OverviewTable::setSelectedLN(int t_ln)
    {
        //qDebug() << "LNs_Table: Selected LN = " << t_ln;
        emit sigLNSelected(m_ldevIndex, t_ln);
    }

    QVariant LN_OverviewTable::headerData(int t_section, Qt::Orientation t_orientation, int t_role) const
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

    QHash<int, QByteArray> LN_OverviewTable::roleNames() const
    {
        return { { Qt::DisplayRole, "value" } };
    }

    int LN_OverviewTable::rowCount(const QModelIndex &t_parent) const
    {
        if (m_ldev) {
            return m_ldev->getItemCount();
        }
        return 0;
    }

    int LN_OverviewTable::columnCount(const QModelIndex &t_parent) const
    {
        return COLUMN_COUNT;
    }

    QVariant LN_OverviewTable::data(const QModelIndex &t_index, int t_role) const
    {
        auto ln = m_ldev->getItem<Core::LogicalNode>(t_index.row());
        if (ln) {
            switch (t_index.column()) {
            case NAME_COLUMN: {
                return QVariant(ln->getName());
            }
            case MOD_COLUMN: {
                return IEC_EnumUserInfo::mod(getInt(ln->getModItem()));
            }
            case BEH_COLUMN: {
                return IEC_EnumUserInfo::beh(getInt(ln->getBehItem()));
            }
            case HEALTH_COLUMN: {
                return IEC_EnumUserInfo::health(getInt(ln->getHealthItem()));
            }
            }
        }
        return QVariant(" ? ");
    }

    void LN_OverviewTable::slotDataUpdated(QSharedPointer<QList<Core::ModelItem*>> t_nodes)
    {
        emit dataChanged(index(0, MOD_COLUMN), index(rowCount() - 1, HEALTH_COLUMN));
    }

    void LN_OverviewTable::slotLDSelected(int t_ld)
    {
        if (m_ldevIndex != t_ld) {
            if (m_ldev) {
                disconnect(m_updConnection);
            }

            beginResetModel();
            m_ldevIndex = t_ld;
            m_ldev = m_ied->model().getLogicalDevice(m_ldevIndex);
            if (m_ldev) {
                m_updConnection = connect(m_ldev.get(), &Core::LogicalDevice::sigDataObjectUpdated,
                                          this, &LN_OverviewTable::slotDataUpdated);
            }
            endResetModel();
        }
    }
}