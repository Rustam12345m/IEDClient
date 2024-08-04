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

#include "backend_interface.hpp"
#include "app_settings.hpp"

#include "models/app_events_table.hpp"
#include "models/history_con_table.hpp"

namespace App
{
    /*
    * Presenter of other application related data and tables
    * */
    class AppBackend : public BackendInterface
    {
        Q_OBJECT
    public:
        AppBackend(IEDConContainer &t_con, EventStorage &t_ev);

        Q_PROPERTY(QAbstractTableModel* appLogsModel READ getEventsModel    CONSTANT)
        Q_PROPERTY(QAbstractItemModel*  lastConnList READ getLastConn_Model CONSTANT)

        Q_INVOKABLE QString getAppVersion();
        Q_INVOKABLE QString getQtVersion();
        Q_INVOKABLE QString getLibVersion();
        Q_INVOKABLE QString getLastStatusMsg();

        Models::AppEventsTable* getEventsModel() const { return m_eventsModel; }
        Models::HistConTable*   getLastConn_Model() const { return m_lastConnModel; }

        void saveCredsToHistory(const Cmd::IEDCredentials &t_cred);

    signals:
        void sigNewStatusMsg();
    public slots:
        void slotNewStatusMessage();

    protected:
        AppSettings m_settings;
        Models::AppEventsTable* m_eventsModel = nullptr;
        Models::HistConTable*   m_lastConnModel = nullptr;
    };
}