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

#pragma once

#include "data_object.hpp"
#include "dataset.hpp"
#include "report_control_block.hpp"
#include "goose_control_block.hpp"
#include "sv_control_block.hpp"

#include "ln_state_table.hpp"

namespace Core
{
	/*
	 * Representation a Logical Node
	 * */
	class LogicalNode : public Item
	{
	public:
		LogicalNode(Item *t_parent, const QString &t_name) : Item(t_parent, t_name)
		{
			m_delimetr = "/"; // Between LDName and LNName
		}

		auto& 	getDO_Table() const {
			return m_doTable;
		}

		ptrDO	mod() const { return m_mod; }
		ptrDO	beh() const { return m_beh; }
		ptrDO	health() const { return m_health; }

		void	push(QSharedPointer< Item > t_child) override {
			if (t_child->name() == "Mod") {
				m_mod = t_child.dynamicCast<DataObject>();
			}
			if (t_child->name() == "Beh") {
				m_beh = t_child.dynamicCast<DataObject>();
			}
			if (t_child->name() == "Health") {
				m_health = t_child.dynamicCast<DataObject>();
			}
			Item::push(t_child);
		}

	protected:
		QSharedPointer<LN_StateTable>	m_doTable;
		QSharedPointer<DataObject>		m_mod;
		QSharedPointer<DataObject>		m_beh;
		QSharedPointer<DataObject>		m_health;

	friend class DataModelBuilder;
	};

	typedef QSharedPointer< LogicalNode >	ptrLN;
}