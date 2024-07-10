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

#include <QThread>

#include "cmd_interface.hpp"
#include "command_queue.hpp"
#include "interface/iec61850_api.hpp"

namespace Cmd
{
    class CmdThread : public QThread
    {
        Q_OBJECT
    private:
        CommandQueue<CmdInterface::ptr>            m_queue;
        Cmd::Interface::ptrIEC61850_API m_api;

    public:
        CmdThread(Cmd::Interface::ptrIEC61850_API t_api);
        ~CmdThread();

        void    putCommand(CmdInterface::ptr t_cmd);

    private:
        void    run();
    };
    typedef QSharedPointer<CmdThread>   ptrCmdThread;
}