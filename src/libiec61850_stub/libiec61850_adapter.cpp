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

#include "libiec61850_adapter.hpp"

extern "C"
{
#include <iec61850_client.h>
}

namespace Libiec61850
{
    namespace
    {
        void     callback_ConnectionHandler(void *t_param, sIedConnection *t_con)
        {
            ApiAdapter *adapter = static_cast<ApiAdapter*>(t_param);
            if (adapter != nullptr) {
                adapter->callbackOnCloseEvent();
            }
        }
    }

    bool ApiAdapter::connect(const Cmd::IEDCredentials &t_creds)
    {
        IedClientError retval = IED_ERROR_OK;

        m_libConn = IedConnection_create();
        IedConnection_connect(m_libConn, &retval, t_creds.ip().toStdString().c_str(), t_creds.port());
        if (retval == IED_ERROR_OK) {
            // Callback for close-events
            IedConnection_installConnectionClosedHandler(m_libConn, &callback_ConnectionHandler, this);

            IedConnection_getDeviceModelFromServer(m_libConn, &retval);
            if (retval != IED_ERROR_OK) {
                qDebug() << "!!! ERROR !!!: Connect, get model with error = " << retval;
            }            
        } else {
            IedConnection_destroy(m_libConn);
            m_libConn = nullptr;
            return false;
        }
        return true;
    }

    void ApiAdapter::disconnect()
    {
        if (m_libConn == nullptr) {
            return;
        }

        IedClientError error = IED_ERROR_OK;
        IedConnection_abort(m_libConn, &error);
        IedConnection_destroy(m_libConn);
        m_libConn = nullptr;
    }

    bool ApiAdapter::isConnected() const
    {
        if (m_libConn == nullptr) {
            return false;
        }

        IedConnectionState retval = IedConnection_getState(m_libConn);
        if (retval == IED_STATE_CLOSED) {
            return false;
        }
        return true;
    }

    QString ApiAdapter::getVersion() const
    {
        char *pv = LibIEC61850_getVersionString();
        return QString("%1").arg(pv);
    }

    Core::DevServIdentity ApiAdapter::getServIdentity() const
    {
        Core::DevServIdentity ident;
        MmsConnection mmsCon = IedConnection_getMmsConnection(m_libConn);

        MmsError error = MMS_ERROR_NONE;
        MmsServerIdentity *identity = MmsConnection_identify(mmsCon, &error);
        if ((error == MMS_ERROR_NONE) && (identity != nullptr)) {
            ident.m_vendor = QString::fromLocal8Bit(identity->vendorName);
            ident.m_model = QString::fromLocal8Bit(identity->modelName);
            ident.m_revision = QString::fromLocal8Bit(identity->revision);
        }

        MmsConnectionParameters param = MmsConnection_getMmsConnectionParameters(mmsCon);
        ident.m_maxPduSize = param.maxPduSize;
        ident.m_dataStructureNestingLevel = param.dataStructureNestingLevel;
        ident.m_maxServOutstandingCalled = param.maxServOutstandingCalled;
        ident.m_maxServOutstandingCalling = param.maxServOutstandingCalling;
        return ident;
    }

    void ApiAdapter::callbackOnCloseEvent()
    {
        emit sigConClosed();
    }
}