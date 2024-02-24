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
	void ModelItem::addSubItem(QSharedPointer<ModelItem> t_child)
	{
		m_items.push_back(t_child);
	}

	QString ModelItem::getValue() const
	{
		if (m_value) {
			return m_value->str();
		}
		return "";
	}

	bool ModelItem::updateValue(ptrModelValue t_newValue)
	{
		if (m_value && (*m_value == *t_newValue)) {
			return false;
		}

		m_value = t_newValue;

		if (m_parent != nullptr) {
			auto nodes = QSharedPointer<QList<ModelItem*>>::create();
			m_parent->notifyFromChild(nodes);
		}
		return true;
	}

	void ModelItem::notifyFromChild(QSharedPointer<QList<ModelItem*>> t_nodes)
	{
		if (m_parent != nullptr) {
			t_nodes->push_front(this);
			m_parent->notifyFromChild(t_nodes);
		}
	}
}