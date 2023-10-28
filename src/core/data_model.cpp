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

#include "data_model.hpp"

#include <QDebug>

namespace Core
{
	namespace
	{
		void 	printTree(QString t_prefix, QSharedPointer<Item> t_item)
		{
			auto &nodeList = t_item->getChildList();
			for (auto node : nodeList) {
				qDebug().noquote() << t_prefix << *node;

				printTree(t_prefix + "  ", node);
			}
		}
	}


	void DataModel::findModelName()
	{
		if (m_child.isEmpty()) {
			return;
		}

		QList<QString> ldNames;
		int minSize = 0;
		for (size_t i=0;i<m_child.size();i++) {
			QString name = m_child[i]->name();
			if (name.size() > minSize) {
				minSize = 0;
			}
			ldNames.push_back(name);
		}

		int inx = 0;
		for (size_t i=0;i<minSize;i++) {
			bool eq = true;
			for (size_t j=0;j<ldNames.size();j++) {
				
			}
		}
	}

	ptrLN DataModel::getLogicalNode(int t_ld, int t_ln)
	{
		auto ld = getChild<Core::LogicalDevice>(t_ld);
		if (ld) {
			return ld->getChild<Core::LogicalNode>(t_ln);
		}
		return nullptr;
	}

	ptrLD DataModel::getLogicalDevice(int t_ld)
	{
		return getChild<Core::LogicalDevice>(t_ld);
	}

	void DataModel::print()
	{
		qDebug() << "IED: " << m_name;

		auto &ldList = m_child;
		for (auto ld : ldList) {
			qDebug() << "  LD: " << *ld;
			printTree("    ", ld);
		}
	}
}