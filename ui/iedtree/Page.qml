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

// Main IED's tree page
FocusScope {
    id: rootID

    SplitView {
        id: splitView

        focus: true
        anchors.fill: parent

        // Delimiter
        handle: SplitDelimeter {
            height: splitView.height
            pressed: SplitHandle.pressed
        }

        // Left pane: IED tree
        Rectangle {
            SplitView.preferredWidth: splitView.width * 0.5
            SplitView.minimumWidth: 300
            SplitView.fillWidth: false

            color: VisualStyle.input.bg

            // Header
            HorizontalHeaderView {
                id: headerID

                anchors {
                    left: treeViewID.left
                    top: parent.top
                    right: parent.right
                }
                boundsBehavior: Flickable.StopAtBounds

                syncView: treeViewID

                delegate: Rectangle {
                    implicitWidth: labelID.implicitWidth + 24 + 10
                    implicitHeight: 30

                    color: VisualStyle.section.bg
                    border.color: VisualStyle.section.border

                    Label {
                        id: labelID

                        anchors.centerIn: parent
                        text: model.display
                        color: VisualStyle.section.text
                    }
                }
            }

            // Full IED data model tree
            TreeView {
                id: treeViewID

                anchors {
                    left: parent.left
                    right: parent.right
                    top: headerID.bottom
                    bottom: filterBar.top
                }

                model: iedBackend.getIED_TreeModel()

                focus: true
                clip: true
                interactive: true
                boundsBehavior: Flickable.StopAtBounds
                flickDeceleration: 100000

                palette.highlight:       VisualStyle.table.selRowColor
                palette.highlightedText: VisualStyle.textColor
                palette.base:            VisualStyle.table.rowColor1
                palette.text:            VisualStyle.textColor

                columnWidthProvider: function(column) {
                    var w = Globals.columnWidthCalculator(headerID, treeViewID, column)
                    if (column === 0) w = Math.max(w, 180)       // Reference
                    else if (column === 1) w = Math.max(w, 180)  // Value
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
                        Qt.callLater(function() {
                            for (var i = 0; i < treeViewID.rows; i++) {
                                if (treeViewID.depth(i) === 0)
                                    treeViewID.expand(i)
                            }
                        })
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

            // Filter bar at the bottom
            Row {
                id: filterBar

                anchors {
                    left: parent.left
                    right: parent.right
                    bottom: parent.bottom
                    margins: 2
                }
                height: 28
                spacing: 2

                TextField {
                    id: filterInput

                    width: parent.width - btnFold.width - btnUnfold.width - parent.spacing * 2
                    height: parent.height

                    placeholderText: "Filter by path, e.g. mag.f or LLN0.Mod"
                    font.pixelSize: 12

                    onTextChanged: {
                        iedBackend.setTreeFilter(text)
                        if (text.length > 0) {
                            Qt.callLater(function() { treeViewID.expandRecursively() })
                        }
                    }
                }
                Button {
                    id: btnFold
                    width: 28
                    height: parent.height
                    icon.source: "qrc:/img/icons/unfold_less.svg"
                    focusPolicy: Qt.NoFocus
                    onClicked: treeViewID.collapseRecursively()
                }
                Button {
                    id: btnUnfold
                    width: 28
                    height: parent.height
                    icon.source: "qrc:/img/icons/unfold_more.svg"
                    focusPolicy: Qt.NoFocus
                    onClicked: treeViewID.expandRecursively()
                }
            }
        }

        // Right pane: empty placeholder for future features
        Rectangle {
            SplitView.minimumWidth: 200
            SplitView.fillWidth: true

            color: VisualStyle.input.bg
        }
    }

    onVisibleChanged: {
        treeViewID.focus = visible
    }
}
