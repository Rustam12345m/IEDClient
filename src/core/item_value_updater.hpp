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

#include "item.hpp"

namespace Core
{
	/*
	 * This class holds pointers to current Items and new values for them.
	 * To prevent race conditions, the values of Item must be updated in the GUI thread.
	 * */
	class ItemValuesUpdater
	{
	public:
		ItemValuesUpdater() {}

		void 	push(ptrItem t_item, ptrValue t_value) {
			m_values.emplace_back(t_item, t_value);
		}

		auto 	update() {
			QList<ptrItem> result; // updated items (new value)
			for (const auto&[item, value] : m_values) {
				if (item->updateValue(value)) {
					result.push_back(item);
				}
			}
			m_values.clear();
			return result;
		}

	private:
		QList< QPair<ptrItem, ptrValue> > 	m_values;
	};
	typedef QSharedPointer< ItemValuesUpdater >	ptrValuesUpdater;
}