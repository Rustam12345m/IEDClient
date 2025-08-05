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
    required property bool selected

    signal sigDownloadFile(int row)
    signal sigRemoveFile(int row)

    id: rootID
    implicitWidth: downloadBtn.width + progressBar.width + removeBtn.width + 5

    Rectangle {
        anchors.fill: parent

        border.width: 1
        border.color: VisualStyle.table.rowBorderColor2
        color: (selected ? VisualStyle.table.rowColor1 : VisualStyle.table.rowColor2)

        Row {
            anchors {
                leftMargin: 5
                rightMargin: 5
                fill: parent
            }
            spacing: 5

            Button {
                id: downloadBtn

                width: rootID.height
                height: rootID.height

                icon.source: "qrc:/img/icons/download.svg"

                onClicked: function() {
                    sigDownloadFile(row)
                }
            }

            ProgressBar {
                id: progressBar

                width: 120
                height: rootID.height

                from: 0.0
                to: 100.0
                value: 0.0

                Text {
                    anchors.centerIn: parent
                    text: progressBar.value + " %"

                    z: 1
                }
            }

            Button {
                id: removeBtn

                width: rootID.height
                height: rootID.height

                icon.source: "qrc:/img/icons/clear.svg"

                onClicked: function() {
                    sigRemoveFile(row)
                }
            }
        }
    }
}
