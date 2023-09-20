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
#include <QDebug>

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
	return 3;
}

QHash<int, QByteArray> FilesTableModel::roleNames() const
{
	return { { FS_ROLE_NAME, "name" }, { FS_ROLE_SIZE, "size" }, { FS_ROLE_MTS, "mts" } };
}

Qt::ItemFlags FilesTableModel::flags(const QModelIndex &t_index) const
{
	return QAbstractTableModel::flags(t_index) | Qt::ItemIsSelectable;
}

QVariant FilesTableModel::headerData(int t_section, Qt::Orientation t_orientation, int t_role) const
{
	switch (t_orientation) {
	case Qt::Horizontal: {
		const char* labels[] = { "File", "Size", "Last modification" };
		return QVariant(labels[t_section % 3]);
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

	switch (t_role) {
	case FS_ROLE_NAME: {
		if (row >= 0 && row < m_tree.m_dir.m_file.size()) {
			return m_tree.m_dir.m_file[row].m_fileName;
		}
		break;
	}
	case FS_ROLE_SIZE: {
		if (row >= 0 && row < m_tree.m_dir.m_file.size()) {
			return QString::number(m_tree.m_dir.m_file[row].m_size);
		}
		break;
	}
	case FS_ROLE_MTS: {
		if (row >= 0 && row < m_tree.m_dir.m_file.size()) {
			return QString::number(m_tree.m_dir.m_file[row].m_mts);
		}
		break;
	}
	}
	return QVariant(" - ");
}

void FilesTableModel::slotDataUpdated()
{
	beginResetModel();
	endResetModel();
}
