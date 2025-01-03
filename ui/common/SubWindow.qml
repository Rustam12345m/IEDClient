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
import QtQuick.Layouts

FocusScope
{
    id: rootID

    property alias title: titleText.text
    property alias color: subWindowID.color
    default property alias contentArea: contentContainerID.data
    property color borderColor: "black"

    signal sigClicked()

    // SubWindow
    Rectangle
    {
        id: subWindowID
        width: parent.width
        height: parent.height

        MouseArea {
            anchors.fill: parent
            // drag.target: subWindowID

            onClicked: function() {
                sigClicked()
            }
        }

        // Frame border
        Rectangle {
            id: frameID
            anchors {
                fill: parent
                margins: 8
                // leftMargin: 6
                // rightMargin: 8
                // topMargin: 10
                // bottomMargin: 10
            }

            color: subWindowID.color
            border.color: rootID.borderColor
            border.width: 2

            Rectangle {
                anchors {
                    margins: 6
                    fill: parent
                }
                border.color: rootID.borderColor
                border.width: 2

                color: subWindowID.color

                Rectangle {
                    anchors {
                        margins: 8
                        // leftMargin: 6
                        // rightMargin: 8
                        topMargin: 12
                        bottomMargin: 12
                        fill: parent
                    }

                    color: subWindowID.color

                    // Content area
                    Item {
                        id: contentContainerID
                        anchors.fill: parent
                    }
                }
            }
        }

        // Title bar
        Rectangle {
            id: titleBarID

            anchors {
                top: parent.top
                horizontalCenter: parent.horizontalCenter
                topMargin: 0
            }

            width: titleText.width + 30
            height: 24
            z: 100500

            color: subWindowID.color

            Text {
                id: titleText
                anchors.centerIn: parent

                color: "black" //"#eeeeee"
                font.pixelSize: 16
                font.bold: true
            }
        }
    }
}

