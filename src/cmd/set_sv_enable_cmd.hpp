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
#include "core/sv_control_block.hpp"

namespace Cmd
{
    /**
     * @brief Command to enable or disable a Sampled Values Control Block
     */
    class SetSVEnable_Cmd : public CmdInterface
    {
        Q_OBJECT
    public:
        SetSVEnable_Cmd(Core::SV_ControlBlock::ptr svcb, bool enable)
            : m_svcb{svcb}, m_enable{enable}
        {}
        ~SetSVEnable_Cmd() override = default;

        void execute(Cmd::Interface::IEC61850_API::ptr api) override;

        static auto create(Core::SV_ControlBlock::ptr svcb, bool enable) {
            return QSharedPointer<SetSVEnable_Cmd>::create(svcb, enable);
        }

    private:
        Core::SV_ControlBlock::ptr m_svcb;
        bool m_enable;
    };
}
