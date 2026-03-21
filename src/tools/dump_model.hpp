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

#include <QThread>

namespace Tools
{
    class DumpModel : public QThread
    {
        Q_OBJECT
    public:
        DumpModel(QObject *parent);
        ~DumpModel();

        void init(const QString &dir, const QString &ip, unsigned int port,
                 bool tls, const QString &name, const QString &pass);

        void run() override;

    signals:
        void    sigProgress(int perc, QString msg);
        void    sigFinished(bool done);

    private:
        QString m_fileDirectory;
        QString m_ip;
        int     m_port = 102;
        bool    m_tls = false;
        QString m_user, m_password;
    };
}