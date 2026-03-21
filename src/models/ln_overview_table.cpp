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
        inline int getInt(Core::ModelItem::ptr item)
        {
            return item ? item->getValue().toInt() : -1;
        }
    }

    LN_OverviewTable::LN_OverviewTable(QObject *parent, Core::IED::ptr ied)
        : QAbstractTableModel(parent), m_ied(ied)
    {
    }

    void LN_OverviewTable::setActiveIED(Core::IED::ptr ied)
    {
        beginResetModel();
        m_ied = ied;
        m_ldev.reset();
        m_ldevIndex = -1;
        endResetModel();
    }

    void LN_OverviewTable::setSelectedLN(int inx)
    {
        //qDebug() << "LNs_Table: Selected LN = " << inx;
        emit sigLNSelected(m_ldevIndex, inx);
    }

    QVariant LN_OverviewTable::headerData(int section, Qt::Orientation orientation, int role) const
    {
        switch (orientation) {
        case Qt::Horizontal: {
            const char* labels[] = { "Name", "Mode", "Beh", "Health" };

            return QVariant(labels[section % COLUMN_COUNT]);
        }
        case Qt::Vertical: {
            break;
        }
        }
        return QVariant();
    }

    QHash<int, QByteArray> LN_OverviewTable::roleNames() const
    {
        return { { Qt::DisplayRole, "display" } };
    }

    int LN_OverviewTable::rowCount(const QModelIndex &parent) const
    {
        if (m_ldev) {
            return m_ldev->getItemCount();
        }
        return 0;
    }

    int LN_OverviewTable::columnCount(const QModelIndex &parent) const
    {
        return COLUMN_COUNT;
    }

    QVariant LN_OverviewTable::data(const QModelIndex &index, int role) const
    {
        auto ln = m_ldev->getItem<Core::LogicalNode>(index.row());
        if (ln) {
            switch (index.column()) {
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

    void LN_OverviewTable::slotDataUpdated(Core::ModelItem::ptrList nodes)
    {
        emit dataChanged(index(0, MOD_COLUMN), index(rowCount() - 1, HEALTH_COLUMN));
    }

    void LN_OverviewTable::slotLDSelected(int ld)
    {
        if (m_ldevIndex != ld) {
            if (m_ldev) {
                disconnect(m_updConnection);
            }

            beginResetModel();
            m_ldevIndex = ld;
            m_ldev = m_ied->model().getLogicalDevice(m_ldevIndex);
            if (m_ldev) {
                m_updConnection = connect(m_ldev.get(), &Core::LogicalDevice::sigDataObjectUpdated,
                                          this, &LN_OverviewTable::slotDataUpdated);
            }
            endResetModel();
        }
    }
}