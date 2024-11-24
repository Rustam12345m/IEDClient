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

pragma Singleton

import QtQuick

QtObject
{
    // Window & Common
    property color backgroundColor1:        "#eeeeee"
    property color backgroundColor2:        "#dddddd"
    property color borderColor:             "#005f87"
    property color textColor:               "#0D0D0D"
    property int   borderWidth:             4
    property int   delimeterWidth:          6
    property bool  boldHeaderText:          false
    property color undefinedColor1:         "pink"
    property color undefinedColor2:         "red"

    // ToolBar
    property color toolBarColor:            "#005f87"

    // Modal
    property color modalColor:              "#005f87"

    // StatusBar
    property QtObject statusBar: QtObject {
        property color color:               "#0087af"
        property color textColor:           "#eeeeee"
        property int height:                24
        property color connectedColor:      "#008700"
        property color disconnectedColor:   "#878787"
    }

    // Tables
    property QtObject table: QtObject {
        property color headerColor:        "#e4e4e4"
        property color headerTextColor:    "black"
        property color selRowColor:        "#e4e4e4"
        property color textColor:          "#444444"
        property color rowColor1:          "#eeeeee"
        property color rowColor2:          "#d0d0d0"
        property color rowBorderColor1:    "#707070"
        property color rowBorderColor2:    "#D9D9D9"
    }

    // LD grid styles
    property QtObject ldGrid: QtObject {
        property color color:               "#D9D9D9"
        property color selColor:            "#595959"
    }

    // TabBar styles
    property QtObject tabBar: QtObject {
        property color selColor:            "#e4e4e4"
        property color selTextColor:        "black"
        property color unselColor:          "#0087af"
        property color unselTextColor:      "#eeeeee"
        property color hoverColor:          "#5fafd7"
        property color hoverTextColor:      "white"
        // Horizontal tabBar
        property int horizontalHeight:      30
        property int horizontalWidth:       125
        property int horizontalSpacing:     0
    }
}
