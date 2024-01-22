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

import GlobalVarsModule

Rectangle
{
	id: rootID

	property alias currentIndex: gridID.currentIndex

	anchors.fill: parent

	color: "gray" //"lightgray"
	opacity: 0.8
	z: 100500
	visible: false

	signal sigSelectedNextPage(int t_id)

	Dialog {
		id: dialogID

		anchors.centerIn: parent
		z: 100501

		width: 635
		height: 65

		modal: true
		closePolicy: Dialog.CloseOnEscape | Dialog.CloseOnPressOutside
		visible: false

		GridView {
			id: gridID

			anchors {
				fill: parent
				margins: 5
			}

			// clip: true
			focus: true
			keyNavigationEnabled: true
			boundsBehavior: Flickable.StopAtBounds

			cellWidth: 100
			cellHeight: 50

			model: ListModel {
				ListElement { name: "Home"; 	page: Globals.Page.START }
				ListElement { name: "LD"; 		page: Globals.Page.LD }
				ListElement { name: "LN"; 		page: Globals.Page.LN }
				ListElement { name: "DS"; 		page: Globals.Page.DS }
				ListElement { name: "RCB"; 		page: Globals.Page.RCB }
				ListElement { name: "FS"; 		page: Globals.Page.FS }
			}

			delegate: Rectangle {
				property int page: model.page

				width: 90
				height: 40

				color: "lightgray"
				border.width: 2
				border.color: GridView.isCurrentItem ? "black" : "lightgray"

				Label {
					anchors.centerIn: parent

					text: model.name
				}
				MouseArea {
					anchors.fill: parent
					acceptedButtons: Qt.LeftButton | Qt.RightButton

					onClicked: function(mouse) {
						sigSelectedNextPage(parent.page)
						rootID.closeNavigationGrid()
					}
				}
			}

			Keys.onPressed: function(event) {
				// console.log("NavigationGrid: Pressed key " + k)
				if (event.key == Qt.Key_Escape) {
					rootID.closeNavigationGrid()
				}
				if (event.key == Qt.Key_Return || event.key == Qt.Key_Enter) {
					sigSelectedNextPage(gridID.currentItem.page)
					rootID.closeNavigationGrid()
				}
			}
		}

		onClosed: function() {
			rootID.closeNavigationGrid()
		}
	}

	function showNavigationGrid() {
		//console.log("NavigationGrid: startLoad")
		parent.enabled = false

		rootID.visible = true
		dialogID.visible = true
	}
	function closeNavigationGrid() {
		//console.log("NavigationGrid: finishLoad")
		rootID.visible = false
		dialogID.visible = false

		parent.enabled = true
	}
}