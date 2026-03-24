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
pragma Singleton

import QtQuick

QtObject
{
    enum Page
    {
        START = 0,
        LD,
        LN,
        DS,
        RCB,
        IED_TREE,
        FS
    }

    enum Panel
    {
        HIDE = 0,
        LD_INFO,
        RCB_PROPERTIES,
        REPORT_DETAIL,
        DS_DETAIL,
        GOOSE_PROPERTIES,
        SV_PROPERTIES
    }


    function setSelectedRow(tableID, row)
    {
        if (tableID.currentRow === row) {
        }

        let idx = tableID.model.index(row, 0);
        tableID.selectionModel.setCurrentIndex(idx, ItemSelectionModel.Clear
                                                    | ItemSelectionModel.Select
                                                    | ItemSelectionModel.Rows);
        tableID.forceActiveFocus()
    }

    function toggleSelectedRow(tableID, row)
    {
        let idx = tableID.model.index(row, 0);
        tableID.selectionModel.setCurrentIndex(idx, ItemSelectionModel.Toggle
                                                    | ItemSelectionModel.Rows);
        tableID.forceActiveFocus()
    }

    function resizeColumnsToContent(headerID, tableID)
    {
        for (let i=0;i<tableID.columns;i++) {
            let w = Math.max(headerID.implicitColumnWidth(i), tableID.implicitColumnWidth(i))
            tableID.setColumnWidth(i, w)
        }
    }

    function resizeColumnsToAvailableWidth(headerID, tableID)
    {
        var iw = []
        let sum = 0, i = 0
        for (i=0;i<tableID.columns;i++) {
            iw[i] = Math.max(headerID.implicitColumnWidth(i), tableID.implicitColumnWidth(i))
            if (iw[i] < 0 || isNaN(iw[i])) {
                iw[i] = 1
            }
            sum = sum + iw[i]
        }
        if (sum === 0) {
            sum = 1
        }
        for (i=0;i<tableID.columns;i++) {
            let w = tableID.width * iw[i] / sum
            if (w < 0 || isNaN(w)) {
                w = 1
            }
            tableID.setColumnWidth(i, w)
        }
    }

    function columnWidthFillSpace(headerID, tableID, column)
    {
        var iw = []
        let sum = 0, i = 0
        for (i=0;i<tableID.columns;i++) {
            iw[i] = Math.max(headerID.implicitColumnWidth(i), tableID.implicitColumnWidth(i))
            if (iw[i] < 0 || isNaN(iw[i])) {
                iw[i] = 1
            }
            sum = sum + iw[i]
        }
        if (sum === 0) {
            sum = 1
        }
        let w = (tableID.width * iw[column] / sum)
        if (w < 0 || isNaN(w)) {
            return 1
        }
        return w;
    }

    function copyRowToClipboard(tableID) {
        if (tableID.currentRow < 0) return
        var parts = []
        var cols = tableID.model.columnCount()
        for (var c = 0; c < cols; c++) {
            var idx = tableID.model.index(tableID.currentRow, c)
            parts.push(tableID.model.data(idx))
        }
        presenter.copyToClipboard(parts.join(";"))
    }

    function copySelectedRowsToClipboard(tableID) {
        var indexes = tableID.selectionModel.selectedIndexes
        if (indexes.length === 0) return

        // Collect unique selected rows
        var rowSet = {}
        for (var i = 0; i < indexes.length; i++) {
            rowSet[indexes[i].row] = true
        }
        var rows = Object.keys(rowSet).map(Number).sort(function(a, b) { return a - b })

        var lines = []
        var cols = tableID.model.columnCount()
        for (var r = 0; r < rows.length; r++) {
            var parts = []
            for (var c = 0; c < cols; c++) {
                var idx = tableID.model.index(rows[r], c)
                parts.push(tableID.model.data(idx))
            }
            lines.push(parts.join(";"))
        }
        presenter.copyToClipboard(lines.join("\n"))
    }

    function selectedRowCount(tableID) {
        var indexes = tableID.selectionModel.selectedIndexes
        if (indexes.length === 0) return 0
        var rowSet = {}
        for (var i = 0; i < indexes.length; i++) {
            rowSet[indexes[i].row] = true
        }
        return Object.keys(rowSet).length
    }

    function columnWidthCalculator(headerID, tableID, column)
    {
        let lw = tableID.columnWidth(column)
        let hw = headerID.implicitColumnWidth(column)
        let cw = tableID.implicitColumnWidth(column)
        let nw = Math.max(hw, cw, 40)
        nw = Math.round(nw)
        if ((nw != lw) && (lw != -1)) {
            Qt.callLater(tableID.forceLayout)
        }
        return nw
    }
}