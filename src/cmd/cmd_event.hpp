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
#include <QMetaType>

namespace Cmd
{
    enum EventMsgType
    {
        UNDEFINED_EVENT = 0,
        START_EVENT,
        PROCESS_EVENT,
        FINISH_EVENT
    };

    /**
     * @brief This class contains messages from the command that is performed:
     * - Request message
     * - Process message
     * - Result message
     */
    class CmdEvent
    {
    public:
        static CmdEvent  StartEvent(const QString &ip, const QString &msg) {
            return { .m_type = START_EVENT, .m_time = QDateTime::currentDateTime(),
                     .m_ip = ip, .m_msg = msg };
        }
        static CmdEvent  ProcessEvent(const QString &ip, const QString &msg, int perc) {
            return { .m_type = PROCESS_EVENT, .m_time = QDateTime::currentDateTime(),
                     .m_ip = ip, .m_msg = msg, .m_perc = perc };
        }
        static CmdEvent  FinishEvent(const QString &ip, const QString &msg, bool result) {
            return { .m_type = FINISH_EVENT, .m_time = QDateTime::currentDateTime(),
                     .m_ip = ip, .m_msg = msg, .m_result = result };
        }

    public:
        EventMsgType    m_type = UNDEFINED_EVENT;
        QDateTime       m_time; // start, process or finish
        QString         m_ip;
        QString         m_msg;
        bool            m_result = false; // for operation
        int             m_perc = 0;
    };
}

Q_DECLARE_METATYPE(Cmd::CmdEvent)
