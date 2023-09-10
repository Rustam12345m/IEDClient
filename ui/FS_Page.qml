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

Item {
	readonly property int cellTextMargin: 20

	HorizontalHeaderView {
		id: headerID

		boundsBehavior: Flickable.StopAtBounds
		anchors.left: tableID.left
		anchors.top: parent.top
		anchors.right: parent.right

		model: mainPres.filesModel
		syncView: tableID
		clip: true

		delegate: Rectangle {
			implicitWidth: text.implicitWidth + cellTextMargin
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
		}
	}

	TableView {
		id: tableID
		model: mainPres.filesModel

		anchors.left: parent.left
		anchors.top: headerID.bottom
		anchors.right: parent.right
		anchors.bottom: parent.bottom

		clip: false
		interactive: false
		boundsBehavior: Flickable.StopAtBounds

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
			tableID.forceLayout()
			//setGoodColumnsWidth()
		}
		/*
		columnWidthProvider: function(col) {
			switch (col) {
			case 0: {
				return width - columnWidth(1) - columnWidth(2)
			}
			default: {
				return Math.max(headerID.implicitColumnWidth(col), implicitColumnWidth(col))
			}
			}
		}
		*/

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
			// File name column
			DelegateChoice {
				column: 0

				delegate: FS_Delegate {
					delegateHeight: cellTextMargin
					text: model.name
				}
			}
			// File size column
			DelegateChoice {
				column: 1

				delegate: FS_Delegate {
					delegateHeight: cellTextMargin
					text: model.size
				}
			}
			// File last modified time column
			DelegateChoice {
				column: 2

				delegate: FS_Delegate {
					delegateHeight: cellTextMargin
					text: model.mts
				}
			}
		}
	}
}
