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

#include "core/data_model.hpp"

namespace Cmd::Interface
{
    class IED_StateAPI
    {
    public:
        virtual ~IED_StateAPI() {}

        virtual Core::ModelStateUpdater::ptr getStatusForAllLD(Core::DataModel::ptr model) = 0;
        virtual Core::ModelStateUpdater::ptr getStatusForAllLN(Core::LogicalDevice::ptr ld) = 0;

        virtual Core::ModelStateUpdater::ptr getValsForLN(Core::LogicalNode::ptr ln) = 0;
        virtual Core::ModelStateUpdater::ptr getValsForDS(Core::DataSet::ptr ds) = 0;
    };
}
