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

#pragma once

#include <QAbstractTableModel>

#include "app/settings.hpp"

class LastConn_TableModel : public QAbstractTableModel
{
	Q_OBJECT
	enum Columns {
		IED_NAME = 0,
		IP_ADDR,
		PORT_COLUMN,
		COLUMNS_COUNT
	};
public:
	LastConn_TableModel(QObject *t_parent, AppSettings &t_ini);

	QVariant headerData(int t_section, Qt::Orientation t_orientation,
						int t_role = Qt::DisplayRole) const override;

	QHash<int, QByteArray> roleNames() const override;

	int rowCount(const QModelIndex &t_parent = QModelIndex()) const override;
	int columnCount(const QModelIndex &t_parent = QModelIndex()) const override;

	QVariant data(const QModelIndex &t_index, int t_role = Qt::DisplayRole) const override;

protected:
	AppSettings& 		m_ini;
	QList<DevConInfo> 	m_con;
};