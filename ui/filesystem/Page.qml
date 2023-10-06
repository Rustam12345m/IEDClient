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

Item {
	HorizontalHeaderView {
		id: headerID

		boundsBehavior: Flickable.StopAtBounds
		anchors.left: tableID.left
		anchors.top: parent.top
		anchors.right: parent.right

		model: mainPres.filesModel
		syncView: tableID

		delegate: Rectangle {
			property var columnIndex: model.column

			implicitWidth: text.implicitWidth + 20
			implicitHeight: 30
			color: "#f6f6f6"
			border.color: "#e4e4e4"

			Label {
				id: text

				anchors.centerIn: parent
				horizontalAlignment: Text.AlignHCenter
				verticalAlignment: Text.AlignVCenter

				text: model[headerID.textRole]
				color: "#ff26282a"
			}

			MouseArea {
				anchors.fill: parent

				onClicked: function() {
					console.log("FilesHeader clicked: " + columnIndex)
				}
			}
		}
	}

	TableView {
		id: tableID
		model: mainPres.filesModel

		anchors {
			left: parent.left
			top: headerID.bottom
			right: parent.right
			bottom: parent.bottom
			//rightMargin: 5
		}

		focus: false
		clip: true
		boundsBehavior: Flickable.StopAtBounds

		function getFilename(row) {
			let idx = tableID.model.index(row, 0)
			return tableID.model.data(idx, "display")
		}

		function setGoodColumnsWidth() {
			const iw = []
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

		onWidthChanged: function() {
			//tableID.forceLayout()
			setGoodColumnsWidth()
		}

		selectionBehavior: TableView.SelectRows
		selectionModel: ItemSelectionModel {
			model: tableID.model

			onCurrentChanged: {
				//console.log(currentIndex)
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
		/*
		ScrollBar.horizontal: ScrollBar {
			policy: ScrollBar.AsNeeded
			active: true
			onActiveChanged: {
				if (!active)
					active = true;
			}
		}
		*/

		delegate: DelegateChooser {
			// Last modified time
			DelegateChoice {
				column: 0

				delegate: FileTableDelegate {
					text: model.display
				}
			}
			// File name
			DelegateChoice {
				column: 1

				delegate: FileTableDelegate {
					text: model.display
				}
			}
			// File size
			DelegateChoice {
				column: 2

				delegate: FileTableDelegate {
					text: model.display
				}
			}
			// Controls
			DelegateChoice {
				column: 3

				delegate: FileControlDelegate {
				}
			}
		}
	}
}