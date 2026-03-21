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
    IED_ModelTree::IED_ModelTree(QObject *parent, Core::IED::ptr ied)
        : QAbstractItemModel(parent), m_ied(ied)
    {
    }

    void IED_ModelTree::setActiveIED(Core::IED::ptr ied)
    {
        beginResetModel();
        m_ied = ied;
        endResetModel();
    }

    Core::ModelItem* IED_ModelTree::rootItem() const
    {
        if (m_ied) {
            return &m_ied->model();
        }
        return nullptr;
    }

    int IED_ModelTree::findRow(Core::ModelItem *item) const
    {
        Core::ModelItem *parent = item->getParent();
        if (!parent) {
            return 0;
        }
        const auto &siblings = parent->getItemList();
        for (int i = 0; i < siblings.size(); ++i) {
            if (siblings[i].get() == item) {
                return i;
            }
        }
        return 0;
    }

    QVariant IED_ModelTree::headerData(int sect, Qt::Orientation orient, int role) const
    {
        if (role != Qt::DisplayRole || orient != Qt::Horizontal) {
            return QVariant();
        }
        switch (sect) {
        case NAME_COLUMN:  return QVariant("Name");
        case VALUE_COLUMN: return QVariant("Value");
        case FC_COLUMN:    return QVariant("FC");
        }
        return QVariant();
    }

    int IED_ModelTree::rowCount(const QModelIndex &parent) const
    {
        if (parent.column() > 0) {
            return 0;
        }

        Core::ModelItem *item = nullptr;
        if (!parent.isValid()) {
            item = rootItem();
        } else {
            item = static_cast<Core::ModelItem*>(parent.internalPointer());
        }

        if (item) {
            return item->getItemCount();
        }
        return 0;
    }

    int IED_ModelTree::columnCount(const QModelIndex &parent) const
    {
        return COLUMN_COUNT;
    }

    QModelIndex IED_ModelTree::index(int row, int column, const QModelIndex &parent) const
    {
        if (!hasIndex(row, column, parent)) {
            return QModelIndex();
        }

        Core::ModelItem *item = nullptr;
        if (!parent.isValid()) {
            item = rootItem();
        } else {
            item = static_cast<Core::ModelItem*>(parent.internalPointer());
        }

        if (item) {
            auto node = item->getItem(row);
            if (node) {
                return createIndex(row, column, node.get());
            }
        }
        return QModelIndex();
    }

    QModelIndex IED_ModelTree::parent(const QModelIndex &index) const
    {
        if (!index.isValid()) {
            return QModelIndex();
        }

        auto *item = static_cast<Core::ModelItem*>(index.internalPointer());
        if (!item) {
            return QModelIndex();
        }

        Core::ModelItem *parent = item->getParent();
        if (!parent || parent == rootItem()) {
            return QModelIndex();
        }

        return createIndex(findRow(parent), 0, parent);
    }

    QVariant IED_ModelTree::data(const QModelIndex &index, int role) const
    {
        if (!index.isValid() || role != Qt::DisplayRole) {
            return QVariant();
        }

        auto *item = static_cast<Core::ModelItem*>(index.internalPointer());
        if (!item) {
            return QVariant();
        }

        switch (index.column()) {
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
