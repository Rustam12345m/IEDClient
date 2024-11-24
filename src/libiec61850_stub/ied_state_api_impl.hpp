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

#include "cmd/interface/ied_state_api.hpp"

namespace Libiec61850
{
    class ApiAdapter;

    class IED_StateAPI_Impl : public Cmd::Interface::IED_StateAPI
    {
    public:
        IED_StateAPI_Impl(ApiAdapter &t_api) : m_api(t_api) {}
        ~IED_StateAPI_Impl() override = default;

        Core::ModelStateUpdater::ptr getStatusForAllLD(Core::DataModel::ptr t_model) override;
        Core::ModelStateUpdater::ptr getStatusForAllLN(Core::LogicalDevice::ptr t_ld) override;

        Core::ModelStateUpdater::ptr getValsForLN(Core::LogicalNode::ptr t_ln) override;
        Core::ModelStateUpdater::ptr getValsForDS(Core::DataSet::ptr t_ds) override;
    
    private:
        ApiAdapter&   m_api;
    };
};