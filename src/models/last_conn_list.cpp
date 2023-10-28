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

#include "last_conn_list.hpp"

LastConn_TableModel::LastConn_TableModel(QObject *t_parent, App::AppSettings &t_ini)
	: QAbstractTableModel(t_parent), m_ini(t_ini)
{
	m_con = m_ini.getDevConList();
	m_con.push_front(App::DevConInfo("Test", "192.168.0.201", 102));
}

QVariant LastConn_TableModel::headerData(int t_section, Qt::Orientation t_orientation, int t_role) const
{
	switch (t_orientation) {
	case Qt::Horizontal: {
		switch (t_section) {
		case Columns::IED_NAME: {
			return QVariant("Name");
		}
		case Columns::IP_ADDR : {
			return QVariant("IP");
		}
		case Columns::PORT_COLUMN : {
			return QVariant("Port");
		}
		}
	}
	case Qt::Vertical: {
		break;
	}
	}
	return QVariant(" ? ");
}

QHash<int, QByteArray> LastConn_TableModel::roleNames() const
{
	return { { Qt::DisplayRole, "display" } };
}

int LastConn_TableModel::rowCount(const QModelIndex &t_parent) const
{
	return m_con.size();
}

int LastConn_TableModel::columnCount(const QModelIndex &t_parent) const
{
	return Columns::COLUMNS_COUNT;
}

QVariant LastConn_TableModel::data(const QModelIndex &t_index, int t_role) const
{
	switch (t_index.column()) {
	case Columns::IED_NAME: {
		return m_con[t_index.row()].name();
	}
	case Columns::IP_ADDR : {
		return m_con[t_index.row()].ip();
	}
	case Columns::PORT_COLUMN : {
		return m_con[t_index.row()].port();
	}
	}
	return QVariant(" ? ");
}