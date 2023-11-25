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

#include <QAbstractListModel>
#include <QList>

#include "core/ied_object.hpp"

namespace App::Models
{
	/*
	* ListModel with properties of selected LD
	* */
	class LD_PropTable : public QAbstractListModel
	{
		Q_OBJECT
		enum Roles
		{
			SECTION_ROLE = Qt::UserRole + 1,
			NAME_ROLE,
			VALUE_ROLE
		};
		struct LD_Property
		{
			QString		node; // LLN0 or LPHD
			QString 	obj; // DO name
			QString 	attr; // DA name

			LD_Property(const QString &t_node, const QString &t_obj, const QString &t_attr)
				: node{t_node}, obj{t_obj}, attr{t_attr} {}
		};

	public:
		LD_PropTable(QObject *t_parent, QSharedPointer<Core::IED_Object> &t_ied);

		void 	setNewIED(QSharedPointer<Core::IED_Object> t_ied);

		QHash<int, QByteArray> roleNames() const override;
		int rowCount(const QModelIndex &t_index = QModelIndex()) const override;
		QVariant data(const QModelIndex &t_index, int t_role = Qt::DisplayRole) const override;

	public slots:
		void 	slotLDSelected(int t_ld);

	private:
		QSharedPointer<Core::IED_Object> m_ied;
		QList<LD_Property> 	m_property;
		int 				m_currentLD = -1;
	};
}