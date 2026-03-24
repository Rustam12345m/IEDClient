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

Item {
    id: rootID

    property string ldRef: ""
    property int numOfSG: 0
    property int actSG: 0
    property int editSG: 0
    property string statusMsg: ""

    visible: numOfSG > 0
    implicitHeight: visible ? col.implicitHeight + 8 : 0

    function refresh() {
        if (ldRef.length === 0) return
        var info = iedBackend.getSGCBInfo(ldRef)
        if (Object.keys(info).length === 0) return
        numOfSG = info.numOfSG
        actSG = info.actSG
        editSG = info.editSG
        statusMsg = editSG > 0 ? "Editing group " + editSG : ""
    }

    function buildModel(count) {
        var items = []
        for (var i = 1; i <= count; i++) items.push(String(i))
        return items
    }

    Rectangle {
        anchors.fill: parent
        color: VisualStyle.section.bg
        border.color: VisualStyle.section.border
        border.width: 1
    }

    ColumnLayout {
        id: col
        anchors { fill: parent; margins: 4 }
        spacing: 4

        // Header
        Text {
            text: "Setting Groups"
            font.bold: true
            font.pixelSize: VisualStyle.fontSizeMedium
            color: VisualStyle.section.text
        }

        // Active SG row
        RowLayout {
            spacing: 8
            Text { text: "Groups: " + rootID.numOfSG; font.pixelSize: VisualStyle.fontSizeMedium; color: VisualStyle.textColor }
            Text { text: "Active:"; font.pixelSize: VisualStyle.fontSizeMedium; color: VisualStyle.textColor }
            ComboBox {
                id: actSGCombo
                model: buildModel(rootID.numOfSG)
                currentIndex: rootID.actSG - 1
                implicitWidth: 60
            }
            Button {
                text: "Apply"
                onClicked: {
                    iedBackend.setActiveSG(rootID.ldRef, actSGCombo.currentIndex + 1)
                }
            }
        }

        // Edit session row
        RowLayout {
            spacing: 8
            Text { text: "Edit:"; font.pixelSize: VisualStyle.fontSizeMedium; color: VisualStyle.textColor }
            ComboBox {
                id: editSGCombo
                model: buildModel(rootID.numOfSG)
                currentIndex: rootID.editSG > 0 ? rootID.editSG - 1 : 0
                implicitWidth: 60
                enabled: rootID.editSG === 0
            }
            Button {
                text: "Start Edit"
                enabled: rootID.editSG === 0
                onClicked: {
                    iedBackend.selectEditSG(rootID.ldRef, editSGCombo.currentIndex + 1)
                }
            }
            Button {
                text: "Cancel"
                enabled: rootID.editSG > 0
                onClicked: {
                    iedBackend.cancelEditSG(rootID.ldRef)
                }
            }
            Button {
                text: "Confirm"
                enabled: rootID.editSG > 0
                onClicked: {
                    iedBackend.confirmEditSG(rootID.ldRef)
                }
            }
        }

        // Status
        Text {
            text: rootID.statusMsg
            font.pixelSize: VisualStyle.fontSizeSmall
            color: rootID.editSG > 0 ? "#2e7d32" : VisualStyle.textColor
            visible: rootID.statusMsg.length > 0
        }
    }

    Connections {
        target: iedBackend
        function onSigSGCBUpdated(updatedLdRef) {
            if (updatedLdRef === rootID.ldRef) {
                rootID.refresh()
            }
        }
    }
}
