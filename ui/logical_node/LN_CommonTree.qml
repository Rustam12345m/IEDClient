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

import "qrc:/common/"

import GlobalVarsModule
import AppStylesModule

// TreeView with DO/DA/SA for the selected Logical Node
FocusScope {
    id: rootID

    // Header
    HorizontalHeaderView {
        id: headerID

        property int sortOrder: 0
        property int sortedColumn: 0

        anchors {
            left: treeViewID.left
            top: parent.top
            right: parent.right
        }
        boundsBehavior: Flickable.StopAtBounds
        // resizableColumns: false

        syncView: treeViewID

        delegate: Rectangle {
            property int column: model.column

            implicitWidth: labelID.implicitWidth + 24 + 10
            implicitHeight: 30

            color: VisualStyle.section.bg
            border.color: VisualStyle.section.border

            Row {
                id: rowID

                anchors.centerIn: parent
                spacing: 5

                Label {
                    id: labelID

                    text: model.display
                    color: VisualStyle.section.text
                }
            }
        }
    }

    // Tree of DO
    TreeView {
        id: treeViewID

        anchors {
            left: parent.left
            right: parent.right
            // top: parent.top
            top: headerID.bottom
            bottom: parent.bottom
        }

        model: iedBackend.getLN_TreeModel()

        focus: true
        clip: true
        interactive: true
        boundsBehavior: Flickable.StopAtBounds

        palette.highlight:       VisualStyle.table.selRowColor
        palette.highlightedText: VisualStyle.textColor
        palette.base:            VisualStyle.table.rowColor1
        palette.text:            VisualStyle.textColor

        columnWidthProvider: function(column) {
            var w = Globals.columnWidthCalculator(headerID, treeViewID, column)
            if (column === 0) w = Math.max(w, 160)       // Name
            else if (column === 1) w = Math.max(w, 180)  // Value: fit "11:11:11 11.11.1970"
            return w
        }

        Keys.onPressed: function(event) {
            if (event.key === Qt.Key_C && (event.modifiers & Qt.ControlModifier)) {
                Globals.copyRowToClipboard(treeViewID)
                event.accepted = true
            }
        }

        selectionBehavior: TableView.SelectRows
        selectionModel: ItemSelectionModel {
            model: treeViewID.model
        }

        delegate: TreeViewDelegate {
            TapHandler {
                acceptedModifiers: Qt.ControlModifier
                onTapped: {
                    if (treeViewID.isExpanded(row))
                        treeViewID.collapseRecursively(row)
                    else
                        treeViewID.expandRecursively(row)
                }
            }
        }

        Connections {
            target: treeViewID.model
            function onModelReset() {
                // All nodes folded by default — user expands manually or via Ctrl+Click
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
    }
}
