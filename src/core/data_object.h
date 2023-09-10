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
	class Attribute: public TNode
	{
		QString		m_value;

	public:
		QString		value() const {
			return m_value;
		}

	public:
		Attribute(TNode *t_parent, const QString &t_name, const QString &t_value)
			: TNode(t_parent, t_name)
		{
			m_name = t_name;
			m_value = t_value;
		}
	};

	/*
	 * Representation a Data Attribute of Data Object
	 * Has important FunctionConstrain = FC
	 * */
	class DataAttribute : public TNode
	{
		QString		m_ref; // full path in the MMS tree
		QString		m_fc;
		int			m_fcNum = -1;
		QString		m_value;

	public:
		QString		ref() const {
			return m_ref;
		}
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
		DataAttribute(TNode *t_parent, const QString &t_name, const QString t_ref,
					  const QString &t_fc, int t_fcNum) : TNode(t_parent, t_name)
		{
			m_ref = t_ref;
			m_fc = t_fc;
			m_fcNum = t_fcNum;
		}

		void		update(const QString &t_value) {
			m_value = t_value;
		}
	};
	typedef std::shared_ptr< DataAttribute >	ptrDA;

	/*
	 * Representation a Data Object of Logical Node
	 * DO have several DA in different FC
	 * */
	class DataObject : public TNode
	{
		QString		m_ref; // full path in the MMS tree

		ptrDA		m_value;
		ptrDA		m_quality;
		ptrDA		m_timestamp;
		ptrDA		m_description;

	public:
		QString		ref() const {
			return m_ref;
		}

		QString		getValue() const {
			if (m_value) {
				return m_value->value();
			}
			return " - ";
		}
		QString		getQuality() const {
			if (m_quality) {
				return m_quality->value();
			}
			return " - ";
		}
		QString		getTimestamp() const {
			if (m_timestamp) {
				return m_timestamp->value();
			}
			return " - ";
		}
		QString		getDesc() const {
			if (m_description) {
				return m_description->value();
			}
			return " - ";
		}

	public:
		DataObject(TNode *t_parent, const QString &t_name, const QString &t_ref)
			: TNode(t_parent, t_name)
		{
			m_ref = t_ref;
		}

		void		addChild(std::shared_ptr< DataAttribute > t_node) {
			m_child.push_back(t_node);

			if (t_node->name() == "stVal" || t_node->name() == "mag") {
				m_value = t_node;
			}
			else if (t_node->name() == "q") {
				m_quality = t_node;
			}
			else if (t_node->name() == "t") {
				m_timestamp = t_node;
			}
			else if (t_node->name() == "d") {
				m_description = t_node;
			}
		}


	};
	typedef std::shared_ptr< DataObject >		ptrDO;
}
