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

#include "item.hpp"
#include "item_value_updater.hpp"

namespace Core
{
	void Item::addSubItem(QSharedPointer<Item> t_child)
	{
		m_items.push_back(t_child);
	}

	QString Item::getValue() const
	{
		if (m_value) {
			return m_value->str();
		}
		return "";
	}

	bool Item::updateValue(ptrValue t_newValue)
	{
		if (m_value && (*m_value == *t_newValue)) {
			return false;
		}

		m_value = t_newValue;

		if (m_parent != nullptr) {
			auto nodes = QSharedPointer<QList<Item*>>::create();
			m_parent->notifyFromChild(nodes);
		}
		return true;
	}

	void Item::notifyFromChild(QSharedPointer<QList<Item*>> t_nodes)
	{
		if (m_parent != nullptr) {
			t_nodes->push_front(this);
			m_parent->notifyFromChild(t_nodes);
		}
	}
}