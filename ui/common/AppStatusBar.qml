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
import QtQuick.Layouts

import AppStylesModule

Rectangle
{
    id: rootID

    height: 24

    color: ColorPalette.statusBarColor
    clip: true

    property alias msgStatusText: msgStatusTextID.text
    property alias pageStatusText: pageStatusTextID.text
    property var appConnStatus

    RowLayout {
        anchors {
            fill: parent

            leftMargin: 5
            rightMargin: 5
        }
        spacing: 10

        Text {
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignVCenter

            id: msgStatusTextID
            height: parent.height

            horizontalAlignment: Text.AlignLeft // HCenter
            verticalAlignment: Text.AlignVCenter

            text: ""

            onTextChanged: {
                if (msgStatusTextID.text != "") {
                    timerStatusMsgID.restart()
                }
            }
        }
        Rectangle {
            Layout.alignment: Qt.AlignVCenter

            width: 1
            height: parent.height - 4
            color: ColorPalette.modalColor
        }
        Text {
            Layout.fillWidth: false
            Layout.alignment: Qt.AlignVCenter
            Layout.preferredWidth: 200

            id: pageStatusTextID
            height: parent.height

            horizontalAlignment: Text.AlignHCenter //Left
            verticalAlignment: Text.AlignVCenter

            font.bold: false
            color: ColorPalette.textColor
            text: ""
        }
        Rectangle {
            Layout.alignment: Qt.AlignVCenter

            width: 1
            height: parent.height - 4
            color: ColorPalette.modalColor
        }
        Row {
            height: parent.height
            spacing: 5

            Text {
                Layout.alignment: Qt.AlignVCenter

                id: conStatusTextID

                horizontalAlignment: Text.AlignLeft
                verticalAlignment: Text.AlignVCenter

                text: appConnStatus.text
            }
            Led {
                Layout.alignment: Qt.AlignVCenter

                height: 20
                width: 20
                color: appConnStatus.isConnected ? "green" : "gray"
            }
        }
    }

    Timer {
        id: timerStatusMsgID

        interval: 5000
        running: false
        repeat: false

        onTriggered: {
            msgStatusTextID.text = "";
        }
    }
}