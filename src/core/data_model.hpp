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

#pragma once

#include <QObject>

#include "logical_device.hpp"

namespace Core
{
	/*
	 * Representation of a model available by MMS of an IED
	 * */
	class DataModel : public QObject, public Item
	{
		Q_OBJECT
	public:
		DataModel() : Item(nullptr, "") {
			m_delimetr = ""; // There isn't a delimetr because it is a top node
		}
		~DataModel();

		ptrLN 	getLogicalNode(int t_ld, int t_ln) {
			auto ld = getChild<Core::LogicalDevice>(t_ld);
			if (ld) {
				return ld->getChild<Core::LogicalNode>(t_ln);
			}
			return nullptr;
		}
		ptrLD 	getLogicalDevice(int t_ld) {
			return getChild<Core::LogicalDevice>(t_ld);
		}

		void	printTree();
		void	update() {
			emit sigUpdated();
		}

	signals:
		void	sigUpdated();
	};
}
