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

#include "cmd/interface/ied_control_api.hpp"

namespace Libiec61850
{
    class ApiAdapter;

    class IED_ControlAPI_Impl : public Cmd::Interface::IED_ControlAPI
    {
    public:
        IED_ControlAPI_Impl(ApiAdapter &t_api) : m_api(t_api) {}
        ~IED_ControlAPI_Impl() override = default;

        bool setRCBValues(const QString &t_rcbRef, bool t_enable,
                          int t_trgOps, uint32_t t_bufTm, uint32_t t_intgPd,
                          const QString &t_rptId, const QString &t_datSet) override;

        bool refreshRCBValues(Core::ReportBlock::ptr t_rcb) override;

    private:
        ApiAdapter& m_api;
    };
};
