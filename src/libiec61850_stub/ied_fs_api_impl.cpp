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

#include "ied_fs_api_impl.hpp"
#include "libiec61850_adapter.hpp"

#include <QFile>

extern "C"
{
#include <iec61850_client.h>
}

namespace Libiec61850
{
    struct DownloadContext
    {
        FILE *fp = nullptr;
        IED_FS_API_Impl *api = nullptr;
        uint32_t fileSize = 0;
        uint32_t bytesTotal = 0;
        int lastPerc = 0;
    };

    namespace
    {
        bool downloadHandler(void *parameter, uint8_t *buffer, uint32_t bytesRead)
        {
            auto *ctx = static_cast<DownloadContext *>(parameter);
            if (bytesRead > 0) {
                if (fwrite(buffer, bytesRead, 1, ctx->fp) != 1) {
                    return false;
                }
                ctx->bytesTotal += bytesRead;
                if (ctx->fileSize > 0) {
                    int perc = static_cast<int>(
                        static_cast<uint64_t>(ctx->bytesTotal) * 100 / ctx->fileSize);
                    if (perc > 100) perc = 100;
                    if (perc != ctx->lastPerc) {
                        ctx->lastPerc = perc;
                        emit ctx->api->sigDownloadProgress(perc);
                    }
                }
            }
            return true;
        }
    }

    int IED_FS_API_Impl::getFileList(Core::DirOn &dir)
    {
        if (m_api.isConnected()) {
            std::string path = dir.name().toStdString();

            IedClientError retval = IED_ERROR_OK;
            LinkedList dirRoot = IedConnection_getFileDirectory(m_api.m_libConn, &retval, path.c_str());
            if (retval != IED_ERROR_OK) {
                // qDebug() << "ApiAdapter: Error getFileDirectory " << QString::fromStdString(path) << ", retval = " << retval;
                return -1;
            }

            if (dirRoot) {
                LinkedList dirEntry = LinkedList_getNext(dirRoot);

                while (dirEntry != nullptr) {
                    FileDirectoryEntry entry = (FileDirectoryEntry)dirEntry->data;

                    QString fname = QString(FileDirectoryEntry_getFileName(entry));
                    uint32_t fsize = FileDirectoryEntry_getFileSize(entry);
                    uint64_t fmodif = FileDirectoryEntry_getLastModified(entry);

                    dir.put(Core::FileOn(fname, fsize, fmodif));

                    dirEntry = LinkedList_getNext(dirEntry);
                }
            }
            LinkedList_destroyDeep(dirRoot, (LinkedListValueDeleteFunction)FileDirectoryEntry_destroy);
        }
        return 0;
    }

    bool IED_FS_API_Impl::download(const QString &filename, const QString &localPath,
                                    uint32_t fileSize)
    {
        if (!m_api.isConnected()) {
            return false;
        }

        FILE *fp = fopen(localPath.toStdString().c_str(), "wb");
        if (!fp) {
            return false;
        }

        DownloadContext ctx;
        ctx.fp = fp;
        ctx.api = this;
        ctx.fileSize = fileSize;

        IedClientError error = IED_ERROR_OK;
        IedConnection_getFile(m_api.m_libConn, &error, filename.toStdString().c_str(),
                              downloadHandler, static_cast<void *>(&ctx));
        fclose(fp);

        if (error != IED_ERROR_OK) {
            QFile::remove(localPath);
            return false;
        }
        return true;
    }

    int IED_FS_API_Impl::remove(const QString &filename)
    {
        if (!m_api.isConnected()) {
            return -1;
        }

        IedClientError retval = IED_ERROR_OK;
        IedConnection_deleteFile(m_api.m_libConn, &retval, filename.toStdString().c_str());
        return (retval != IED_ERROR_OK);
    }
}