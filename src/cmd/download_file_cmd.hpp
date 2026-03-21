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

#include "cmd_interface.hpp"

namespace Cmd
{
    /**
     * @brief This class realizes downloading file from the IED
     */
    class DownloadFileCmd : public CmdInterface
    {
        Q_OBJECT
    public:
        DownloadFileCmd(const QString &name, uint32_t fileSize = 0)
            : m_filename(name), m_fileSize(fileSize)
        {
        }
        ~DownloadFileCmd() {}

        void    execute(Cmd::Interface::IEC61850_API::ptr api) override;

        // Create new command like Builder pattern
        static auto create(const QString &name, uint32_t fileSize = 0) {
            return QSharedPointer<DownloadFileCmd>::create(name, fileSize);
        }

    signals:
        void    sigDownloadProgress(int perc);

    private:
        QString  m_filename;
        uint32_t m_fileSize = 0;
    };
}
