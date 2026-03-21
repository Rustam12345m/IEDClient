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

#include "ied_file_table.hpp"

#include <QDateTime>
#include <QDebug>

namespace
{
    QString     convertTimestampMsToUserString(uint64_t ms)
    {
        uint64_t sec = ms / 1000;
        QDateTime dt = QDateTime::fromSecsSinceEpoch(sec);

        return dt.toString("HH:mm:ss dd.MM.yyyy"); // ms isn't important
    }
}

namespace App::Models
{
    IED_FileTable::IED_FileTable(QObject *parent, Core::IED::ptr ied)
        : QAbstractTableModel(parent), m_ied(ied)
    {
        connect(&m_ied->fs(), SIGNAL(sigFS_Updated()), this, SLOT(slotDataUpdated()));
    }

    void IED_FileTable::setActiveIED(Core::IED::ptr ied)
    {
        beginResetModel();
        m_ied = ied;
        connect(&m_ied->fs(), SIGNAL(sigFS_Updated()), this, SLOT(slotDataUpdated()));
        endResetModel();
    }

    int IED_FileTable::rowCount(const QModelIndex &parent) const
    {
        return m_ied->fs().m_dir.m_file.size();
    }

    int IED_FileTable::columnCount(const QModelIndex &parent) const
    {
        return FS_COLUMN_COUNT;
    }

    QHash<int, QByteArray> IED_FileTable::roleNames() const
    {
        return { { Qt::DisplayRole, "display"}, { ROLE_SORT_VALUE, "sort_value" } };
    }

    Qt::ItemFlags IED_FileTable::flags(const QModelIndex &index) const
    {
        return QAbstractTableModel::flags(index) | Qt::ItemIsSelectable;
    }

    QVariant IED_FileTable::headerData(int column, Qt::Orientation orientation, int role) const
    {
        if (orientation != Qt::Horizontal) {
            return QVariant();
        }

        switch (column) {
        case FS_INDEX_COLUMN: {
            return QVariant::fromValue(SortHeaderValue("№", true));
        }
        case FS_DATE_COLUMN: {
            return QVariant::fromValue(SortHeaderValue("Last modification", true));
        }
        case FS_NAME_COLUMN: {
            return QVariant::fromValue(SortHeaderValue("Name", true));
        }
        case FS_SIZE_COLUMN: {
            return QVariant::fromValue(SortHeaderValue("Size", true));
        }
        case FS_FILE_COLUMN: {
            return QVariant::fromValue(SortHeaderValue("Controls", false));
        }
        }
        return QVariant("");
    }

    QVariant IED_FileTable::data(const QModelIndex &index, int role) const
    {
        //qDebug() << "FS: Data " << index.column() << " role = " << role;

        int row = index.row();
        if ((row >= 0) && (row < m_ied->fs().m_dir.m_file.size())) {
            if (role == ComRoles::ROLE_SORT_VALUE) {
                // values for sorting process
                switch (index.column()) {
                case FS_INDEX_COLUMN: {
                    return QVariant(qlonglong(row + 1));
                }
                case FS_DATE_COLUMN: {
                    return QVariant(qlonglong(m_ied->fs().m_dir.m_file[row].m_mts));
                }
                case FS_NAME_COLUMN: {
                    return m_ied->fs().m_dir.m_file[row].m_fileName;
                }
                case FS_SIZE_COLUMN: {
                    return QVariant(qlonglong(m_ied->fs().m_dir.m_file[row].m_size));
                }
                case FS_FILE_COLUMN: {
                    return "";
                }
                }
            } else {
                // for user
                switch (index.column()) {
                case FS_INDEX_COLUMN: {
                    return QString("%1").arg(row + 1);
                }
                case FS_DATE_COLUMN: {
                    return convertTimestampMsToUserString(m_ied->fs().m_dir.m_file[row].m_mts);
                }
                case FS_NAME_COLUMN: {
                    return m_ied->fs().m_dir.m_file[row].m_fileName;
                }
                case FS_SIZE_COLUMN: {
                    return QString::number((double)m_ied->fs().m_dir.m_file[row].m_size / 1024, 'f', 1) + " KB";
                }
                case FS_FILE_COLUMN: {
                    return "";
                }
                }
            }
        }
        return QVariant(" - ");
    }

    void IED_FileTable::slotRemoveFile(int row)
    {
        if (row < 0 || row >= m_ied->fs().getCount()) {
            return;
        }

        beginRemoveRows(QModelIndex(), row, row);

        // Remove from Tree
        m_ied->fs().removeFileFromList(row);

        endRemoveRows();
    }

    void IED_FileTable::slotDataUpdated()
    {
        beginResetModel();
        endResetModel();
        emit sigFS_Updated();
    }
}