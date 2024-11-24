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

#include "sub_attribute.hpp"

namespace Core
{
    #define FC_STUFF(M) \
        M(ST, 0, "Status information") \
        M(MX, 1, "Measurands - analog values") \
        M(SP, 2, "Setpoint") \
        M(SV, 3, "Substitution") \
        M(CF, 4, "Configuration") \
        M(DC, 5, "Description") \
        M(SG, 6, "Setting group") \
        M(SE, 7, "Setting group editable") \
        M(SR, 8, "Service response / Service tracking") \
        M(OR, 9, "Operate received") \
        M(BL, 10, "Blocking") \
        M(EX, 11, "Extended definition") \
        M(CO, 12, "Control") \
        M(US, 13, "Unicast SV") \
        M(MS, 14, "Multicast SV") \
        M(RP, 15, "Unbuffered report") \
        M(BR, 16, "Buffered report") \
        M(LG, 17, "Log control blocks") \
        M(GO, 18, "Goose control blocks")

    enum FC_ENUM
    {
        #define X(Name, Num, Desc) Name = Num,
            FC_STUFF(X)
        #undef X

        COUNT,
        UNDEFINED = -1
    };

    /**
     * @brief Representation of a Data Attribute of Data Object
     * 
     * Has important FunctionConstrain = FC
     */
    class DataAttribute : public ModelItem
    {
    public:
        using ptr = QSharedPointer< DataAttribute >;

        DataAttribute(ModelItem *t_parent, const QString &t_name, const QString &t_fc)
            : ModelItem(t_parent, t_name)
        {
            m_fc = fcStringToNum(t_fc);
            m_delimetr = "."; // Between DOName and DAName
        }

        FC_ENUM    fcNum() const {
            return m_fc;
        }
        QString    fcStr() const {
            return fcNumToString(m_fc);
        }

        static const char* fcNumToString(FC_ENUM t_num);
        static const char* fcNumToDescription(FC_ENUM t_num);
        static FC_ENUM fcStringToNum(const QString &t_num);

    protected:
        FC_ENUM     m_fc = FC_ENUM::UNDEFINED;
    };
}
