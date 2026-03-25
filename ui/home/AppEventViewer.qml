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

import GlobalVarsModule
import AppStylesModule

import "qrc:/common/"

// Event log table — embedded as content in a modal SubWindow
FocusScope
{
    id: rootID

    signal sigClose()

    onVisibleChanged: {
        if (visible) tableID.forceActiveFocus()
    }

    ResizableHeaderView {
        id: headerID

        anchors {
            top: parent.top
            left: parent.left
            right: parent.right
        }

        tableRef: tableID
        columnNames: ["Date and Time", "Source", "Description"]
        defaultWidths: [170, 240, -1]
    }

    // Scrollable event table
    TableView {
        id: tableID

        anchors {
            top: headerID.bottom
            left: parent.left
            right: parent.right
            bottom: closeRowID.top
            bottomMargin: 4
        }

        model: appBackend.appLogsModel

        focus: true
        clip: true
        interactive: true
        keyNavigationEnabled: true
        boundsBehavior: Flickable.StopAtBounds

        columnWidthProvider: function(col) {
            return headerID.getColumnWidth(col)
        }

        onWidthChanged: forceLayout()

        property int selVer: 0
        property bool multiSelect: false

        selectionBehavior: TableView.SelectRows
        selectionModel: ItemSelectionModel {
            model: tableID.model
            onSelectionChanged: tableID.selVer++
        }

        onCurrentRowChanged: {
            if (currentRow >= 0 && !multiSelect) {
                Globals.setSelectedRow(tableID, currentRow)
            }
        }

        delegate: TextDelegate {
            delegateHeight: VisualStyle.rowHeight
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
        }

        ScrollBar.vertical: ScrollBar {
            policy: ScrollBar.AsNeeded
            active: true
            stepSize: 0.05

            onActiveChanged: {
                if (!active) active = true
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
            if (event.key === Qt.Key_Up) {
                if (tableID.currentRow > 0) {
                    Globals.setSelectedRow(tableID, tableID.currentRow - 1)
                }
                event.accepted = true
                return
            }
            if (event.key === Qt.Key_Down) {
                if (tableID.currentRow < tableID.rows - 1) {
                    Globals.setSelectedRow(tableID, tableID.currentRow + 1)
                }
                event.accepted = true
                return
            }
        }
    }

    // Close button row
    Item {
        id: closeRowID

        anchors {
            left: parent.left
            right: parent.right
            bottom: parent.bottom
        }
        height: 36

        Button {
            anchors.centerIn: parent
            text: "Close"
            focusPolicy: Qt.NoFocus

            onClicked: rootID.sigClose()
        }
    }
}
