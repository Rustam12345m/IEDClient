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
import QtQuick.Layouts

import GlobalVarsModule
import AppStylesModule

import "qrc:/common/"

FocusScope {
    id: rootID

    // Table header
    TableHeader {
        id: headerID

        defDelegateWidth: 80
        defDelegateHeight: 30

        anchors {
            top: parent.top
            left: parent.left
            right: parent.right
        }
    }

    // Watchlist table
    TableView {
        id: tableID

        anchors {
            top: headerID.bottom
            bottom: controlsBar.top
            left: parent.left
            right: parent.right
        }

        model: iedBackend.getWatchlistModel()

        focus: true
        clip: true
        interactive: true
        boundsBehavior: Flickable.StopAtBounds

        property int selVer: 0
        property bool multiSelect: false

        selectionBehavior: TableView.SelectRows
        selectionModel: ItemSelectionModel {
            model: tableID.model
            onSelectionChanged: tableID.selVer++
        }

        columnWidthProvider: function(column) {
            var w = Globals.columnWidthCalculator(headerID, tableID, column)
            if (column === 1) w = Math.max(w, 160)       // Reference
            else if (column === 3) w = Math.max(w, 120)  // Value
            return w
        }

        onCurrentRowChanged: {
            if (currentRow >= 0 && !multiSelect) {
                Globals.setSelectedRow(tableID, currentRow)
            }
        }

        delegate: TextDelegate {
            delegateHeight: 30
            selected: { tableID.selVer; return tableID.selectionModel.isSelected(tableID.model.index(row, 0)) }
            text: model.display
            textAlign: Text.AlignLeft

            onSigClick: function(row) {
                Globals.setSelectedRow(tableID, row)
            }
            onSigCtrlClick: function(row) {
                tableID.multiSelect = true
                Globals.toggleSelectedRow(tableID, row)
                tableID.multiSelect = false
            }
        }

        Connections {
            target: tableID.model
            function onDataChanged() {
                tableID.forceLayout()
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
            if (event.key === Qt.Key_Delete) {
                var indexes = tableID.selectionModel.selectedIndexes
                if (indexes.length === 0) return
                var rowSet = {}
                for (var j = 0; j < indexes.length; j++) {
                    rowSet[indexes[j].row] = true
                }
                var rows = Object.keys(rowSet).map(Number).sort(function(a, b) { return b - a })
                for (var k = 0; k < rows.length; k++) {
                    iedBackend.removeFromWatchlist(rows[k])
                }
                event.accepted = true
                return
            }
            if (event.key === Qt.Key_F5) {
                iedBackend.updateWatchlistValues()
                event.accepted = true
                return
            }
        }
    }

    // Empty state placeholder
    Text {
        anchors.centerIn: tableID
        text: "Select tree item and press Insert to watch"
        color: VisualStyle.textColor
        font.pixelSize: 14
        horizontalAlignment: Text.AlignHCenter
        visible: tableID.rows === 0
    }

    // Controls bar at the bottom
    Rectangle {
        id: controlsBar

        anchors {
            bottom: parent.bottom
            left: parent.left
            right: parent.right
        }
        height: 30
        color: VisualStyle.toolBarColor

        RowLayout {
            anchors.fill: parent
            anchors.leftMargin: 5
            anchors.rightMargin: 5
            spacing: 5

            Text {
                text: "WATCHLIST"
                color: VisualStyle.statusBar.textColor
                font.pixelSize: 12
                font.bold: true
            }

            Item { Layout.fillWidth: true }

            // Interval selector
            ComboBox {
                id: intervalCombo
                Layout.preferredWidth: 70
                Layout.preferredHeight: 24
                model: ["1s", "2s", "5s", "10s", "30s", "60s"]
                currentIndex: 2
                font.pixelSize: 11

                property var intervals: [1000, 2000, 5000, 10000, 30000, 60000]
            }

            // Start/Stop toggle
            ToolBarButton {
                width: 24
                height: 24
                icon: updateTimer.running
                      ? "qrc:/img/icons/pause.svg"
                      : "qrc:/img/icons/play_circle_outline.svg"
                prompt: updateTimer.running ? "Stop auto-update" : "Start auto-update"
                onSigClicked: {
                    if (updateTimer.running) {
                        updateTimer.stop()
                    } else {
                        iedBackend.updateWatchlistValues()
                        updateTimer.interval = intervalCombo.intervals[intervalCombo.currentIndex]
                        updateTimer.start()
                    }
                }
            }

            // Manual refresh
            ToolBarButton {
                width: 24
                height: 24
                icon: "qrc:/img/icons/refresh.svg"
                prompt: "Refresh watchlist values"
                onSigClicked: {
                    iedBackend.updateWatchlistValues()
                }
            }

            // Clear all
            ToolBarButton {
                width: 24
                height: 24
                icon: "qrc:/img/icons/clear_all.svg"
                prompt: "Clear watchlist"
                onSigClicked: {
                    iedBackend.clearWatchlist()
                }
            }
        }
    }

    // Auto-update timer
    Timer {
        id: updateTimer
        interval: 5000
        running: false
        repeat: true

        onTriggered: {
            iedBackend.updateWatchlistValues()
        }
    }

    onVisibleChanged: {
        if (!visible) {
            updateTimer.stop()
        }
    }
}
