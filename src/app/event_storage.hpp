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

#include <QObject>
#include <QMutex>

#include <array>

#include "cmd/cmd_event.hpp"

namespace App
{
    class EventStorage : public QObject
    {
        Q_OBJECT
    public:
        static constexpr int Capacity = 256;

        EventStorage() = default;

        QString getLastMessage();

        void    putEventToStorage(Cmd::CmdEvent event);

    signals:
        void    sigNewEvent(Cmd::CmdEvent event);

    private:
        mutable QMutex                              m_lock;
        std::array<Cmd::CmdEvent, Capacity>         m_buffer{};
        int                                         m_head = 0;
        int                                         m_count = 0;
    };
}
