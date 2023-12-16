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

#include "history_con_table.hpp"

namespace App::Models
{
	HistConTable::HistConTable(QObject *t_parent, App::AppSettings &t_ini)
		: QAbstractTableModel(t_parent), m_ini(t_ini)
	{
		m_con = m_ini.getDevConList();
		m_con.push_front(App::DevConInfo("Test", "192.168.0.201", 102));
	}

	QVariant HistConTable::headerData(int t_column, Qt::Orientation t_orientation, int t_role) const
	{
		const QString headerNames[] = { "№", "Name", "IP", "Port", "Last connection" };
		if (t_column >= 0 && t_column < COLUMNS_COUNT) {
			return QVariant(headerNames[t_column]);
		}
		return QVariant(" ? ");
	}

	QHash<int, QByteArray> HistConTable::roleNames() const
	{
		return { { Qt::DisplayRole, "display" } };
	}

	int HistConTable::rowCount(const QModelIndex &t_parent) const
	{
		return m_con.size();
	}

	int HistConTable::columnCount(const QModelIndex &t_parent) const
	{
		return Columns::COLUMNS_COUNT;
	}

	QVariant HistConTable::data(const QModelIndex &t_index, int t_role) const
	{
		switch (t_index.column()) {
		case Columns::NUMBER: {
			return QVariant(t_index.row() + 1);
		}
		case Columns::IED_NAME: {
			return m_con[t_index.row()].name();
		}
		case Columns::IP_ADDR : {
			return m_con[t_index.row()].ip();
		}
		case Columns::PORT_COLUMN : {
			return m_con[t_index.row()].port();
		}
		case Columns::LAST_CONNECT: {
			return QVariant("HZ");
		}
		}
		return QVariant(" ? ");
	}
}