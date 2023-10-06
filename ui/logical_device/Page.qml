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
import QtQuick.Layouts
import QtQuick.Controls

import "qrc:/global/"

// LD page
Item {
	id: root

	readonly property int blkSpace: 30
	readonly property int blkWidth: 240
	readonly property int blkHeight: 200
	readonly property int blkBorder: 4

	signal sigNextPageSignal(int page)
	signal sigLDeviceChanged(int current, string name)

	// Grid of Logical devices
	GridView {
		id: gridView

		anchors.fill: parent
		anchors.margins: blkSpace

		cellWidth: blkWidth + 2 * blkBorder + blkSpace
		cellHeight: blkHeight + 2 * blkBorder + blkSpace

		focus: true
		keyNavigationEnabled: true
		boundsBehavior: Flickable.StopAtBounds

		model: mainPres.ldModel

		delegate: LogDeviceDelegate {
			blkWidth: root.blkWidth
			blkHeight: root.blkHeight
			blkBorder: root.blkBorder

			selected: (gridView.currentIndex == index)

			onSigLeftClicked: {
				gridView.focus = true
				gridView.currentIndex = index

				sigLDeviceChanged(index, name);
			}
			onSigDLeftClicked: {
				if (gridView.currentIndex === index) {
					//gridView.currentIndex = -1
				} else {
					gridView.currentIndex = index
				}
				//console.log("Double click on: " + index)

				sigLDeviceChanged(index, name)
				sigNextPageSignal(Globals.Page.LN)
			}
			onSigLN_Page: {
				sigNextPageSignal(Globals.Page.LN)
			}
			onSigDS_Page: {
				sigNextPageSignal(Globals.Page.DS)
			}
			onSigRCB_Page: {
				sigNextPageSignal(Globals.Page.RCB)
			}
		}

		onCurrentIndexChanged: function() {
			sigLDeviceChanged(currentIndex, currentItem.name);
		}
	}
}