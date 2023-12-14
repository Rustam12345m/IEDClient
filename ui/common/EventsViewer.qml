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
import QtQuick.Layouts

Window {
	title: qsTr("IEDClient - System messages")

	id: root

	width: 800
	height: 300
	visible: true

	Rectangle {
		anchors.fill: parent

		HorizontalHeaderView {
			id: header

			anchors.left: tableID.left
			anchors.top: parent.top
			anchors.right: parent.right

			boundsBehavior: Flickable.StopAtBounds
			syncView: tableID

			delegate: Rectangle {
				implicitWidth: text.implicitWidth + 20
				implicitHeight: 30
				color: "#f6f6f6"
				border.color: "#e4e4e4"

				Label {
					id: text

					anchors.centerIn: parent

					horizontalAlignment: Text.AlignHCenter
					verticalAlignment: Text.AlignVCenter

					text: model[header.textRole]
					color: "#ff26282a"
				}
			}
		}

		TableView {
			id: tableID


			anchors.left: parent.left
			anchors.top: header.bottom
			anchors.right: parent.right
			anchors.bottom: parent.bottom

			clip: true
			interactive: true
			boundsBehavior: Flickable.StopAtBounds

			model: comBackend.eventsModel

			selectionBehavior: TableView.SelectRows
			selectionModel: ItemSelectionModel {
				model: tableID.model

				onCurrentChanged: {
					//console.log("Select current changed: " + currentIndex)
				}
			}

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
				setGoodColumnsWidth()
			}
			//columnWidthProvider: calcGoodWidthFoColumn

			ScrollBar.vertical: ScrollBar {
				policy: ScrollBar.AsNeeded
				active: true
				onActiveChanged: {
					if (!active) {
						active = true;
					}
				}
			}

			delegate: Rectangle {
				required property bool selected

				implicitWidth: textArea.implicitWidth + 20
				implicitHeight: 20

				color: (selected ? "lightgray" : "white")
				border.color: (selected ? "black" : "lightgray")
				border.width: 1

				Text {
					id: textArea
					text: display
					anchors.centerIn: parent
				}
				MouseArea {
					anchors.fill: parent
					acceptedButtons: Qt.LeftButton | Qt.RightButton

					onClicked: function(mouse) {
						let idx = tableID.model.index(row, 0);
						tableID.selectionModel.setCurrentIndex(idx, ItemSelectionModel.Clear
																	| ItemSelectionModel.Select
																	| ItemSelectionModel.Rows);
					}
				}
			}
		}
	}
}