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

#include "sub_attribute.hpp"

namespace Core
{
	/*
	 * Representation of a Data Attribute of Data Object
	 * Has important FunctionConstrain = FC
	 * */
	class DataAttribute : public ModelItem
	{
	public:
		QString		fc() const {
			return m_fc;
		}
		int			fcNum() const {
			return m_fcNum;
		}

	public:
		DataAttribute(ModelItem *t_parent, const QString &t_name, const QString &t_fc, int t_fcNum)
			: ModelItem(t_parent, t_name)
		{
			m_fc = t_fc;
			m_fcNum = t_fcNum;
			m_delimetr = "."; // Between DOName and DAName
		}
	
	protected:
		QString		m_fc;
		int			m_fcNum = -1;
	};
	typedef QSharedPointer< DataAttribute >		ptrDA;
}