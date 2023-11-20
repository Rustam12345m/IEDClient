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

#include "all_ds_tablemodel.hpp"

AllDS_TableModel::AllDS_TableModel(QObject *t_parent, QSharedPointer<Core::IED_Object> &t_ied)
	: QAbstractTableModel(t_parent), m_ied(t_ied)
{
}

void AllDS_TableModel::setNewIED(QSharedPointer<Core::IED_Object> t_ied)
{
	beginResetModel();
	m_ied = t_ied;
	endResetModel();
}

void AllDS_TableModel::setSelectedDS(int t_ln)
{
	emit sigDSSelected(m_currentDS, t_ln);
}

QVariant AllDS_TableModel::headerData(int t_section, Qt::Orientation t_orientation, int t_role) const
{
	switch (t_orientation) {
	case Qt::Horizontal: {
		const char* labels[] = { "LD", "LN", "DS" };
		return QVariant(labels[t_section % ColumnsCount]);
	}
	case Qt::Vertical: {
		break;
	}
	}
	return QVariant();
}

QHash<int, QByteArray> AllDS_TableModel::roleNames() const
{
	return { { Qt::DisplayRole, "value" } };
}

int AllDS_TableModel::rowCount(const QModelIndex &t_parent) const
{
	/*
	auto ld = m_ied->model().getItem<Core::LogicalDevice>(m_currentDS);
	if (ld) {
		return ld->getItemCount();
	}
	*/
	return 30;
}

int AllDS_TableModel::columnCount(const QModelIndex &t_parent) const
{
	return ColumnsCount;
}

QVariant AllDS_TableModel::data(const QModelIndex &t_index, int t_role) const
{
	switch (t_index.column()) {
	case DS_LD_Column: {
		return QVariant(QString("LD %1").arg(t_index.row()));
	}
	case DS_LN_Column: {
		return QVariant(QString("LN %1").arg(t_index.row()));
	}
	case DS_NameColumn: {
		return QVariant(QString("DS %1").arg(t_index.row()));
	}
	}
	return QVariant(" ? ");
}

void AllDS_TableModel::slotDataUpdated()
{
}