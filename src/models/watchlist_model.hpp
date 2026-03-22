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

namespace App::Models
{
    class WatchlistModel : public QAbstractTableModel
    {
        Q_OBJECT
    public:
        enum ColumnType
        {
            COL_LD = 0,
            COL_REFERENCE,
            COL_FC,
            COL_VALUE,
            COLUMN_COUNT
        };

        struct WatchItem
        {
            QString ref;
            QString fc;
            QString value;
        };

        explicit WatchlistModel(QObject *parent = nullptr);

        int rowCount(const QModelIndex &parent = QModelIndex()) const override;
        int columnCount(const QModelIndex &parent = QModelIndex()) const override;
        QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
        QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

        Q_INVOKABLE bool addItem(const QString &ref, const QString &fc,
                                 const QString &value = " - ");
        Q_INVOKABLE void removeItem(int row);
        Q_INVOKABLE void clear();
        Q_INVOKABLE int itemCount() const { return m_items.size(); }

        QList<WatchItem> getItems() const { return m_items; }
        void setItems(const QList<WatchItem> &items);
        void updateValues(const QVariantList &results);

    signals:
        void sigItemsChanged();

    private:
        QList<WatchItem> m_items;
    };
}
