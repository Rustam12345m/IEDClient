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

// Sampled Values Control Block Properties
Item
{
    id: rootID

    property int defRowHeight: VisualStyle.rowHeight
    property int defTextPadding: 5

    property var svModel: null

    signal sigEnable()
    signal sigDisable()

    function loadFromModel() {
        if (!svModel) return
        svEnaLed.color    = svModel.selectedSvEna() ? "#4CAF50" : "gray"
        svEnaText.text    = svModel.selectedSvEna() ? "Yes" : "No"
        svTypeText.text   = svModel.selectedIsMulticast() ? "MSVCB" : "USVCB"
        svIdText.text     = svModel.selectedSvId()
        datSetText.text   = svModel.selectedDatSet()
        confRevText.text  = String(svModel.selectedConfRev())
        smpRateText.text  = String(svModel.selectedSmpRate())
        noAsduText.text   = String(svModel.selectedNoASDU())
    }

    onSvModelChanged: loadFromModel()

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
                    text: qsTr("SV Control Block")
                }
            }

            // Enabled
            RowLayout {
                width: parent.width
                height: defRowHeight + 2

                Text {
                    Layout.preferredWidth: 60
                    height: defRowHeight
                    text: "Enabled"
                    horizontalAlignment: Text.AlignRight
                    verticalAlignment: Text.AlignVCenter
                }
                Row {
                    spacing: 5
                    Layout.leftMargin: defTextPadding

                    Rectangle {
                        id: svEnaLed
                        anchors.verticalCenter: parent.verticalCenter
                        width: 12; height: 12; radius: 6
                        color: "gray"
                    }
                    Text {
                        id: svEnaText
                        anchors.verticalCenter: parent.verticalCenter
                        text: "No"
                        color: VisualStyle.textColor
                    }
                }
                Item { Layout.fillWidth: true }
            }

            // Type (read-only)
            RowLayout {
                width: parent.width
                height: defRowHeight + 2

                Text {
                    Layout.preferredWidth: 60
                    height: defRowHeight
                    text: "Type"
                    horizontalAlignment: Text.AlignRight
                    verticalAlignment: Text.AlignVCenter
                }
                Text {
                    id: svTypeText
                    Layout.fillWidth: true
                    Layout.leftMargin: defTextPadding
                    height: defRowHeight
                    text: ""
                    verticalAlignment: Text.AlignVCenter
                    color: VisualStyle.textColor
                }
            }

            // SV ID (read-only)
            RowLayout {
                width: parent.width
                height: defRowHeight + 2

                Text {
                    Layout.preferredWidth: 60
                    height: defRowHeight
                    text: "SV ID"
                    horizontalAlignment: Text.AlignRight
                    verticalAlignment: Text.AlignVCenter
                }
                Text {
                    id: svIdText
                    Layout.fillWidth: true
                    Layout.leftMargin: defTextPadding
                    height: defRowHeight
                    text: ""
                    verticalAlignment: Text.AlignVCenter
                    color: VisualStyle.textColor
                    elide: Text.ElideRight
                }
            }

            // DataSet (read-only)
            RowLayout {
                width: parent.width
                height: defRowHeight + 2

                Text {
                    Layout.preferredWidth: 60
                    height: defRowHeight
                    text: "DataSet"
                    horizontalAlignment: Text.AlignRight
                    verticalAlignment: Text.AlignVCenter
                }
                Text {
                    id: datSetText
                    Layout.fillWidth: true
                    Layout.leftMargin: defTextPadding
                    height: defRowHeight
                    text: ""
                    verticalAlignment: Text.AlignVCenter
                    color: VisualStyle.textColor
                    elide: Text.ElideRight
                }
            }

            // ConfRev (read-only)
            RowLayout {
                width: parent.width
                height: defRowHeight + 2

                Text {
                    Layout.preferredWidth: 60
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

        // Sampling section (read-only)
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
                    text: qsTr("Sampling")
                }
            }

            RowLayout {
                width: parent.width
                height: defRowHeight + 2

                Text {
                    Layout.preferredWidth: 60
                    height: defRowHeight
                    text: "SmpRate"
                    horizontalAlignment: Text.AlignRight
                    verticalAlignment: Text.AlignVCenter
                }
                Text {
                    id: smpRateText
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
                    Layout.preferredWidth: 60
                    height: defRowHeight
                    text: "NoASDU"
                    horizontalAlignment: Text.AlignRight
                    verticalAlignment: Text.AlignVCenter
                }
                Text {
                    id: noAsduText
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
                    onClicked: sigEnable()
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
