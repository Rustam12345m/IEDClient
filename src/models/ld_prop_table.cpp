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

#include "ld_prop_table.hpp"

namespace App::Models
{
	LD_PropTable::LD_PropTable(QObject *t_parent, Core::ptrIED_Object t_ied)
		: QAbstractListModel(t_parent), m_ied{t_ied}
	{
		m_ldProp.append(PropertyItem("General information", "Name", ""));
		m_ldProp.append(PropertyItem("General information", "LN", ""));
		m_ldProp.append(PropertyItem("General information", "DS", ""));
		m_ldProp.append(PropertyItem("General information", "RCB", ""));
		m_ldProp.append(PropertyItem("General information", "GOOSE", ""));
		m_ldProp.append(PropertyItem("General information", "SV", ""));

		m_ldProp.append(PropertyItem("LLN0", "vendor", 	"NamPlt"));
		m_ldProp.append(PropertyItem("LLN0", "swRev", 	"NamPlt"));
		m_ldProp.append(PropertyItem("LLN0", "d", 		"NamPlt"));
		m_ldProp.append(PropertyItem("LLN0", "configRev", "NamPlt"));
		m_ldProp.append(PropertyItem("LLN0", "ldNs", 	"NamPlt"));

		m_ldProp.append(PropertyItem("LPHD1", "vendor", "PhyNam"));
		m_ldProp.append(PropertyItem("LPHD1", "hwRev",  "PhyNam"));
		m_ldProp.append(PropertyItem("LPHD1", "swRev",  "PhyNam"));
		m_ldProp.append(PropertyItem("LPHD1", "serNum", "PhyNam"));
		m_ldProp.append(PropertyItem("LPHD1", "model",  "PhyNam"));

		// Dev
		m_devProp.append(PropertyItem("IED information", "Vendor", ""));
		m_devProp.append(PropertyItem("IED information", "Model", ""));
		m_devProp.append(PropertyItem("IED information", "Revision", ""));
		m_devProp.append(PropertyItem("Connection", "Max PDU", ""));
	}

	void LD_PropTable::setNewIED(Core::ptrIED_Object t_ied)
	{
		beginResetModel();
		m_ied = t_ied;
		endResetModel();
	}

	QHash<int, QByteArray> LD_PropTable::roleNames() const
	{
		return { { SECTION_ROLE, "section" }, { NAME_ROLE, "name" }, { VALUE_ROLE, "value" } };
	}

	int LD_PropTable::rowCount(const QModelIndex &t_index) const
	{
		if (m_currentLD >= 0) {
			return m_ldProp.count();
		} else {
			return m_devProp.count();
		}
	}

	QVariant LD_PropTable::data(const QModelIndex &t_index, int t_role) const
	{
		if (m_currentLD >= 0) {
			return dataLD(t_index, t_role);
		} else {
			return dataIED(t_index, t_role);
		}
	}

	QVariant LD_PropTable::dataLD(const QModelIndex &t_index, int t_role) const
	{
		int row = t_index.row();
		if (row < 0 || row >= m_ldProp.size()) {
			return QVariant("");
		}

		switch (t_role) {
		case SECTION_ROLE: {
			if (m_ldProp[row].obj.isEmpty())
				return QVariant(m_ldProp[row].section);
			return QVariant(m_ldProp[row].section + " / " + m_ldProp[row].obj);
		}
		case NAME_ROLE: {
			return QVariant(m_ldProp[row].name);
		}
		case VALUE_ROLE: {
			Core::ptrLD ld = m_ied->model().getLogicalDevice(m_currentLD);
			if (ld) {
				if (m_ldProp[row].obj.isEmpty()) {
					// LD's common properties like: count of LN, DS or RCB
					if (m_ldProp[row].name == "Name") {
						return ld->name();
					} else if (m_ldProp[row].name == "LN") {
						return QVariant(QString::number(ld->getItemCount()));
					} else if (m_ldProp[row].name == "RCB") {
						return QVariant(QString("%1 B / %2 U").arg(7).arg(3));
					}
				} else {
					// DataModel LD parameters
					auto item = ld->find(m_ldProp[row].section, m_ldProp[row].obj, m_ldProp[row].name);
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

	QVariant LD_PropTable::dataIED(const QModelIndex &t_index, int t_role) const
	{
		int row = t_index.row();
		if (row < 0 || row >= m_devProp.size()) {
			return QVariant("");
		}

		switch (t_role) {
		case SECTION_ROLE: {
			return QVariant(m_devProp[row].section);
		}
		case NAME_ROLE: {
			return QVariant(m_devProp[row].name);
		}
		case VALUE_ROLE: {
			if (m_devProp[row].name == "Vendor") {
				return QVariant(m_ied->identify().m_vendor);
			} else if (m_devProp[row].name == "Model") {
				return QVariant(m_ied->identify().m_model);
			} else if (m_devProp[row].name == "Revision") {
				return QVariant(m_ied->identify().m_revision);
			} else if (m_devProp[row].name == "Max PDU") {
				return QVariant(m_ied->identify().m_maxPduSize);
			}
			break;
		}
		}
		return QVariant(" - ");
	}

	void LD_PropTable::slotLDSelected(int t_ld)
	{
		beginResetModel();
		m_currentLD = t_ld;
		endResetModel();
		//emit dataChanged(index(0, 0), index(rowCount() - 1, 0), { SECTION_ROLE, NAME_ROLE, VALUE_ROLE });
	}
}