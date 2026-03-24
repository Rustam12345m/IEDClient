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
#include "core/sgcb.hpp"

namespace Cmd
{
    class RefreshSGCB_Cmd : public CmdInterface
    {
        Q_OBJECT
    public:
        RefreshSGCB_Cmd(Core::SGCB::ptr sgcb)
            : m_sgcb{sgcb}
        {}
        ~RefreshSGCB_Cmd() override = default;

        void execute(Cmd::Interface::IEC61850_API::ptr api) override;

        static auto create(Core::SGCB::ptr sgcb) {
            return QSharedPointer<RefreshSGCB_Cmd>::create(sgcb);
        }

    signals:
        void sigSGCBRefreshed(QString ldRef);

    private:
        Core::SGCB::ptr m_sgcb;
    };
}
