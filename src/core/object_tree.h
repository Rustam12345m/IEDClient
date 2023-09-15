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

#include <QObject>

#include "logical_device.h"

namespace Core
{
	/*
	 * Representation of a tree available by MMS of an IED
	 * */
	class ObjectTree : public QObject, public TNode
	{
		Q_OBJECT
	public:
		ObjectTree() : TNode(nullptr, "") {
			m_delimetr = ""; // There isn't a delimetr because it is a top node
		}

		void	printTree();
		void	update() {
			emit sigUpdated();
		}

	signals:
		void	sigUpdated();
	};
}
