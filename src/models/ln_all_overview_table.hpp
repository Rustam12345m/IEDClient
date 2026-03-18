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

#pragma once

#include <QAbstractListModel>

#include "core/ied.hpp"

namespace App::Models
{
    /**
     * @brief Flat list model showing all LNs across all LDs, with LD section headers
     */
    class LN_AllOverviewTable : public QAbstractListModel
    {
        Q_OBJECT
    public:
        enum Roles
        {
            SECTION_ROLE = Qt::UserRole + 1,
            LD_INDEX_ROLE,
            LN_INDEX_ROLE,
            NAME_ROLE,
            MOD_ROLE,
            BEH_ROLE,
            HEALTH_ROLE
        };

        LN_AllOverviewTable(QObject *t_parent, Core::IED::ptr t_ied);

        void setActiveIED(Core::IED::ptr t_ied);

        Q_INVOKABLE void selectLN(int t_row);

        QHash<int, QByteArray> roleNames() const override;
        int rowCount(const QModelIndex &t_parent = QModelIndex()) const override;
        QVariant data(const QModelIndex &t_index, int t_role = Qt::DisplayRole) const override;

    signals:
        void sigLNSelected(int t_ld, int t_ln);

    public slots:
        void slotDataUpdated(Core::ModelItem::ptrList t_nodes);

    private:
        struct FlatEntry
        {
            int                     ldIndex;
            int                     lnIndex;
            Core::LogicalNode::ptr  ln;
            QString                 ldName;
        };

        void rebuildFlatList();
        void connectToUpdates(bool t_connect);

        Core::IED::ptr              m_ied;
        QList<FlatEntry>            m_entries;
        QList<QMetaObject::Connection> m_connections;
    };
}
