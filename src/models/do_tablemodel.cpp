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

#include "do_tablemodel.hpp"

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

DO_TableModel::DO_TableModel(QObject *t_parent, QSharedPointer<Core::IED_Object> &t_ied)
	: QAbstractTableModel(t_parent), m_ied(t_ied)
{
}

void DO_TableModel::setNewIED(QSharedPointer<Core::IED_Object> t_ied)
{
	beginResetModel();
	m_ied = t_ied;
	endResetModel();
}

QVariant DO_TableModel::headerData(int t_column, Qt::Orientation t_orientation, int t_role) const
{
	if (t_orientation != Qt::Horizontal) {
		return QVariant();
	}

	switch (t_column) {
	case DO_NAME_COLUMN: {
		return QVariant::fromValue(SortHeaderValue("Name", true));
	}
	case DO_FC_COLUMN: {
		return QVariant::fromValue(SortHeaderValue("FC", true));
	}
	case DO_VALUE_COLUMN: {
		return QVariant::fromValue(SortHeaderValue("Value", true));
	}
	case DO_QUALITY_COLUMN: {
		return QVariant::fromValue(SortHeaderValue("Quality", true));
	}
	case DO_TS_COLUMN: {
		return QVariant::fromValue(SortHeaderValue("Timestamp", true));
	}
	case DO_DESC_COLUMN: {
		return QVariant::fromValue(SortHeaderValue("Description", false));
	}
	}
	return QVariant("");
}

QHash<int, QByteArray> DO_TableModel::roleNames() const
{
	return { { Qt::DisplayRole, "display" }, { Qt::UserRole + 1, "sort_value" } };
}

int DO_TableModel::rowCount(const QModelIndex &t_parent) const
{
	auto ln = m_ied->model().getLogicalNode(m_currentLD, m_currentLN);
	if (ln) {
		return ln->getDO_Table()->size();
	}
	return 0;
}

int DO_TableModel::columnCount(const QModelIndex &t_parent) const
{
	return 6;
}

QVariant DO_TableModel::data(const QModelIndex &t_index, int t_role) const
{
	//qDebug() << "DO_TableModel: " << QString("index = %1 %2, role = %3").arg(t_index.row()).arg(t_index.column()).arg(t_role);
	int row = t_index.row(), column = t_index.column();

	auto ln = m_ied->model().getLogicalNode(m_currentLD, m_currentLN);
	if (ln) {
		auto doTable = ln->getDO_Table();
		if (t_role == ComRoles::ROLE_SORT_VALUE) {
			// for sorting process
			switch (column) {
			case DO_NAME_COLUMN: {
				return QVariant(doTable->name(row));
			}
			case DO_FC_COLUMN: {
				return QVariant(doTable->fc(row));
			}
			case DO_VALUE_COLUMN: {
				return QVariant(doTable->value(row));
			}
			case DO_QUALITY_COLUMN: {
				return QVariant(doTable->quality(row));
			}
			case DO_TS_COLUMN: {
				return QVariant(doTable->timestamp(row));
			}
			case DO_DESC_COLUMN: {
				return QVariant(doTable->description(row));
			}
			}
		} else {
			// for user interface
			switch (column) {
			case DO_NAME_COLUMN: {
				return QVariant(filterDOName(doTable->name(row)));
			}
			case DO_FC_COLUMN: {
				return QVariant(doTable->fc(row));
			}
			case DO_VALUE_COLUMN: {
				return QVariant(doTable->value(row));
			}
			case DO_QUALITY_COLUMN: {
				return QVariant(doTable->quality(row));
			}
			case DO_TS_COLUMN: {
				return QVariant(doTable->timestamp(row));
			}
			case DO_DESC_COLUMN: {
				return QVariant(doTable->description(row));
			}
			}
		}
	}
	return QVariant(" ? ");
}

void DO_TableModel::getSelectedLN(int &t_ld, int &t_ln)
{
	t_ld = m_currentLD;
	t_ln = m_currentLN;
}

void DO_TableModel::slotDataUpdated(bool t_done)
{
	//qDebug() << "DO_TableModel: slotDataUpdated";

	emit dataChanged(index(0, DO_VALUE_COLUMN), index(rowCount() - 1, DO_TS_COLUMN));
}

void DO_TableModel::slotLNSelected(int t_ld, int t_ln)
{
	//qDebug() << "DO_TableModel: ld = " << t_ld << " ln = " << t_ln;
	if ((m_currentLD != t_ld) || (m_currentLN != t_ln)) {
		beginResetModel();
		m_currentLD = t_ld;
		m_currentLN = t_ln;
		endResetModel();
	}
}