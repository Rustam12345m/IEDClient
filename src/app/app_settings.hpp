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
#include <QSettings>

#include "cmd/ied_credentials.hpp"
#include "config/conf_connection_info.hpp"

namespace App
{
    static const int         DefSaveHistoryLength = 10;
    static const QString     ConfigFileName = "iedclient_config.xml";
    typedef QList< App::ConfConnectionInfo >    lisHistConnInfo;

    class AppSettings : public QObject
    {
        Q_OBJECT
    public:
        AppSettings();
        AppSettings(const QString &t_filepath);
        ~AppSettings() = default;

        lisHistConnInfo    getConnectionList();
        void     putConnectionToConfig(const Cmd::IEDCredentials &t_creds, const QString &t_ied);

    signals:
        void     sigConfUpdated();

    private:
        int      readConfigFile(const QString &t_filepath, lisHistConnInfo &t_list);
        int      writeConfigFile(const QString &t_filepath, lisHistConnInfo &t_list);

    private:
        QString m_confFilepath;
    };
}