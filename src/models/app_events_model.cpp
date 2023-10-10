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

#include "app_events_model.hpp"

AppEventsModel::AppEventsModel(QObject *parent) : QAbstractTableModel{parent}
{
}

int AppEventsModel::rowCount(const QModelIndex &t_parent) const
{
	return 50;
}

int AppEventsModel::columnCount(const QModelIndex &t_parent) const
{
	return 3;
}

QHash<int, QByteArray> AppEventsModel::roleNames() const
{
	return { {Qt::DisplayRole, "display"} };
}

Qt::ItemFlags AppEventsModel::flags(const QModelIndex &t_index) const
{
	return QAbstractTableModel::flags(t_index) | Qt::ItemIsSelectable;
}

QVariant AppEventsModel::headerData(int t_section, Qt::Orientation t_orientation, int t_role) const
{
	if (t_role != Qt::DisplayRole || t_orientation != Qt::Horizontal) {
		return QVariant();
	}

	switch (t_section) {
	case DATE_COLUMN: {
		return "Date and Time";
	}
	case SOURCE_COLUMN: {
		return "Source";
	}
	case DESC_COLUMN: {
		return "Description";
	}
	}
	return QVariant("");
}

QVariant AppEventsModel::data(const QModelIndex &t_index, int t_role) const
{
	return QString("Events (%1, %2)").arg(t_index.column()).arg(t_index.row());
}