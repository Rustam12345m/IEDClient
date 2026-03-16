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

#include "ied_model_tree.hpp"

namespace App::Models
{
    IED_ModelTree::IED_ModelTree(QObject *t_parent, Core::IED::ptr t_ied)
        : QAbstractItemModel(t_parent), m_ied(t_ied)
    {
    }

    void IED_ModelTree::setActiveIED(Core::IED::ptr t_ied)
    {
        beginResetModel();
        m_ied = t_ied;
        endResetModel();
    }

    Core::ModelItem* IED_ModelTree::rootItem() const
    {
        if (m_ied) {
            return &m_ied->model();
        }
        return nullptr;
    }

    int IED_ModelTree::findRow(Core::ModelItem *t_item) const
    {
        Core::ModelItem *parent = t_item->getParent();
        if (!parent) {
            return 0;
        }
        const auto &siblings = parent->getItemList();
        for (int i = 0; i < siblings.size(); ++i) {
            if (siblings[i].get() == t_item) {
                return i;
            }
        }
        return 0;
    }

    QVariant IED_ModelTree::headerData(int t_sect, Qt::Orientation t_orient, int t_role) const
    {
        if (t_role != Qt::DisplayRole || t_orient != Qt::Horizontal) {
            return QVariant();
        }
        switch (t_sect) {
        case NAME_COLUMN:  return QVariant("Name");
        case VALUE_COLUMN: return QVariant("Value");
        case FC_COLUMN:    return QVariant("FC");
        }
        return QVariant();
    }

    int IED_ModelTree::rowCount(const QModelIndex &t_parent) const
    {
        if (t_parent.column() > 0) {
            return 0;
        }

        Core::ModelItem *item = nullptr;
        if (!t_parent.isValid()) {
            item = rootItem();
        } else {
            item = static_cast<Core::ModelItem*>(t_parent.internalPointer());
        }

        if (item) {
            return item->getItemCount();
        }
        return 0;
    }

    int IED_ModelTree::columnCount(const QModelIndex &t_parent) const
    {
        return COLUMN_COUNT;
    }

    QModelIndex IED_ModelTree::index(int t_row, int t_column, const QModelIndex &t_parent) const
    {
        if (!hasIndex(t_row, t_column, t_parent)) {
            return QModelIndex();
        }

        Core::ModelItem *item = nullptr;
        if (!t_parent.isValid()) {
            item = rootItem();
        } else {
            item = static_cast<Core::ModelItem*>(t_parent.internalPointer());
        }

        if (item) {
            auto node = item->getItem(t_row);
            if (node) {
                return createIndex(t_row, t_column, node.get());
            }
        }
        return QModelIndex();
    }

    QModelIndex IED_ModelTree::parent(const QModelIndex &t_index) const
    {
        if (!t_index.isValid()) {
            return QModelIndex();
        }

        auto *item = static_cast<Core::ModelItem*>(t_index.internalPointer());
        if (!item) {
            return QModelIndex();
        }

        Core::ModelItem *parent = item->getParent();
        if (!parent || parent == rootItem()) {
            return QModelIndex();
        }

        return createIndex(findRow(parent), 0, parent);
    }

    QVariant IED_ModelTree::data(const QModelIndex &t_index, int t_role) const
    {
        if (!t_index.isValid() || t_role != Qt::DisplayRole) {
            return QVariant();
        }

        auto *item = static_cast<Core::ModelItem*>(t_index.internalPointer());
        if (!item) {
            return QVariant();
        }

        switch (t_index.column()) {
        case NAME_COLUMN:
            return QVariant(item->getName());
        case FC_COLUMN: {
            auto *da = dynamic_cast<Core::DataAttribute*>(item);
            if (da) {
                return QVariant(da->fcStr());
            }
            return QVariant("");
        }
        case VALUE_COLUMN:
            return QVariant(item->getValue());
        }
        return QVariant();
    }
}
