/*
 *  main.cpp
 *
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

#include "ld_grid_model.hpp"

LD_GridModel::LD_GridModel(QObject *t_parent, QSharedPointer<Core::IED_Object> &t_ied)
	: QAbstractListModel(t_parent), m_ied(t_ied)
{
	connect(&m_ied->model(), SIGNAL(sigUpdated()), this, SLOT(slotDataUpdated()));
}

void LD_GridModel::setNewIED(QSharedPointer<Core::IED_Object> t_ied)
{
	beginResetModel();
	m_ied = t_ied;
	connect(&m_ied->model(), SIGNAL(sigUpdated()), this, SLOT(slotDataUpdated()));
	endResetModel();
}

QHash<int, QByteArray> LD_GridModel::roleNames() const
{
	return { { LD_ROLE_NAME, "name" } };
}

int LD_GridModel::rowCount(const QModelIndex &t_index) const
{
	return m_ied->model().getChildCount();
}

QVariant LD_GridModel::data(const QModelIndex &t_index, int t_role) const
{
	auto ld = m_ied->model().getLogicalDevice(t_index.row());
	if (ld) {
		return ld->name();
	}
	return QVariant(" - ");
}

void LD_GridModel::setSelectedLD(int t_ld)
{
	//qDebug() << "LD_GridModel: Selected LD = " << t_ld;
	emit sigLDSelected(t_ld);
}

void LD_GridModel::slotDataUpdated()
{
	beginResetModel();
	endResetModel();
}