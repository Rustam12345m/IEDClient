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

#include "logical_node.hpp"

namespace Core
{
    /**
     * @brief Representation a Logical Device of an IED
     */
    class LogicalDevice : public QObject, public ModelItem
    {
        Q_OBJECT
    public:
        using ptr = QSharedPointer< LogicalDevice >;

        LogicalDevice(ModelItem *t_parent, const QString &t_name) 
            : QObject(nullptr), ModelItem(t_parent, t_name)
        {
            m_delimetr = ""; // There isn't a delimetr between IEDName and LDName
        }

        LogicalNode::ptr     lln0() const { return m_lln0; }
        LogicalNode::ptr     lphd1() const { return m_lphd1; }

        void    addSubItem(ModelItem::ptr t_node) override {
            QString name = t_node->getName();
            if (name.contains("LLN0")) {
                m_lln0 = t_node.dynamicCast<LogicalNode>();
            }
            else if (name.contains("LPHD1")) {
                m_lphd1 = t_node.dynamicCast<LogicalNode>();
            }

            ModelItem::addSubItem(t_node);
        }

    signals:
        void     sigDataObjectUpdated(ModelItem::ptrList t_nodes);

    protected:
         void    notifyFromChild(ModelItem::ptrList t_nodes) override {
            emit sigDataObjectUpdated(t_nodes);
        };

    protected:
        LogicalNode::ptr    m_lln0; // LN0
        LogicalNode::ptr    m_lphd1; // LPHD1

    friend class DataModel;
    friend class DataModelBuilder;
    };
}
