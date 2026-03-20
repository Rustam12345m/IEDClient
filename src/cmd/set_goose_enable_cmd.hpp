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
#include "core/goose_control_block.hpp"

namespace Cmd
{
    /**
     * @brief Command to enable or disable a GOOSE Control Block
     */
    class SetGooseEnable_Cmd : public CmdInterface
    {
        Q_OBJECT
    public:
        SetGooseEnable_Cmd(Core::GooseControlBlock::ptr t_gocb, bool t_enable)
            : m_gocb{t_gocb}, m_enable{t_enable}
        {}
        ~SetGooseEnable_Cmd() override = default;

        void execute(Cmd::Interface::IEC61850_API::ptr t_api) override;

        static auto create(Core::GooseControlBlock::ptr t_gocb, bool t_enable) {
            return QSharedPointer<SetGooseEnable_Cmd>::create(t_gocb, t_enable);
        }

    private:
        Core::GooseControlBlock::ptr m_gocb;
        bool m_enable;
    };
}
