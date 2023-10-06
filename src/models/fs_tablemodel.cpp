/*
 *  main.cpp
 *
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

#include "fs_tablemodel.hpp"

#include <QDateTime>
#include <QDebug>

namespace {
	QString 	convertTimestampMsToUserString(uint64_t t_ms)
	{
		uint64_t sec = t_ms / 1000;
		QDateTime dt = QDateTime::fromSecsSinceEpoch(sec);

		return dt.toString("HH:mm:ss dd.MM.yyyy"); // ms isn't important
	}
}


FilesTableModel::FilesTableModel(QObject *t_parent, Core::FS_Tree &t_tree)
	: QAbstractTableModel(t_parent), m_tree{t_tree}
{
	connect(&m_tree, SIGNAL(sigFS_Updated()), this, SLOT(slotDataUpdated()));
}

FilesTableModel::~FilesTableModel()
{
}

int FilesTableModel::rowCount(const QModelIndex &t_parent) const
{
	return m_tree.m_dir.m_file.size();
}

int FilesTableModel::columnCount(const QModelIndex &t_parent) const
{
	return FS_COLUMN_COUNT;
}

QHash<int, QByteArray> FilesTableModel::roleNames() const
{
	return { { Qt::DisplayRole, "display"} };
}

Qt::ItemFlags FilesTableModel::flags(const QModelIndex &t_index) const
{
	return QAbstractTableModel::flags(t_index) | Qt::ItemIsSelectable;
}

QVariant FilesTableModel::headerData(int t_section, Qt::Orientation t_orientation, int t_role) const
{
	switch (t_orientation) {
	case Qt::Horizontal: {
		const char* labels[] = { "Last modification",  "Name", "Size", "Control" };
		return QVariant(labels[t_section % FS_COLUMN_COUNT]);
	}
	case Qt::Vertical: {
		break;
	}
	}
	return "[ - ]";
}

QVariant FilesTableModel::data(const QModelIndex &t_index, int t_role) const
{
	int row = t_index.row();
	if ((row >= 0) && (row < m_tree.m_dir.m_file.size())) {
		switch (t_index.column()) {
		case FS_NAME_COLUMN: {
			return m_tree.m_dir.m_file[row].m_fileName;
		}
		case FS_SIZE_COLUMN: {
			return QString::number((double)m_tree.m_dir.m_file[row].m_size / 1024, 'f', 1) + " KB";
		}
		case FS_DATE_COLUMN: {
			return convertTimestampMsToUserString(m_tree.m_dir.m_file[row].m_mts);
		}
		}
	}
	return QVariant(" - ");
}

void FilesTableModel::slotDataUpdated()
{
	beginResetModel();
	endResetModel();
}