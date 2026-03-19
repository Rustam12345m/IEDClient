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

import QtQuick.Layouts

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
        // console.log("FS_Table: Download file N" + t_row)

        Globals.setSelectedRow(tableID, t_row)
        fsBackend.downloadFile(getFilename(t_row))
    }
    function cmdRemoveFile(t_row) {
        // console.log("Control: Remove file N" + t_row)

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

        delegate: DelegateChooser {
            DelegateChoice {
                column: 4

                delegate: FS_ControlDelegate {
                    selected: (tableID.currentRow == row)

                    onSigDownloadFile: function(t_row) {
                        rootID.cmdDownloadFile(t_row)
                    }
                    onSigRemoveFile: function(t_row) {
                        Globals.setSelectedRow(tableID, t_row)
                        deleteConfirmDialog.askDelete(t_row)
                    }
                }
            }
            DelegateChoice {
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
            if (event.key === Qt.Key_C && (event.modifiers & Qt.ControlModifier)) {
                Globals.copyRowToClipboard(tableID)
                event.accepted = true
                return
            }
            if (event.key == Qt.Key_Return || event.key == Qt.Key_Enter) {
                rootID.cmdDownloadFile(tableID.currentRow)
                event.accepted = true
                return
            }
            if (event.key === Qt.Key_Delete) {
                deleteConfirmDialog.askDelete(tableID.currentRow)
                event.accepted = true
                return
            }
            event.accepted = false
        }
    }

    Text {
        anchors.centerIn: parent
        text: "No files available"
        color: VisualStyle.textColor
        font.pixelSize: 14
        visible: tableID.rows === 0
    }

    // Delete confirmation dialog
    ModalDialog {
        id: deleteConfirmDialog

        property int pendingRow: -1

        title: "Delete File"
        dialogWidth: 420
        dialogHeight: 180

        function askDelete(t_row) {
            pendingRow = t_row
            deleteFileNameText.text = "Delete \"" + getFilename(t_row) + "\"?"
            open()
        }

        ColumnLayout {
            anchors.fill: parent
            spacing: 10

            Text {
                id: deleteFileNameText

                Layout.fillWidth: true
                Layout.topMargin: 10

                wrapMode: Text.Wrap
                horizontalAlignment: Text.AlignHCenter
                color: VisualStyle.statusBar.textColor
                font.pixelSize: 14
            }

            Item { Layout.fillHeight: true }

            RowLayout {
                Layout.alignment: Qt.AlignHCenter
                spacing: 20

                Button {
                    text: "Delete"
                    onClicked: {
                        rootID.cmdRemoveFile(deleteConfirmDialog.pendingRow)
                        deleteConfirmDialog.close()
                    }
                }
                Button {
                    text: "Cancel"
                    onClicked: {
                        deleteConfirmDialog.close()
                    }
                }
            }
        }
    }

    onVisibleChanged: {
        if (visible) {
            tableID.forceActiveFocus()
        }
    }
}
