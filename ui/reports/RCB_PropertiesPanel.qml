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
    property int defRowHeight: 30
    property int defTextPadding: 5
    property int defNameWidth: 100
    readonly property string sectionColor: "#f6f6f6"

    //anchors.fill: parent
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
                color: sectionColor

                border.width: 1
                border.color: "lightgray"

                Text {
                    anchors.fill: parent

                    font.bold: VisualStyle.boldHeaderText
                    //font.pixelSize: 16

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
                color: sectionColor

                border.width: 1
                border.color: "lightgray"

                Text {
                    anchors.fill: parent

                    font.bold: VisualStyle.boldHeaderText
                    //font.pixelSize: 16

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
                color: sectionColor

                border.width: 1
                border.color: "lightgray"

                Text {
                    anchors.fill: parent

                    font.bold: VisualStyle.boldHeaderText
                    //font.pixelSize: 16

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
                    checked: true
                    text: "Data change"
                }
                CheckBox {
                    checked: true
                    text: "Quality change"
                }
                CheckBox {
                    checked: false
                    text: "Data update"
                }
                CheckBox {
                    checked: true
                    text: "Integrity"
                }
                CheckBox {
                    checked: false
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
                color: "lightgray"
            }
            RowLayout {
                width: parent.width
                height: defRowHeight + 12
                spacing: 0

                Rectangle {
                    Layout.preferredWidth: parent.width / 2
                    height: parent.height

                    // border.color: "green"
                    // border.width: 4
                    color: "green"

                    Button {
                        anchors.centerIn: parent
                        text: "Enable"
                    }
                }
                Rectangle {
                    Layout.preferredWidth: parent.width / 2
                    height: parent.height

                    // border.color: "red"
                    // border.width: 4
                    color: "red"

                    Button {
                        anchors.centerIn: parent
                        text: "Disable"
                    }
                }
            }
        }
    }
}
