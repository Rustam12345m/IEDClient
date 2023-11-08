/*
 *  main.cpp
 *
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

#include "ln_flat_state.hpp"
#include "logical_node.hpp"

namespace Core
{
	QString LN_FlatState::name(int t_row) const
	{
		if (t_row >= 0 && t_row < m_table.size()) {
			return m_table[t_row].m_path;
		}
		return "";
	}
	QString LN_FlatState::fc(int t_row) const
	{
		if (t_row >= 0 && t_row < m_table.size()) {
			return m_table[t_row].m_fc;
		}
		return "";
	}
	QString LN_FlatState::value(int t_row) const
	{
		if (t_row >= 0 && t_row < m_table.size()) {
			return m_table[t_row].m_do->getValue();
		}
		return "";
	}
	QString LN_FlatState::quality(int t_row) const
	{
		if (t_row >= 0 && t_row < m_table.size()) {
			return m_table[t_row].m_do->getQuality();
		}
		return "";
	}
	QString LN_FlatState::timestamp(int t_row) const
	{
		if (t_row >= 0 && t_row < m_table.size()) {
			return m_table[t_row].m_do->getTimestamp();
		}
		return "";
	}
	QString LN_FlatState::description(int t_row) const
	{
		if (t_row >= 0 && t_row < m_table.size()) {
			return m_table[t_row].m_do->getDesc();
		}
		return "";
	}

	namespace
	{
		void		DFS(const QList<QSharedPointer<Item>> &t_nodes,
						QSharedPointer<LN_FlatState> t_table,
						QSharedPointer<Item> t_root,
						QSharedPointer<DataObject> t_do,
						const QString &t_fc)
		{
			for (auto node : t_nodes) {
				auto &subNode = node->getItemList();
				if (subNode.empty()) {
					t_table->table().emplace_back(LN_FlatState::Row(node->ref(t_root.get()), t_fc, t_do));
				} else {
					DFS(subNode, t_table, t_root, t_do, t_fc);
				}
			}
		}
	}

	QSharedPointer<LN_FlatState> LN_FlatBuilder::create(QSharedPointer<LogicalNode> t_ln)
	{
		auto flatTableDO = QSharedPointer<LN_FlatState>::create(t_ln);

		for (size_t i=0;i<t_ln->getItemCount();i++) {
			auto dObj = t_ln->getItem<DataObject>(i);

			for (size_t j=0;j<dObj->getItemCount();j++) {
				auto dAttr = dObj->getItem<DataAttribute>(j);

				const QList<QString> FC = { "ST", "MX" };
				const QList<QString> ATTR = { "q", "t", "d" };

				if (ATTR.contains(dAttr->name())) {
					continue;
				}
				if (!FC.contains(dAttr->fc())) {
					continue;
				}

				if (dAttr->getItemCount() == 0) {
					flatTableDO->table().emplace_back(LN_FlatState::Row(dAttr->ref(t_ln.get()), dAttr->fc(), dObj));
				} else {
					DFS(dAttr->getItemList(), flatTableDO, t_ln, dObj, dAttr->fc());
				}
			}
		}
		return flatTableDO;
	}
}