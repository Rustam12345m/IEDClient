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

#include "data_model.hpp"

#include <QDebug>

namespace Core
{
	namespace
	{
		void 	printTree(QString t_prefix, QSharedPointer<Item> t_item)
		{
			auto &nodeList = t_item->getItemList();
			for (auto node : nodeList) {
				qDebug().noquote() << t_prefix << *node;

				printTree(t_prefix + "  ", node);
			}
		}
	}


	void DataModel::calcIEDNameFromLD()
	{
		if (m_items.isEmpty()) {
			m_name = "Undefined";
		} else if (m_items.size() == 1) {
			// We have only one LD
			m_name = m_items[0]->name().first(m_items[0]->name().size() / 2);
		} else {
			// We have more than one LD
			QList<QString> ldNames;
			size_t minSize = 0;
			for (auto ld : m_items) {
				QString name = ld->name();
				if (name.size() > minSize) {
					minSize = name.size();
				}
				ldNames.push_back(name);
			}

			for (size_t i=0;i<minSize;i++) {
				QChar letter = ldNames[0].at(i);

				for (size_t j=1;j<ldNames.size();j++) {
					if (ldNames[j].at(i) != letter) {
						m_name = ldNames[0].first(i);
						i = minSize;
						break;
					}
				}
			}
		}

		// Change LDs' names
		for (size_t i=0;i<m_items.size();i++) {
			ptrLD ld = getLogicalDevice(i);
			QString name = ld->name();
			ld->setName(name.right(name.size() - m_name.size()));
		}
	}

	ptrLN DataModel::getLogicalNode(int t_ld, int t_ln)
	{
		auto ld = getItem<Core::LogicalDevice>(t_ld);
		if (ld) {
			return ld->getItem<Core::LogicalNode>(t_ln);
		}
		return nullptr;
	}

	ptrLD DataModel::getLogicalDevice(int t_ld)
	{
		return getItem<Core::LogicalDevice>(t_ld);
	}

	void DataModel::print()
	{
		qDebug() << "IED: " << m_name;

		auto &ldList = m_items;
		for (auto ld : ldList) {
			qDebug() << "  LD: " << *ld;
			printTree("    ", ld);
		}
	}
}