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

FocusScope
{
    id: rootID

    property var tabModels: [
        iedBackend.getBRCB_ComModel(),
        iedBackend.getURCB_ComModel(),
        iedBackend.getGOSE_ComModel(),
        iedBackend.getSV_ComModel()
    ]
    property var activeModel: tabModels[rcbTabBarID.currentIndex]
    property bool isRCBTab: rcbTabBarID.currentIndex <= 1
    property bool isBuffered: rcbTabBarID.currentIndex === 0
    property int selectedRCBIndex: -1
    property bool panelVisible: false

    signal sigRCBRowSelected(bool isRCB)
    signal sigReportDetail(var detail)

    function resizeColumnsOnPage() {
        Globals.resizeColumnsToContent(headerID, tableID)
    }

    // TabBar: Different types of CB
    Rectangle {
        id: rectRcbViewTabBar

        anchors {
            top: parent.top
            bottom: parent.bottom
            left: parent.left
        }
        width: 30

        CustomTabBar {
            id: rcbTabBarID

            anchors.fill: parent

            leftSide: true
            cellWidth: 30
            color: VisualStyle.toolBarColor

            model: ListModel {
                ListElement { title: "BRCB" }
                ListElement { title: "URCB" }
                ListElement { title: "GOOSE" }
                ListElement { title: "SV" }
            }

            onSigTabSelected: function(index) {
                tableID.model = rootID.tabModels[index]
                sigRCBRowSelected(index <= 1 && tableID.currentRow >= 0)
            }
        }
    }

    SplitView {
        id: splitViewID

        orientation: Qt.Vertical

        anchors {
            left: rectRcbViewTabBar.right
            right: parent.right
            top: parent.top
            bottom: parent.bottom
        }

        // Top pane: RCB overview
        Item {
            id: topPane
            SplitView.preferredHeight: splitViewID.height * 0.5
            SplitView.minimumHeight: 80

            // Header for Table below
            TableHeader {
                id: headerID

                defDelegateWidth: 60
                defDelegateHeight: 30

                anchors {
                    left: parent.left
                    right: parent.right
                    top: parent.top
                }
            }

            // Table of RCB on the IED
            TableView {
                id: tableID

                anchors {
                    left: parent.left
                    right: parent.right
                    top: headerID.bottom
                    bottom: parent.bottom
                }

                model: rootID.tabModels[0]

                focus: true
                reuseItems: true
                keyNavigationEnabled: true

                clip: true
                interactive: true
                boundsBehavior: Flickable.StopAtBounds

                selectionBehavior: TableView.SelectRows
                selectionModel: ItemSelectionModel {
                    model: tableID.model
                }
                columnWidthProvider: function(t_column) {
                    return Globals.columnWidthCalculator(headerID, tableID, t_column)
                }

                delegate: DelegateChooser {
                    // Enabled column with LED indicator
                    DelegateChoice {
                        column: 0

                        delegate: Item {
                            implicitWidth: 60
                            implicitHeight: 30

                            required property bool selected

                            Rectangle {
                                anchors.fill: parent
                                border.color: VisualStyle.table.rowBorderColor2
                                color: (rootID.selectedRCBIndex == row)
                                       ? VisualStyle.table.selRowColor
                                       : VisualStyle.table.rowColor1

                                Row {
                                    anchors.centerIn: parent
                                    spacing: 5

                                    Rectangle {
                                        anchors.verticalCenter: parent.verticalCenter
                                        width: 12; height: 12; radius: 6
                                        color: model.display === "Yes" ? "#4CAF50" : "gray"
                                    }
                                    Text {
                                        anchors.verticalCenter: parent.verticalCenter
                                        text: model.display
                                        color: VisualStyle.textColor
                                    }
                                }
                            }

                            MouseArea {
                                anchors.fill: parent
                                onClicked: {
                                    tableID.focus = true
                                    Globals.setSelectedRow(tableID, row)
                                    rootID.selectedRCBIndex = row
                                }
                                onDoubleClicked: {
                                    if (rootID.isRCBTab) {
                                        rootID.panelVisible = !rootID.panelVisible
                                        sigRCBRowSelected(rootID.panelVisible)
                                    }
                                }
                            }
                        }
                    }

                    // All other columns
                    DelegateChoice {
                        delegate: TextDelegate {
                            delegateHeight: 30
                            selected: (rootID.selectedRCBIndex == row)

                            textAlign: (column === 4 || column === 5) ? Text.AlignLeft : Text.AlignHCenter
                            text: model.display

                            onSigClick: function(row, col) {
                                tableID.focus = true
                                Globals.setSelectedRow(tableID, row)
                                rootID.selectedRCBIndex = row
                            }
                            onSigDoubleClick: function(row, col) {
                                if (rootID.isRCBTab) {
                                    rootID.panelVisible = !rootID.panelVisible
                                    sigRCBRowSelected(rootID.panelVisible)
                                }
                            }
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
                ScrollBar.horizontal: ScrollBar {
                    policy: ScrollBar.AsNeeded
                    active: true

                    onActiveChanged: {
                        if (!active) {
                            active = true;
                        }
                    }
                }

                onCurrentRowChanged: {
                    if (currentRow >= 0) {
                        rootID.selectedRCBIndex = currentRow
                        if (rootID.isRCBTab) {
                            tableID.model.setSelectedRCB(currentRow)
                            if (rootID.panelVisible) {
                                sigRCBRowSelected(true)
                            }
                        }
                    }
                }

                Keys.onPressed: function(event) {
                    if (event.key === Qt.Key_C && (event.modifiers & Qt.ControlModifier)) {
                        Globals.copyRowToClipboard(tableID)
                        event.accepted = true
                        return
                    }
                    if ((event.key === Qt.Key_Return || event.key === Qt.Key_Enter)
                            && rootID.isRCBTab && currentRow >= 0) {
                        rootID.panelVisible = !rootID.panelVisible
                        if (rootID.panelVisible) {
                            tableID.model.setSelectedRCB(currentRow)
                            sigRCBRowSelected(true)
                        } else {
                            sigRCBRowSelected(false)
                        }
                        event.accepted = true
                        return
                    }
                }
            }
        }

        // Bottom pane: Received reports for selected RCB
        Item {
            id: bottomPane
            SplitView.fillHeight: true
            SplitView.minimumHeight: 60
            visible: rootID.isRCBTab

            RCB_ReportsTable {
                id: reportsTableID
                anchors.fill: parent

                onSigReportDetail: function(detail) {
                    rootID.sigReportDetail(detail)
                }
            }
        }
    }

    onVisibleChanged: {
        tableID.focus = visible
    }
}
