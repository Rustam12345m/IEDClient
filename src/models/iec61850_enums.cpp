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

#include "iec61850_enums.hpp"

static struct MBH_Value {
    QString     text;
    QString     tip;
    QString     color;
} mod_beh_status[] = {
    { .text = "On",       .tip = "ON",      .color = "green"},
    { .text = "Blk",      .tip = "BLOCKED", .color = "orange" },
    { .text = "Test",     .tip = "TEST",    .color = "orange" },
    { .text = "Test/Blk", .tip = "TEST and BLOCKED", .color = "red" },
    { .text = "Off",      .tip = "OFF",     .color = "darkred" }
},
health_status[] = {
    { .text = "Ok",       .tip = "OK",        .color = "green" },
    { .text = "Warn",     .tip = "WARNING",   .color = "orange" },
    { .text = "Alarm",    .tip = "ALARM",     .color = "red" }
};
#define MBH(val, num, list)      do { \
        if (num >= 0 && num < sizeof(list)/sizeof(*list)) { \
            val["num"] = num; \
            val["text"] = list[num].text; \
            val["tip"] = list[num].tip; \
            val["color"] = list[num].color; \
        } else { \
            val["num"] = -1; \
            val["text"] = " - "; \
            val["tip"] = ""; \
            val["color"] = "white"; \
        } \
    } while(0);

namespace App::Models
{
    QVariant IEC_EnumUserInfo::mod(int t_mod)
    {
        QVariantMap retval;
        MBH(retval, t_mod, mod_beh_status);
        return retval;
    }

    QVariant IEC_EnumUserInfo::beh(int t_beh)
    {
        return mod(t_beh);
    }

    QVariant IEC_EnumUserInfo::health(int t_health)
    {
        QVariantMap retval;
        MBH(retval, t_health, health_status);
        return retval;
    }

    QVariant IEC_EnumUserInfo::sim(int t_value)
    {
        QVariantMap retval;
        retval["num"] = -1;
        retval["text"] = "Off";
        retval["tip"] = "OFF";
        retval["color"] = "green";
        return retval;
    }

    QVariant IEC_EnumUserInfo::blk(int t_value)
    {
        QVariantMap retval;
        retval["num"] = -1;
        retval["text"] = "Off";
        retval["tip"] = "OFF";
        retval["color"] = "green";
        return retval;
    }
};