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

#include "data_attribute.hpp"

namespace Core
{
	/*
	 * Representation a Data Object of Logical Node
	 * DO have several DA in different FC
	 * */
	class DataObject : public ModelItem
	{
	public:
		QString		getValue() const override {
			if (m_value) {
				return m_value->getValue();
			}
			return " - ";
		}
		QString		getQuality() const {
			if (m_quality) {
				return m_quality->getValue();
			}
			return " - ";
		}
		QString		getTimestamp() const {
			if (m_timestamp) {
				return m_timestamp->getValue();
			}
			return " - ";
		}
		QString		getDesc() const {
			if (m_description) {
				return m_description->getValue();
			}
			return " - ";
		}

		void		addSubItem(QSharedPointer< ModelItem > t_node) override {
			m_items.push_back(t_node);

            ptrDA da = t_node.staticCast<DataAttribute>();
            if (!da) {
                return;
            }

			if (t_node->getName() == "stVal" || t_node->getName() == "mag") {
				m_value = da;
			}
			else if (t_node->getName() == "q") {
				m_quality = da;
			}
			else if (t_node->getName() == "t") {
				m_timestamp = da;
			}
			else if (t_node->getName() == "d") {
				m_description = da;
			}
		}

	public:
		DataObject(ModelItem *t_parent, const QString &t_name)
			: ModelItem(t_parent, t_name)
		{
			m_delimetr = "."; // Between LNName and DOName
		}

	protected:
		ptrDA		m_value;
		ptrDA		m_quality;
		ptrDA		m_timestamp;
		ptrDA		m_description;
        ptrModelItem m_fc[Core::FC_ENUM::COUNT];
	};
	typedef QSharedPointer< DataObject > 	ptrDO;
}