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

#include "ds_overview_table.hpp"

namespace App::Models
{
    DS_OverviewTable::DS_OverviewTable(QObject *parent, Core::IED::ptr ied)
        : QAbstractListModel(parent), m_ied(ied)
    {
    }

    void DS_OverviewTable::setActiveIED(Core::IED::ptr ied)
    {
        beginResetModel();
        m_ied = ied;
        endResetModel();
    }

    void DS_OverviewTable::setSelectedDS(int ds)
    {
        m_currentDS = ds;
        emit sigDSSelected(m_currentDS);
        emit sigSelectRow(m_currentDS);
    }

    QHash<int, QByteArray> DS_OverviewTable::roleNames() const
    {
        return { { SECTION_ROLE, "section" }, { NAME_ROLE, "name" }, { VALUE_ROLE, "value" } };
    }

    int DS_OverviewTable::rowCount(const QModelIndex &parent) const
    {
        return m_ied->model().getDataSetList().count();
    }

    QVariant DS_OverviewTable::data(const QModelIndex &index, int role) const
    {
        int row = index.row();
        auto getDataSetList = m_ied->model().getDataSetList();
        if (row >= getDataSetList.count()) {
            return QVariant(" ? ");
        }

        switch (role) {
        case SECTION_ROLE: {
            return QVariant(getDataSetList[row]->Core::ModelItem::getParent()->getReference());
        }
        case NAME_ROLE: {
            return QVariant(getDataSetList[row]->getName());
        }
        case VALUE_ROLE: {
            return QVariant(QString("# %1").arg(getDataSetList[row]->getItemCount()));
        }
        }
        return QVariant("??");
    }

    void DS_OverviewTable::slotDataUpdated()
    {
    }
}