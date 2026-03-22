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

#include "ln_all_overview_table.hpp"
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

    LN_AllOverviewTable::LN_AllOverviewTable(QObject *parent, Core::IED::ptr ied)
        : QAbstractListModel(parent), m_ied(ied)
    {
    }

    void LN_AllOverviewTable::setActiveIED(Core::IED::ptr ied)
    {
        beginResetModel();

        connectToUpdates(false);
        m_ied = ied;
        rebuildFlatList();
        connectToUpdates(true);

        endResetModel();
    }

    void LN_AllOverviewTable::selectLN(int row)
    {
        if (row >= 0 && row < m_entries.size()) {
            const auto &entry = m_entries[row];
            emit sigLNSelected(entry.ldIndex, entry.lnIndex);
        }
    }

    void LN_AllOverviewTable::setFilter(const QString &text)
    {
        m_filter = text.trimmed();
        beginResetModel();
        rebuildFlatList();
        endResetModel();
    }

    QHash<int, QByteArray> LN_AllOverviewTable::roleNames() const
    {
        return {
            { SECTION_ROLE,  "section"  },
            { LD_INDEX_ROLE, "ldIndex"  },
            { LN_INDEX_ROLE, "lnIndex"  },
            { NAME_ROLE,     "name"     },
            { MOD_ROLE,      "mod"      },
            { BEH_ROLE,      "beh"      },
            { HEALTH_ROLE,   "health"   }
        };
    }

    int LN_AllOverviewTable::rowCount(const QModelIndex &parent) const
    {
        return m_entries.size();
    }

    QVariant LN_AllOverviewTable::data(const QModelIndex &index, int role) const
    {
        if (!index.isValid() || index.row() >= m_entries.size())
            return {};

        const auto &entry = m_entries[index.row()];

        switch (role) {
        case SECTION_ROLE:
            return entry.ldName;
        case LD_INDEX_ROLE:
            return entry.ldIndex;
        case LN_INDEX_ROLE:
            return entry.lnIndex;
        case NAME_ROLE:
            return entry.ln ? entry.ln->getName() : "?";
        case MOD_ROLE:
            return entry.ln ? IEC_EnumUserInfo::mod(getInt(entry.ln->getModItem())) : QVariant();
        case BEH_ROLE:
            return entry.ln ? IEC_EnumUserInfo::beh(getInt(entry.ln->getBehItem())) : QVariant();
        case HEALTH_ROLE:
            return entry.ln ? IEC_EnumUserInfo::health(getInt(entry.ln->getHealthItem())) : QVariant();
        }

        return {};
    }

    void LN_AllOverviewTable::slotDataUpdated(Core::ModelItem::ptrList nodes)
    {
        if (!m_entries.isEmpty()) {
            emit dataChanged(index(0), index(m_entries.size() - 1),
                             { MOD_ROLE, BEH_ROLE, HEALTH_ROLE });
        }
    }

    void LN_AllOverviewTable::slotLDSelected(int ld)
    {
        for (int i = 0; i < m_entries.size(); i++) {
            if (m_entries[i].ldIndex == ld) {
                emit sigSelectRow(i);
                return;
            }
        }
    }

    void LN_AllOverviewTable::rebuildFlatList()
    {
        m_entries.clear();
        if (!m_ied)
            return;

        auto &model = m_ied->model();
        for (size_t ldIdx = 0; ldIdx < model.getItemCount(); ldIdx++) {
            auto ld = model.getLogicalDevice(ldIdx);
            if (!ld)
                continue;

            QString ldName = ld->getReference();
            for (size_t lnIdx = 0; lnIdx < ld->getItemCount(); lnIdx++) {
                auto ln = ld->getItem<Core::LogicalNode>(lnIdx);
                if (ln) {
                    if (!m_filter.isEmpty()) {
                        QString fullName = ldName + "/" + ln->getName();
                        if (!fullName.contains(m_filter, Qt::CaseInsensitive)) {
                            continue;
                        }
                    }
                    m_entries.append({
                        static_cast<int>(ldIdx),
                        static_cast<int>(lnIdx),
                        ln,
                        ldName
                    });
                }
            }
        }
    }

    void LN_AllOverviewTable::connectToUpdates(bool doConnect)
    {
        if (!doConnect) {
            for (auto &conn : m_connections) {
                disconnect(conn);
            }
            m_connections.clear();
            return;
        }

        if (!m_ied)
            return;

        auto &model = m_ied->model();
        for (size_t i = 0; i < model.getItemCount(); i++) {
            auto ld = model.getLogicalDevice(i);
            if (ld) {
                auto conn = connect(ld.get(), &Core::LogicalDevice::sigDataObjectUpdated,
                                    this, &LN_AllOverviewTable::slotDataUpdated);
                m_connections.append(conn);
            }
        }
    }
}
