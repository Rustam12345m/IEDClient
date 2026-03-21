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

#include "ln_common_tree.hpp"

namespace App::Models
{
    LN_CommonTree::LN_CommonTree(QObject *parent, Core::IED::ptr ied)
        : QAbstractItemModel(parent), m_ied(ied)
    {
    }

    void LN_CommonTree::setActiveIED(Core::IED::ptr ied)
    {
        beginResetModel();
        m_ied = ied;
        m_lnode.reset();
        endResetModel();
    }

    QVariant LN_CommonTree::headerData(int sect, Qt::Orientation orient, int role) const
    {
        switch (sect) {
        case NAME_COLUMN: {
            return QVariant("Name");
        }
        case FC_COLUMN: {
            return QVariant("FC");
        }
        case VALUE_COLUMN: {
            return QVariant("Value");
        }
        }
        return QVariant("");
    }

    int LN_CommonTree::rowCount(const QModelIndex &parent) const
    {
        if (parent.column() > 0) {
            return 0;
        }

        Core::ModelItem *item = nullptr;
        if (!parent.isValid()) {
            item = m_lnode.get();
        } else {
            item = static_cast<Core::ModelItem*>(parent.internalPointer());
        }

        if (item != nullptr) {
            return item->getItemCount();
        }
        return 0;
    }

    int LN_CommonTree::columnCount(const QModelIndex &parent) const
    {
        return COLUMN_COUNT;
    }

    QModelIndex LN_CommonTree::index(int row, int column, const QModelIndex &parent) const
    {
        if (!hasIndex(row, column, parent)) {
            return QModelIndex();
        }

        Core::ModelItem *item = nullptr;
        if (!parent.isValid()) {
            item = m_lnode.get();
        } else {
            item = static_cast<Core::ModelItem*>(parent.internalPointer());
        }

        if (item != nullptr) {
            auto node = item->getItem(row);
            if (node) {
                return createIndex(row, column, node.get());
            }
        }
        return QModelIndex();
    }

    QModelIndex LN_CommonTree::parent(const QModelIndex &index) const
    {
        if (!index.isValid()) {
            return QModelIndex();
        }

        auto *item = static_cast<Core::ModelItem*>(index.internalPointer());
        if (!item) {
            return QModelIndex();
        }

        Core::ModelItem *parentItem = item->getParent();
        if (!parentItem || parentItem == m_lnode.get()) {
            return QModelIndex();
        }

        // Find the row of parentItem within its own parent
        Core::ModelItem *grandParent = parentItem->getParent();
        if (!grandParent) {
            return QModelIndex();
        }

        const auto &siblings = grandParent->getItemList();
        for (int i = 0; i < siblings.size(); i++) {
            if (siblings[i].get() == parentItem) {
                return createIndex(i, 0, parentItem);
            }
        }
        return QModelIndex();
    }

    QVariant LN_CommonTree::data(const QModelIndex &index, int role) const
    {
        if (!index.isValid() || (role != Qt::DisplayRole)) {
            return QVariant();
        }

        Core::ModelItem *item = static_cast<Core::ModelItem*>(index.internalPointer());
        if (item != nullptr) {
            switch (index.column()) {
            case NAME_COLUMN: {
                return QVariant(item->getName());
            }
            case FC_COLUMN: {
                auto *da = dynamic_cast<Core::DataAttribute*>(item);
                if (da) {
                    return QVariant(da->fcStr());
                }
                return QVariant("");
            }
            case VALUE_COLUMN: {
                if (item->getItemCount() == 0) {
                    return QVariant(item->getValue());
                }
                return QVariant("");
            }
            }
        }
        return QVariant(" ? ");
    }

    void LN_CommonTree::slotDataUpdated(Core::ModelItem::ptrList nodes)
    {
        if (rowCount() > 0) {
            emit dataChanged(index(0, VALUE_COLUMN), index(rowCount() - 1, VALUE_COLUMN));
        }
    }

    void LN_CommonTree::slotLNSelected(int ld, int ln)
    {
        // qDebug() << "LN_CommonTree: ld = " << ld << " ln = " << ln;

        Core::LogicalNode::ptr lnNode = m_ied->model().getLogicalNode(ld, ln);
        if (lnNode != m_lnode) {
            if (m_lnode) {
                disconnect(m_updConnection);
            }

            beginResetModel();
            m_lnode = lnNode;
            if (m_lnode) {
                m_updConnection = connect(m_lnode.get(), &Core::LogicalNode::sigDataObjectUpdated,
                                        this, &LN_CommonTree::slotDataUpdated);
            }
            endResetModel();
        }
    }
}