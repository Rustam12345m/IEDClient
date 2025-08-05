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

#pragma once

#include "model_item_value.hpp"

#include <QList>
#include <QDebug>

namespace Core
{
    class ModelItem;
    class ModelStateUpdater;

    /**
     * @class ModelItem
     * #brief A basic class for all elements in the data model of an IED.
     *
     * The data model represents a graph with leaves as this Item. Each Item can hold other Items as leaves.
     * The Item's value must be updated either in the GUI thread using signals/slots or by locking a QMutex.
     */
    class ModelItem
    {
    public:
        using ptr = QSharedPointer< ModelItem >;
        using ptrList = QSharedPointer< QList< ModelItem* > >;

        ModelItem(ModelItem *t_parent, const QString &t_name)
            : m_parent(t_parent), m_name(t_name)
        {}
        ModelItem() = delete;
        virtual ~ModelItem() {}

        QString       getName() const {
            return m_name;
        }
        QString       getParentName() const {
            if (m_parent) {
                return m_parent->getName();
            }
            return "";
        }
        ModelItem*    getParent() const {
            return m_parent;
        }
        QString       getReference(ModelItem *t_root=nullptr) {
            // Make full reference to item in this model
            QString path;
            if (m_parent && (m_parent != t_root)) {
                path = m_parent->getReference(t_root) + m_delimetr;
            }
            return path + m_name;
        }

        auto&         getItemList() const {
            return m_items;
        }
        size_t        getItemCount() const {
            return m_items.size();
        }

        template<typename T>
        QSharedPointer< T > getItem(int t_inx) {
            if ((t_inx >= 0) && (t_inx < m_items.size())) {
                return m_items[t_inx].staticCast<T>();
            }
            return nullptr;
        }
        ModelItem::ptr getItem(int t_inx) {
            return ModelItem::getItem<ModelItem>(t_inx);
        }
        virtual QString getValue() const;

        ModelItem::ptr findSubItem(const QString &t_name) {
            for (auto it : m_items) {
                if (it->getName() == t_name) {
                    return it;
                }
            }
            return nullptr;
        }
        template <typename... Names>
        ModelItem::ptr findSubItem(const QString &t_first, Names... rest) {
            for (auto it : m_items) {
                if (it->getName() == t_first) {
                    return it->findSubItem(rest...);
                }
            }
            return nullptr;
        }

        virtual void   addSubItem(ModelItem::ptr t_child);
        virtual bool   updateValue(ModelItemValue::ptr t_newValue);

    protected:
        virtual void   notifyFromChild(ModelItem::ptrList t_nodes);

    protected:
        ModelItem*     m_parent = nullptr;
        QString        m_name;
        QString        m_delimetr = "/"; // Current node and its children
        ModelItemValue::ptr   m_value;
        QList<ModelItem::ptr> m_items; // List of children

    friend class DataModelBuilder;
    };
}
