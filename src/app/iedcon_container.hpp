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

#include "core/ied.hpp"
#include "cmd/cmd_thread.hpp"
#include "cmd/ied_credentials.hpp"
#include "libiec61850_stub/libiec61850_adapter.hpp"

namespace App
{
    /*
     * This object holds all stuff associated with IED that was connected to
     * */
    class IEDConContainer : public QObject
    {
        Q_OBJECT
    public:
        IEDConContainer() {
            allocateNewConnection(Cmd::IEDCredentials());
        }

        bool     isConnected() const {
            return m_api->isConnected();
        }

        void     allocateNewConnection(const Cmd::IEDCredentials &t_cred) {
            m_cmdThread.clear();
            m_api.clear();
            m_ied.clear();

            m_cred = t_cred;
            m_ied = QSharedPointer<Core::IED>::create();

            auto apiImpl =  Libiec61850::ptrAdapter::create();
            connect(apiImpl.get(), &Cmd::Interface::IEC61850_API::sigConClosed,
                    this, &IEDConContainer::slotConClosed);
            m_api = apiImpl;

            m_cmdThread = QSharedPointer<Cmd::CmdThread>::create(m_api);
        }

    signals:
        void     sigConClosed();

    public slots:
        void     slotConClosed() {
            emit sigConClosed();
        }

    public:
        Core::ptrIED            m_ied;
        Cmd::IEDCredentials     m_cred; // Information about ip/port/etc
        Cmd::ptrCmdThread       m_cmdThread;
        Libiec61850::ptrAdapter m_api; // Connection & API
    };
}