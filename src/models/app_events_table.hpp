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

#include <array>

#include "cmd/cmd_event.hpp"

namespace App::Models
{
    class AppEventsTable : public QAbstractTableModel
    {
        Q_OBJECT
        enum ColumnType {
            DATE_COLUMN = 0,
            SOURCE_COLUMN,
            DESC_COLUMN,
            COLUMN_COUNT
        };

    public:
        static constexpr int Capacity = 256;

        explicit AppEventsTable(QObject *parent = nullptr);
        ~AppEventsTable() = default;

        int rowCount(const QModelIndex &parent = QModelIndex()) const override;
        int columnCount(const QModelIndex &parent = QModelIndex()) const override;

        QHash<int, QByteArray> roleNames() const override;
        Qt::ItemFlags flags(const QModelIndex &index) const override;

        Q_INVOKABLE QVariant headerData(int section, Qt::Orientation orientation,
                                        int role = Qt::DisplayRole) const override;

        QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    public slots:
        void addEvent(Cmd::CmdEvent event);

    private:
        std::array<Cmd::CmdEvent, Capacity> m_buffer{};
        int m_head = 0;
        int m_count = 0;
    };
}
