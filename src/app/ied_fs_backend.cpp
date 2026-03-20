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

#include "ied_fs_backend.hpp"

namespace App
{
    IED_FS_Backend::IED_FS_Backend(IEDConContainer &t_con, EventStorage &t_ev)
        : BackendInterface(t_con, t_ev)
    {
        m_fsModel = new Models::IED_FileTable(this, m_con.m_ied);

        m_sortedModel = new Models::SortProxyModel(this);
        m_sortedModel->setSourceModel(m_fsModel);
    }

    QString IED_FS_Backend::fsPageStatus()
    {
        auto [count, size] = m_con.m_ied->fs().getFS_StatInfo();
        if (size < 1024 * 1024) {
            return QString("Total %1 files. %2 KB").arg(count).arg(size / 1024);
        }
        return QString("Total %1 files. %2 MB").arg(count).arg(size / (1024 * 1024));
    }

    void IED_FS_Backend::updateFilesDirectory(const QString &t_path)
    {
        auto cmd = Cmd::GetFileList::create(m_con.m_ied->fs(), t_path);
        putCmdToQueue(cmd);
    }

    void IED_FS_Backend::downloadFile(const QString &t_filename, uint32_t t_fileSize)
    {
        auto cmd = Cmd::DownloadFileCmd::create(t_filename, t_fileSize);

        connect(cmd.get(), &Cmd::DownloadFileCmd::sigDownloadProgress,
                this, &IED_FS_Backend::sigDownloadProgress);

        putCmdToQueue(cmd);
    }

    void IED_FS_Backend::removeFile(const QString &t_filename, int t_row)
    {
        // qDebug() << "IED_FS_Backend: Remove file " << t_filename;
        auto cmd = Cmd::RemoveFileCMD::create(t_filename, t_row);

        connect(cmd.get(), &Cmd::RemoveFileCMD::sigFileRemoved, m_fsModel, &Models::IED_FileTable::slotRemoveFile);
        putCmdToQueue(cmd);
    }

    void IED_FS_Backend::slotConnected(bool t_done)
    {
        m_fsModel->setActiveIED(m_con.m_ied);
    }
}