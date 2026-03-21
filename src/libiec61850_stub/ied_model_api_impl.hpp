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

#include "cmd/interface/ied_model_api.hpp"

namespace Libiec61850
{
    class ApiAdapter;

    class IED_ModelAPI_Impl : public Cmd::Interface::IED_ModelAPI
    {
    public:
        IED_ModelAPI_Impl(ApiAdapter &api) : m_api(api) {};
        ~IED_ModelAPI_Impl() override = default;

        int     fetchDataModel(Core::DataModelBuilder &builder) override;

    private:
        int     fetchLN_DO(Core::DataModelBuilder &builder);
        int     fetchLN_DS(Core::DataModelBuilder &builder);
        int     fetchLN_RCB(Core::DataModelBuilder &builder);
        int     fetchLN_GOCB(Core::DataModelBuilder &builder);
        int     fetchLN_SVCB(Core::DataModelBuilder &builder);

    private:
        ApiAdapter&   m_api;
    };
};