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

#include <QString>
#include <QDateTime>

namespace Cmd
{
    enum EventMsgType
    {
        CMD_EVENT_UNDEF_EV = 0,
        CMD_EVENT_START_EV,
        CMD_EVENT_PROCESS_EV,
        CMD_EVENT_FINISH_EV
    };

    /*
     * This class contains messages from the command that is performed:
     * - Request message
     * - Process message
     * - Result message
     * */
    class CmdEventInfo
    {
    public:
        static CmdEventInfo  StartEvent(const QString &t_ip, const QString &t_msg) {
            return { .m_type = CMD_EVENT_START_EV, .m_time = QDateTime::currentDateTime(),
                     .m_ip = t_ip, .m_msg = t_msg };
        }
        static CmdEventInfo  ProcessEvent(const QString &t_ip, const QString &t_msg, int t_perc) {
            return { .m_type = CMD_EVENT_PROCESS_EV, .m_time = QDateTime::currentDateTime(),
                     .m_ip = t_ip, .m_msg = t_msg, .m_perc = t_perc };
        }
        static CmdEventInfo  FinishEvent(const QString &t_ip, const QString &t_msg, bool t_result) {
            return { .m_type = CMD_EVENT_FINISH_EV, .m_time = QDateTime::currentDateTime(),
                     .m_ip = t_ip, .m_msg = t_msg, .m_result = t_result };
        }

    public:
        EventMsgType    m_type = CMD_EVENT_UNDEF_EV;
        QDateTime       m_time; // start, process or finish
        QString         m_ip;
        QString         m_msg;
        bool            m_result = false; // for operation
        int             m_perc = 0;
    };
}