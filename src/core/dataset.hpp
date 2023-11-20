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

#include <QObject>

#include "data_object.hpp"

namespace Core
{
	class DataSetElement : public Item
	{
	public:
		DataSetElement(Item *t_parent, const QString &t_name) : Item(t_parent, t_name)
		{}
	};

	/*
	 * Representation a DataSet of LD
	 * */
	class DataSet : public QObject, public Item
	{
		Q_OBJECT
	public:
		DataSet(Item *t_parent, const QString &t_name, const QString &t_ref) : Item(t_parent, t_name)
		{
			m_ref = t_ref;
		}

		const QString ref() const { return m_ref; }

	signals:
		void	sigUpdated();

	private:
		bool 	m_isDeletable;
		QString	m_ref;
	};
}