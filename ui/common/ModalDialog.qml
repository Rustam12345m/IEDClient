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

// Reusable TUI-style modal dialog with overlay backdrop and double-line border.
// Uses Popup so it is positioned relative to the application window.
Popup
{
    id: rootID

    property string title: ""
    property alias dialogWidth: rootID.width
    property alias dialogHeight: rootID.height
    property color bgColor: VisualStyle.modalColor
    default property alias contentArea: contentContainerID.data

    modal: true
    focus: true
    closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside

    anchors.centerIn: Overlay.overlay

    padding: 0
    background: Item {}

    Overlay.modal: Rectangle {
        color: Qt.rgba(
            VisualStyle.overlayColor.r,
            VisualStyle.overlayColor.g,
            VisualStyle.overlayColor.b,
            0.75)
    }

    // The dialog window content
    Rectangle {
        id: bgID
        anchors.fill: parent
        color: rootID.bgColor

        // Outer border
        Rectangle {
            id: outerFrameID
            anchors {
                fill: parent
                margins: 8
            }
            color: rootID.bgColor
            border.color: "white"
            border.width: 2

            // Inner border
            Rectangle {
                anchors {
                    fill: parent
                    margins: 6
                }
                color: rootID.bgColor
                border.color: "white"
                border.width: 2

                // Content area
                Item {
                    id: contentContainerID
                    anchors {
                        fill: parent
                        margins: 8
                        topMargin: 12
                        bottomMargin: 12
                    }
                }
            }
        }

        // Title bar
        Rectangle {
            anchors {
                top: parent.top
                horizontalCenter: parent.horizontalCenter
            }
            width: titleText.width + 30
            height: 24
            z: 100500
            color: rootID.bgColor

            Text {
                id: titleText
                anchors.centerIn: parent
                text: rootID.title
                color: VisualStyle.statusBar.textColor
                font.pixelSize: 16
                font.bold: true
            }
        }
    }
}
