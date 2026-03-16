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
import Qt.labs.qmlmodels

import GlobalVarsModule
import AppStylesModule

import "qrc:/common/"

// Received reports for the particular RCB
FocusScope
{
    id: rootID

    // Header for Table below
    TableHeader {
        id: headerID

        defDelegateWidth: 60
        defDelegateHeight: 30

        anchors {
            left: tableID.left
            top: parent.top
            right: parent.right
        }
    }

    // Table of received reports
    TableView {
        id: tableID

        anchors {
            left: parent.left
            top: headerID.bottom
            right: parent.right
            bottom: parent.bottom
        }

        model: iedBackend.getReportsModel()

        focus: true
        keyNavigationEnabled: true
        reuseItems: true

        interactive: true
        clip: true
        boundsBehavior: Flickable.StopAtBounds

        columnWidthProvider: function(t_column) {
            return Globals.columnWidthCalculator(headerID, tableID, t_column)
        }

        selectionBehavior: TableView.SelectRows
        selectionModel: ItemSelectionModel {
            model: tableID.model
        }

        delegate: TextDelegate {
            delegateHeight: 30
            selected: (tableID.currentRow == row)

            textAlign: Text.AlignHCenter
            text: model.display

            onSigClick: function(row, col) {
                Globals.setSelectedRow(tableID, row)
            }

            onSigDoubleClick: function(row, col) {
                Globals.setSelectedRow(tableID, row)
                var detail = tableID.model.getReportDetail(row)
                if (detail && detail.entries) {
                    reportDetailDialog.showReport(detail)
                }
            }
        }

        ScrollBar.vertical: ScrollBar {
            policy: ScrollBar.AsNeeded
            active: true
            onActiveChanged: {
                if (!active) {
                    active = true;
                }
            }
        }

        Keys.onPressed: function(event) {
            if (event.key === Qt.Key_Return || event.key === Qt.Key_Enter) {
                if (tableID.currentRow >= 0) {
                    var detail = tableID.model.getReportDetail(tableID.currentRow)
                    if (detail && detail.entries) {
                        reportDetailDialog.showReport(detail)
                    }
                    event.accepted = true
                }
            }
        }
    }

    // Empty state placeholder
    Text {
        anchors.centerIn: parent
        text: "No reports received"
        color: VisualStyle.textColor
        font.pixelSize: 14
        visible: tableID.rows === 0
    }

    // Report detail modal
    ModalDialog {
        id: reportDetailDialog

        title: "Report Detail"
        dialogWidth: 700
        dialogHeight: 500

        function showReport(detail) {
            headerInfoID.detail = detail
            entriesModel.clear()
            for (var i = 0; i < detail.entries.length; i++) {
                entriesModel.append(detail.entries[i])
            }
            open()
        }

        Column {
            anchors {
                fill: parent
                margins: 8
            }
            spacing: 4

            // Report header info
            Column {
                id: headerInfoID
                width: parent.width
                spacing: 2

                property var detail: ({})

                Repeater {
                    model: [
                        { key: "Seq #:",    val: headerInfoID.detail.seqNum || "" },
                        { key: "Timestamp:", val: headerInfoID.detail.timestamp || "" },
                        { key: "RCB Ref:",  val: headerInfoID.detail.rcbRef || "" },
                        { key: "DataSet:",  val: headerInfoID.detail.dataSetRef || "" },
                        { key: "Reason:",   val: headerInfoID.detail.reason || "" }
                    ]

                    delegate: Row {
                        spacing: 8
                        leftPadding: 4

                        Text {
                            width: 80
                            text: modelData.key
                            color: VisualStyle.statusBar.textColor
                            font.pixelSize: 12
                            font.family: "Monospace"
                            font.bold: true
                            horizontalAlignment: Text.AlignRight
                            verticalAlignment: Text.AlignVCenter
                        }
                        Text {
                            text: modelData.val !== undefined ? String(modelData.val) : ""
                            color: VisualStyle.statusBar.textColor
                            font.pixelSize: 12
                            font.family: "Monospace"
                            verticalAlignment: Text.AlignVCenter
                        }
                    }
                }
            }

            // Separator
            Rectangle {
                width: parent.width
                height: 1
                color: "white"
            }

            // Column headers for entries
            Row {
                width: parent.width
                spacing: 0
                leftPadding: 4

                Text {
                    width: parent.width * 0.45
                    text: "Reference"
                    color: VisualStyle.statusBar.textColor
                    font.pixelSize: 11
                    font.family: "Monospace"
                    font.bold: true
                }
                Text {
                    width: parent.width * 0.35
                    text: "Value"
                    color: VisualStyle.statusBar.textColor
                    font.pixelSize: 11
                    font.family: "Monospace"
                    font.bold: true
                }
                Text {
                    width: parent.width * 0.20
                    text: "Reason"
                    color: VisualStyle.statusBar.textColor
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
                    leftPadding: 4

                    Text {
                        width: parent.width * 0.45
                        text: name
                        color: VisualStyle.statusBar.textColor
                        font.pixelSize: 11
                        font.family: "Monospace"
                        elide: Text.ElideRight
                    }
                    Text {
                        width: parent.width * 0.35
                        text: value
                        color: VisualStyle.statusBar.textColor
                        font.pixelSize: 11
                        font.family: "Monospace"
                        elide: Text.ElideRight
                    }
                    Text {
                        width: parent.width * 0.20
                        text: reason
                        color: VisualStyle.statusBar.textColor
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

    onVisibleChanged: {
        tableID.focus = visible
    }
}
