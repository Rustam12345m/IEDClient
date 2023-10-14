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

#include "ld_property_model.hpp"

#include <QAbstractListModel>
#include <QStringList>

LD_PropertyModel::LD_PropertyModel(QObject *t_parent) : QAbstractListModel(t_parent)
{
	addData("LLN0", "vendor", 	"Value 11");
	addData("LLN0", "swRev", 	"Value 2");
	addData("LLN0", "d", 		"Value 3");
	addData("LLN0", "configRev", "Value 4");
	addData("LLN0", "ldNs", 	"Value 5");

	addData("LPHD1", "vendor",	"Value 1");
	addData("LPHD1", "hwRev", 	"Value 22");
	addData("LPHD1", "swRev", 	"Value 3");
	addData("LPHD1", "serNum", 	"Value 4");
	addData("LPHD1", "model", 	"Value 55");
}

int LD_PropertyModel::rowCount(const QModelIndex &t_index) const
{
	return m_data.count();
}

QVariant LD_PropertyModel::data(const QModelIndex &index, int role) const
{
	if (!index.isValid()) {
		return QVariant();
	}

	const ListItem &item = m_data[index.row()];

	switch (role) {
	case SECTION_ROLE: {
		return QVariant(item.section);
	}
	case NAME_ROLE: {
		return QVariant(item.name);
	}
	case VALUE_ROLE: {
		return QVariant(item.value);
	}
	}
	return QVariant();
}

QHash<int, QByteArray> LD_PropertyModel::roleNames() const
{
	return { { SECTION_ROLE, "section" }, { NAME_ROLE, "name" }, { VALUE_ROLE, "value" } };
}

void LD_PropertyModel::addData(const QString &t_sect, const QString &t_name, const QString &t_value)
{
	beginInsertRows(QModelIndex(), rowCount(), rowCount());
	m_data.append({t_sect, t_name, t_value});
	endInsertRows();
}