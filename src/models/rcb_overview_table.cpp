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

#include "rcb_overview_table.hpp"

namespace App::Models
{
	RCB_OverviewTable::RCB_OverviewTable(QObject *t_parent, QSharedPointer<Core::IED> t_ied)
		: QAbstractTableModel(t_parent), m_ied(t_ied)
	{
	}

	void RCB_OverviewTable::setSelectedRCB(int t_inx)
	{
		m_currentRCB = t_inx;
		emit sigRCBSelected(m_currentRCB);
	}

	void RCB_OverviewTable::setActiveIED(QSharedPointer<Core::IED> t_ied)
	{
		beginResetModel();
		m_ied = t_ied;
		endResetModel();
	}

	QVariant RCB_OverviewTable::headerData(int t_column, Qt::Orientation t_orientation, int t_role) const
	{
		if (t_orientation != Qt::Horizontal) {
			return QVariant();
		}

		switch (t_column) {
		case RCB_ENA_COLUMN: {
			return QVariant(QString("Enabled"));
		}
		case RCB_RESV_COLUMN: {
			return QVariant(QString("Reserved"));
		}
		case RCB_ID_COLUMN: {
			return QVariant(QString("Report ID"));
		}
		case RCB_OWNER_COLUMN: {
			return QVariant(QString("Owner"));
		}
		case RCB_DS_COLUMN: {
			return QVariant(QString("DataSet"));
		}
		case RCB_TRIG_COLUMN: {
			return QVariant(QString("Triggers"));
		}
		case RCB_CREV_COLUMN: {
			return QVariant(QString("ConfRev"));
		}
		case RCB_BUFF_COLUMN: {
			return QVariant(QString("Buffer (ms)"));
		}
		case RCB_INTEGRITY_COLUMN: {
			return QVariant(QString("Integrity (ms)"));
		}
		}
		return QVariant("");
	}

	QHash<int, QByteArray> RCB_OverviewTable::roleNames() const
	{
		return { { Qt::DisplayRole, "display" } };
	}

	int RCB_OverviewTable::rowCount(const QModelIndex &t_parent) const
	{
		return 7;
	}

	int RCB_OverviewTable::columnCount(const QModelIndex &t_parent) const
	{
		return COLUMN_COUNT;
	}

	QVariant RCB_OverviewTable::data(const QModelIndex &t_index, int t_role) const
	{
		//qDebug() << "RCB_OverviewTable: " << QString("index = %1 %2, role = %3").arg(t_index.row()).arg(t_index.column()).arg(t_role);
		int row = t_index.row(), column = t_index.column();

		switch (column) {
		case RCB_ENA_COLUMN: {
			return QVariant(QString("Ena_%1").arg(row));
			break;
		}
		case RCB_RESV_COLUMN: {
			return QVariant(QString("Resv_%1").arg(row));
			break;
		}
		case RCB_ID_COLUMN: {
			return QVariant(QString("RepID_%1").arg(row));
			break;
		}
		case RCB_OWNER_COLUMN: {
			return QVariant(QString("192.168.127.%1").arg(row));
			break;
		}
		case RCB_DS_COLUMN: {
			return QVariant(QString("DS_%1").arg(row));
			break;
		}
		case RCB_TRIG_COLUMN: {
			return QVariant(QString("TRIG_%1").arg(row));
			break;
		}
		case RCB_CREV_COLUMN: {
			return QVariant(QString("CRev_%1").arg(row));
			break;
		}
		case RCB_BUFF_COLUMN: {
			return QVariant(QString("BuffTime_%1").arg(row));
			break;
		}
		case RCB_INTEGRITY_COLUMN: {
			return QVariant(QString("Integrity_%1").arg(row));
			break;
		}
		}
		return QVariant(" ? ");
	}

	void RCB_OverviewTable::slotDataUpdated(bool t_done)
	{
		emit dataChanged(index(0, RCB_ENA_COLUMN), index(rowCount() - 1, COLUMN_COUNT));
	}
}