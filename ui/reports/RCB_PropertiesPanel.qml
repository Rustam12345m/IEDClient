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

import GlobalVarsModule
import AppStylesModule

import "qrc:/common/"

// RCB Settings
Item
{
    id: rootID

    property int defRowHeight: VisualStyle.rowHeight
    property int defTextPadding: 5
    property int defNameWidth: 100

    property var rcbModel: null

    function loadFromModel() {
        if (!rcbModel) return
        rcbID.text = rcbModel.selectedRptId()
        dataSetID.text = rcbModel.selectedDsRef()

        var trg = rcbModel.selectedTrgOps()
        chkDataChange.checked   = (trg & 2) !== 0
        chkQualChange.checked   = (trg & 4) !== 0
        chkDataUpdate.checked   = (trg & 8) !== 0
        chkIntegrity.checked    = (trg & 16) !== 0
        chkGI.checked           = (trg & 32) !== 0
    }

    onRcbModelChanged: loadFromModel()

    clip: true

    Column {
        anchors.fill: parent

        spacing: 8

        // Main section
        Column {
            width: parent.width
            spacing: 2

            Rectangle {
                width: parent.width
                height: defRowHeight
                color: VisualStyle.section.bg

                border.width: 1
                border.color: VisualStyle.section.border

                Text {
                    anchors.fill: parent

                    font.bold: VisualStyle.boldHeaderText

                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter

                    text: qsTr("Report control block")
                }
            }
            RowLayout {
                width: parent.width
                height: defRowHeight + 8

                Text {
                    Layout.preferredWidth: 60

                    height: defRowHeight

                    text: "ID"
                    horizontalAlignment: Text.AlignRight
                    verticalAlignment: Text.AlignVCenter
                }
                TextField {
                    Layout.fillWidth: true

                    id: rcbID
                    height: defRowHeight

                    readOnly: true
                    text: ""
                }
            }
            RowLayout {
                width: parent.width

                Text {
                    Layout.preferredWidth: 60
                    height: defRowHeight

                    text: "DataSet"
                    horizontalAlignment: Text.AlignRight
                    verticalAlignment: Text.AlignVCenter
                }
                TextField {
                    Layout.fillWidth: true
                    id: dataSetID
                    height: defRowHeight

                    readOnly: true
                    text: ""
                }
            }
        }

        // Optional fields section
        Column {
            width: parent.width
            spacing: 0

            Rectangle {
                width: parent.width
                height: defRowHeight
                color: VisualStyle.section.bg

                border.width: 1
                border.color: VisualStyle.section.border

                Text {
                    anchors.fill: parent

                    font.bold: VisualStyle.boldHeaderText

                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter

                    text: qsTr("Optional fields")
                }
            }
            GridLayout {
                width: parent.width

                columns: 2
                columnSpacing: 0
                rowSpacing: 0

                CheckBox {
                    checked: true
                    text: "SeqNum"
                }
                CheckBox {
                    checked: true
                    text: "Timestamp"
                }
                CheckBox {
                    checked: true
                    text: "Reason"
                }
                CheckBox {
                    checked: true
                    text: "Overflow"
                }
                CheckBox {
                    checked: true
                    text: "Entry ID"
                }
                CheckBox {
                    checked: true
                    text: "ConfRev"
                }
                CheckBox {
                    checked: false
                    text: "DataSetName"
                }
                CheckBox {
                    checked: false
                    text: "Reference"
                }
            }
        }

        // Triggers section
        Column {
            width: parent.width
            spacing: 0

            Rectangle {
                width: parent.width
                height: defRowHeight
                color: VisualStyle.section.bg

                border.width: 1
                border.color: VisualStyle.section.border

                Text {
                    anchors.fill: parent

                    font.bold: VisualStyle.boldHeaderText

                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter

                    text: qsTr("Triggers")
                }
            }
            GridLayout {
                width: parent.width

                columns: 1
                columnSpacing: 0
                rowSpacing: 0

                CheckBox {
                    id: chkDataChange
                    text: "Data change"
                }
                CheckBox {
                    id: chkQualChange
                    text: "Quality change"
                }
                CheckBox {
                    id: chkDataUpdate
                    text: "Data update"
                }
                CheckBox {
                    id: chkIntegrity
                    text: "Integrity"
                }
                CheckBox {
                    id: chkGI
                    text: "General Interrogation"
                }
            }
        }

        // Controls section
        Column {
            width: parent.width
            spacing: 0

            Rectangle {
                height: 4
                width: parent.width
                color: VisualStyle.section.border
            }
            RowLayout {
                width: parent.width
                height: defRowHeight + 12
                spacing: 4

                Button {
                    Layout.fillWidth: true
                    text: "Enable"
                }
                Button {
                    Layout.fillWidth: true
                    text: "Disable"
                }
            }
        }
    }
}
