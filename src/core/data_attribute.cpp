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

#include "data_attribute.hpp"

namespace Core
{
    const char* DataAttribute::fcNumToString(FC_ENUM num)
    {
        switch (num) {
        #define _(name, num_, desc) case name: return #name;
            FC_STUFF(_)
        #undef _
        default: {
            return "";
        }
        }
    }

    const char* DataAttribute::fcNumToDescription(FC_ENUM num)
    {
        switch (num) {
        #define _(name, num_, desc) case name: return desc;
            FC_STUFF(_)
        #undef _
        default: {
            return "";
        }
        }
    }

    FC_ENUM DataAttribute::fcStringToNum(const QString &num)
    {
        #define X(name, num_, desc) if (num == #name) return FC_ENUM::name;
            FC_STUFF(X)
        #undef X
        return FC_ENUM::UNDEFINED;
    }
}