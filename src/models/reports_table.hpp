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

#include <QAbstractTableModel>

#include "core/ied.hpp"
#include "core/report_storage.hpp"

namespace App::Models
{
    class RCB_OverviewTable;

    class ReportsTable : public QAbstractTableModel
    {
        Q_OBJECT
        enum ColumnType {
            SEQ_COLUMN = 0,
            TIMESTAMP_COLUMN,
            REASON_COLUMN,
            VALUES_COLUMN,

            COLUMN_COUNT
        };
    public:
        ReportsTable(QObject *t_parent, Core::IED::ptr t_ied);

        void     setActiveIED(Core::IED::ptr t_ied);

        QVariant headerData(int t_section, Qt::Orientation t_orientation,
                            int t_role = Qt::DisplayRole) const override;
        QHash<int, QByteArray> roleNames() const override;
        int      rowCount(const QModelIndex &t_parent = QModelIndex()) const override;
        int      columnCount(const QModelIndex &t_parent = QModelIndex()) const override;

        QVariant data(const QModelIndex &t_index, int t_role = Qt::DisplayRole) const override;

    public slots:
        void     slotDataUpdated();
        void     slotRCBSelected(int t_inx);
        void     slotReportReceived();

    private:
        void     switchStorage(Core::ReportStorage *t_storage);
        static QString reasonToString(int t_reason);
        QString  rcbRefForSender(int t_inx) const;

        Core::IED::ptr m_ied;
        Core::ReportStorage *m_storage = nullptr;
        QMetaObject::Connection m_storageConn;
    };
}
