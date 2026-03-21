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
    signal sigItemDetail(string doRef, var detail)

    property bool detailPanelOpen: false

    function emitItemDetail(row) {
        var detail = tableID.model.getItemDetail(row)
        if (detail && detail.length > 0) {
            var refIdx = tableID.model.index(row, 1)
            var doRef = tableID.model.data(refIdx) || ""
            sigItemDetail(doRef, detail)
        }
    }

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

        property int selVer: 0
        property bool multiSelect: false

        selectionBehavior: TableView.SelectRows
        selectionModel: ItemSelectionModel {
            model: tableID.model
            onSelectionChanged: tableID.selVer++
        }

        columnWidthProvider: function(t_column) {
            var w = Globals.columnWidthCalculator(headerID, tableID, t_column)
            if (t_column === 1) w = Math.max(w, 160)       // LD
            else if (t_column === 2) w = Math.max(w, 160)  // Reference
            else if (t_column === tableID.columns - 1) w = Math.max(w, 160) // Value
            return w
        }

        delegate: TextDelegate {
            delegateHeight: defDelegateHeight
            selected: { tableID.selVer; return tableID.selectionModel.isSelected(tableID.model.index(row, 0)) }

            textAlign: Text.AlignLeft
            text: model.display

            onSigClick: function(row, col) {
                Globals.setSelectedRow(tableID, row)
            }
            onSigCtrlClick: function(row, col) {
                tableID.multiSelect = true
                Globals.toggleSelectedRow(tableID, row)
                tableID.multiSelect = false
            }
            onSigDoubleClick: function(row, col) {
                if (Globals.selectedRowCount(tableID) > 1) return
                Globals.setSelectedRow(tableID, row)
                detailPanelOpen = !detailPanelOpen
                if (detailPanelOpen) {
                    emitItemDetail(row)
                } else {
                    sigItemDetail("", [])
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

        onCurrentRowChanged: {
            if (currentRow >= 0 && !multiSelect) {
                Globals.setSelectedRow(tableID, currentRow)
            }
            if (currentRow >= 0 && detailPanelOpen) {
                emitItemDetail(currentRow)
            }
        }

        Connections {
            target: tableID.model

            function onDataChanged() {
                tableID.forceLayout()
            }
        }

        Keys.onPressed: function(event) {
            if (event.key === Qt.Key_C && (event.modifiers & Qt.ControlModifier)) {
                Globals.copySelectedRowsToClipboard(tableID)
                event.accepted = true
                return
            }
            if (event.key === Qt.Key_A && (event.modifiers & Qt.ControlModifier)) {
                for (var i = 0; i < tableID.rows; i++) {
                    tableID.selectionModel.select(
                        tableID.model.index(i, 0),
                        ItemSelectionModel.Select | ItemSelectionModel.Rows)
                }
                event.accepted = true
                return
            }
            if ((event.key === Qt.Key_Return || event.key === Qt.Key_Enter)
                    && currentRow >= 0 && Globals.selectedRowCount(tableID) <= 1) {
                detailPanelOpen = !detailPanelOpen
                if (detailPanelOpen) {
                    emitItemDetail(currentRow)
                } else {
                    sigItemDetail("", [])
                }
                event.accepted = true
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
}
