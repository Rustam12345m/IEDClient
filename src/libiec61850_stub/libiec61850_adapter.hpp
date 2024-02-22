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

#include "cmd/interface/iec61850_api.hpp"

#include "ied_model_api_impl.hpp"
#include "ied_state_api_impl.hpp"
#include "ied_control_api_impl.hpp"
#include "ied_fs_api_impl.hpp"

struct sIedConnection; // Forward declaration

namespace Libiec61850
{
    class Libiec61850_Adapter : public Cmd::Interface::IEC61850_API
    {
    public:
        Libiec61850_Adapter() : m_model(*this), m_state(*this), m_fs(*this) {}
        ~Libiec61850_Adapter() override {}

        bool    connect(const Cmd::IEDCredentials &t_creds) override;
        void    disconnect() override;
        bool    isConnected() const override;

        QString getVersion() const override;
        Core::DevServIdentity  getServIdentity() const override;

        Cmd::Interface::IED_ModelAPI&      model() override { return m_model; }
        Cmd::Interface::IED_StateAPI&      state() override { return m_state; }
        Cmd::Interface::IED_ControlAPI&    control() override { return m_control; }
        Cmd::Interface::IED_FS_API&        fs() override { return m_fs; }

    public:
        void 	callbackOnCloseEvent();

    private:
		// libiec61850 stuff
		sIedConnection*         m_libConn = nullptr;

        IED_ModelAPI_Impl       m_model;
        IED_StateAPI_Impl       m_state;
        IED_ControlAPI_Impl     m_control;
        IED_FS_API_Impl         m_fs;

    friend class IED_ModelAPI_Impl;
    friend class IED_StateAPI_Impl;
    friend class IED_ControlAPI_Impl;
    friend class IED_FS_API_Impl;
    };
    typedef QSharedPointer<Libiec61850_Adapter>    ptrAdapter;
}