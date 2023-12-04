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

#include "ln_state_table.hpp"
#include "logical_node.hpp"

namespace Core
{
	QSharedPointer<LN_StateTable> LN_StateTableBuilder::create(QSharedPointer<LogicalNode> t_ln)
	{
		auto sigTable = QSharedPointer<LN_StateTable>::create();

		LN_StateTable::OneSignal prototype;

		recursFindSignals(sigTable, t_ln, t_ln, prototype);
		return sigTable;
	}

	void LN_StateTableBuilder::recursFindSignals(QSharedPointer<LN_StateTable> t_table,
												 QSharedPointer<Item> t_root, QSharedPointer<Item> t_item,
												 LN_StateTable::OneSignal t_prototype)
	{
		ptrItem item = t_item;

		// Find Q, TS, Desc elements
		for (size_t i=0;i<item->getItemCount();i++) {
			ptrItem child = item->getItem(i);
			if (child->name() == "q") {
				t_prototype.m_quality = child;
				continue;
			}
			if (child->name() == "t") {
				t_prototype.m_timestamp = child;
				continue;
			}
			if (child->name() == "d") {
				t_prototype.m_desc = child;
				continue;
			}
		}

		const QList<QString> FC = { "ST", "MX" };
		const QList<QString> ATTR = { "q", "t", "d" };

		// Create signals
		for (size_t i=0;i<item->getItemCount();i++) {
			ptrItem child = item->getItem(i);

			ptrDA da = child.dynamicCast<Core::DataAttribute>();
			if (da) {
				auto fc = da->fc();
				if (!FC.contains(da->fc())) {
					continue;
				}
				t_prototype.m_fc = da->fc();
			}

			if (ATTR.contains(child->name())) {
				continue;
			}

			if (child->getItemCount() == 0) {
				// Leaf
				t_prototype.m_name = child->ref(t_root.get());
				t_prototype.m_value = child;
				t_table->m_signals.emplace_back(t_prototype);
			} else {
				recursFindSignals(t_table, t_root, child, t_prototype);
			}
		}
	}
}