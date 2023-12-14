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

#include "ds_signals_table.hpp"

namespace App::Models
{
	DS_SignalsTable::DS_SignalsTable(QObject *t_parent, QSharedPointer<Core::IED_Object> t_ied)
		: QAbstractTableModel(t_parent), m_ied(t_ied)
	{
	}

	void DS_SignalsTable::setNewIED(QSharedPointer<Core::IED_Object> t_ied)
	{
		beginResetModel();
		m_ied = t_ied;
		endResetModel();
	}

	QVariant DS_SignalsTable::headerData(int t_column, Qt::Orientation t_orientation, int t_role) const
	{
		if (t_orientation != Qt::Horizontal) {
			return QVariant();
		}

		switch (t_column) {
		case DS_REF_COLUMN: {
			return QVariant("Reference");
		}
		case DS_FC_COLUMN: {
			return QVariant("FC");
		}
		case DS_VALUE_COLUMN: {
			return QVariant("Value");
		}
		}
		return QVariant("");
	}

	QHash<int, QByteArray> DS_SignalsTable::roleNames() const
	{
		return { { Qt::DisplayRole, "display" } };
	}

	int DS_SignalsTable::rowCount(const QModelIndex &t_parent) const
	{
		auto dsList = m_ied->model().dsList();
		if (m_currentDS >= 0 && m_currentDS < dsList.count()) {
			return dsList[m_currentDS]->getItemCount();
		}
		return 0;
	}

	int DS_SignalsTable::columnCount(const QModelIndex &t_parent) const
	{
		return ColumnsCount;
	}

	QVariant DS_SignalsTable::data(const QModelIndex &t_index, int t_role) const
	{
		int row = t_index.row(), column = t_index.column();
		auto dsList = m_ied->model().dsList();

		if (m_currentDS >= 0 && m_currentDS < dsList.count()) {
			auto dsItem = dsList[m_currentDS]->getItem<Core::DataSetEntity>(row);
			if (dsItem) {
				switch (column) {
				case DS_REF_COLUMN: {
					return QVariant(dsItem->name());
				}
				case DS_FC_COLUMN: {
					return QVariant(dsItem->fc());
				}
				case DS_VALUE_COLUMN: {
					auto item = dsItem->item();
					if (item) {
						return QVariant(item->value());
					}
					break;
				}
				}
			}
		}
		return QVariant(" ? ");
	}

	void DS_SignalsTable::slotDataUpdated(bool t_done)
	{
		emit dataChanged(index(0, DS_VALUE_COLUMN), index(rowCount() - 1, ColumnsCount));
	}

	void DS_SignalsTable::slotDSSelected(int t_ds)
	{
		if (m_currentDS != t_ds) {
			beginResetModel();
			m_currentDS = t_ds;
			endResetModel();
		}
	}
}