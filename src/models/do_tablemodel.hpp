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

#include <QAbstractTableModel>

#include "core/ied_tree.hpp"

class DO_TableModel : public QAbstractTableModel
{
	Q_OBJECT
	Q_PROPERTY(int currentLD READ getCurrentLD WRITE setCurrentLD NOTIFY sigChangedLD)
	Q_PROPERTY(int currentLN READ getCurrentLN WRITE setCurrentLN NOTIFY sigChangedLN)

	Core::IED_Tree&	m_tree;
	int					m_currentLD = -1; // current index of Logical Device
	int					m_currentLN = -1; // current index of Logical Node

public:
	int			getCurrentLD() const { return m_currentLD; }
	int			getCurrentLN() const { return m_currentLN; }
	void		setCurrentLD(int t_inx);
	void		setCurrentLN(int t_inx);

public:
	DO_TableModel(QObject *t_parent, Core::IED_Tree &t_tree);

	QVariant headerData(int t_section, Qt::Orientation t_orientation,
						int t_role = Qt::DisplayRole) const override;

	QHash<int, QByteArray> roleNames() const override;

	int rowCount(const QModelIndex &t_parent = QModelIndex()) const override;
	int columnCount(const QModelIndex &t_parent = QModelIndex()) const override;

	QVariant data(const QModelIndex &t_index, int t_role = Qt::DisplayRole) const override;

signals:
	void	sigChangedLD(int t_new);
	void	sigChangedLN(int t_new);
};
