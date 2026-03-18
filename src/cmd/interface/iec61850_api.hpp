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
#include <QSharedPointer>

#include "core/server_identify.hpp"
#include "cmd/ied_credentials.hpp"

#include "ied_model_api.hpp"
#include "ied_state_api.hpp"
#include "ied_control_api.hpp"
#include "ied_fs_api.hpp"

namespace Cmd::Interface
{
    class IEC61850_API : public QObject
    {
        Q_OBJECT
    public:
        using ptr = QSharedPointer< IEC61850_API >;

        virtual ~IEC61850_API() {}

        virtual QString connect(const Cmd::IEDCredentials &t_creds) = 0;
        virtual void disconnect() = 0;
        virtual bool isConnected() const = 0;

        virtual QString getVersion() const = 0;
        virtual Core::DevServIdentity getServIdentity() const = 0;

        // API
        virtual IED_ModelAPI&   model() = 0;
        virtual IED_StateAPI&   state() = 0;
        virtual IED_ControlAPI& control() = 0;
        virtual IED_FS_API&     fs() = 0;

    signals:
        void sigFoundNode(const QString t_path);
        void sigConClosed();
    };
}