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
                    bottom: parent.bottom
                }

                model: iedBackend.getIED_TreeModel()

                focus: true
                clip: true
                interactive: true
                boundsBehavior: Flickable.StopAtBounds

                palette.highlight:       VisualStyle.table.selRowColor
                palette.highlightedText: VisualStyle.textColor
                palette.base:            VisualStyle.table.rowColor1
                palette.text:            VisualStyle.textColor

                columnWidthProvider: function(t_column) {
                    return Globals.columnWidthCalculator(headerID, treeViewID, t_column)
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
