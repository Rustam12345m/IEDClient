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

#include "ld_settings_table.hpp"

namespace
{
    // remove: .stVal and .f
    QString removeSomeParts(const QString &name)
    {
        if (name.endsWith(".stVal")) {
            return name.first(name.size() - 6);
        } else if (name.endsWith(".f")) {
            return name.first(name.size() - 2);
        }
        return name;
    }

    inline QString orDash(const QString &v)
    {
        if (v.isEmpty() || v.startsWith("error ")) return QStringLiteral(" - ");
        return v;
    }
}

namespace App::Models
{
    LD_SettingsTable::LD_SettingsTable(QObject *parent, Core::IED::ptr ied)
        : QAbstractTableModel(parent), m_ied(ied)
    {
    }

    void LD_SettingsTable::setActiveIED(Core::IED::ptr ied)
    {
        beginResetModel();
        m_ied = ied;
        m_ldev.reset();
        endResetModel();
    }

    Core::LN_SignalMatrix::ptr LD_SettingsTable::getMatrix() const
    {
        if (!m_ldev) return nullptr;
        return m_ldev->getSettingsMatrix();
    }

    int LD_SettingsTable::rowCount(const QModelIndex &) const
    {
        auto matrix = getMatrix();
        return matrix ? matrix->size() : 0;
    }

    int LD_SettingsTable::columnCount(const QModelIndex &) const
    {
        return COLUMN_COUNT;
    }

    QVariant LD_SettingsTable::headerData(int column, Qt::Orientation orientation, int role) const
    {
        if (orientation != Qt::Horizontal) return QVariant();
        switch (column) {
        case COL_LN:      return QVariant::fromValue(SortHeaderValue("Name", true));
        case COL_REF:     return QVariant::fromValue(SortHeaderValue("Reference", true));
        case COL_FC:      return QVariant::fromValue(SortHeaderValue("FC", true));
        case COL_VALUE:   return QVariant::fromValue(SortHeaderValue("Value", true));
        case COL_DESC:    return QVariant::fromValue(SortHeaderValue("Description", false));
        }
        return QVariant("");
    }

    QHash<int, QByteArray> LD_SettingsTable::roleNames() const
    {
        return { { Qt::DisplayRole, "display" }, { ComRoles::ROLE_SORT_VALUE, "sort_value" } };
    }

    QVariant LD_SettingsTable::data(const QModelIndex &index, int role) const
    {
        int row = index.row(), col = index.column();
        auto matrix = getMatrix();
        if (!matrix) return QVariant(" ? ");

        if (role == ComRoles::ROLE_SORT_VALUE) {
            switch (col) {
            case COL_LN:      return QVariant(matrix->lnName(row));
            case COL_REF:     return QVariant(matrix->name(row));
            case COL_FC:      return QVariant(matrix->fc(row));
            case COL_VALUE:   return QVariant(orDash(matrix->value(row)));
            case COL_DESC:    return QVariant(matrix->description(row));
            }
        } else {
            switch (col) {
            case COL_LN:      return QVariant(matrix->lnName(row));
            case COL_REF:     return QVariant(removeSomeParts(matrix->name(row)));
            case COL_FC:      return QVariant(matrix->fc(row));
            case COL_VALUE:   return QVariant(orDash(matrix->value(row)));
            case COL_DESC:    return QVariant(matrix->description(row));
            }
        }
        return QVariant(" ? ");
    }

    void LD_SettingsTable::slotDataUpdated(Core::ModelItem::ptrList nodes)
    {
        if (!nodes || nodes->empty()) return;

        // At LogicalDevice level, nodes = [LN, DO, DA, ...].
        // Match any node against row base (DataObject).
        auto matrixPtr = getMatrix();
        if (!matrixPtr) return;
        auto &rows = matrixPtr->getRows();
        for (int i = 0; i < rows.size(); i++) {
            auto base = rows[i].base().get();
            for (auto *node : *nodes) {
                if (node == base) {
                    emit dataChanged(index(i, 0), index(i, COL_VALUE));
                    break;
                }
            }
        }
    }

    void LD_SettingsTable::slotLNSelected(int ld, int ln)
    {
        auto ldev = m_ied->model().getLogicalDevice(ld);
        Core::LogicalNode::ptr lnode;
        if (ldev) {
            lnode = ldev->getItem<Core::LogicalNode>(ln);
        }
        bool isLLN0 = lnode && ldev && (lnode == ldev->lln0());
        auto newLD = isLLN0 ? ldev : Core::LogicalDevice::ptr{};

        if (newLD == m_ldev) return;

        if (m_ldev) {
            disconnect(m_updConnection);
        }

        beginResetModel();
        m_ldev = newLD;
        if (m_ldev) {
            m_updConnection = connect(m_ldev.get(),
                &Core::LogicalDevice::sigDataObjectUpdated,
                this, &LD_SettingsTable::slotDataUpdated);
        }
        endResetModel();
    }
}
