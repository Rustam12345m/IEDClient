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

#include "ld_property_model.hpp"

LD_PropertyModel::LD_PropertyModel(QObject *t_parent, QSharedPointer<Core::IED_Object> &t_ied)
	: QAbstractListModel(t_parent), m_ied{t_ied}
{
	m_property.append(LD_Property("General information", "", "Name"));
	m_property.append(LD_Property("General information", "", "LN"));
	m_property.append(LD_Property("General information", "", "DS"));
	m_property.append(LD_Property("General information", "", "BRCB"));
	m_property.append(LD_Property("General information", "", "URCB"));
	m_property.append(LD_Property("General information", "", "GOOSE"));
	m_property.append(LD_Property("General information", "", "SV"));

	m_property.append(LD_Property("LLN0", "NamPlt", "vendor"));
	m_property.append(LD_Property("LLN0", "NamPlt", "swRev"));
	m_property.append(LD_Property("LLN0", "NamPlt", "d"));
	m_property.append(LD_Property("LLN0", "NamPlt", "configRev"));
	m_property.append(LD_Property("LLN0", "NamPlt", "ldNs"));

	m_property.append(LD_Property("LPHD1", "PhyNam", "vendor"));
	m_property.append(LD_Property("LPHD1", "PhyNam", "hwRev"));
	m_property.append(LD_Property("LPHD1", "PhyNam", "swRev"));
	m_property.append(LD_Property("LPHD1", "PhyNam", "serNum"));
	m_property.append(LD_Property("LPHD1", "PhyNam", "model"));
}

void LD_PropertyModel::setNewIED(QSharedPointer<Core::IED_Object> t_ied)
{
	beginResetModel();
	m_ied = t_ied;
	//connect(&m_ied->model(), SIGNAL(sigUpdated()), this, SLOT(slotDataUpdated()));
	endResetModel();
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
		return QVariant("");
	}

	switch (t_role) {
	case SECTION_ROLE: {
		return QVariant(m_property[row].node);
	}
	case NAME_ROLE: {
		return QVariant(m_property[row].attr);
	}
	case VALUE_ROLE: {
		Core::ptrLD ld = m_ied->model().getLogicalDevice(m_currentLD);
		if (ld) {
			if (m_property[row].obj.isEmpty()) {
				if (m_property[row].attr == "Name") {
					return ld->name();
				} else if (m_property[row].attr == "LN") {
					return QVariant(QString::number(ld->getItemCount()));
				}
			} else {
				auto item = ld->find(m_property[row].node, m_property[row].obj, m_property[row].attr);
				if (item) {
					return item->value();
				}
			}
			return " ? ";
		}
		break;
	}
	}

	return QVariant(" - ");
}

void LD_PropertyModel::slotLDSelected(int t_ld)
{
	m_currentLD = t_ld;
	emit dataChanged(index(0, 0), index(rowCount() - 1, 0), { VALUE_ROLE });
}