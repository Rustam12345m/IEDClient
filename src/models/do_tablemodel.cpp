/*
 *  main.cpp
 *
 *  Copyright 2023 Rustam Mustafin
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

#include "do_tablemodel.h"

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
		const char* labels[] = { "Name", "Value", "Quality", "Timestamp", "Description" };
		return QVariant(labels[t_section % 5]);
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
	QHash<int, QByteArray> roles;
	roles[NameRole] = "name";
	roles[ValueRole] = "value";
	roles[QualityRole] = "quality";
	roles[TimestampRole] = "timestamp";
	roles[DescRole] = "desc";
	return roles;
}

int DO_TableModel::rowCount(const QModelIndex &t_parent) const
{
	auto ld = m_tree.getChild<Core::LogicalDevice>(m_currentLD);
	if (ld) {
		auto ln = ld->getChild<Core::LogicalNode>(m_currentLN);
		if (ln) {
			return ln->getTable().size();
			//return ln->getChildCount();
		}
	}
	return 0;
}

int DO_TableModel::columnCount(const QModelIndex &t_parent) const
{
	return 5;
}

QVariant DO_TableModel::data(const QModelIndex &t_index, int t_role) const
{
	auto ld = m_tree.getChild<Core::LogicalDevice>(m_currentLD);
	if (ld) {
		auto ln = ld->getChild<Core::LogicalNode>(m_currentLN);
		if (ln) {
			auto &doTable = ln->getTable();
			int row = t_index.row();
			if (row >= 0 && row < doTable.size()) {
				if (t_role == NameRole) {
					return QVariant(doTable[row].name());
				}
			}
			/*
			auto dObj = ln->getChild<Core::DataObject>(t_index.row());
			if (dObj) {
				switch (t_role) {
				case NameRole: {
					return QVariant(dObj->name());
				}
				case ValueRole: {
					return QVariant(dObj->getValue());
				}
				case QualityRole: {
					return QVariant(dObj->getQuality());
				}
				case TimestampRole: {
					return QVariant(dObj->getTimestamp());
				}
				case DescRole: {
					return QVariant(dObj->getDesc());
				}
				}
			}
			*/
		}
	}
	return QVariant(" ? ");
}
