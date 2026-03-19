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

#include "download_file_cmd.hpp"

#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include <QStandardPaths>

namespace Cmd
{
    void DownloadFileCmd::execute(Cmd::Interface::IEC61850_API::ptr t_api)
    {
        qDebug() << "CMD: DownloadFileCmd" << m_filename;

        emit sigCmdEvent(CmdEvent::StartEvent("",
                QString("Download file: %1").arg(m_filename)));

        emit sigCmdEvent(CmdEvent::ProcessEvent("",
                QString("Download file: %1 in progress").arg(m_filename), 50));

        QString downloadDir = QStandardPaths::writableLocation(QStandardPaths::DownloadLocation);
        if (downloadDir.isEmpty()) {
            downloadDir = QDir::currentPath();
        }

        QString localName = QFileInfo(m_filename).fileName();
        if (localName.isEmpty()) {
            localName = m_filename;
        }
        QString localPath = downloadDir + "/" + localName;

        bool ok = t_api->fs().download(m_filename, localPath);

        if (ok) {
            emit sigCmdEvent(CmdEvent::FinishEvent("",
                    QString("Download file: saved to %1").arg(localPath), true));
        } else {
            emit sigCmdEvent(CmdEvent::FinishEvent("",
                    QString("Download file: failed to download %1").arg(m_filename), false));
        }
    }
}
