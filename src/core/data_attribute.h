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
	class SubAttribute: public TNode
	{
		QString		m_value;

	public:
		QString		value() const {
			return m_value;
		}
		void 		setValue(const QString &t_value) {
			m_value = t_value;
		}

	public:
		SubAttribute(TNode *t_parent, const QString &t_name)
			: TNode(t_parent, t_name)
		{
			m_delimetr = "."; // Between DAName and SAName
		}
	};


	/*
	 * Representation a Data Attribute of Data Object
	 * Has important FunctionConstrain = FC
	 * */
	class DataAttribute : public TNode
	{
		QString		m_fc;
		int			m_fcNum = -1;
		QString		m_value;

	public:
		QString		fc() const {
			return m_fc;
		}
		int			fcNum() const {
			return m_fcNum;
		}
		QString		value() const {
			return m_value;
		}

	public:
		DataAttribute(TNode *t_parent, const QString &t_name, const QString &t_fc, int t_fcNum)
			: TNode(t_parent, t_name)
		{
			m_fc = t_fc;
			m_fcNum = t_fcNum;
			m_delimetr = "."; // Between DOName and DAName
		}

		void		update(const QString &t_value) {
			m_value = t_value;
		}
	};
	typedef QSharedPointer< DataAttribute >		ptrDA;
}
