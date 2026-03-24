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

#include "dump_model.hpp"
#include "dump_writer.hpp"
#include "libiec61850_stub/libiec61850_tools.hpp"

#include <QDebug>

namespace Tools
{
    DumpModel::DumpModel(QObject *parent) : QThread(parent)
    {
    }

    DumpModel::~DumpModel()
    {
    }

    void DumpModel::init(const QString &dir, const QString &ip, unsigned int port,
                         bool tls, const QString &name, const QString &pass)
    {
        m_fileDirectory = dir;
        m_ip = ip;
        m_port = port;
        m_tls = tls;
        m_user = name;
        m_password = pass;
    }

    void DumpModel::run()
    {
        for (int i=0;i<10;i++) {
            emit sigProgress(i * 5, QString("Progress is %1").arg(i * 5));
            QThread::sleep(1);
        }
        emit sigFinished(true);
        this->deleteLater();
    }
}