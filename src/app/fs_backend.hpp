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

#include "backend_base.hpp"
#include "models/sort_proxy_model.hpp"
#include "models/dev_fs_table.hpp"

namespace App
{
	/*
	* Presenter of Filesystem on IED for QML page
	* */
	class FS_Backend : public BackendBase
	{
		Q_OBJECT
	public:
		FS_Backend(AppConContainer &t_con);
		~FS_Backend() = default;

		// Properties for QML
		Q_PROPERTY(QAbstractTableModel* 	filesModel 	READ getModel 		CONSTANT)
		Q_PROPERTY(QAbstractItemModel* 		sortModel 	READ getSortedModel CONSTANT)

		QAbstractTableModel*	getModel() const { return m_fsModel; }
		QAbstractItemModel* 	getSortedModel() const { return m_sortedModel; }

		// API for QML
		Q_INVOKABLE QString fsPageStatus();
		Q_INVOKABLE void 	updateFilesDirectory(const QString &t_path);
		Q_INVOKABLE void 	downloadFile(const QString &t_filename);
		Q_INVOKABLE void 	removeFile(const QString &t_filename, int t_row);

		void 				slotConnected(bool t_done) override;

	protected:
		Models::DevFS_Table* 	m_fsModel = nullptr;
		Models::SortProxyModel* m_sortedModel = nullptr;
	};
}