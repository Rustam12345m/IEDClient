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

#include "dev_fs_table.hpp"

#include <QDateTime>
#include <QDebug>

namespace
{
	QString 	convertTimestampMsToUserString(uint64_t t_ms)
	{
		uint64_t sec = t_ms / 1000;
		QDateTime dt = QDateTime::fromSecsSinceEpoch(sec);

		return dt.toString("HH:mm:ss dd.MM.yyyy"); // ms isn't important
	}
}

namespace App::Models
{
	DevFS_Table::DevFS_Table(QObject *t_parent, QSharedPointer<Core::IED_Object> t_ied)
		: QAbstractTableModel(t_parent), m_ied(t_ied)
	{
		connect(&m_ied->fs(), SIGNAL(sigFS_Updated()), this, SLOT(slotDataUpdated()));
	}

	void DevFS_Table::setNewIED(QSharedPointer<Core::IED_Object> t_ied)
	{
		beginResetModel();
		m_ied = t_ied;
		connect(&m_ied->fs(), SIGNAL(sigFS_Updated()), this, SLOT(slotDataUpdated()));
		endResetModel();
	}

	int DevFS_Table::rowCount(const QModelIndex &t_parent) const
	{
		return m_ied->fs().m_dir.m_file.size();
	}

	int DevFS_Table::columnCount(const QModelIndex &t_parent) const
	{
		return FS_COLUMN_COUNT;
	}

	QHash<int, QByteArray> DevFS_Table::roleNames() const
	{
		return { { Qt::DisplayRole, "display"}, { ROLE_SORT_VALUE, "sort_value" } };
	}

	Qt::ItemFlags DevFS_Table::flags(const QModelIndex &t_index) const
	{
		return QAbstractTableModel::flags(t_index) | Qt::ItemIsSelectable;
	}

	QVariant DevFS_Table::headerData(int t_column, Qt::Orientation t_orientation, int t_role) const
	{
		if (t_orientation != Qt::Horizontal) {
			return QVariant();
		}

		switch (t_column) {
		case FS_INDEX_COLUMN: {
			return QVariant::fromValue(SortHeaderValue("N", true));
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

	QVariant DevFS_Table::data(const QModelIndex &t_index, int t_role) const
	{
		//qDebug() << "FS: Data " << t_index.column() << " role = " << t_role;

		int row = t_index.row();
		if ((row >= 0) && (row < m_ied->fs().m_dir.m_file.size())) {
			if (t_role == ComRoles::ROLE_SORT_VALUE) {
				// values for sorting process
				switch (t_index.column()) {
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
				switch (t_index.column()) {
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

	void DevFS_Table::slotRemoveFile(int t_row)
	{
		if (t_row < 0 || t_row >= m_ied->fs().getCount()) {
			return;
		}

		beginRemoveRows(QModelIndex(), t_row, t_row);

		// Remove from Tree
		m_ied->fs().removeFileFromList(t_row);

		endRemoveRows();
	}

	void DevFS_Table::slotDataUpdated()
	{
		beginResetModel();
		endResetModel();
		emit sigFS_Updated();
	}
}