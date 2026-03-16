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

#include "event_storage.hpp"

namespace App
{
    QString EventStorage::getLastMessage()
    {
        QMutexLocker locker(&m_lock);
        if (m_count == 0) {
            return {};
        }
        int last = (m_head + m_count - 1) % Capacity;
        return m_buffer[last].m_msg;
    }

    void EventStorage::putEventToStorage(Cmd::CmdEvent t_event)
    {
        {
            QMutexLocker locker(&m_lock);
            int pos = (m_head + m_count) % Capacity;
            m_buffer[pos] = t_event;

            if (m_count < Capacity) {
                ++m_count;
            } else {
                m_head = (m_head + 1) % Capacity;
            }
        }

        emit sigNewEvent(t_event);
    }
}
