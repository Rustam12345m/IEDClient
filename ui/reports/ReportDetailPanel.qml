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

// Report detail panel — shown in the right-side property panel
Item
{
    id: rootID

    function showReport(detail) {
        headerInfoID.detail = detail
        entriesModel.clear()
        if (detail.entries) {
            for (var i = 0; i < detail.entries.length; i++) {
                entriesModel.append(detail.entries[i])
            }
        }
    }

    function clear() {
        headerInfoID.detail = ({})
        entriesModel.clear()
    }

    Column {
        anchors {
            fill: parent
            margins: 4
        }
        spacing: 2

        // Section header
        Rectangle {
            width: parent.width
            height: VisualStyle.rowHeight
            color: VisualStyle.section.bg
            border.width: 1
            border.color: VisualStyle.section.border

            Text {
                anchors.fill: parent
                font.bold: VisualStyle.boldHeaderText
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                text: qsTr("Report detail")
            }
        }

        // Report header info
        Column {
            id: headerInfoID
            width: parent.width
            spacing: 1

            property var detail: ({})

            Repeater {
                model: [
                    { key: "Seq #:",    val: headerInfoID.detail.seqNum || "" },
                    { key: "Time:",     val: headerInfoID.detail.timestamp || "" },
                    { key: "RCB:",      val: headerInfoID.detail.rcbRef || "" },
                    { key: "DataSet:",  val: headerInfoID.detail.dataSetRef || "" },
                    { key: "Reason:",   val: headerInfoID.detail.reason || "" }
                ]

                delegate: Row {
                    spacing: 4
                    leftPadding: 2

                    Text {
                        width: 55
                        text: modelData.key
                        color: VisualStyle.textColor
                        font.pixelSize: 11
                        font.family: "Monospace"
                        font.bold: true
                        horizontalAlignment: Text.AlignRight
                        verticalAlignment: Text.AlignVCenter
                    }
                    Text {
                        width: rootID.width - 65
                        text: modelData.val !== undefined ? String(modelData.val) : ""
                        color: VisualStyle.textColor
                        font.pixelSize: 11
                        font.family: "Monospace"
                        verticalAlignment: Text.AlignVCenter
                        wrapMode: Text.WrapAnywhere
                    }
                }
            }
        }

        // Separator
        Rectangle {
            width: parent.width
            height: 1
            color: VisualStyle.section.border
        }

        // Column headers for entries
        Row {
            width: parent.width
            spacing: 0
            leftPadding: 2

            Text {
                width: parent.width * 0.55
                text: "Reference"
                color: VisualStyle.textColor
                font.pixelSize: 11
                font.family: "Monospace"
                font.bold: true
            }
            Text {
                width: parent.width * 0.25
                text: "Value"
                color: VisualStyle.textColor
                font.pixelSize: 11
                font.family: "Monospace"
                font.bold: true
            }
            Text {
                width: parent.width * 0.20
                text: "Reason"
                color: VisualStyle.textColor
                font.pixelSize: 11
                font.family: "Monospace"
                font.bold: true
            }
        }

        // Scrollable entries list
        ListView {
            id: entriesListID
            width: parent.width
            height: parent.height - y
            clip: true
            boundsBehavior: Flickable.StopAtBounds

            model: ListModel { id: entriesModel }

            delegate: Row {
                width: entriesListID.width
                spacing: 0
                leftPadding: 2

                Text {
                    width: parent.width * 0.55
                    text: name
                    color: VisualStyle.textColor
                    font.pixelSize: 11
                    font.family: "Monospace"
                    wrapMode: Text.WrapAnywhere
                }
                Text {
                    width: parent.width * 0.25
                    text: value
                    color: VisualStyle.textColor
                    font.pixelSize: 11
                    font.family: "Monospace"
                    wrapMode: Text.WrapAnywhere
                }
                Text {
                    width: parent.width * 0.20
                    text: reason
                    color: VisualStyle.textColor
                    font.pixelSize: 11
                    font.family: "Monospace"
                    elide: Text.ElideRight
                }
            }

            ScrollBar.vertical: ScrollBar {
                policy: ScrollBar.AsNeeded
                active: true
            }
        }
    }
}
