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

#pragma once

#include <QAbstractListModel>

#include "core/ied_tree.hpp"

class LD_ListModel : public QAbstractListModel
{
	Q_OBJECT
	enum LD_ModelRole {
		LD_ROLE_NAME = Qt::UserRole + 1
	};

	Core::IED_Tree&	m_tree;

public:
	LD_ListModel(QObject *t_parent, Core::IED_Tree &t_tree);

	int			rowCount(const QModelIndex &t_index = QModelIndex()) const override;
	QVariant	data(const QModelIndex &t_index, int t_role = Qt::DisplayRole) const override;

protected:
	QHash<int, QByteArray> roleNames() const override;

public slots:
	void		slotDataUpdated();
};
