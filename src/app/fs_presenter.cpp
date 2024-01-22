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

#include "fs_presenter.hpp"

namespace App
{
	FS_Backend::FS_Backend(AppConContainer &t_con) : BackendBase(t_con)
	{
		m_fsModel = new Models::DevFS_Table(this, m_con.m_iedObj);

		m_sortedModel = new Models::SortProxyModel(this);
		m_sortedModel->setSourceModel(m_fsModel);
	}

	Q_INVOKABLE QString FS_Backend::fsPageStatus()
	{
		auto [count, size] = m_con.m_iedObj->fs().getFS_StatInfo();
		if (size < 1024 * 1024) {
			return QString("Total %1 files. %2 KB").arg(count).arg(size / 1024);
		}
		return QString("Total %1 files. %2 MB").arg(count).arg(size / (1024 * 1024));
	}

	void FS_Backend::updateFilesDirectory(const QString &t_path)
	{
		auto cmd = Core::Cmd::GetFileList::create(m_con.m_iedObj->fs(), t_path);
		putCmdToQueue(cmd);
	}

	void FS_Backend::downloadFile(const QString &t_filename)
	{
		qDebug() << "FS_Backend: Download file " << t_filename;
		auto cmd = Core::Cmd::DownloadFileCmd::create(t_filename);
		putCmdToQueue(cmd);
	}

	void FS_Backend::removeFile(const QString &t_filename, int t_row)
	{
		qDebug() << "FS_Backend: Remove file " << t_filename;
		auto cmd = Core::Cmd::RemoveFile::create(t_filename, t_row);

		connect(cmd.get(), &Core::Cmd::RemoveFile::sigFileRemoved, m_fsModel, &Models::DevFS_Table::slotRemoveFile);
		putCmdToQueue(cmd);
	}

	void FS_Backend::slotConnected(bool t_done)
	{
		m_fsModel->setNewIED(m_con.m_iedObj);
	}
}