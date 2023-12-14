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

import "qrc:/common/"

import GlobalVarsModule
import AppStylesModule

FocusScope {
	id: rootID

	// Vertical TabBar: CB
	Rectangle {
		id: rectCBViewTabBar

		anchors {
			left: parent.left
			top: parent.top
			bottom: parent.bottom
		}

		width: 30
		//border.width: 1
		//border.color: "black"// "#f6f6f6"

		ListView {
			id: cbViewTabBar

			anchors {
				fill: parent
			}

			model: ListModel {
				ListElement { title: "Buffered" }
				ListElement { title: "Unbuffered" }
				ListElement { title: "GOOSE" }
				ListElement { title: "SV" }
			}

			delegate: Item {
				width: cbViewTabBar.width
				height: 120

				Rectangle {
					width: parent.width
					height: parent.height

					color: (cbViewTabBar.currentIndex === index) ? "lightgray" : "#f6f6f6"
					border.width: 1
					border.color: "black"

					Text {
						rotation: -90
						anchors.top: parent.top
						anchors.horizontalCenter: parent.horizontalCenter

						text: title
						anchors.centerIn: parent
					}
					MouseArea {
						anchors.fill: parent
						onClicked: {
							cbViewTabBar.currentIndex = index
							//cbSignalsStack.currentIndex = index
						}
					}
				}
			}
		}
	}

	// Header for Table below
	HorizontalHeaderView {
		id: headerID

		anchors {
			left: rectCBViewTabBar.right
			top: parent.top
			right: parent.right
		}
		boundsBehavior: Flickable.StopAtBounds
		resizableColumns: false

		syncView: tableID

		delegate: Rectangle {
			implicitWidth: labelID.implicitWidth + 10
			implicitHeight: 30

			color: "#f6f6f6"
			border.color: "#e4e4e4"

			Row {
				anchors.centerIn: parent
				spacing: 5

				Label {
					id: labelID

					text: model.display
					color: "#ff26282a"
				}
			}
		}
	}

	// Table of RCB on the IED
	TableView {
		id: tableID

		anchors {
			left: rectCBViewTabBar.right
			right: parent.right
			top: headerID.bottom
			bottom: parent.bottom
		}

		model: devBackend.getRCB_ComModel()

		focus: true
		reuseItems: true
		keyNavigationEnabled: true

		clip: true
		interactive: true
		boundsBehavior: Flickable.StopAtBounds

		columnWidthProvider: function(t_column) {
			return Globals.calcColumnsWidth(headerID, tableID, t_column)
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

		delegate: TextDelegate {
			delegateHeight: 30
			selected: (tableID.currentRow == row)

			textAlign: Text.AlignHCenter
			text: model.display

			onSigClick: function(row, col) {
				Globals.setSelectedRow(tableID, row)
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
		}
	}

	onVisibleChanged: {
		tableID.focus = visible
	}
}