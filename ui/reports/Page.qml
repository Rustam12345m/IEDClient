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
    property int selectedRCBIndex: tableID.currentRow
    property bool panelVisible: false

    signal sigRCBRowSelected(bool isRCB)

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

            //selectedColor: "white"
            //unselectedColor: VisualStyle.toolBarColor

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

    // Header for Table below
    TableHeader {
        id: headerID

        defDelegateWidth: 60
        defDelegateHeight: 30

        anchors {
            left: rectRcbViewTabBar.right
            right: parent.right
            top: parent.top
        }

        // resizableColumns: false
    }

    // Table of RCB on the IED
    TableView {
        id: tableID

        anchors {
            left: rectRcbViewTabBar.right
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
            /*
            onCurrentChanged: {
                console.log(currentIndex)
            }
            */
        }
        columnWidthProvider: function(t_column) {
            return Globals.columnWidthCalculator(headerID, tableID, t_column)
        }

        delegate: TextDelegate {
            delegateHeight: 30
            selected: (tableID.currentRow == row)

            textAlign: Text.AlignHCenter
            text: model.display

            onSigClick: function(row, col) {
                tableID.focus = true
                if (selected && rootID.isRCBTab) {
                    rootID.panelVisible = !rootID.panelVisible
                    sigRCBRowSelected(rootID.panelVisible)
                    return
                }
                Globals.setSelectedRow(tableID, row)
                if (rootID.isRCBTab) {
                    tableID.model.setSelectedRCB(row)
                    rootID.panelVisible = true
                }
                sigRCBRowSelected(rootID.isRCBTab)
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

        Keys.onPressed: function(event) {
        }
    }

    onVisibleChanged: {
        tableID.focus = visible
    }
}
