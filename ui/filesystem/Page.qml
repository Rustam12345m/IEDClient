/*
 *  main.qml
 *
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

// Filesystem page
FocusScope {
	HorizontalHeaderView {
		id: headerID

		property int sortOrder: 0
		property int sortedColumn: 0

		anchors {
			left: tableID.left
			top: parent.top
			right: parent.right
		}
		boundsBehavior: Flickable.StopAtBounds
		resizableColumns: false

		syncView: tableID

		delegate: Rectangle {
			property int column: model.column
			property bool sortable: model.display.sortable

			implicitWidth: labelID.implicitWidth + 10
			implicitHeight: 30

			color: "#f6f6f6"
			border.color: "#e4e4e4"

			Row {
				anchors.centerIn: parent
				spacing: 5

				Label {
					id: labelID

					text: model.display.text
					color: "#ff26282a"
				}
				Image {
					visible: (headerID.sortedColumn == column)
					source: (headerID.sortOrder == 0) ? "qrc:/img/icons/keyboard_arrow_down.svg"
													  : "qrc:/img/icons/keyboard_arrow_up.svg"

					width: 24
					height: 24
				}
			}
			MouseArea {
				anchors.fill: parent

				onClicked: function(msx) {
					if (sortable == false) {
						return
					}

					if (headerID.sortedColumn != column) {
						headerID.sortedColumn = column
						headerID.sortOrder = 0
					}

					if (headerID.sortOrder == 0) {
						headerID.sortOrder = 1
					} else {
						headerID.sortOrder = 0
					}
					tableID.model.sort(parent.column, headerID.sortOrder)
				}
			}
		}
	}

	// Table of files on the IED
	TableView {
		id: tableID
		model: fsBackend.sortModel//filesModel

		anchors {
			left: parent.left
			top: headerID.bottom
			right: parent.right
			bottom: parent.bottom
			//rightMargin: 5
		}

		focus: true
		keyNavigationEnabled: true
		reuseItems: true

		interactive: true
		clip: true
		boundsBehavior: Flickable.StopAtBounds

		function getFilename(row) {
			let idx = tableID.model.index(row, 2)
			return tableID.model.data(idx, "display")
		}

		function setSelectedRow(t_row) {
			if (tableID.currentRow === t_row) {
				return;
			}
			let idx = tableID.model.index(t_row, 0);
			tableID.selectionModel.setCurrentIndex(idx, ItemSelectionModel.Clear
														| ItemSelectionModel.Select
														| ItemSelectionModel.Rows);
		}
		function callDownloadFile(t_row) {
			console.log("FS_Table: Download file N" + t_row)

			setSelectedRow(t_row)
			fsBackend.downloadFile(tableID.getFilename(t_row))
		}

		function setGoodColumnsWidth() {
			var iw = []
			let sum = 0, i = 0
			for (i=0;i<columns;i++) {
				iw[i] = Math.max(headerID.implicitColumnWidth(i), implicitColumnWidth(i))
				sum = sum + iw[i]
			}
			if (sum === 0) {
				sum = 1
			}
			for (i=0;i<columns;i++) {
				setColumnWidth(i, width * iw[i] / sum)
			}
		}
		function calcColumnsWidth(t_column) {
			var iw = []
			let sum = 0, i = 0
			for (i=0;i<columns;i++) {
				iw[i] = Math.max(headerID.implicitColumnWidth(i), implicitColumnWidth(i))
				sum = sum + iw[i]
			}
			if (sum === 0) {
				sum = 1
			}
			return (width * iw[t_column] / sum)
		}
		columnWidthProvider: function(t_column) {
			return calcColumnsWidth(t_column)
		}

		selectionBehavior: TableView.SelectRows
		selectionModel: ItemSelectionModel {
			model: tableID.model
			/*
			onCurrentChanged: {
				console.log(currentIndex)
			}
			*/
		}

		delegate: DelegateChooser {
			// Index
			DelegateChoice {
				column: 0

				delegate: FileTableDelegate {
					selected: (tableID.currentRow == row)
					text: model.display

					onSigSelectRow: function(t_row) {
						tableID.setSelectedRow(t_row)
					}
					onSigDownloadFile: function(t_row) {
						tableID.callDownloadFile(t_row)
					}
				}
			}

			// Last modified time
			DelegateChoice {
				column: 1

				delegate: FileTableDelegate {
					selected: (tableID.currentRow == row)
					text: model.display

					onSigSelectRow: function(t_row) {
						tableID.setSelectedRow(t_row)
					}
					onSigDownloadFile: function(t_row) {
						tableID.callDownloadFile(t_row)
					}
				}
			}

			// File name
			DelegateChoice {
				column: 2

				delegate: FileTableDelegate {
					selected: (tableID.currentRow == row)
					text: model.display

					onSigSelectRow: function(t_row) {
						tableID.setSelectedRow(t_row)
					}
					onSigDownloadFile: function(t_row) {
						tableID.callDownloadFile(t_row)
					}
				}
			}

			// File size
			DelegateChoice {
				column: 3

				delegate: FileTableDelegate {
					selected: (tableID.currentRow == row)
					text: model.display

					onSigSelectRow: function(t_row) {
						tableID.setSelectedRow(t_row)
					}
					onSigDownloadFile: function(t_row) {
						tableID.callDownloadFile(t_row)
					}
				}
			}

			// Controls
			DelegateChoice {
				column: 4

				delegate: FileControlDelegate {
					selected: (tableID.currentRow == row)

					onSigDownloadFile: function(t_row) {
						tableID.callDownloadFile(t_row)
					}

					onSigRemoveFile: function(row) {
						console.log("Control: Remove file N" + row)
						fsBackend.removeFile(tableID.getFilename(row), row)
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

		Keys.onPressed: function(event) {
			//console.log("FS_Table: Key pressed " + event.key + ", currentIndex = " + tableID.currentRow)

			if (event.key == Qt.Key_Return || event.key == Qt.Key_Enter) {
				fsBackend.downloadFile(tableID.getFilename(tableID.currentRow))
			}
		}
	}

	onVisibleChanged: {
		//console.log("FS_Table: Focus " + visible)
		tableID.focus = visible
	}
}