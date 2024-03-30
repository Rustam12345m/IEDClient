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

Rectangle
{
    id: rootID

    required property bool pressed

    implicitWidth: ColorPalette.delimeterWidth
    width: ColorPalette.delimeterWidth

    color: rootID.pressed ? "#707070" : ColorPalette.borderColor

    Rectangle {
        anchors.centerIn: parent

        width: 2
        height: 40
        radius: 1

        // color: rootID.pressed ? "#202020" : "#707070"
        color: rootID.pressed ? "white" : "#707070"
    }

    containmentMask: Item {
        width: ColorPalette.delimeterWidth
        height: rootID.height
    }
}