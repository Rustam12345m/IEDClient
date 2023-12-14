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
	/*
	* This class represents the model for TableView which contains information
	* about files on the IED
	* */
	class DevFS_Table : public QAbstractTableModel
	{
		Q_OBJECT
		enum Columns
		{
			FS_INDEX_COLUMN = 0,
			FS_DATE_COLUMN,
			FS_NAME_COLUMN,
			FS_SIZE_COLUMN,
			FS_FILE_COLUMN,

			FS_COLUMN_COUNT
		};

	private:
		QSharedPointer<Core::IED_Object> m_ied;

	public:
		DevFS_Table(QObject *t_parent, QSharedPointer<Core::IED_Object> t_ied);
		~DevFS_Table() = default;

		void 		setNewIED(QSharedPointer<Core::IED_Object> t_ied);

		int			rowCount(const QModelIndex &t_parent = QModelIndex()) const override;
		int			columnCount(const QModelIndex &t_parent = QModelIndex()) const override;

		QHash<int, QByteArray>	roleNames() const override;
		Qt::ItemFlags			flags(const QModelIndex &t_index) const override;

		QVariant	headerData(int t_section, Qt::Orientation t_orientation,
								int t_role = Qt::DisplayRole) const;
		QVariant	data(const QModelIndex &t_index, int role = Qt::DisplayRole) const override;

	signals:
		void 		sigFS_Updated();

	public slots:
		void		slotDataUpdated();
		void 		slotRemoveFile(int t_row);
	};
}