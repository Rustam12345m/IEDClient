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

#include <QAbstractTableModel>

#include "core/ied.hpp"

namespace App::Models
{
	class ReportsTable : public QAbstractTableModel
	{
		Q_OBJECT
		enum Columns {
			NameColumn = 0,
			ModeColumn,
			BehColumn,
			HealthColumn,

			ColumnsCount
		};
	public:
		ReportsTable(QObject *t_parent, QSharedPointer<Core::IED> t_ied);

		void 	setActiveIED(QSharedPointer<Core::IED> t_ied);

		QVariant headerData(int t_section, Qt::Orientation t_orientation,
							int t_role = Qt::DisplayRole) const override;
		QHash<int, QByteArray> roleNames() const override;
		int		rowCount(const QModelIndex &t_parent = QModelIndex()) const override;
		int 	columnCount(const QModelIndex &t_parent = QModelIndex()) const override;

		QVariant data(const QModelIndex &t_index, int t_role = Qt::DisplayRole) const override;

	public slots:
		void 	slotDataUpdated();
		void 	slotRCBSelected(int t_inx);

	private:
		QSharedPointer<Core::IED> 	m_ied;
		int		m_currentRCB = -1;
	};
}