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

// Table with DataObjects for concrete DataSet
FocusScope {
	id: rootID

	readonly property int defDelegateHeight: 30
	readonly property int defDelegateWidth: 60

	signal sigLeftOrRightKey()
	signal sigForceFocus()

	property var globals: Globals {}

	function resizeColumns() {
		globals.resizeColumnsToContent(headerID, tableID)
	}

	// Header for TableView below
	HorizontalHeaderView {
		id: headerID

		anchors {
			left: tableID.left
			top: parent.top
			right: parent.right
		}
		boundsBehavior: Flickable.StopAtBounds
		resizableColumns: false

		syncView: tableID

		delegate: Rectangle {
			implicitWidth: Math.max(textArea.implicitWidth + 10, defDelegateWidth)
			implicitHeight: defDelegateHeight

			color: "#f6f6f6"
			border.color: "#e4e4e4"

			Label {
				id: textArea
				anchors.fill: parent

				horizontalAlignment: Text.AlignHCenter
				verticalAlignment: Text.AlignVCenter

				//font.bold: true
				text: model.display
				color: "#ff26282a"
			}
		}
	}

	// Table of DO for a selected DS
	TableView {
		id: tableID

		anchors {
			leftMargin: 5
			left: parent.left
			right: parent.right
			top: headerID.bottom
			bottom: parent.bottom
		}

		model: devBackend.dataSetModel

		focus: true
		clip: true
		interactive: true
		boundsBehavior: Flickable.StopAtBounds

		columnWidthProvider: function(t_column) {
			return globals.calcColumnsWidth(headerID, tableID, t_column)
		}

		selectionBehavior: TableView.SelectRows
		selectionModel: ItemSelectionModel {
			model: tableID.model

			onCurrentChanged: {
				Qt.callLater(rootID.resizeColumns)
			}
		}

		delegate: TextDelegate {
			delegateHeight: defDelegateHeight
			selected: (tableID.currentRow == row)

			textAlign: Text.AlignHCenter
			text: model.display

			onSigClick: function(row, col) {
				globals.setSelectedRow(tableID, row)
				sigForceFocus()
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