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

#include "app_backend.hpp"
#include "version.hpp"

#include <QVersionNumber>

namespace App
{
    AppBackend::AppBackend(IEDConContainer &con, EventStorage &ev)
        : BackendInterface(con, ev)
    {
        m_eventsModel = new Models::AppEventsTable(this);
        m_lastConnModel = new Models::HistConTable(this, m_settings);

        connect(&m_events, &EventStorage::sigNewEvent, this, [this](Cmd::CmdEvent) {
            emit sigNewStatusMsg();
        });
        connect(&m_events, &EventStorage::sigNewEvent, m_eventsModel, &Models::AppEventsTable::addEvent);
    }

    QString AppBackend::getAppVersion()
    {
        return QString("%1").arg(PROJECT_VERSION);
    }

    QString AppBackend::getQtVersion()
    {
        return QString("%1").arg(qVersion());
    }

    QString AppBackend::getLibVersion()
    {
        return m_con.m_api->getVersion();
    }

    QString AppBackend::getLastStatusMsg()
    {
        return m_events.getLastMessage();
    }

    void AppBackend::saveCredsToHistory(const Cmd::IEDCredentials &cred)
    {
        m_settings.putConnectionToConfig(cred, m_con.m_ied->model().getName());
    }

    WatchlistRefs AppBackend::getWatchlistForDevice(const Cmd::IEDCredentials &creds)
    {
        return m_settings.getWatchlistForDevice(creds);
    }

    void AppBackend::saveWatchlistForDevice(const Cmd::IEDCredentials &creds, const WatchlistRefs &wl)
    {
        m_settings.saveWatchlistForDevice(creds, wl);
    }
}
