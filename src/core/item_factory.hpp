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

#include <QSharedPointer>

#include "ied_tree.hpp"
#include "logical_device.hpp"
#include "logical_node.hpp"
#include "data_object.hpp"

namespace Core
{
	class ItemFactory
	{
	public:
		// Object Tree <- base node for IED Data Object tree
		static QSharedPointer<Item>		createTree() {
			return QSharedPointer<IED_Tree>::create();
		}

		// Logical Device
		static QSharedPointer<Item>		createLD(Item *t_parent, const QString &t_name) {
			return QSharedPointer<LogicalDevice>::create(t_parent, t_name);
		}

		// Logical Node
		static QSharedPointer<Item>		createLN(Item *t_parent, const QString &t_name) {
			return QSharedPointer<LogicalNode>::create(t_parent, t_name);
		}

		// Data Object
		static QSharedPointer<Item>		createDO(Item *t_parent, const QString &t_name) {
			return QSharedPointer<DataObject>::create(t_parent, t_name);
		}

		// Data Attribute
		static QSharedPointer<Item>		createDA(Item *t_parent, const QString &t_name,
					  							const QString &t_fc, int t_fcNum) {
			return QSharedPointer<DataAttribute>::create(t_parent, t_name, t_fc, t_fcNum);
		}

		// Sub Attribute
		static QSharedPointer<Item>		createSA(Item *t_parent, const QString &t_name) {
			return QSharedPointer<SubAttribute>::create(t_parent, t_name);
		}
	};
}