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

class RCB_MainTableModel : public QAbstractTableModel
{
	Q_OBJECT
	enum Columns {
		RCB_ENA_COLUMN = 0,
		RCB_RESV_COLUMN,
		RCB_ID_COLUMN,
		RCB_OWNER_COLUMN,
		RCB_DS_COLUMN,
		RCB_TRIG_COLUMN,
		RCB_CREV_COLUMN,
		RCB_BUFF_COLUMN,
		RCB_INTEGRITY_COLUMN,

		ColumnsCount
	};

public:
	RCB_MainTableModel(QObject *t_parent, QSharedPointer<Core::IED_Object> &t_ied);

	void 		setNewIED(QSharedPointer<Core::IED_Object> t_ied);

	QVariant headerData(int t_section, Qt::Orientation t_orientation,
						int t_role = Qt::DisplayRole) const override;
	QHash<int, QByteArray> roleNames() const override;

	int 	rowCount(const QModelIndex &t_parent = QModelIndex()) const override;
	int 	columnCount(const QModelIndex &t_parent = QModelIndex()) const override;
	QVariant data(const QModelIndex &t_index, int t_role = Qt::DisplayRole) const override;

public slots:
	void 	slotDataUpdated(bool t_done);

private:
	QSharedPointer<Core::IED_Object> m_ied;
};