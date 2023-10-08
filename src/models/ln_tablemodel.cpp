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

void LN_TableModel::setCurrentLD(int t_inx)
{
	if (m_currentLD != t_inx) {
		beginResetModel();
		m_currentLD = t_inx;
		endResetModel();

		emit sigCurrentLD();
	}
}

QVariant LN_TableModel::headerData(int t_section, Qt::Orientation t_orientation, int t_role) const
{
	switch (t_orientation) {
	case Qt::Horizontal: {
		const char* labels[] = { "Name", "Mode", "Beh", "Health" };

		return QVariant(labels[t_section % 4]);
	}
	case Qt::Vertical: {
		break;
	}
	}
	return QString::number(t_section + 1);
}

QHash<int, QByteArray> LN_TableModel::roleNames() const
{
	QHash<int, QByteArray> roles;
	roles[NameRole] = "name";
	roles[ModeRole] = "mode";
	roles[BehRole] = "beh";
	roles[HealthRole] = "health";
	return roles;
}

int LN_TableModel::rowCount(const QModelIndex &t_parent) const
{
	auto ld = m_ied->tree().getChild<Core::LogicalDevice>(m_currentLD);
	if (ld) {
		return ld->getChildCount();
	}
	return 0;
}

int LN_TableModel::columnCount(const QModelIndex &t_parent) const
{
	return 4;
}

QVariant LN_TableModel::data(const QModelIndex &t_index, int t_role) const
{
	auto ld = m_ied->tree().getChild<Core::LogicalDevice>(m_currentLD);
	if (ld) {
		auto ln = ld->getChild<Core::LogicalNode>(t_index.row());
		if (ln) {
			switch (t_role) {
			case NameRole: {
				return QVariant(ln->name());
			}
			case ModeRole: {
				return QVariant("M");
			}
			case BehRole: {
				return QVariant("B");
			}
			case HealthRole: {
				return QVariant("H");
			}
			}
		}
	}
	return QVariant(" ? ");
}