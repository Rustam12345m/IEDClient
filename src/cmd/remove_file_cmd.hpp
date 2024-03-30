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
    /*
     * This class realizes downloading file from the IED
     * */
    class RemoveFileCMD : public CmdInterface
    {
        Q_OBJECT
    public:
        RemoveFileCMD(const QString &t_name, int t_fileIndex)
            : m_filename(t_name), m_fileIndex(t_fileIndex)
        {
        }
        ~RemoveFileCMD() {}

        void    execute(Cmd::Interface::ptrIEC61850_API t_api) override;

        // Create new command like Builder pattern
        static auto create(const QString &t_name, int t_row) {
            return QSharedPointer<RemoveFileCMD>::create(t_name, t_row);
        }

    signals:
        void    sigFileRemoved(int t_row);

    protected:
        QString m_filename;
        int     m_fileIndex = -1; // row in the table
    };
}