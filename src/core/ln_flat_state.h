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

#include "tnode.h"

namespace Core
{
	// Forward declaration
	class DataObject;
	class LogicalNode;

	/*
	 * Representation Logical Node as a Table by particular FC or their combination
	 * 1. ST + MX + DC(d)
	 * 2. CO
	 * 3. CF + SG ?
	 * 
	 * ST + MX + DC(d):
	 * - Path relate on LN
	 * - FC
	 * - Value (stVal, mag)
	 * - Quality
	 * - Timestamp
	 * - Description
	 * */
	class LN_FlatState
	{
	public:
		struct Row
		{
			QString 					m_path;
			QSharedPointer<DataObject>	m_do;
			QString 					m_fc; // Functional constraint

			Row(const QString &t_path, const QString &t_fc, QSharedPointer<DataObject> t_do)
				: m_path{t_path}, m_fc(t_fc), m_do(t_do)
			{
			}
		};

	public:
		LN_FlatState(QSharedPointer<LogicalNode> t_ln) : m_ln(t_ln) {}

		int 		size() const {
			return m_table.size();
		}
		auto& 		table() {
			return m_table;
		}

		QString		name(int t_row) const;
		QString 	fc(int t_row) const;
		QString 	value(int t_row) const;
		QString 	quality(int t_row) const;
		QString 	timestamp(int t_row) const;
		QString 	description(int t_row) const;

	protected:
		QList<Row>					m_table;
		QSharedPointer<LogicalNode> m_ln;
	};

	class LN_FlatBuilder
	{
	public:
		static QSharedPointer<LN_FlatState> create(QSharedPointer<LogicalNode> t_ln);
	};
}
