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

#include "item.hpp"

namespace Core
{
	/*
	 * Representation of Sub-Attribute of Data Attribute
	 * simpleIOGenericIO/GGIO1.SPCSO1.stVal
	 * simpleIOGenericIO/GGIO1.AnIn1.mag.f
	 *
	 * SubAttr: f
	 * DA: stVal, mag
	 * DO: SPCSO1, AnIn1
	 * LN: GGIO1
	 * */
	class SubAttribute: public Item
	{
	public:
		SubAttribute(Item *t_parent, const QString &t_name)
			: Item(t_parent, t_name)
		{
			m_delimetr = "."; // Between DAName and SAName
		}

		QString		value() override {
			return m_value;
		}
		void 		update(const QString &t_value) override {
			m_value = t_value;
		}
	
	protected:
		void 	debug(QDebug &t_debug) const {
			t_debug << m_name << " = " << m_value;
		}

	private:
		QString		m_value;
	};
	typedef QSharedPointer<SubAttribute>	ptrSDA;
}