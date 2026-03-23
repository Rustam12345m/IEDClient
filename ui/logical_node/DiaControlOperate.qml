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

// Modal dialog for Direct Control / SBO operations
ModalDialog
{
    id: rootID

    title: "Control"
    dialogWidth: 420
    dialogHeight: 390

    property string objectRef: ""
    property int ctlModel: -1
    property int ctlValType: -1
    property string statusMsg: ""
    property bool lastResultSuccess: true

    signal sigOperate(string ref, int model, int valType, var value)
    signal sigSelect(string ref, int model, int valType, var value)
    signal sigCancel(string ref)

    function openControl(ref) {
        objectRef = ref
        ctlModel = -1
        ctlValType = -1
        statusMsg = "Loading..."
        rootID.open()
    }

    function setControlInfo(model, valType) {
        ctlModel = model
        ctlValType = valType
        statusMsg = (model === 0) ? "Status only — not controllable" : ""
    }

    function setResult(success, message) {
        lastResultSuccess = success
        statusMsg = message
    }

    function getValue() {
        switch (ctlValType) {
        case 0: return boolSwitch.checked   // Boolean
        case 1: return intSpin.value        // Integer
        case 2: return intSpin.value        // Unsigned
        case 3: return parseFloat(floatField.text) || 0.0  // Float
        default: return null
        }
    }

    function modelName(m) {
        switch (m) {
        case 0: return "Status Only"
        case 1: return "Direct (Normal)"
        case 2: return "SBO (Normal)"
        case 3: return "Direct (Enhanced)"
        case 4: return "SBO (Enhanced)"
        default: return "Loading..."
        }
    }

    function valTypeName(v) {
        switch (v) {
        case 0: return "Boolean"
        case 1: return "Integer"
        case 2: return "Unsigned"
        case 3: return "Float"
        default: return "Unknown"
        }
    }

    Connections {
        target: iedBackend
        function onSigCommandTermination(objRef, success, addCause) {
            if (rootID.visible && objRef === rootID.objectRef) {
                rootID.lastResultSuccess = success
                rootID.statusMsg = (success ? "CommandTermination+: " : "CommandTermination-: ") + addCause
            }
        }
    }

    ColumnLayout {
        anchors {
            fill: parent
            margins: 8
        }
        spacing: 8

        // Object reference
        RowLayout {
            Layout.fillWidth: true
            Text {
                text: "Object:"
                color: VisualStyle.statusBar.textColor
                font.bold: true
                font.pixelSize: 12
            }
            Text {
                text: rootID.objectRef
                color: VisualStyle.statusBar.textColor
                font.pixelSize: 12
                Layout.fillWidth: true
                elide: Text.ElideMiddle
            }
        }

        // Control model
        RowLayout {
            Layout.fillWidth: true
            Text {
                text: "Model:"
                color: VisualStyle.statusBar.textColor
                font.bold: true
                font.pixelSize: 12
            }
            Text {
                text: modelName(rootID.ctlModel)
                color: VisualStyle.statusBar.textColor
                font.pixelSize: 12
            }
        }

        // Value type
        RowLayout {
            Layout.fillWidth: true
            visible: rootID.ctlModel > 0
            Text {
                text: "Type:"
                color: VisualStyle.statusBar.textColor
                font.bold: true
                font.pixelSize: 12
            }
            Text {
                text: valTypeName(rootID.ctlValType)
                color: VisualStyle.statusBar.textColor
                font.pixelSize: 12
            }
        }

        // Separator
        Rectangle {
            Layout.fillWidth: true
            height: 1
            color: VisualStyle.statusBar.textColor
            visible: rootID.ctlModel > 0
        }

        // Value input — Boolean
        RowLayout {
            Layout.fillWidth: true
            visible: rootID.ctlModel > 0 && rootID.ctlValType === 0
            Text {
                text: "Value:"
                color: VisualStyle.statusBar.textColor
                font.bold: true
                font.pixelSize: 12
            }
            Switch {
                id: boolSwitch
                text: checked ? "true" : "false"
            }
        }

        // Value input — Integer / Unsigned
        RowLayout {
            Layout.fillWidth: true
            visible: rootID.ctlModel > 0 && (rootID.ctlValType === 1 || rootID.ctlValType === 2)
            Text {
                text: "Value:"
                color: VisualStyle.statusBar.textColor
                font.bold: true
                font.pixelSize: 12
            }
            SpinBox {
                id: intSpin
                from: rootID.ctlValType === 2 ? 0 : -2147483647
                to: 2147483647
                value: 0
                editable: true
            }
        }

        // Value input — Float
        RowLayout {
            Layout.fillWidth: true
            visible: rootID.ctlModel > 0 && rootID.ctlValType === 3
            Text {
                text: "Value:"
                color: VisualStyle.statusBar.textColor
                font.bold: true
                font.pixelSize: 12
            }
            TextField {
                id: floatField
                text: "0.0"
                validator: DoubleValidator {}
                Layout.preferredWidth: 120
            }
        }

        // Control flags
        CheckBox {
            id: testCheck
            text: "Test mode"
            visible: rootID.ctlModel > 0
        }
        RowLayout {
            Layout.fillWidth: true
            visible: rootID.ctlModel > 0
            CheckBox {
                id: interlockCheck
                text: "Interlock check"
            }
            CheckBox {
                id: synchroCheck
                text: "Synchrocheck"
            }
        }

        // Spacer
        Item { Layout.fillHeight: true }

        // Status message
        Text {
            Layout.fillWidth: true
            text: rootID.statusMsg
            color: rootID.lastResultSuccess ? "yellow" : "red"
            font.pixelSize: 12
            wrapMode: Text.WordWrap
            visible: rootID.statusMsg.length > 0
        }

        // Buttons
        RowLayout {
            Layout.fillWidth: true
            spacing: 8

            Button {
                text: "Select"
                visible: rootID.ctlModel === 2 || rootID.ctlModel === 4
                onClicked: {
                    rootID.lastResultSuccess = true
                    rootID.statusMsg = "Selecting..."
                    iedBackend.setTestMode(testCheck.checked)
                    iedBackend.setInterlockCheck(interlockCheck.checked)
                    iedBackend.setSynchroCheck(synchroCheck.checked)
                    rootID.sigSelect(rootID.objectRef, rootID.ctlModel,
                                     rootID.ctlValType, rootID.getValue())
                }
            }
            Button {
                text: "Operate"
                enabled: rootID.ctlModel > 0
                onClicked: {
                    rootID.lastResultSuccess = true
                    rootID.statusMsg = "Operating..."
                    iedBackend.setTestMode(testCheck.checked)
                    iedBackend.setInterlockCheck(interlockCheck.checked)
                    iedBackend.setSynchroCheck(synchroCheck.checked)
                    rootID.sigOperate(rootID.objectRef, rootID.ctlModel,
                                      rootID.ctlValType, rootID.getValue())
                }
            }
            Button {
                text: "Cancel"
                visible: rootID.ctlModel > 0
                onClicked: {
                    rootID.statusMsg = "Cancelling..."
                    rootID.sigCancel(rootID.objectRef)
                }
            }
            Item { Layout.fillWidth: true }
            Button {
                text: "Close"
                onClicked: rootID.close()
            }
        }
    }
}
