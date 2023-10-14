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

LD_PropertyModel::LD_PropertyModel(QObject *t_parent, QSharedPointer<Core::IED_Object> &t_ied)
	: QAbstractListModel(t_parent), m_ied{t_ied}
{
	m_property.append(LD_Property("LLN0", "vendor"));
	m_property.append(LD_Property("LLN0", "swRev"));
	m_property.append(LD_Property("LLN0", "d"));
	m_property.append(LD_Property("LLN0", "configRev"));
	m_property.append(LD_Property("LLN0", "ldNs"));

	m_property.append(LD_Property("LPHD1", "vendor"));
	m_property.append(LD_Property("LPHD1", "hwRev"));
	m_property.append(LD_Property("LPHD1", "swRev"));
	m_property.append(LD_Property("LPHD1", "serNum"));
	m_property.append(LD_Property("LPHD1", "model"));
}

QHash<int, QByteArray> LD_PropertyModel::roleNames() const
{
	return { { SECTION_ROLE, "section" }, { NAME_ROLE, "name" }, { VALUE_ROLE, "value" } };
}

int LD_PropertyModel::rowCount(const QModelIndex &t_index) const
{
	return m_property.count();
}

QVariant LD_PropertyModel::data(const QModelIndex &t_index, int t_role) const
{
	int row = t_index.row();
	if (row < 0 || row >= m_property.size()) {
		return QVariant();
	}

	switch (t_role) {
	case SECTION_ROLE: {
		return QVariant(m_property[row].section);
	}
	case NAME_ROLE: {
		return QVariant(m_property[row].name);
	}
	case VALUE_ROLE: {
		return QVariant(QString("LD = %1; Row = %2").arg(m_currentLD).arg(row));
	}
	}
	return QVariant();
}

void LD_PropertyModel::slotLDSelected(int t_ld)
{
	m_currentLD = t_ld;
	emit dataChanged(index(0, 0), index(rowCount() - 1, 0), { VALUE_ROLE });
}