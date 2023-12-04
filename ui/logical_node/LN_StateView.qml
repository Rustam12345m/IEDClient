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

import "qrc:/global/"

// Table with DataObjects for concrete Logical Node
FocusScope {
	id: rootID

	readonly property int defDelegateHeight: 30

	signal sigLeftOrRightKey()
	signal sigForceFocus()

	property var globals: Globals {}

	function resizeColumns() {
		globals.resizeColumnsToContent(headerID, tableID)
	}

	// Header for TableView below with DO
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

			implicitWidth: labelID.implicitWidth + 24 + 10
			implicitHeight: 30

			color: "#f6f6f6"
			border.color: "#e4e4e4"

			Row {
				id: rowID

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

	// Table of DO for a selected LN
	TableView {
		id: tableID

		anchors {
			leftMargin: 5
			left: parent.left
			right: parent.right
			top: headerID.bottom
			bottom: parent.bottom
		}

		model: devBackend.doModel

		focus: true
		keyNavigationEnabled: true
		reuseItems: true

		clip: true
		interactive: true
		boundsBehavior: Flickable.StopAtBounds

		columnWidthProvider: function(t_column) {
			return globals.calcColumnsWidth(headerID, tableID, t_column)
		}

		selectionBehavior: TableView.SelectRows
		selectionModel: ItemSelectionModel {
			model: tableID.model
		}

		delegate: TextDelegate {
			delegateHeight: defDelegateHeight
			selected: (tableID.currentRow == row)

			textAlign: (column == 0) ? Text.AlignLeft : Text.AlignHCenter
			text: model.display

			onSigClick: function(row, col) {
				globals.setSelectedRow(tableID, row)
				sigForceFocus()
			}
		}

		ScrollBar.vertical: ScrollBar {
			policy: ScrollBar.AsNeeded
			active: true
			stepSize: 0.25

			onActiveChanged: {
				if (!active) {
					active = true;
				}
			}
		}

		Keys.onPressed: function(event) {
			//console.log("DO_Table: Key pressed " + event.key)
			if (event.key == Qt.Key_Left || event.key == Qt.Key_Right || event.key == Qt.Key_Tab) {
				sigLeftOrRightKey()
				event.accepted = true
			}
			event.accepted = false
		}

		Connections {
			target: devBackend.doModel

			function onDataChanged() {
				Qt.callLater(rootID.resizeColumns)
			}
		}
	}
}