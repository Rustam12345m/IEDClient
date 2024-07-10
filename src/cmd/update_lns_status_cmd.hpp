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
    class UpdateLNs_StatusCmd : public CmdInterface
    {
        Q_OBJECT
    public:
        UpdateLNs_StatusCmd(Core::IED::ptr t_ied, Core::LogicalDevice::ptr t_ld)
            : m_ied(t_ied), m_ld(t_ld)
        {
        }
        ~UpdateLNs_StatusCmd() override {}

        void    execute(Cmd::Interface::IEC61850_API::ptr t_api) override;

        static auto create(Core::IED::ptr t_ied, Core::LogicalDevice::ptr t_ld) {
            return QSharedPointer<UpdateLNs_StatusCmd>::create(t_ied, t_ld);
        }
    
    signals:
        void     sigModelValues(Core::ModelStateUpdater::ptr t_vals);

    private:
        Core::IED::ptr              m_ied;
        Core::LogicalDevice::ptr    m_ld;
    };
}