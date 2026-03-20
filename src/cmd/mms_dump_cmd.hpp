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
#include "ied_credentials.hpp"

namespace Cmd
{
    /**
     * @brief Dumps the entire MMS model (domains, variables, datasets) to a text file.
     * Makes its own MMS connection — independent from the main IED connection.
     * Uses raw MmsConnection API — no IEC 61850 abstraction.
     */
    class MmsDumpCmd : public CmdInterface
    {
        Q_OBJECT
    public:
        MmsDumpCmd(const IEDCredentials &t_cred) : m_cred(t_cred) {}
        ~MmsDumpCmd() = default;

        void execute(Cmd::Interface::IEC61850_API::ptr t_api) override;

        static auto create(const IEDCredentials &t_cred) {
            return QSharedPointer<MmsDumpCmd>::create(t_cred);
        }

    private:
        IEDCredentials m_cred;
    };
}
