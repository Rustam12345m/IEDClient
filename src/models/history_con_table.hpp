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

#include "app/app_settings.hpp"

namespace App::Models
{
    class HistConTable : public QAbstractTableModel
    {
        Q_OBJECT
        enum ColumnType {
            NUMBER = 0,
            IED_NAME,
            IP_ADDR,
            PORT_COLUMN,
            LAST_CONNECT,
            COLUMNS_COUNT
        };

    public:
        HistConTable(QObject *parent, App::AppSettings &conf);

        QVariant headerData(int column, Qt::Orientation orientation,
                            int role = Qt::DisplayRole) const override;

        QHash<int, QByteArray> roleNames() const override;

        int rowCount(const QModelIndex &parent = QModelIndex()) const override;
        int columnCount(const QModelIndex &parent = QModelIndex()) const override;

        QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

        Q_INVOKABLE void removeFromHistory(int idx);

    public slots:
        void     slotAppConfigUpdated();

    protected:
        App::AppSettings&       m_settings;
        App::lisHistConnInfo    m_con;
    };
}