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

#include "iedcon_container.hpp"
#include "event_storage.hpp"

// All known commands
#include "cmd/connect_cmd.hpp"
#include "cmd/disconnect_cmd.hpp"
#include "cmd/update_lds_status_cmd.hpp"
#include "cmd/update_lns_status_cmd.hpp"
#include "cmd/update_ln_cmd.hpp"
#include "cmd/update_rcbs_cmd.hpp"
#include "cmd/update_dataset_cmd.hpp"
#include "cmd/get_filelist_cmd.hpp"
#include "cmd/download_file_cmd.hpp"
#include "cmd/remove_file_cmd.hpp"

namespace App
{
    /**
     * @brief Interface for all Backends
     */
    class BackendInterface : public QObject
    {
        Q_OBJECT
    public:
        BackendInterface(IEDConContainer &con, EventStorage &ev)
            : m_con(con), m_events(ev)
        {
        }
        BackendInterface() = delete;
        virtual ~BackendInterface() {}

        BackendInterface& operator=(const BackendInterface &obj) = delete;

    protected:
        void    putCmdToQueue(Cmd::CmdInterface::ptr cmd);

    signals:
        void    sigCmdProgress(int perc, QString msg);
        void    sigCmdFinished(bool done);
        void    sigCmdError(QString msg);

    public slots:
        void         slotCmdEvent(Cmd::CmdEvent ev);
        virtual void slotConnected(bool done);

    protected:
        IEDConContainer& m_con;
        EventStorage&    m_events;
    };
}
