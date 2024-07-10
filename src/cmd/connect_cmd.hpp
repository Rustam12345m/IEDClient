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

#include "cmd_interface.hpp"
#include "core/ied.hpp"
#include "cmd/ied_credentials.hpp"

namespace Cmd
{
    /*
     * This class realizes connecting to IED through pure TCP or TLS
     * */
    class ConnectCmd : public CmdInterface
    {
        Q_OBJECT
    public:
        ConnectCmd(const IEDCredentials &t_cred, Core::IED::ptr t_ied)
                : m_cred(t_cred), m_ied(t_ied) {}
        ~ConnectCmd() {}

        void    execute(Cmd::Interface::ptrIEC61850_API t_api) override;

        static auto create(const IEDCredentials &t_cred, Core::IED::ptr t_ied) {
            return QSharedPointer<ConnectCmd>::create(t_cred, t_ied);
        }

    private slots:
        void     slotFoundNewLN(const QString &t_ref);

    private:
        IEDCredentials m_cred;
        Core::IED::ptr m_ied;
        int            m_percProgress = 0;
    };
}