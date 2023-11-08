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

#include "ds_table_model.hpp"

namespace {
	// remove: .stVal and .mag.f
	QString 	filterDOName(const QString &t_name)
	{
		if (t_name.endsWith(".stVal")) {
			return t_name.first(t_name.size() - 6);
		} else if (t_name.endsWith(".mag.f")) {
			return t_name.first(t_name.size() - 6);
		}
		return t_name;
	}
}

DS_TableModel::DS_TableModel(QObject *t_parent, QSharedPointer<Core::IED_Object> &t_ied)
	: QAbstractTableModel(t_parent), m_ied(t_ied)
{
}

void DS_TableModel::setNewIED(QSharedPointer<Core::IED_Object> t_ied)
{
	beginResetModel();
	m_ied = t_ied;
	endResetModel();
}

QVariant DS_TableModel::headerData(int t_column, Qt::Orientation t_orientation, int t_role) const
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

QHash<int, QByteArray> DS_TableModel::roleNames() const
{
	return { { Qt::DisplayRole, "display" } };
}

int DS_TableModel::rowCount(const QModelIndex &t_parent) const
{
	/*
	auto ln = m_ied->model().getLogicalNode(m_currentDS, m_currentLN);
	if (ln) {
		return ln->getDO_Table()->size();
	}
	return 0;
	*/
	return 7;
}

int DS_TableModel::columnCount(const QModelIndex &t_parent) const
{
	return ColumnsCount;
}

QVariant DS_TableModel::data(const QModelIndex &t_index, int t_role) const
{
	//qDebug() << "DS_TableModel: " << QString("index = %1 %2, role = %3").arg(t_index.row()).arg(t_index.column()).arg(t_role);
	int row = t_index.row(), column = t_index.column();

	switch (column) {
	case DS_REF_COLUMN: {
		return QVariant(QString("Ref_%1").arg(row));
	}
	case DS_FC_COLUMN: {
		return QVariant(QString("FC_%1").arg(row));
	}
	case DS_VALUE_COLUMN: {
		return QVariant(QString("Value_%1").arg(row));
	}
	}

	/*
	auto ln = m_ied->model().getLogicalNode(m_currentDS, m_currentLN);
	if (ln) {
		auto doTable = ln->getDO_Table();
		if (t_role == ComRoles::ROLE_SORT_VALUE) {
			// for sorting process
			switch (column) {
			case DS_REF_COLUMN: {
				return QVariant(doTable->name(row));
			}
			case DS_FC_COLUMN: {
				return QVariant(doTable->fc(row));
			}
			case DS_VALUE_COLUMN: {
				return QVariant(doTable->value(row));
			}
			}
		} else {
			// for user interface
			switch (column) {
			case DS_REF_COLUMN: {
				return QVariant(filterDOName(doTable->name(row)));
			}
			case DS_FC_COLUMN: {
				return QVariant(doTable->fc(row));
			}
			case DS_VALUE_COLUMN: {
				return QVariant(doTable->value(row));
			}
			}
		}
	}
	*/
	return QVariant(" ? ");
}

void DS_TableModel::slotDataUpdated(bool t_done)
{
}

void DS_TableModel::slotDSSelected(int t_ds)
{
	if (m_currentDS != t_ds) {
		beginResetModel();
		m_currentDS = t_ds;
		endResetModel();
	}
}