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

import "qrc:/common/"

import GlobalVarsModule
import AppStylesModule

// Special delegate for Mod-Beh-Health cells in LN_Table
Item
{
    id: rootID

    required property int delegateHeight
    required property int delegateWidth
    required property bool selected
    required property var value

    property alias prompt: toolTip.text
    property alias text: textFild.text

    implicitHeight: delegateHeight
    implicitWidth: delegateWidth

    signal sigClick(int row, int col)
    signal sigDoubleClick(int row, int col)

    Rectangle {
        anchors.fill: parent

        clip: true
        border.color: VisualStyle.table.rowBorderColor2
        color: (selected ? VisualStyle.table.selRowColor : VisualStyle.table.rowColor1)

        Row {
            anchors.centerIn: parent
            spacing: 5

            Rectangle {
                id: led

                anchors.verticalCenter: parent.verticalCenter
                width: 12
                height: 12
                radius: 6
                color: rootID.value.color
            }

            Text {
                id: textFild

                anchors.verticalCenter: parent.verticalCenter
                horizontalAlignment: Text.AlignLeft
                verticalAlignment: Text.AlignVCenter

                font.bold: false
                color: VisualStyle.textColor
                text: rootID.value.text
            }
        }
        ToolTip {
            id: toolTip
            text: rootID.value.tip
            delay: 200
            visible: mouseArea.containsMouse && (text != "")
        }
    }

    MouseArea {
        id: mouseArea

        anchors.fill: parent
        acceptedButtons: Qt.LeftButton | Qt.RightButton
        //hoverEnabled: true

        onClicked: function(mouse) {
            sigClick(row, 1)
        }
        onDoubleClicked: function(mouse) {
            sigDoubleClick(row, 1)
        }
    }
}
