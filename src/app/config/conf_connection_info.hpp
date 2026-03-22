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
#include <QList>
#include <QPair>
#include "cmd/ied_credentials.hpp"

namespace App
{
    typedef QList<QPair<QString,QString>> WatchlistRefs;

    class ConfConnectionInfo
    {
    public:
        ConfConnectionInfo(const Cmd::IEDCredentials &creds,
                           const QString &iedName, const QString &date)
            : m_creds(creds), m_iedName(iedName), m_date(date)
        {
        }
        ConfConnectionInfo() = delete;

        const auto ip() const { return m_creds.ip(); }
        const auto port() const { return m_creds.port(); }
        const auto tls() const { return m_creds.tls(); }
        const auto ied() const { return m_iedName; }
        const auto date() const { return m_date; }

        const WatchlistRefs& watchlist() const { return m_watchlist; }
        void setWatchlist(const WatchlistRefs &wl) { m_watchlist = wl; }

        bool operator==(const ConfConnectionInfo &right) const {
            return m_creds == right.m_creds;
        }

    private:
        Cmd::IEDCredentials m_creds;
        QString m_iedName;
        QString m_date;
        WatchlistRefs m_watchlist;
    };
}
