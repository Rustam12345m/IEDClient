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
    const char* DataAttribute::fcNumToString(FC_ENUM t_num)
    {
        switch (t_num) {
        #define _(name, num, desc) case name: return #name;
            FC_STUFF(_)
        #undef _
        default: {
            return "";
        }
        }
    }

    const char* DataAttribute::fcNumToDescription(FC_ENUM t_num)
    {
        switch (t_num) {
        #define _(name, num, desc) case name: return desc;
            FC_STUFF(_)
        #undef _
        default: {
            return "";
        }
        }
    }

    FC_ENUM DataAttribute::fcStringToNum(const QString &t_num)
    {
        #define X(name, num, desc) if (t_num == #name) return FC_ENUM::name;
            FC_STUFF(X)
        #undef X
        return FC_ENUM::UNDEFINED;
    }
}