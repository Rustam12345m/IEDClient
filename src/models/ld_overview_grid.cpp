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

#include "ld_overview_grid.hpp"

namespace App::Models
{
	LD_OverviewGrid::LD_OverviewGrid(QObject *t_parent, QSharedPointer<Core::IED_Object> t_ied)
		: QAbstractListModel(t_parent), m_ied(t_ied)
	{
	}

	void LD_OverviewGrid::setNewIED(QSharedPointer<Core::IED_Object> t_ied)
	{
		beginResetModel();
		m_ied = t_ied;
		endResetModel();
	}

	QHash<int, QByteArray> LD_OverviewGrid::roleNames() const
	{
		return { { LD_ROLE_VALUES, "ld_object" } };
	}

	int LD_OverviewGrid::rowCount(const QModelIndex &t_index) const
	{
		return m_ied->model().getItemCount();
	}

	QVariant LD_OverviewGrid::data(const QModelIndex &t_index, int t_role) const
	{
		auto ld = m_ied->model().getLogicalDevice(t_index.row());
		if (ld) {
			QVariantMap ldObj;
			ldObj["name"] = ld->getName();

			auto ln0 = ld->lln0();
			if (ln0) {
				ldObj["mod"] = ln0->getModItem() ? ln0->getModItem()->getValue() : "?";
				ldObj["beh"] = ln0->getBehItem() ? ln0->getBehItem()->getValue() : "?";
				ldObj["health"] = ln0->getHealthItem() ? ln0->getHealthItem()->getValue() : "?";;
				ldObj["sim"] = "?";
				ldObj["blk"] = "?";
			} else {
				ldObj["mod"] = "?";
				ldObj["beh"] = "?";
				ldObj["health"] = "?";
				ldObj["sim"] = "?";
				ldObj["blk"] = "?";
			}
			return ldObj;
		}
		return QVariant(" - ");
	}

	void LD_OverviewGrid::setSelectedLD(int t_ld)
	{
		//qDebug() << "LD_OverviewGrid: Selected LD = " << t_ld;
		emit sigLDSelected(t_ld);
		beginResetModel();
		endResetModel();
	}

	void LD_OverviewGrid::slotDataUpdated(bool t_status)
	{
		beginResetModel();
		endResetModel();
	}
}