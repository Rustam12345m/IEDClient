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

    height: VisualStyle.statusBar.height

    color: VisualStyle.statusBar.color
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

            horizontalAlignment: Text.AlignLeft
            verticalAlignment: Text.AlignVCenter
            elide: Text.ElideRight

            text: ""
            color: VisualStyle.statusBar.textColor

            onTextChanged: {
                if (msgStatusTextID.text != "") {
                    timerStatusMsgID.restart()
                }
            }
        }
        // TX/RX byte counters
        RowLayout {
            spacing: 4
            visible: appConnStatus.isConnected

            property string txText: "0"
            property string rxText: "0"

            function formatKB(bytes) {
                return (bytes / 1024).toFixed(1)
            }

            Text {
                text: "\u2191" // ↑
                color: VisualStyle.statusBar.textColor
                font.pixelSize: VisualStyle.fontSizeSmall
                Layout.alignment: Qt.AlignVCenter
            }
            Text {
                id: txValueText
                text: parent.txText + " KB"
                color: VisualStyle.statusBar.textColor
                font.pixelSize: VisualStyle.fontSizeSmall
                Layout.alignment: Qt.AlignVCenter
            }
            Text {
                text: "\u2193" // ↓
                color: VisualStyle.statusBar.textColor
                font.pixelSize: VisualStyle.fontSizeSmall
                Layout.alignment: Qt.AlignVCenter
                Layout.leftMargin: 4
            }
            Text {
                id: rxValueText
                text: parent.rxText + " KB"
                color: VisualStyle.statusBar.textColor
                font.pixelSize: VisualStyle.fontSizeSmall
                Layout.alignment: Qt.AlignVCenter
            }

            Timer {
                interval: 1000
                running: appConnStatus.isConnected
                repeat: true
                onTriggered: {
                    parent.txText = parent.formatKB(presenter.getTxBytes())
                    parent.rxText = parent.formatKB(presenter.getRxBytes())
                }
            }
        }
        Rectangle {
            Layout.alignment: Qt.AlignVCenter

            width: 1
            height: parent.height - 4
            color: VisualStyle.modalColor
        }
        Text {
            Layout.fillWidth: false
            Layout.alignment: Qt.AlignVCenter

            id: pageStatusTextID
            height: parent.height
            Layout.maximumWidth: 400

            horizontalAlignment: Text.AlignLeft
            verticalAlignment: Text.AlignVCenter
            elide: Text.ElideRight

            font.bold: VisualStyle.boldHeaderText
            color: VisualStyle.statusBar.textColor
            text: ""
        }
        Rectangle {
            Layout.alignment: Qt.AlignVCenter

            width: 1
            height: parent.height - 4
            color: VisualStyle.modalColor
        }
        RowLayout {
            spacing: 5

            Text {
                Layout.alignment: Qt.AlignVCenter

                id: conStatusTextID

                horizontalAlignment: Text.AlignLeft
                verticalAlignment: Text.AlignVCenter

                color: VisualStyle.statusBar.textColor
                text: appConnStatus.text
            }
            Led {
                Layout.alignment: Qt.AlignVCenter

                height: 20
                width: 20
                color: appConnStatus.isConnected
                       ? VisualStyle.statusBar.connectedColor
                       : VisualStyle.statusBar.disconnectedColor
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
