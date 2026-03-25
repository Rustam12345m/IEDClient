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

// Resizable column header for TableView.
// Uses a custom Row+Repeater instead of HorizontalHeaderView so that
// drag handles between columns can resize them on Qt < 6.5.
//
// Usage:
//   property var tableRef   — bind to the TableView id
//   property var columnNames — string list of header labels
//   property var defaultWidths — initial column widths (-1 = fill)
Item {
    id: rootID

    property var tableRef
    property var columnNames: []
    property var defaultWidths: []

    // Internal mutable widths array
    property var colWidths: []

    implicitHeight: VisualStyle.rowHeight
    height: VisualStyle.rowHeight

    Component.onCompleted: {
        // Copy defaults into mutable array
        var w = []
        for (var i = 0; i < defaultWidths.length; i++) w.push(defaultWidths[i])
        colWidths = w
    }

    function getColumnWidth(col) {
        if (col < 0 || col >= colWidths.length) return 100
        if (colWidths[col] > 0) return colWidths[col]
        // Fill: total width minus fixed columns
        var used = 0
        for (var i = 0; i < colWidths.length; i++) {
            if (colWidths[i] > 0) used += colWidths[i]
        }
        return Math.max(80, rootID.width - used)
    }

    Row {
        anchors.fill: parent

        Repeater {
            model: rootID.columnNames.length

            Rectangle {
                width: rootID.getColumnWidth(index)
                height: rootID.height
                color: VisualStyle.section.bg
                border.color: VisualStyle.section.border

                Text {
                    anchors.centerIn: parent
                    text: index < rootID.columnNames.length ? rootID.columnNames[index] : ""
                    color: VisualStyle.section.text
                    font.bold: VisualStyle.boldHeaderText
                }

                // Drag handle at the right edge
                MouseArea {
                    id: dragHandle
                    width: 12
                    height: parent.height
                    x: parent.width - 6
                    cursorShape: Qt.SplitHCursor
                    visible: index < rootID.columnNames.length - 1

                    property real dragStartGlobalX: 0
                    property real dragStartW: 0

                    onPressed: function(mouse) {
                        var global = mapToGlobal(mouse.x, 0)
                        dragStartGlobalX = global.x
                        dragStartW = parent.width
                    }

                    onPositionChanged: function(mouse) {
                        if (!pressed) return
                        var global = mapToGlobal(mouse.x, 0)
                        var delta = global.x - dragStartGlobalX
                        var newW = Math.max(40, dragStartW + delta)
                        var ws = rootID.colWidths.slice()
                        ws[index] = newW
                        rootID.colWidths = ws
                        if (rootID.tableRef) rootID.tableRef.forceLayout()
                    }
                }
            }
        }
    }
}
