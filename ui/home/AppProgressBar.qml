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

import AppStylesModule

import "qrc:/common/"

ModalDialog
{
    id: rootID

    title: "Please wait..."
    dialogWidth: 500
    dialogHeight: 150
    closePolicy: Popup.NoAutoClose

    ColumnLayout {
        anchors.fill: parent
        spacing: 10

        Text {
            id: progressText

            Layout.fillWidth: true
            Layout.topMargin: 10

            horizontalAlignment: Text.AlignHCenter
            font.bold: VisualStyle.boldHeaderText
            font.pixelSize: 14
            color: VisualStyle.statusBar.textColor

            text: "Please wait..."
        }

        ProgressBar {
            id: progressBar

            Layout.fillWidth: true
            Layout.leftMargin: 10
            Layout.rightMargin: 10

            from: 0.0
            to: 100.0
            value: 0.0

            Text {
                id: progressValue

                anchors.centerIn: parent
                z: 1

                text: ""
                color: VisualStyle.statusBar.textColor
            }
        }

        Item { Layout.fillHeight: true }
    }

    function isActive() {
        return rootID.visible
    }
    function startLoad() {
        progressBar.value = 0
        progressText.text = "Please wait..."
        progressValue.text = ""
        rootID.open()
    }
    function finishLoad() {
        rootID.close()
    }
    function updateLoad(t_perc, t_msg) {
        progressBar.value = t_perc
        progressValue.text = t_perc + " %"
        progressText.text = t_msg
    }
}
