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

// LD page
FocusScope {
	id: rootID

	readonly property int blkSpace: 30
	readonly property int blkWidth: 200
	readonly property int blkHeight: 150
	readonly property int blkBorder: 4

	signal sigActivatePage(int page)

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

		model: devBackend.getLD_GridModel()

		delegate: LD_GridDelegate {
			blkWidth: rootID.blkWidth
			blkHeight: rootID.blkHeight
			blkBorder: rootID.blkBorder

			selected: (gridView.currentIndex == index)

			onSigLeftClicked: {
				gridView.focus = true
				gridView.currentIndex = index
			}
			onSigDLeftClicked: {
				if (gridView.currentIndex != index) {
					gridView.currentIndex = index
				}
				sigActivatePage(Globals.Page.LN)
			}

			onSigLN_Page: {
				sigActivatePage(Globals.Page.LN)
			}
			onSigDS_Page: {
				sigActivatePage(Globals.Page.DS)
			}
			onSigRCB_Page: {
				sigActivatePage(Globals.Page.RCB)
			}
		}

		onCurrentIndexChanged: {
			gridView.model.setSelectedLD(gridView.currentIndex)
		}

		// Reset selection
		MouseArea {
			anchors.fill: parent
			acceptedButtons: Qt.LeftButton

			onClicked: function(msx) {
				if (gridView.indexAt(mouseX, mouseY) === -1) {
					gridView.currentIndex = -1
					msx.accepted = true
				} else {
					msx.accepted = false
				}
			}
			propagateComposedEvents: true
		}

		onVisibleChanged: {
			if ((gridView.currentIndex < 0) && (gridView.count > 0)) {
				gridView.currentIndex = 0
			}
		}
	}

	Keys.onPressed: function(event) {
		//console.log("LD_Page: Key pressed " + event.key)
		if (event.key == Qt.Key_Return || event.key == Qt.Key_Enter) {
			sigActivatePage(Globals.Page.LN)
		}
	}

	onVisibleChanged: {
		//console.log("LD_Grid: Focus " + visible)
		if (visible) {
			gridView.focus = true
		} else {
			gridView.focus = false
		}
	}
}