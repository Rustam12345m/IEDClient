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
import QtQuick.Layouts

// LN page
Item {
	function updateLDeviceIndex(t_ld) {
		tableLN.updateLDeviceIndex(t_ld)
		tableDO.updateLNodeIndex(t_ld, tableLN.currentLNodeIndex)
	}

	SplitView {
		id: splitView
		anchors.fill: parent

		handle: Rectangle {
			id: handleDelegate
			implicitWidth: 4
			implicitHeight: 4
			color: SplitHandle.pressed ? "black"
				: (SplitHandle.hovered ? "gray" : "gray")

			containmentMask: Item {
				x: (handleDelegate.width - width) / 2
				width: 20
				height: splitView.height
			}
		}

		// Table of LN
		Rectangle {
			SplitView.minimumWidth: 300
			SplitView.fillWidth: false
			color: "white"

			LN_Table {
				id: tableLN
				anchors.fill: parent

				onSigCurrentLNodeChanged: function(t_ld, t_ln) {
					console.log("DO_Table got signal from LN_Table: ld = ", t_ld, " ln = ", t_ln)

					tableDO.updateLNodeIndex(t_ld, t_ln)
				}
			}
		}

		// Table of DO for this LN
		Rectangle {
			SplitView.minimumWidth: 100
			SplitView.fillWidth: true
			color: "white"

			DO_Table {
				id: tableDO
				anchors.fill: parent
			}
		}

		// Update data
		Timer {
			id: lnPageTimer

			interval: 1000
			running: false
			repeat: true

			onTriggered: function() {
				mainPres.updateLNodeData(tableDO.currentLDevice, tableDO.currentLNode)
			}
		}

		onVisibleChanged: {
			//console.log("LN_Page: visible changed: " + visible)
			if (visible) {
				//console.log("LN_Page: Start update")
				lnPageTimer.start()
			} else {
				//console.log("LN_Page: Stop update")
				lnPageTimer.stop()
			}
		}
	}
}
