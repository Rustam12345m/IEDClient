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

#include "backend_base.hpp"
#include "models/files_model.hpp"

namespace App
{
	/*
	* Presenter of Filesystem on IED for QML page
	* */
	class FS_Backend : public BackendBase
	{
		Q_OBJECT
	public:
		FS_Backend(ConnectionObject &t_con);
		~FS_Backend() = default;

		// Properties for QML
		Q_PROPERTY(FilesTableModel* filesModel READ getFilesModel CONSTANT)
		FilesTableModel*	getFilesModel() const { return m_fsModel; }

		// API for QML
		Q_INVOKABLE QString getFS_TextStatus();
		Q_INVOKABLE void 	updateFilesDirectory(const QString &t_path);
		Q_INVOKABLE void 	downloadFile(const QString &t_filename);
		Q_INVOKABLE void 	removeFile(const QString &t_filename, int t_row);

		void 				slotNewIED() override;

	protected:
		FilesTableModel*	m_fsModel = nullptr;
	};
}