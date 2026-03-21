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

#include "ld_properties_table.hpp"

namespace App::Models
{
    LD_PropertiesTable::LD_PropertiesTable(QObject *parent, Core::IED::ptr ied)
        : QAbstractListModel(parent), m_ied{ied}
    {
        m_ldProp.append(PropertyItem("General information", "Name", ""));
        m_ldProp.append(PropertyItem("General information", "LN", ""));
        m_ldProp.append(PropertyItem("General information", "DS", ""));
        m_ldProp.append(PropertyItem("General information", "RCB", ""));
        m_ldProp.append(PropertyItem("General information", "GOOSE", ""));
        m_ldProp.append(PropertyItem("General information", "SV", ""));

        m_ldProp.append(PropertyItem("LLN0", "vendor",     "NamPlt"));
        m_ldProp.append(PropertyItem("LLN0", "swRev",     "NamPlt"));
        m_ldProp.append(PropertyItem("LLN0", "d",         "NamPlt"));
        m_ldProp.append(PropertyItem("LLN0", "configRev", "NamPlt"));
        m_ldProp.append(PropertyItem("LLN0", "ldNs",     "NamPlt"));

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

    void LD_PropertiesTable::setActiveIED(Core::IED::ptr ied)
    {
        beginResetModel();
        m_ied = ied;
        endResetModel();
    }

    QHash<int, QByteArray> LD_PropertiesTable::roleNames() const
    {
        return { { SECTION_ROLE, "section" }, { NAME_ROLE, "name" }, { VALUE_ROLE, "value" } };
    }

    int LD_PropertiesTable::rowCount(const QModelIndex &index) const
    {
        if (m_currentLD >= 0) {
            return m_ldProp.count();
        } else {
            return m_devProp.count();
        }
    }

    QVariant LD_PropertiesTable::data(const QModelIndex &index, int role) const
    {
        if (m_currentLD >= 0) {
            return dataLD(index, role);
        } else {
            return dataIED(index, role);
        }
    }

    QVariant LD_PropertiesTable::dataLD(const QModelIndex &index, int role) const
    {
        int row = index.row();
        if (row < 0 || row >= m_ldProp.size()) {
            return QVariant("");
        }

        switch (role) {
        case SECTION_ROLE: {
            if (m_ldProp[row].obj.isEmpty())
                return QVariant(m_ldProp[row].section);
            return QVariant(m_ldProp[row].section + " / " + m_ldProp[row].obj);
        }
        case NAME_ROLE: {
            return QVariant(m_ldProp[row].name);
        }
        case VALUE_ROLE: {
            Core::LogicalDevice::ptr ld = m_ied->model().getLogicalDevice(m_currentLD);
            if (ld) {
                if (m_ldProp[row].obj.isEmpty()) {
                    // LD's common properties like: count of LN, DS or RCB
                    QString ldRef = ld->getReference();
                    if (m_ldProp[row].name == "Name") {
                        return ld->getName();
                    } else if (m_ldProp[row].name == "LN") {
                        return QVariant(QString::number(ld->getItemCount()));
                    } else if (m_ldProp[row].name == "DS") {
                        int count = 0;
                        for (const auto &ds : m_ied->model().getDataSetList())
                            if (ds->ref().startsWith(ldRef)) ++count;
                        return QVariant(QString::number(count));
                    } else if (m_ldProp[row].name == "RCB") {
                        int brcb = 0, urcb = 0;
                        for (const auto &rcb : m_ied->model().getReportCBList()) {
                            if (rcb->lnRef().startsWith(ldRef)) {
                                if (rcb->isBuffered()) ++brcb; else ++urcb;
                            }
                        }
                        return QVariant(QString("%1 B / %2 U").arg(brcb).arg(urcb));
                    } else if (m_ldProp[row].name == "GOOSE") {
                        int count = 0;
                        for (const auto &gcb : m_ied->model().getGO_CBList())
                            if (gcb->lnRef().startsWith(ldRef)) ++count;
                        return QVariant(QString::number(count));
                    } else if (m_ldProp[row].name == "SV") {
                        int count = 0;
                        for (const auto &sv : m_ied->model().getSV_CBList())
                            if (sv->lnRef().startsWith(ldRef)) ++count;
                        return QVariant(QString::number(count));
                    }
                } else {
                    // DataModel LD parameters
                    auto item = ld->findSubItem(m_ldProp[row].section, m_ldProp[row].obj, m_ldProp[row].name);
                    if (item) {
                        return item->getValue();
                    }
                }
                return " ? ";
            }
            break;
        }
        }
        return QVariant(" - ");
    }

    QVariant LD_PropertiesTable::dataIED(const QModelIndex &index, int role) const
    {
        int row = index.row();
        if (row < 0 || row >= m_devProp.size()) {
            return QVariant("");
        }

        switch (role) {
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

    void LD_PropertiesTable::slotLDSelected(int ld)
    {
        beginResetModel();
        m_currentLD = ld;
        endResetModel();
        //emit dataChanged(index(0, 0), index(rowCount() - 1, 0), { SECTION_ROLE, NAME_ROLE, VALUE_ROLE });
    }
}