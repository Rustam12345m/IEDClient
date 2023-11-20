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

#include "ln_tablemodel.hpp"

LN_TableModel::LN_TableModel(QObject *t_parent, QSharedPointer<Core::IED_Object> &t_ied)
	: QAbstractTableModel(t_parent), m_ied(t_ied)
{
}

void LN_TableModel::setNewIED(QSharedPointer<Core::IED_Object> t_ied)
{
	beginResetModel();
	m_ied = t_ied;
	endResetModel();
}

void LN_TableModel::setSelectedLN(int t_ln)
{
	//qDebug() << "LN_TableModel: Selected LN = " << t_ln;
	emit sigLNSelected(m_currentLD, t_ln);
}

QVariant LN_TableModel::headerData(int t_section, Qt::Orientation t_orientation, int t_role) const
{
	switch (t_orientation) {
	case Qt::Horizontal: {
		const char* labels[] = { "Name", "Mode", "Beh", "Health" };

		return QVariant(labels[t_section % ColumnsCount]);
	}
	case Qt::Vertical: {
		break;
	}
	}
	return QVariant();
}

QHash<int, QByteArray> LN_TableModel::roleNames() const
{
	return { { Qt::DisplayRole, "value" } };
}

int LN_TableModel::rowCount(const QModelIndex &t_parent) const
{
	auto ld = m_ied->model().getItem<Core::LogicalDevice>(m_currentLD);
	if (ld) {
		return ld->getItemCount();
	}
	return 0;
}

int LN_TableModel::columnCount(const QModelIndex &t_parent) const
{
	return ColumnsCount;
}

QVariant LN_TableModel::data(const QModelIndex &t_index, int t_role) const
{
	auto ln = m_ied->model().getLogicalNode(m_currentLD, t_index.row());
	if (ln) {
		switch (t_index.column()) {
		case NameColumn: {
			return QVariant(ln->name());
		}
		case ModeColumn: {
			// return QVariant("M");
			return 1;
		}
		case BehColumn: {
			// return QVariant("B");
			return 2;
		}
		case HealthColumn: {
			// return QVariant("H");
			return 3;
		}
		}
	}
	return QVariant(" ? ");
}

void LN_TableModel::slotDataUpdated()
{
	emit dataChanged(index(0, ModeColumn), index(rowCount() - 1, HealthColumn));
}

void LN_TableModel::slotLDSelected(int t_ld)
{
	if (m_currentLD != t_ld) {
		beginResetModel();
		m_currentLD = t_ld;
		endResetModel();
	}
}