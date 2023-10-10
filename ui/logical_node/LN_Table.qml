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

// Table of all Logical Nodes for one Logical Device
FocusScope {
	readonly property int delegateHeight: 30
	readonly property int delegateWidth: 60

	property int currentLDeviceIndex: tableID.model.currentLD
	property alias currentLNodeIndex: tableID.currentRow

	signal sigLNodeSelected(int t_ld, int t_ln);
	signal sigLeftOrRightKey()

	function updateLDeviceIndex(t_ld) {
		tableID.model.currentLD = t_ld
	}

	HorizontalHeaderView {
		id: headerID

		anchors {
			rightMargin: 5
			left: parent.left
			top: parent.top
			right: parent.right
		}
		boundsBehavior: Flickable.StopAtBounds

		syncView: tableID

		delegate: Rectangle {
			property var paramModel: model

			implicitWidth: Math.max(textArea.implicitWidth + 10, delegateWidth)
			implicitHeight: delegateHeight

			color: "#f6f6f6"
			border.color: "#e4e4e4"

			Label {
				id: textArea
				anchors.centerIn: parent
				horizontalAlignment: Text.AlignHCenter
				verticalAlignment: Text.AlignVCenter

				//font.bold: true
				text: model[headerID.textRole]
				color: "#ff26282a"
			}

			MouseArea {
				anchors.fill: parent

				onClicked: function(ev) {
					console.log("Table headerID clicked: " + model.column)
				}
			}

			Component.onCompleted: {
			}
		}
	}

	TableView {
		id: tableID

		anchors {
			rightMargin: 5

			left: parent.left
			right: parent.right
			top: headerID.bottom
			bottom: parent.bottom
		}
		model: ldBackend.lnModel

		focus: true
		interactive: true
		boundsBehavior: Flickable.StopAtBounds

		selectionBehavior: TableView.SelectRows
		selectionModel: ItemSelectionModel {
			model: tableID.model

			onCurrentChanged: {
				sigLNodeSelected(model.currentLD, currentIndex.row)
			}
		}

		/*
		onWidthChanged: function() {
			tableID.forceLayout()
		}
		*/
		columnWidthProvider: function(column) {
			switch (column) {
			case 0: {
				let res = width - 3 * delegateWidth
				return res
			}
			default: {
				let v1 = header.implicitColumnWidth(column)
				let v2 = implicitColumnWidth(column)
				let res = (v1 > v2) ? v1 : v2
				return Math.max(res, delegateWidth)
			}
			}
		}

		delegate: DelegateChooser {
			// Name column
			DelegateChoice {
				column: 0

				delegate: Item {
					property bool selected: (tableID.currentRow == row)

					implicitWidth: textName.implicitWidth + 10
					implicitHeight: delegateHeight

					Rectangle {
						color: (selected ? "lightgray" : "white")
						anchors.fill: parent
						border.color: (selected ? "black" : "lightgray")

						Text {
							id: textName
							anchors.centerIn: parent
							text: model.name
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

			// Mode column
			DelegateChoice {
				column: 1

				delegate: Item {
					property bool selected: (tableID.currentRow == row)
					implicitWidth: delegateWidth
					implicitHeight: delegateHeight

					Rectangle {
						color: "white"
						anchors.fill: parent
						border.color: (selected ? "black" : "lightgray")

						Text {
							id: textMode
							anchors.centerIn: parent
							text: model.mode
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

			// Beh column
			DelegateChoice {
				column: 2

				delegate: Item {
					property bool selected: (tableID.currentRow == row)
					implicitWidth: delegateWidth
					implicitHeight: delegateHeight

					Rectangle {
						color: "white"
						anchors.fill: parent
						border.color: (selected ? "black" : "lightgray")

						Text {
							id: textBeh
							anchors.centerIn: parent
							text: model.beh
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

			// Health column
			DelegateChoice {
				column: 3

				delegate: Item {
					property bool selected: (tableID.currentRow == row)
					implicitWidth: delegateWidth
					implicitHeight: delegateHeight

					Rectangle {
						color: "white"
						anchors.fill: parent
						border.color: (selected ? "black" : "lightgray")

						Text {
							id: textHealth
							anchors.centerIn: parent
							text: model.health
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

		ScrollBar.vertical: ScrollBar {
			policy: ScrollBar.AsNeeded
			active: true
			onActiveChanged: {
				if (!active) {
					active = true;
				}
			}
		}

		Component.onCompleted: function() {
			//tableID.forceLayout()
		}

		Keys.onPressed: function(event) {
			//console.log("LN_Table: Key pressed " + event.key)
			if (event.key == Qt.Key_Left || event.key == Qt.Key_Right || event.key == Qt.Key_Tab) {
				sigLeftOrRightKey()
				event.accepted = true
			}
			event.accepted = false
		}
	}

	onVisibleChanged: {
		//console.log("LN_Table: Focus " + visible)
		/*
		if (visible) {
			tableID.focus = true
		} else {
			tableID.focus = false
		}
		*/
	}
}