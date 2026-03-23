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

// Settings attributes table with double-click to write
Item
{
    id: rootID

    readonly property int defDelegateHeight: 30

    SortTableHeader {
        id: headerID

        anchors {
            left: tableID.left
            top: parent.top
            right: parent.right
        }
    }

    TableView {
        id: tableID

        anchors {
            left: parent.left
            right: parent.right
            top: headerID.bottom
            bottom: parent.bottom
        }

        model: iedBackend.getLN_SettingsModel()

        focus: true
        keyNavigationEnabled: true
        reuseItems: true

        clip: true
        interactive: true
        boundsBehavior: Flickable.StopAtBounds

        columnWidthProvider: function(column) {
            return Globals.columnWidthCalculator(headerID, tableID, column)
        }

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
            delegateHeight: defDelegateHeight
            selected: { tableID.selVer; return tableID.selectionModel.isSelected(tableID.model.index(row, 0)) }

            textAlign: (column == 0) ? Text.AlignLeft : Text.AlignRight
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
                var ref = iedBackend.getSettingsItemRef(row)
                var fc = iedBackend.getSettingsItemFC(row)
                var val = iedBackend.getSettingsItemValue(row)
                if (ref.length > 0) {
                    writeDialog.openWrite(ref, fc, ref, val)
                }
            }
        }

        ScrollBar.vertical: ScrollBar {
            policy: ScrollBar.AsNeeded
            active: true
            stepSize: 0.25

            onActiveChanged: {
                if (!active) { active = true }
            }
        }
        ScrollBar.horizontal: ScrollBar {
            policy: ScrollBar.AsNeeded
            active: true
            stepSize: 0.25

            onActiveChanged: {
                if (!active) { active = true }
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
            event.accepted = false
        }
    }

    Text {
        anchors.centerIn: parent
        text: "No settings"
        color: VisualStyle.textColor
        font.pixelSize: 14
        visible: tableID.rows === 0
    }

    DiaChangeValue {
        id: writeDialog

        onSigWrite: function(ref, fcStr, val) {
            iedBackend.writeValue(ref, fcStr, val)
        }
    }

    Connections {
        target: iedBackend
        function onSigWriteResult(ref, success, message) {
            if (writeDialog.isActive()) {
                writeDialog.setResult(success, message)
            }
        }
    }
}
