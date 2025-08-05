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

// Filesystem page
Item
{
    id: rootID

    function resizeColumnsOnPage() {
        Globals.resizeColumnsToContent(headerID, tableID)
    }
    function getFilename(t_row) {
        let idx = tableID.model.index(t_row, 2)
        return tableID.model.data(idx, "display")
    }
    function cmdDownloadFile(t_row) {
        console.log("FS_Table: Download file N" + t_row)

        Globals.setSelectedRow(tableID, t_row)
        fsBackend.downloadFile(getFilename(t_row))
    }
    function cmdRemoveFile(t_row) {
        console.log("Control: Remove file N" + t_row)

        fsBackend.removeFile(getFilename(t_row), t_row)
    }

    // Header for Table below with columns for Files
    SortTableHeader {
        id: headerID

        focus: false
        // resizableColumns: false

        anchors {
            top: parent.top
            left: tableID.left
            right: parent.right
        }
    }

    // Table of files on the IED
    TableView {
        id: tableID

        anchors {
            top: headerID.bottom
            bottom: parent.bottom
            left: parent.left
            right: parent.right
        }

        model: fsBackend.sortModel //fsBackend.filesModel

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
            text: model.display

            onSigClick: function(t_row) {
                Globals.setSelectedRow(tableID, t_row)
            }
            onSigDoubleClick: function(t_row) {
                rootID.cmdDownloadFile(t_row)
            }
        }

        ScrollBar.vertical: ScrollBar {
            policy: ScrollBar.AlwaysOn
            active: true
            stepSize: 0.25

            onActiveChanged: {
                if (!active) {
                    active = true;
                }
            }
        }

        Keys.onPressed: function(event) {
            console.log("FS: Key pressed " + event.key)

            if (event.key == Qt.Key_Return || event.key == Qt.Key_Enter) {
                rootID.cmdDownloadFile(tableID.currentRow)
                event.accepted = true
                return
            }
            event.accepted = false
        }
    }

    onVisibleChanged: {
        if (visible) {
            tableID.forceActiveFocus()
        }
    }
}
