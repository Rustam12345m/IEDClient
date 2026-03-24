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

import QtQuick
import QtQuick.Controls

import AppStylesModule

Item
{
    property alias color: led.color
    property alias prompt: toolTip.text

    function setStatus(color, prompt) {
        led.color = color
        toolTip.text = prompt
    }

    width: 20
    height: 20

    Rectangle {
        id: led

        anchors.centerIn: parent

        height: 16
        width: 16
        radius: height/2

        color: VisualStyle.statusInactiveColor

        MouseArea {
            id: mouseArea
            anchors.fill: parent
            hoverEnabled: true
        }
    }
    ToolTip {
        id: toolTip
        text: ""
        delay: 200
        visible: mouseArea.containsMouse && (text != "")
    }
}
