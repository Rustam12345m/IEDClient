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
    /**
     * @brief This class realizes connecting to IED through pure TCP or TLS
     */
    class ConnectCmd : public CmdInterface
    {
        Q_OBJECT
    public:
        ConnectCmd(const IEDCredentials &cred, Core::IED::ptr ied)
                : m_cred(cred), m_ied(ied)
        {}
        ~ConnectCmd() {}

        void    execute(Cmd::Interface::IEC61850_API::ptr api) override;

        static auto create(const IEDCredentials &cred, Core::IED::ptr ied) {
            return QSharedPointer<ConnectCmd>::create(cred, ied);
        }

    private slots:
        void     slotFoundNewLN(const QString &ref);

    private:
        IEDCredentials m_cred;
        Core::IED::ptr m_ied;
        int m_percProgress = 0;
    };
}
