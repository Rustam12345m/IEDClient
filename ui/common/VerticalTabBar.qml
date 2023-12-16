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

import AppStylesModule

import "qrc:/common/"

// Vertical TabBar
Item
{
	id: rootID

	property bool leftSide: false
	property alias model: vertTabBar.model

	signal sigTabSelected(int index)
	
	width: 30

	ListView {
		id: vertTabBar

		anchors.fill: parent

		// model: ListModel {
		// 	ListElement { title: "Test 1" }
		// 	ListElement { title: "Test 2" }
		// }

		delegate: Item {
			width: vertTabBar.width
			height: 120

			Rectangle {
				width: parent.width
				height: parent.height
				color: (vertTabBar.currentIndex === index) ? "lightgray" : "#f6f6f6"

				Text {
					rotation: rootID.leftSide ? -90 : 90
					anchors {
						centerIn: parent
					}

					text: title
				}
				MouseArea {
					anchors.fill: parent
					onClicked: {
						vertTabBar.currentIndex = index
						sigTabSelected(index)
					}
				}
			}
		}
	}
}