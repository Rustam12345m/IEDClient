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

#include "models_stub.hpp"
#include "core/ied_object.hpp"

namespace App::Models
{
	class LN_SignalTree : public QAbstractItemModel
	{
		Q_OBJECT
		enum Columns {
			NAME_COLUMN = 0,
			VALUE_COLUMN,
			FC_COLUMN,

			COLUMN_COUNT
		};

	public:
		LN_SignalTree(QObject *t_parent, QSharedPointer<Core::IED_Object> t_ied);

		void 	setNewIED(QSharedPointer<Core::IED_Object> t_ied);

		QVariant 	headerData(int t_sect, Qt::Orientation t_orient, int t_role = Qt::DisplayRole) const;

		int 		rowCount(const QModelIndex &t_parent = QModelIndex()) const override;
		int 		columnCount(const QModelIndex &t_parent = QModelIndex()) const override;
		QModelIndex index(int t_row, int t_column, const QModelIndex &t_parent = QModelIndex()) const override;
		QModelIndex parent(const QModelIndex &t_index) const override;
		QVariant 	data(const QModelIndex &t_index, int t_role = Qt::DisplayRole) const override;

	public slots:
		void 	slotLNSelected(int t_ld, int t_ln);
		void 	slotDataUpdated(QSharedPointer<QList<Core::Item*>> t_nodes);

	private:
		QSharedPointer<Core::IED_Object> m_ied;
		Core::ptrLN	 					 m_lnode;
		QMetaObject::Connection 		 m_updConnection; // signal from LogicalNode
	};
}