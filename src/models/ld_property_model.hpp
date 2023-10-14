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

#include <QAbstractListModel>
#include <QStringList>

class LD_PropertyModel : public QAbstractListModel
{
	Q_OBJECT
	enum Roles
	{
		SECTION_ROLE = Qt::UserRole + 1,
		NAME_ROLE,
		VALUE_ROLE
	};

public:
	explicit LD_PropertyModel(QObject *t_parent = nullptr);

	int rowCount(const QModelIndex &t_index = QModelIndex()) const override;
	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
	QHash<int, QByteArray> roleNames() const override;

	void addData(const QString &t_sect, const QString &t_name, const QString &t_value);

private:
	struct ListItem
	{
		QString		section;
		QString 	name;
		QString 	value;
	};

	QList<ListItem> m_data;
};