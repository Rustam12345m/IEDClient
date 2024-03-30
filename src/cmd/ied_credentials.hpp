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
#include <QVariant>

namespace Cmd
{
    class IEDCredentials
    {
    public:
        IEDCredentials() = default;
        IEDCredentials(const QString &t_ip, int t_port, bool t_tls, const QString &t_login, const QString &t_pass)
            : m_ip(t_ip), m_port(t_port), m_tls(t_tls), m_login(t_login), m_password(t_pass)
        {}
        IEDCredentials(const QVariantMap &t_data)
        {
            m_ip = t_data.value("ip").toString();
            m_port = t_data.value("port").toInt();
            m_tls = t_data.value("tls").toBool();
            m_login = t_data.value("login").toString();
            m_password = t_data.value("password").toString();
        }

        const auto     ip() const { return m_ip; }
        const auto     port() const { return m_port; }
        const auto     tls() const { return m_tls; }
        const auto     login() const { return m_login; }
        const auto     password() const { return m_password; }

        bool operator==(const IEDCredentials &t_right) {
            return (m_ip == t_right.m_ip)
                    && (m_port == t_right.m_port)
                    && (m_tls == t_right.m_tls)
                    /* && (m_login == t_right.m_login) */
                    /* && (m_password == t_right.m_password)*/;
        }

    protected:
        QString     m_ip;
        int         m_port = 0;
        bool        m_tls = false;
        QString     m_login;
        QString     m_password;
    };
};