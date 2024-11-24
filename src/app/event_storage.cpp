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

#include <QDebug>

namespace App
{
    QString EventStorage::getLastMessage()
    {
        QString retval;
        m_lock.lock();
        if (!m_events.empty()) {
            retval = m_events.back().m_msg;
        }
        m_lock.unlock();
        return retval;
    }

    void EventStorage::putEventToStorage(Cmd::CmdEvent t_event)
    {
        m_lock.lock();
        m_events.push_back(t_event);
        m_lock.unlock();

        emit sigNewEvent();
    }
}