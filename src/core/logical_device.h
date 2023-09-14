/*
 *  main.cpp
 *
 *  Copyright 2023 Rustam Mustafin
 *
 *  This file is part of IEDMaster.
 *
 *  IEDMaster is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  IEDMaster is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with IEDMaster.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  See COPYING file for the complete license text.
 * */

#pragma once

#include "logical_node.h"
#include "dataset.h"
#include "report_block.h"

namespace Core
{
	/*
	 * Representation a Logical Device of an IED
	 * */
	class LogicalDevice : public TNode
	{
		ptrLN		m_lln0;
		ptrLN		m_lphd;

	public:
		LogicalDevice(TNode *t_parent, const QString &t_name) : TNode(t_parent, t_name)
		{
		}

		void		addChild(QSharedPointer< LogicalNode > t_node) {
			m_child.push_back(t_node);

			QString name = t_node->name();
			if (name.contains("LLN0")) {
				m_lln0 = t_node;
			}
			else if (name.contains("LPHD")) {
				m_lphd = t_node;
			}
		}
	};
	typedef QSharedPointer< LogicalDevice >	ptrLD;
}
