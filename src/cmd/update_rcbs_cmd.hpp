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

namespace Cmd
{
    /**
     * @brief This class realizes downloading file from the IED
     */
    class UpdateRCBs_Cmd : public CmdInterface
    {
        Q_OBJECT
    public:
        UpdateRCBs_Cmd(Core::IED::ptr ied) : m_ied{ied}
        {
        }
        ~UpdateRCBs_Cmd() = default;

        void    execute(Cmd::Interface::IEC61850_API::ptr api) override;

        // Create new command like Builder pattern
        static auto create(Core::IED::ptr ied) {
            return QSharedPointer<UpdateRCBs_Cmd>::create(ied);
        }

    private:
        Core::IED::ptr m_ied;
    };
}
