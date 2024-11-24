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

extern "C"
{
#include <iec61850_client.h>
}

namespace Libiec61850
{
    namespace 
    {
        bool     getFileAsyncHandler(uint32_t invokeId, void* parameter, IedClientError err, uint32_t originalInvokeId,
                                    uint8_t* buffer, uint32_t bytesRead, bool moreFollows)
        {
            if ((err != IED_ERROR_OK) || (moreFollows == false)) {
                if (err == IED_ERROR_OK) {
                    printf("Received %d bytes\n", bytesRead);
                }
                printf("File transfer complete (err:%d)\n", err);
            }
            else {
                printf("Received %d bytes\n", bytesRead);
            }
            return true;
        }
    }

    int IED_FS_API_Impl::getFileList(Core::DirOn &t_dir)
    {
        if (m_api.isConnected()) {
            std::string path = t_dir.name().toStdString();

            IedClientError retval = IED_ERROR_OK;
            LinkedList dirRoot = IedConnection_getFileDirectory(m_api.m_libConn, &retval, path.c_str());
            if (retval != IED_ERROR_OK) {
                qDebug() << "ApiAdapter: Error getFileDirectory " << QString::fromStdString(path) << ", retval = " << retval;
                return -1;
            }

            if (dirRoot) {
                LinkedList dirEntry = LinkedList_getNext(dirRoot);

                while (dirEntry != nullptr) {
                    FileDirectoryEntry entry = (FileDirectoryEntry)dirEntry->data;

                    QString fname = QString(FileDirectoryEntry_getFileName(entry));
                    uint32_t fsize = FileDirectoryEntry_getFileSize(entry);
                    uint64_t fmodif = FileDirectoryEntry_getLastModified(entry);

                    t_dir.put(Core::FileOn(fname, fsize, fmodif));

                    dirEntry = LinkedList_getNext(dirEntry);
                }
            }
            LinkedList_destroyDeep(dirRoot, (LinkedListValueDeleteFunction)FileDirectoryEntry_destroy);
        }
        return 0;
    }

    void IED_FS_API_Impl::download(const QString &t_filename)
    {
        if (!m_api.isConnected()) {
            return;
        }

        IedClientError error = IED_ERROR_OK;
        uint32_t id = IedConnection_getFileAsync(m_api.m_libConn, &error, t_filename.toStdString().c_str(),
                                                getFileAsyncHandler, nullptr);
    }

    int IED_FS_API_Impl::remove(const QString &t_filename)
    {
        if (!m_api.isConnected()) {
            return -1;
        }

        IedClientError retval = IED_ERROR_OK;
        IedConnection_deleteFile(m_api.m_libConn, &retval, t_filename.toStdString().c_str());
        return (retval != IED_ERROR_OK);
    }
}