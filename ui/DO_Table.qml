/*
 *  main.qml
 *
 *  Copyright 2023 Rustam Mustafin
 *
 *  This file is part of IEDMaster.
 *
 *  IEDMaster is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  IEDMaster is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with IEDMaster.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  See COPYING file for the complete license text.
 * */

import QtQuick
import QtQuick.Controls
import Qt.labs.qmlmodels

// Table with DataObjects for concrete Logical Node
Item {
	readonly property int defDelegateHeight: 30
	property int currentLDevice: tableID.model.currentLD
	property int currentLNode: tableID.model.currentLN

	function updateLNodeIndex(t_ld, t_ln) {
		tableID.model.currentLD = t_ld;
		tableID.model.currentLN = t_ln;
	}

	HorizontalHeaderView {
		id: header

		boundsBehavior: Flickable.StopAtBounds
		anchors {
			leftMargin: 5
			left: parent.left
			top: parent.top
			right: parent.right
		}

		syncView: tableID
		clip: true

		delegate: Rectangle {
			implicitWidth: text.implicitWidth + 10
			implicitHeight: defDelegateHeight

			color: "#f6f6f6"
			border.color: "#e4e4e4"

			Label {
				id: text
				anchors.centerIn: parent
				horizontalAlignment: Text.AlignHCenter
				verticalAlignment: Text.AlignVCenter

				//font.bold: true
				text: model[header.textRole]
				color: "#ff26282a"
			}
		}
	}

	TableView {
		id: tableID
		anchors {
			leftMargin: 5
			left: parent.left
			right: parent.right
			top: header.bottom
			bottom: parent.bottom
		}
		model: mainPres.doModel

		clip: true
		interactive: true
		boundsBehavior: Flickable.StopAtBounds

		function calcGoodWidthFoColumn(col) {
			const iw = []
			let sum = 0
			for (let i=0;i<columns;i++) {
				iw[i] = Math.max(header.implicitColumnWidth(i), implicitColumnWidth(i))
				sum = sum + iw[i]
			}
			if (sum === 0) sum = 1
			return width * (iw[col] / sum)
		}
		function setGoodColumnsWidth() {
			const iw = []
			let sum = 0, i = 0
			for (i=0;i<columns;i++) {
				iw[i] = Math.max(header.implicitColumnWidth(i), implicitColumnWidth(i))
				sum = sum + iw[i]
			}
			if (sum === 0) {
				sum = 1
			}
			for (i=0;i<columns;i++) {
				setColumnWidth(i, width * iw[i] / sum)
			}
		}

		onWidthChanged: function() {
			//setGoodColumnsWidth()
			//tableID.forceLayout()
		}
		//columnWidthProvider: calcGoodWidthFoColumn
		columnWidthProvider: function(column) {
			switch (column) {
			case 4: {
				return width - columnWidth(0) - columnWidth(1) - columnWidth(2) - columnWidth(3)
			}
			default: {
				return Math.max(header.implicitColumnWidth(column), implicitColumnWidth(column))
			}
			}
		}

		selectionBehavior: TableView.SelectRows
		selectionModel: ItemSelectionModel {
			model: tableID.model

			onCurrentChanged: {
				console.log("Select current changed: " + currentIndex)
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

		delegate: DelegateChooser {
			// Name column
			DelegateChoice {
				column: 0

				delegate: TextDelegate {
					delegateHeight: defDelegateHeight
					text: model.name
				}
			}

			// Value column
			DelegateChoice {
				column: 1

				delegate: TextDelegate {
					delegateHeight: defDelegateHeight
					text: model.value
				}
			}

			// Quality column
			DelegateChoice {
				column: 2

				delegate: TextDelegate {
					delegateHeight: defDelegateHeight
					text: model.quality
				}
			}

			// Timestamp column
			DelegateChoice {
				column: 3

				delegate: TextDelegate {
					delegateHeight: defDelegateHeight
					text: model.timestamp
				}
			}

			// Description column
			DelegateChoice {
				column: 4

				delegate: TextDelegate {
					delegateHeight: defDelegateHeight
					text: model.desc
				}
			}
		}
	}
}
