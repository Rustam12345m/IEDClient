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

// GOOSE Control Block Properties
Item
{
    id: rootID

    property int defRowHeight: VisualStyle.rowHeight
    property int defTextPadding: 5

    property var gooseModel: null

    signal sigEnable(string goId, string datSet)
    signal sigDisable()
    signal sigGoToDataSet(string dsRef)

    function loadFromModel() {
        if (!gooseModel) return
        gooseIdField.text   = gooseModel.selectedGoId()
        confRevText.text    = String(gooseModel.selectedConfRev())
        minTimeText.text    = gooseModel.selectedMinTime() + " ms"
        maxTimeText.text    = gooseModel.selectedMaxTime() + " ms"
        appIdText.text      = "0x" + ("0000" + gooseModel.selectedAppId().toString(16)).slice(-4).toUpperCase()
        vlanIdText.text     = String(gooseModel.selectedVlanId())
        vlanPriText.text    = String(gooseModel.selectedVlanPriority())

        // Populate DataSet ComboBox
        var refs = iedBackend.getDataSetRefs()
        var currentDs = gooseModel.selectedDatSet()
        var idx = refs.indexOf(currentDs)

        if (currentDs.length > 0 && idx < 0) {
            refs.unshift(currentDs)
            datSetCombo.unknownValue = true
            idx = 0
        } else {
            datSetCombo.unknownValue = false
        }
        datSetCombo.model = refs
        datSetCombo.currentIndex = idx >= 0 ? idx : 0
    }

    onGooseModelChanged: loadFromModel()

    clip: true

    Column {
        anchors.fill: parent
        spacing: 2

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
                    text: qsTr("GOOSE Control Block")
                }
            }

            // GOOSE ID (editable)
            RowLayout {
                width: parent.width
                height: defRowHeight + 2

                Text {
                    Layout.preferredWidth: 70
                    height: defRowHeight
                    text: "ID"
                    horizontalAlignment: Text.AlignRight
                    verticalAlignment: Text.AlignVCenter
                }
                TextField {
                    Layout.fillWidth: true
                    id: gooseIdField
                    height: defRowHeight
                    text: ""
                    activeFocusOnPress: true
                    selectByMouse: true
                }
            }

            // DataSet (editable via ComboBox)
            RowLayout {
                width: parent.width
                height: defRowHeight + 2

                Text {
                    Layout.preferredWidth: 70
                    height: defRowHeight
                    text: "DataSet"
                    horizontalAlignment: Text.AlignRight
                    verticalAlignment: Text.AlignVCenter
                }
                ComboBox {
                    Layout.fillWidth: true
                    id: datSetCombo

                    property bool unknownValue: false

                    delegate: ItemDelegate {
                        width: datSetCombo.width
                        contentItem: Text {
                            text: modelData
                            color: (index === 0 && datSetCombo.unknownValue) ? VisualStyle.errorTextColor : VisualStyle.textColor
                            font.bold: index === datSetCombo.currentIndex
                            elide: Text.ElideRight
                        }
                        highlighted: datSetCombo.highlightedIndex === index
                        background: Rectangle {
                            color: index === datSetCombo.currentIndex
                                   ? VisualStyle.table.selRowColor
                                   : (highlighted ? palette.highlight : "transparent")
                        }
                    }
                }
                Button {
                    width: defRowHeight
                    height: defRowHeight
                    icon.source: "qrc:/img/icons/arrow_forward.svg"
                    focusPolicy: Qt.NoFocus
                    onClicked: sigGoToDataSet(datSetCombo.currentText)
                }
            }

            // ConfRev (read-only)
            RowLayout {
                width: parent.width
                height: defRowHeight + 2

                Text {
                    Layout.preferredWidth: 70
                    height: defRowHeight
                    text: "ConfRev"
                    horizontalAlignment: Text.AlignRight
                    verticalAlignment: Text.AlignVCenter
                }
                Text {
                    id: confRevText
                    Layout.fillWidth: true
                    Layout.leftMargin: defTextPadding
                    height: defRowHeight
                    text: ""
                    verticalAlignment: Text.AlignVCenter
                    color: VisualStyle.textColor
                }
            }
        }

        // Timing section (read-only)
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
                    text: qsTr("Timing")
                }
            }

            RowLayout {
                width: parent.width
                height: defRowHeight + 2

                Text {
                    Layout.preferredWidth: 70
                    height: defRowHeight
                    text: "MinTime"
                    horizontalAlignment: Text.AlignRight
                    verticalAlignment: Text.AlignVCenter
                }
                Text {
                    id: minTimeText
                    Layout.fillWidth: true
                    Layout.leftMargin: defTextPadding
                    height: defRowHeight
                    text: ""
                    verticalAlignment: Text.AlignVCenter
                    color: VisualStyle.textColor
                }
            }
            RowLayout {
                width: parent.width
                height: defRowHeight + 2

                Text {
                    Layout.preferredWidth: 70
                    height: defRowHeight
                    text: "MaxTime"
                    horizontalAlignment: Text.AlignRight
                    verticalAlignment: Text.AlignVCenter
                }
                Text {
                    id: maxTimeText
                    Layout.fillWidth: true
                    Layout.leftMargin: defTextPadding
                    height: defRowHeight
                    text: ""
                    verticalAlignment: Text.AlignVCenter
                    color: VisualStyle.textColor
                }
            }
        }

        // Network section (read-only)
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
                    text: qsTr("Network")
                }
            }

            RowLayout {
                width: parent.width
                height: defRowHeight + 2

                Text {
                    Layout.preferredWidth: 70
                    height: defRowHeight
                    text: "AppId"
                    horizontalAlignment: Text.AlignRight
                    verticalAlignment: Text.AlignVCenter
                }
                Text {
                    id: appIdText
                    Layout.fillWidth: true
                    Layout.leftMargin: defTextPadding
                    height: defRowHeight
                    text: ""
                    verticalAlignment: Text.AlignVCenter
                    color: VisualStyle.textColor
                }
            }
            RowLayout {
                width: parent.width
                height: defRowHeight + 2

                Text {
                    Layout.preferredWidth: 70
                    height: defRowHeight
                    text: "VLAN Id"
                    horizontalAlignment: Text.AlignRight
                    verticalAlignment: Text.AlignVCenter
                }
                Text {
                    id: vlanIdText
                    Layout.fillWidth: true
                    Layout.leftMargin: defTextPadding
                    height: defRowHeight
                    text: ""
                    verticalAlignment: Text.AlignVCenter
                    color: VisualStyle.textColor
                }
            }
            RowLayout {
                width: parent.width
                height: defRowHeight + 2

                Text {
                    Layout.preferredWidth: 70
                    height: defRowHeight
                    text: "VLAN Pri"
                    horizontalAlignment: Text.AlignRight
                    verticalAlignment: Text.AlignVCenter
                }
                Text {
                    id: vlanPriText
                    Layout.fillWidth: true
                    Layout.leftMargin: defTextPadding
                    height: defRowHeight
                    text: ""
                    verticalAlignment: Text.AlignVCenter
                    color: VisualStyle.textColor
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
                height: defRowHeight + 6
                spacing: 4

                Button {
                    Layout.fillWidth: true
                    text: "Enable"
                    onClicked: sigEnable(gooseIdField.text, datSetCombo.currentText)
                }
                Button {
                    Layout.fillWidth: true
                    text: "Disable"
                    onClicked: sigDisable()
                }
            }
        }
    }
}
