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
	class DataSetEntity : public Item
	{
	public:
		DataSetEntity(Item *t_parent, const QString &t_ref, const QString &t_fc)
			: Item(t_parent, t_ref), m_fc{t_fc}
		{}

		const QString 	ref() const { return m_name; }
		const QString 	fc() const { return m_fc; }
		ptrItem 		item() { return m_item; }

	private:
		QString 	m_fc;
		ptrItem		m_item;
	};

	/*
	 * Representation a DataSet of IED
	 * */
	class DataSet : public Item
	{
	public:
		DataSet(Item *t_parent, const QString &t_name, const QString &t_lnRef, bool t_del)
			: Item(t_parent, t_name), m_lnReference{t_lnRef}, m_isDeletable{t_del}
		{}

		const QString ref() const { return m_lnReference; }

	private:
		QString	m_lnReference;
		bool 	m_isDeletable = false;
	};

	typedef QSharedPointer< DataSet > 	ptrDataSet;
}