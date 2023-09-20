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

DO_TableModel::DO_TableModel(QObject *t_parent, Core::ObjectTree &t_tree)
	: QAbstractTableModel(t_parent), m_tree{t_tree}
{
}

void DO_TableModel::setCurrentLD(int t_inx)
{
	qDebug() << "DO_TableModel: setCurrentLD " << t_inx;

	beginResetModel();
	m_currentLD = t_inx;
	endResetModel();

	emit sigChangedLD(t_inx);
}

void DO_TableModel::setCurrentLN(int t_inx)
{
	qDebug() << "DO_TableModel: setCurrentLN " << t_inx;

	beginResetModel();
	m_currentLN = t_inx;
	endResetModel();

	emit sigChangedLN(t_inx);
}

QVariant DO_TableModel::headerData(int t_section, Qt::Orientation t_orientation, int t_role) const
{
	switch (t_orientation) {
	case Qt::Horizontal: {
		const char* labels[] = { "Name", "FC", "Value", "Quality", "Timestamp", "Description" };
		return QVariant(labels[t_section % 6]);
		break;
	}
	case Qt::Vertical: {
		break;
	}
	}
	return QString::number(t_section + 1);
}

QHash<int, QByteArray> DO_TableModel::roleNames() const
{
	return { { Qt::DisplayRole, "display" } };
}

int DO_TableModel::rowCount(const QModelIndex &t_parent) const
{
	auto ld = m_tree.getChild<Core::LogicalDevice>(m_currentLD);
	if (ld) {
		auto ln = ld->getChild<Core::LogicalNode>(m_currentLN);
		if (ln) {
			return ln->getDO_Table()->size();
		}
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

	auto ln = m_tree.getLogicalNode(m_currentLD, m_currentLN);
	if (ln) {
		auto doTable = ln->getDO_Table();
		switch (column) {
		case 0: {
			return QVariant(doTable->name(row));
		}
		case 1: {
			return QVariant(doTable->fc(row));
		}
		case 2: {
			return QVariant(doTable->value(row));
		}
		case 3: {
			return QVariant(doTable->quality(row));
		}
		case 4: {
			return QVariant(doTable->timestamp(row));
		}
		case 5: {
			return QVariant(doTable->description(row));
		}
		}
	}
	return QVariant(" ? ");
}
