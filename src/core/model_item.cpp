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

#include "model_item.hpp"
#include "model_values_updater.hpp"

namespace Core
{
    void ModelItem::addSubItem(ModelItem::ptr child)
    {
        m_items.push_back(child);
    }

    QString ModelItem::getValue() const
    {
        if (m_value) {
            return m_value->str();
        }
        return "";
    }

    bool ModelItem::updateValue(ModelItemValue::ptr newValue)
    {
        if (m_value && (*m_value == *newValue)) {
            return false;
        }

        m_value = newValue;

        if (m_parent != nullptr) {
            auto nodes = ModelItem::ptrList::create();
            m_parent->notifyFromChild(nodes);
        }
        return true;
    }

    void ModelItem::notifyFromChild(ModelItem::ptrList nodes)
    {
        if (m_parent != nullptr) {
            nodes->push_front(this);
            m_parent->notifyFromChild(nodes);
        }
    }
}