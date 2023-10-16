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
import QtQuick.Layouts

// LN page
FocusScope {
	id: root

	SplitView {
		id: splitView

		anchors.fill: parent

		// Delimiter
		handle: Rectangle {
			id: handleDelegate
			implicitWidth: 4
			implicitHeight: 4
			color: SplitHandle.pressed ? "black" : "gray"
			//color: SplitHandle.pressed ? "gray" : "lightgray"
				//: (SplitHandle.hovered ? "gray" : "gray")

			containmentMask: Item {
				x: (handleDelegate.width - width) / 2
				width: 20
				height: splitView.height
			}
		}

		// Area for LogicalNode's table
		Rectangle {
			SplitView.minimumWidth: 300
			SplitView.fillWidth: false
			color: "white"

			// Table
			LN_Table {
				id: tableLN

				anchors.fill: parent

				onSigLeftOrRightKey: function() {
					console.log("LN_Page: Activate DO_Table")
					tableLN.focus = false
					tableDO.focus = true
				}
			}
		}

		// Area for DataObject's table
		Rectangle {
			SplitView.minimumWidth: 100
			SplitView.fillWidth: true
			color: "white"

			// Table
			DO_Table {
				id: tableDO

				anchors.fill: parent

				onSigLeftOrRightKey: function() {
					console.log("LN_Page: Activate LN_Table")
					tableLN.focus = true
					tableDO.focus = false
				}
			}
		}

		// Update data
		Timer {
			id: lnPageTimer

			interval: 1000
			running: false
			repeat: true

			onTriggered: function() {
				//ldBackend.updateDO_Table(tableDO.currentLDevice, tableDO.currentLNode)
			}
		}
	}

	onVisibleChanged: {
		if (visible) {
			tableLN.focus = true
			tableDO.focus = false

			lnPageTimer.start()
		} else {
			tableLN.focus = false
			tableDO.focus = false

			lnPageTimer.stop()
		}
	}

	Keys.onPressed: function(event) {
		console.log("LN_Page: Key pressed " + event.key)

		if (event.key == Qt.Key_Left) {
			console.log("LN_Page: Activate LN_Table")
			tableLN.focus = true
			tableDO.focus = false
			event.accepted = true
		}
		if (event.key == Qt.Key_Right) {
			console.log("LN_Page: Activate DO_Table")
			tableLN.focus = false
			tableDO.focus = true
			event.accepted = true
		}
	}
}