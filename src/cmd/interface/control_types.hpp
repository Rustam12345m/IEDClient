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

#include <QVariant>

namespace Cmd::Interface
{
    enum class CtlModel : int {
        StatusOnly     = 0,
        DirectNormal   = 1,
        SBONormal      = 2,
        DirectEnhanced = 3,
        SBOEnhanced    = 4
    };

    enum class CtlValType : int {
        Boolean  = 0,
        Integer  = 1,
        Unsigned = 2,
        Float    = 3,
        Unknown  = -1
    };

    struct ControlInfo {
        CtlModel   model   = CtlModel::StatusOnly;
        CtlValType valType = CtlValType::Unknown;
    };
}
