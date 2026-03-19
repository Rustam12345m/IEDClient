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

ModalDialog {
    id: rootID

    property string doReference: ""
    property alias text: msgText.text
    property alias value: valueBox.text

    signal sigResult(bool t_user, string t_ref, string t_value)

    title: "Change Value"
    dialogWidth: 450
    dialogHeight: 200
    closePolicy: Popup.CloseOnEscape

    ColumnLayout {
        anchors.fill: parent
        spacing: 10

        Label {
            id: msgText

            Layout.fillWidth: true
            Layout.topMargin: 10

            horizontalAlignment: Text.AlignHCenter
            color: VisualStyle.statusBar.textColor
            font.pixelSize: 14

            text: ""
        }

        TextField {
            id: valueBox

            Layout.fillWidth: true
            Layout.leftMargin: 10
            Layout.rightMargin: 10

            verticalAlignment: Text.AlignVCenter
            focus: true
            font.pixelSize: 14

            text: ""

            Keys.onReturnPressed: {
                sigResult(true, rootID.doReference, valueBox.text)
                rootID.close()
            }
            Keys.onEnterPressed: {
                sigResult(true, rootID.doReference, valueBox.text)
                rootID.close()
            }
        }

        Item { Layout.fillHeight: true }

        RowLayout {
            Layout.alignment: Qt.AlignHCenter
            Layout.bottomMargin: 5
            spacing: 20

            Button {
                text: "Ok"
                onClicked: {
                    sigResult(true, rootID.doReference, valueBox.text)
                    rootID.close()
                }
            }
            Button {
                text: "Cancel"
                onClicked: {
                    rootID.close()
                }
            }
        }
    }

    function isActive() {
        return rootID.visible
    }

    function open(t_ref, t_msg, t_value) {
        rootID.doReference = t_ref
        msgText.text = t_msg
        valueBox.text = t_value
        rootID.open()
    }
}
