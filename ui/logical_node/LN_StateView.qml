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

// Table with DataObjects for concrete Logical Node
Item
{
    id: rootID

    readonly property int defDelegateHeight: 30

    function resizeColumnsToContent() {
        Globals.resizeColumnsToContent(headerID, tableID)
    }

    // Header for TableView below with DO
    SortTableHeader {
        id: headerID

        anchors {
            left: tableID.left
            top: parent.top
            right: parent.right
        }

        // resizableColumns: false
    }

    // Table of DO for a selected LN
    TableView {
        id: tableID

        anchors {
            left: parent.left
            right: parent.right
            top: headerID.bottom
            bottom: parent.bottom
        }

        model: iedBackend.getLN_StateModel()

        focus: true
        keyNavigationEnabled: true
        reuseItems: true

        clip: true
        interactive: true
        boundsBehavior: Flickable.StopAtBounds

        columnWidthProvider: function(t_column) {
            return Globals.columnWidthCalculator(headerID, tableID, t_column)
        }

        selectionBehavior: TableView.SelectRows
        selectionModel: ItemSelectionModel {
            model: tableID.model
        }

        delegate: TextDelegate {
            delegateHeight: defDelegateHeight
            selected: (tableID.currentRow == row)

            textAlign: (column == 0) ? Text.AlignLeft : Text.AlignRight
            text: model.display

            onSigClick: function(row, col) {
                Globals.setSelectedRow(tableID, row)
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
            }
        }

        Connections {
            target: iedBackend.getLN_StateModel()

            function onDataChanged() {
            }

            function onModelReset() {
            }
        }
    }

    Text {
        anchors.centerIn: parent
        text: "No data objects"
        color: VisualStyle.textColor
        font.pixelSize: 14
        visible: tableID.rows === 0
    }
}
