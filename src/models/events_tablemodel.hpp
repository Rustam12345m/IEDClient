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

class EventsTableModel : public QAbstractTableModel
{
	Q_OBJECT
public:
	explicit EventsTableModel(QObject *parent = nullptr);
	~EventsTableModel() override;

	int rowCount(const QModelIndex &t_parent = QModelIndex()) const override;
	int columnCount(const QModelIndex &t_parent = QModelIndex()) const override;

	QHash<int, QByteArray> roleNames() const override {
		return { {Qt::DisplayRole, "display"} };
	}
	Qt::ItemFlags flags(const QModelIndex &t_index) const override {
		return QAbstractTableModel::flags(t_index) | Qt::ItemIsEditable;
	}

	Q_INVOKABLE QVariant headerData(int t_section, Qt::Orientation t_orientation,
									int t_role = Qt::DisplayRole) const;

	QVariant data(const QModelIndex &t_index, int t_role = Qt::DisplayRole) const override;
	bool setData(const QModelIndex &t_index, const QVariant &t_value, int t_role = Qt::EditRole) override;
};
