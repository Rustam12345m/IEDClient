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

#include "ied_tree.hpp"

#include <QDebug>

namespace Core
{
	void IED_Tree::printTree()
	{
		qDebug() << "IED: " << m_name;

		for (size_t i=0;i<m_child.size();i++) {
			qDebug() << "  LD: " << m_child[i]->name();

			auto &lnList = m_child[i]->getChildList();
			for (size_t j=0;j<lnList.size();j++) {
				qDebug() << "    LN: " << lnList[j]->name();

				auto &doList = lnList[j]->getChildList();
				for (size_t k=0;k<doList.size();k++) {
					qDebug() << "      DO: " << doList[k]->name();

					for (size_t z=0;z<doList[k]->getChildCount();z++) {
						auto daAttr = doList[k]->getChild<DataAttribute>(z);

						qDebug() << "        DA: " << daAttr->name() << ", FC = " << daAttr->fc();
					}
				}
			}
		}
	}
}
