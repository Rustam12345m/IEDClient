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

#include "events_tablemodel.h"

EventsTableModel::EventsTableModel(QObject *parent) : QAbstractTableModel{parent}
{
}

EventsTableModel::~EventsTableModel()
{
}

int EventsTableModel::rowCount(const QModelIndex &t_parent) const
{
	return 50;
}

int EventsTableModel::columnCount(const QModelIndex &t_parent) const
{
	return 3;
}

QVariant EventsTableModel::headerData(int t_section, Qt::Orientation t_orientation, int t_role) const
{
	//qDebug() << section << orientation << role;
	if (t_role != Qt::DisplayRole) {
		return QVariant();
	}

	switch (t_orientation) {
	case Qt::Horizontal: {
		const char* labels[] = { "Date and Time", "Source", "Description" };
		return QVariant(labels[t_section % 3]);
		break;
	}
	case Qt::Vertical: {
		break;
	}
	}
	return QString::number(t_section + 1);
}

QVariant EventsTableModel::data(const QModelIndex &t_index, int t_role) const
{
	//qDebug() << "EventsModel: index " << index << ", role " << role;

	switch (t_role) {
	case Qt::DisplayRole: {
		return QString("Events (%1, %2)").arg(t_index.column()).arg(t_index.row());
	}
	default: {
		break;
	}
	}

	return QVariant();
}

bool EventsTableModel::setData(const QModelIndex &t_index, const QVariant &t_value, int t_role)
{
	qDebug() << "EventsModel: setData index" << t_index << ", role " << t_role << ", value = " << t_value;
	return false;
}
