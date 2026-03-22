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

        LN_AllOverviewTable(QObject *parent, Core::IED::ptr ied);

        void setActiveIED(Core::IED::ptr ied);

        Q_INVOKABLE void selectLN(int row);
        Q_INVOKABLE void setFilter(const QString &text);

        QHash<int, QByteArray> roleNames() const override;
        int rowCount(const QModelIndex &parent = QModelIndex()) const override;
        QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    signals:
        void sigLNSelected(int ld, int ln);
        void sigSelectRow(int row);

    public slots:
        void slotDataUpdated(Core::ModelItem::ptrList nodes);
        void slotLDSelected(int ld);

    private:
        struct FlatEntry
        {
            int                     ldIndex;
            int                     lnIndex;
            Core::LogicalNode::ptr  ln;
            QString                 ldName;
        };

        void rebuildFlatList();
        void connectToUpdates(bool doConnect);

        Core::IED::ptr              m_ied;
        QList<FlatEntry>            m_entries;
        QString                     m_filter;
        QList<QMetaObject::Connection> m_connections;
    };
}
