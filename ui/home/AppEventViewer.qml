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

import AppStylesModule

// Event log table — embedded as content in a modal SubWindow
Item
{
    id: rootID

    signal sigClose()

    // Column header synced to the table below
    HorizontalHeaderView {
        id: headerID

        anchors {
            top: parent.top
            left: parent.left
            right: parent.right
        }

        syncView: tableID
        boundsBehavior: Flickable.StopAtBounds

        delegate: Rectangle {
            implicitHeight: VisualStyle.rowHeight
            color: VisualStyle.section.bg
            border.color: VisualStyle.section.border

            Text {
                anchors.centerIn: parent
                text: model[headerID.textRole]
                color: VisualStyle.section.text
                font.bold: VisualStyle.boldHeaderText
            }
        }
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

        clip: true
        interactive: true
        boundsBehavior: Flickable.StopAtBounds

        selectionBehavior: TableView.SelectRows
        selectionModel: ItemSelectionModel {
            model: tableID.model
        }

        columnWidthProvider: function(col) {
            if (col === 0) return 160
            if (col === 1) return 120
            // Last column (Description) fills remaining space
            return Math.max(50, tableID.width - 280)
        }

        onWidthChanged: forceLayout()

        delegate: Rectangle {
            required property bool selected

            implicitHeight: VisualStyle.rowHeight
            color: selected ? VisualStyle.table.selRowColor : VisualStyle.table.rowColor1
            border.color: VisualStyle.table.rowBorderColor2
            border.width: 1

            Text {
                anchors {
                    verticalCenter: parent.verticalCenter
                    left: parent.left
                    leftMargin: 4
                    right: parent.right
                    rightMargin: 4
                }
                text: display
                color: VisualStyle.statusBar.textColor
                elide: Text.ElideRight
            }

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    let idx = tableID.model.index(row, 0)
                    tableID.selectionModel.setCurrentIndex(idx,
                        ItemSelectionModel.Clear | ItemSelectionModel.Select | ItemSelectionModel.Rows)
                }
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
