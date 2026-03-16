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

// Table with DataObjects for concrete DataSet
FocusScope
{
    id: rootID

    readonly property int defDelegateHeight: 30
    readonly property int defDelegateWidth: 60

    signal sigLeftOrRightKey()

    function resizeColumnsToContent(){
        Globals.resizeColumnsToContent(headerID, tableID)
    }

    // Header for TableView below
    TableHeader {
        id: headerID

        defDelegateWidth: rootID.defDelegateWidth
        defDelegateHeight: rootID.defDelegateHeight

        anchors {
            top: parent.top
            left: parent.left
            right: parent.right
        }

        // resizableColumns: false
    }

    // Table of DO for a selected DS
    TableView {
        id: tableID

        anchors {
            top: headerID.bottom
            bottom: parent.bottom
            left: parent.left
            right: parent.right
        }

        model: iedBackend.getDS_ItemModel()

        focus: true
        clip: true
        interactive: true
        boundsBehavior: Flickable.StopAtBounds

        selectionBehavior: TableView.SelectRows
        selectionModel: ItemSelectionModel {
            model: tableID.model

            // onCurrentChanged: {
            // }
        }
        columnWidthProvider: function(t_column) {
            return Globals.columnWidthCalculator(headerID, tableID, t_column)
        }

        delegate: TextDelegate {
            delegateHeight: defDelegateHeight
            selected: (tableID.currentRow == row)

            textAlign: Text.AlignLeft
            text: model.display

            onSigClick: function(row, col) {
                Globals.setSelectedRow(tableID, row)
            }
            onSigDoubleClick: function(row, col) {
                Globals.setSelectedRow(tableID, row)
                var detail = tableID.model.getItemDetail(row)
                if (detail && detail.length > 0) {
                    // Get the DO reference from column 1 (Reference column)
                    var refIdx = tableID.model.index(row, 1)
                    var doRef = tableID.model.data(refIdx) || ""
                    itemDetailDialog.showDetail(doRef, detail)
                }
            }
        }

        ScrollBar.vertical: ScrollBar {
            policy: ScrollBar.AsNeeded
            active: true
            stepSize: 0.25

            onActiveChanged: {
                if (!active) {
                    active = true;
                }
            }
        }
        ScrollBar.horizontal: ScrollBar {
            policy: ScrollBar.AsNeeded
            active: true
            stepSize: 0.25

            onActiveChanged: {
                if (!active) {
                    active = true;
                }
            }
        }

        Keys.onPressed: function(event) {
            if (event.key === Qt.Key_C && (event.modifiers & Qt.ControlModifier)) {
                Globals.copyRowToClipboard(tableID)
                event.accepted = true
                return
            }
            if (event.key === Qt.Key_Return || event.key === Qt.Key_Enter) {
                if (tableID.currentRow >= 0) {
                    var detail = tableID.model.getItemDetail(tableID.currentRow)
                    if (detail && detail.length > 0) {
                        var refIdx = tableID.model.index(tableID.currentRow, 1)
                        var doRef = tableID.model.data(refIdx) || ""
                        itemDetailDialog.showDetail(doRef, detail)
                    }
                    event.accepted = true
                }
                return
            }
            if (event.key == Qt.Key_Left || event.key == Qt.Key_Right || event.key == Qt.Key_Tab) {
                sigLeftOrRightKey()
                event.accepted = true
                return
            }
            event.accepted = false
        }
    }

    // Item detail modal for complex [object] values
    ModalDialog {
        id: itemDetailDialog

        title: "Data Object Detail"
        dialogWidth: 600
        dialogHeight: 400

        property string doReference: ""

        function showDetail(ref, detail) {
            doReference = ref
            detailModel.clear()
            for (var i = 0; i < detail.length; i++) {
                detailModel.append(detail[i])
            }
            open()
        }

        MouseArea {
            anchors.fill: parent
            onClicked: itemDetailDialog.close()
        }

        Column {
            anchors {
                fill: parent
                margins: 4
            }
            spacing: 4

            // DO reference header
            Row {
                width: parent.width
                spacing: 8
                leftPadding: 4

                Text {
                    text: "DO:"
                    color: VisualStyle.statusBar.textColor
                    font.pixelSize: 12
                    font.family: "Monospace"
                    font.bold: true
                    verticalAlignment: Text.AlignVCenter
                }
                Text {
                    text: itemDetailDialog.doReference
                    color: VisualStyle.statusBar.textColor
                    font.pixelSize: 12
                    font.family: "Monospace"
                    verticalAlignment: Text.AlignVCenter
                }
            }

            Rectangle {
                width: parent.width
                height: 1
                color: "white"
            }

            // Column headers
            Row {
                width: parent.width
                spacing: 0
                leftPadding: 4

                Text {
                    width: parent.width * 0.55
                    text: "Reference"
                    color: VisualStyle.statusBar.textColor
                    font.pixelSize: 11
                    font.family: "Monospace"
                    font.bold: true
                }
                Text {
                    width: parent.width * 0.45
                    text: "Value"
                    color: VisualStyle.statusBar.textColor
                    font.pixelSize: 11
                    font.family: "Monospace"
                    font.bold: true
                }
            }

            ListView {
                id: detailListID
                width: parent.width
                height: parent.height - y
                clip: true
                boundsBehavior: Flickable.StopAtBounds

                model: ListModel { id: detailModel }

                delegate: Row {
                    width: detailListID.width
                    spacing: 0
                    leftPadding: 4

                    Text {
                        width: parent.width * 0.55
                        text: name
                        color: VisualStyle.statusBar.textColor
                        font.pixelSize: 11
                        font.family: "Monospace"
                        wrapMode: Text.WrapAnywhere
                    }
                    Text {
                        width: parent.width * 0.45
                        text: value
                        color: VisualStyle.statusBar.textColor
                        font.pixelSize: 11
                        font.family: "Monospace"
                        wrapMode: Text.WrapAnywhere
                    }
                }

                ScrollBar.vertical: ScrollBar {
                    policy: ScrollBar.AsNeeded
                    active: true
                }
            }
        }
    }
}
