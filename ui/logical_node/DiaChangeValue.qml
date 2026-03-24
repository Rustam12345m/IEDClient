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
    property string fc: ""
    property string statusMsg: ""
    property bool lastResultSuccess: true

    signal sigWrite(string ref, string fcStr, string val)

    title: "Write Value"
    dialogWidth: 450
    dialogHeight: 260
    closePolicy: Popup.CloseOnEscape

    function openWrite(ref, fcStr, msg, val) {
        rootID.doReference = ref
        rootID.fc = fcStr
        msgText.text = msg
        valueBox.text = val
        rootID.statusMsg = ""
        rootID.lastResultSuccess = true
        rootID.open()
        valueBox.forceActiveFocus()
        valueBox.selectAll()
    }

    function setResult(success, message) {
        lastResultSuccess = success
        statusMsg = message
    }

    function isActive() {
        return rootID.visible
    }

    function fcDescription(f) {
        switch (f) {
        case "SP": return "Setpoint"
        case "SV": return "Substitution"
        case "SE": return "Setting (editable)"
        case "CF": return "Configuration"
        case "DC": return "Description"
        case "BL": return "Blocking"
        default: return f
        }
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 8
        spacing: 8

        // Reference
        Label {
            id: msgText
            Layout.fillWidth: true
            color: VisualStyle.statusBar.textColor
            font.pixelSize: VisualStyle.fontSizeMedium
            elide: Text.ElideMiddle
            text: ""
        }

        // FC
        Label {
            Layout.fillWidth: true
            color: VisualStyle.statusBar.textColor
            font.pixelSize: VisualStyle.fontSizeMedium
            text: "FC: " + rootID.fc + " (" + fcDescription(rootID.fc) + ")"
            visible: rootID.fc.length > 0
        }

        // Value input
        TextField {
            id: valueBox

            Layout.fillWidth: true

            verticalAlignment: Text.AlignVCenter
            focus: true
            font.pixelSize: 14

            text: ""

            Keys.onReturnPressed: {
                rootID.sigWrite(rootID.doReference, rootID.fc, valueBox.text)
            }
            Keys.onEnterPressed: {
                rootID.sigWrite(rootID.doReference, rootID.fc, valueBox.text)
            }
        }

        Item { Layout.fillHeight: true }

        // Status message
        Text {
            Layout.fillWidth: true
            text: rootID.statusMsg
            color: rootID.lastResultSuccess ? VisualStyle.successColor : VisualStyle.errorTextColor
            font.pixelSize: VisualStyle.fontSizeMedium
            wrapMode: Text.WordWrap
            visible: rootID.statusMsg.length > 0
        }

        RowLayout {
            Layout.alignment: Qt.AlignHCenter
            Layout.bottomMargin: 5
            spacing: 20

            Button {
                text: "Write"
                onClicked: {
                    rootID.statusMsg = "Writing..."
                    rootID.lastResultSuccess = true
                    rootID.sigWrite(rootID.doReference, rootID.fc, valueBox.text)
                }
            }
            Button {
                text: "Close"
                onClicked: {
                    rootID.close()
                }
            }
        }
    }
}
