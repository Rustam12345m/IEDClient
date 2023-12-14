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

#include "ds_common_table.hpp"

namespace App::Models
{
	DS_CommonLModel::DS_CommonLModel(QObject *t_parent, QSharedPointer<Core::IED_Object> t_ied)
		: QAbstractListModel(t_parent), m_ied(t_ied)
	{
	}

	void DS_CommonLModel::setNewIED(QSharedPointer<Core::IED_Object> t_ied)
	{
		beginResetModel();
		m_ied = t_ied;
		endResetModel();
	}

	void DS_CommonLModel::setSelectedDS(int t_ds)
	{
		m_currentDS = t_ds;
		emit sigDSSelected(m_currentDS);
	}

	QHash<int, QByteArray> DS_CommonLModel::roleNames() const
	{
		return { { SECTION_ROLE, "section" }, { NAME_ROLE, "name" }, { VALUE_ROLE, "value" } };
	}

	int DS_CommonLModel::rowCount(const QModelIndex &t_parent) const
	{
		return m_ied->model().dsList().count();
	}

	QVariant DS_CommonLModel::data(const QModelIndex &t_index, int t_role) const
	{
		int row = t_index.row();
		auto dsList = m_ied->model().dsList();
		if (row >= dsList.count()) {
			return QVariant(" ? ");
		}

		switch (t_role) {
		case SECTION_ROLE: {
			return QVariant(dsList[row]->parent()->ref());
		}
		case NAME_ROLE: {
			return QVariant(dsList[row]->name());
		}
		case VALUE_ROLE: {
			return QVariant(QString::number(dsList[row]->getItemCount()));
		}
		}
		return QVariant("??");
	}

	void DS_CommonLModel::slotDataUpdated()
	{
		//emit dataChanged(index(0, DS_LD_Column), index(rowCount() - 1, ColumnsCount));
	}
}