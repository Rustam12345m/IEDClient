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

#include "datasets_table.hpp"

namespace App::Models
{
	DataSetsTable::DataSetsTable(QObject *t_parent, QSharedPointer<Core::IED_Object> &t_ied)
		: QAbstractTableModel(t_parent), m_ied(t_ied)
	{
	}

	void DataSetsTable::setNewIED(QSharedPointer<Core::IED_Object> t_ied)
	{
		beginResetModel();
		m_ied = t_ied;
		endResetModel();
	}

	void DataSetsTable::setSelectedDS(int t_ln)
	{
		emit sigDSSelected(m_currentDS, t_ln);
	}

	QVariant DataSetsTable::headerData(int t_section, Qt::Orientation t_orientation, int t_role) const
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

	QHash<int, QByteArray> DataSetsTable::roleNames() const
	{
		return { { Qt::DisplayRole, "value" } };
	}

	int DataSetsTable::rowCount(const QModelIndex &t_parent) const
	{
		return m_ied->model().dsList().count();
	}

	int DataSetsTable::columnCount(const QModelIndex &t_parent) const
	{
		return ColumnsCount;
	}

	QVariant DataSetsTable::data(const QModelIndex &t_index, int t_role) const
	{
		auto dsList = m_ied->model().dsList();
		if (t_index.row() >= dsList.count()) {
			return QVariant(" ? ");
		}

		switch (t_index.column()) {
		case DS_LD_Column: {
			return QVariant(dsList[t_index.row()]->name());
			//return QVariant(QString("LD %1").arg(t_index.row()));
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

	void DataSetsTable::slotDataUpdated()
	{
		emit dataChanged(index(0, DS_LD_Column), index(rowCount() - 1, ColumnsCount));
	}
}