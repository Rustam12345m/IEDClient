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

	function actSignalsTable() {
		lnSignalsStack.currentIndex = 0
	}
	function actSignalsTree() {
		lnSignalsStack.currentIndex = 1
	}
	function switchSignalsView() {
		lnSignalsStack.currentIndex = (lnSignalsStack.currentIndex == 0) ? 1 : 0
	}

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
				onSigForceFocus: function() {
					tableLN.focus = true
					tableDO.focus = false
				}
				onSigSelectedNewLN: function() {
					devBackend.updateDO_Table(tableDO.currentLDevice, tableDO.currentLNode)
				}
			}
		}

		// Area for DataObject's table
		Rectangle {
			SplitView.minimumWidth: 100
			SplitView.fillWidth: true
			color: "white"

			StackLayout {
				id: lnSignalsStack

				anchors.fill: parent

				// Table DO signals
				LN_SignalsTable {
					id: tableDO

					//anchors.fill: parent

					onSigLeftOrRightKey: function() {
						console.log("LN_Page: Activate LN_Table")
						tableLN.focus = true
						tableDO.focus = false
					}
					onSigForceFocus: function() {
						tableLN.focus = false
						tableDO.focus = true
					}
				}

				// Tree DO signals
				LN_SignalsTree {
					id: treeDO

					//anchors.fill: parent
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
				//devBackend.updateDO_Table(tableDO.currentLDevice, tableDO.currentLNode)
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