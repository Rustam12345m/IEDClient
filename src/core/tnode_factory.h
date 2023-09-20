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

#include "object_tree.h"
#include "logical_device.h"
#include "logical_node.h"
#include "data_object.h"

namespace Core
{
	class TNodeFactory
	{
	public:
		// Object Tree <- base node for IED Data Object tree
		static QSharedPointer<ObjectTree>		createObjectTree() {
			return QSharedPointer<ObjectTree>::create();
		}

		// Logical Device
		static QSharedPointer<LogicalDevice>	createLD(TNode *t_parent, const QString &t_name) {
			return QSharedPointer<LogicalDevice>::create(t_parent, t_name);
		}

		// Logical Node
		static QSharedPointer<LogicalNode>		createLN(TNode *t_parent, const QString &t_name) {
			return QSharedPointer<LogicalNode>::create(t_parent, t_name);
		}

		// Data Object
		static QSharedPointer<DataObject>		createDO(TNode *t_parent, const QString &t_name) {
			return QSharedPointer<DataObject>::create(t_parent, t_name);
		}

		// Data Attribute
		static QSharedPointer<DataAttribute>	createDA(TNode *t_parent, const QString &t_name,
					  									const QString &t_fc, int t_fcNum) {
			return QSharedPointer<DataAttribute>::create(t_parent, t_name, t_fc, t_fcNum);
		}

		// Sub Attribute
		static QSharedPointer<SubAttribute>		createSA(TNode *t_parent, const QString &t_name) {
			return QSharedPointer<SubAttribute>::create(t_parent, t_name);
		}
	};
}