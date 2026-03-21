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


    function setSelectedRow(t_tableID, t_row)
    {
        if (t_tableID.currentRow === t_row) {
            // return;
        }

        let idx = t_tableID.model.index(t_row, 0);
        t_tableID.selectionModel.setCurrentIndex(idx, ItemSelectionModel.Clear
                                                    | ItemSelectionModel.Select
                                                    | ItemSelectionModel.Rows);
        t_tableID.forceActiveFocus()
    }

    function toggleSelectedRow(t_tableID, t_row)
    {
        let idx = t_tableID.model.index(t_row, 0);
        t_tableID.selectionModel.setCurrentIndex(idx, ItemSelectionModel.Toggle
                                                    | ItemSelectionModel.Rows);
        t_tableID.forceActiveFocus()
    }

    function resizeColumnsToContent(t_headerID, t_tableID)
    {
        for (let i=0;i<t_tableID.columns;i++) {
            let w = Math.max(t_headerID.implicitColumnWidth(i), t_tableID.implicitColumnWidth(i))
            t_tableID.setColumnWidth(i, w)
        }
    }

    function resizeColumnsToAvailableWidth(t_headerID, t_tableID)
    {
        var iw = []
        let sum = 0, i = 0
        for (i=0;i<t_tableID.columns;i++) {
            iw[i] = Math.max(t_headerID.implicitColumnWidth(i), t_tableID.implicitColumnWidth(i))
            if (iw[i] < 0 || isNaN(iw[i])) {
                iw[i] = 1
            }
            sum = sum + iw[i]
        }
        if (sum === 0) {
            sum = 1
        }
        for (i=0;i<t_tableID.columns;i++) {
            let w = t_tableID.width * iw[i] / sum
            if (w < 0 || isNaN(w)) {
                w = 1
            }
            t_tableID.setColumnWidth(i, w)
        }
    }

    function columnWidthFillSpace(t_headerID, t_tableID, t_column)
    {
        var iw = []
        let sum = 0, i = 0
        for (i=0;i<t_tableID.columns;i++) {
            iw[i] = Math.max(t_headerID.implicitColumnWidth(i), t_tableID.implicitColumnWidth(i))
            if (iw[i] < 0 || isNaN(iw[i])) {
                iw[i] = 1
            }
            sum = sum + iw[i]
        }
        if (sum === 0) {
            sum = 1
        }
        let w = (t_tableID.width * iw[t_column] / sum)
        if (w < 0 || isNaN(w)) {
            return 1
        }
        return w;
    }

    function copyRowToClipboard(t_tableID) {
        if (t_tableID.currentRow < 0) return
        var parts = []
        var cols = t_tableID.model.columnCount()
        for (var c = 0; c < cols; c++) {
            var idx = t_tableID.model.index(t_tableID.currentRow, c)
            parts.push(t_tableID.model.data(idx))
        }
        presenter.copyToClipboard(parts.join(";"))
    }

    function copySelectedRowsToClipboard(t_tableID) {
        var indexes = t_tableID.selectionModel.selectedIndexes
        if (indexes.length === 0) return

        // Collect unique selected rows
        var rowSet = {}
        for (var i = 0; i < indexes.length; i++) {
            rowSet[indexes[i].row] = true
        }
        var rows = Object.keys(rowSet).map(Number).sort(function(a, b) { return a - b })

        var lines = []
        var cols = t_tableID.model.columnCount()
        for (var r = 0; r < rows.length; r++) {
            var parts = []
            for (var c = 0; c < cols; c++) {
                var idx = t_tableID.model.index(rows[r], c)
                parts.push(t_tableID.model.data(idx))
            }
            lines.push(parts.join(";"))
        }
        presenter.copyToClipboard(lines.join("\n"))
    }

    function selectedRowCount(t_tableID) {
        var indexes = t_tableID.selectionModel.selectedIndexes
        if (indexes.length === 0) return 0
        var rowSet = {}
        for (var i = 0; i < indexes.length; i++) {
            rowSet[indexes[i].row] = true
        }
        return Object.keys(rowSet).length
    }

    function columnWidthCalculator(t_headerID, t_tableID, t_column)
    {
        let lw = t_tableID.columnWidth(t_column)
        let hw = t_headerID.implicitColumnWidth(t_column)
        let cw = t_tableID.implicitColumnWidth(t_column)
        let nw = Math.max(hw, cw, 40)
        nw = Math.round(nw)
        if ((nw != lw) && (lw != -1)) {
            Qt.callLater(t_tableID.forceLayout)
        }
        return nw
    }
}