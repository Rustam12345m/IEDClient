/*
 *  main.cpp
 *
 *  Copyright 2023-2023 Rustam Mustafin
 *
 *  This file is part of IEDMaster.
 *
 *  IEDMaster is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  IEDMaster is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with IEDMaster.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  See COPYING file for the complete license text.
 * */

#include "ld_listmodel.h"

LD_ListModel::LD_ListModel(QObject *t_parent, Core::IED_Tree &t_tree)
	: QAbstractListModel(t_parent), m_tree{t_tree}
{
	connect(&m_tree, SIGNAL(sigUpdated()), this, SLOT(slotDataUpdated()));
}

int LD_ListModel::rowCount(const QModelIndex &t_index) const
{
	return m_tree.getNodeCount();
}

QVariant LD_ListModel::data(const QModelIndex &t_index, int t_role) const
{
	auto ld = m_tree.getChildPtr(t_index.row());
	if (ld) {
		return ld->name();
	}
	return QVariant(" - ");
}

QHash<int, QByteArray> LD_ListModel::roleNames() const
{
	return { {LD_ROLE_NAME, "name"} };
}

void LD_ListModel::slotDataUpdated()
{
	beginResetModel();
	endResetModel();
}
