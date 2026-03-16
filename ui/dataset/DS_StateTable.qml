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
            if (event.key == Qt.Key_Left || event.key == Qt.Key_Right || event.key == Qt.Key_Tab) {
                sigLeftOrRightKey()
                event.accepted = true
            }
            event.accepted = false
        }
    }
}
