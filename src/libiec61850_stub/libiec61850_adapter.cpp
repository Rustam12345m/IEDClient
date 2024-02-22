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
		void 	callback_ConnectionHandler(void *t_param, sIedConnection *t_con)
		{
			Libiec61850_Adapter *adapter = static_cast<Libiec61850_Adapter*>(t_param);
			if (adapter != nullptr) {
				adapter->callbackOnCloseEvent();
			}
		}
	}

    bool Libiec61850_Adapter::connect(const Cmd::IEDCredentials &t_creds)
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

    void Libiec61850_Adapter::disconnect()
    {
        if (m_libConn == nullptr) {
			return;
		}

		IedClientError error = IED_ERROR_OK;
		IedConnection_abort(m_libConn, &error);
		IedConnection_destroy(m_libConn);
		m_libConn = nullptr;
    }

    bool Libiec61850_Adapter::isConnected() const
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

    QString Libiec61850_Adapter::getVersion() const
    {
        char *pv = LibIEC61850_getVersionString();
		return QString("%1").arg(pv);
    }

    Core::DevServIdentity Libiec61850_Adapter::getServIdentity() const
    {
        return Core::DevServIdentity();
    }

    void Libiec61850_Adapter::callbackOnCloseEvent()
    {
        emit sigConClosed();
    }
}