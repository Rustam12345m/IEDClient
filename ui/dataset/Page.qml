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

// DS page
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

			containmentMask: Item {
				x: (handleDelegate.width - width) / 2
				width: 20
				height: splitView.height
			}
		}

		// Area for TableView of all DS
		Rectangle {
			SplitView.minimumWidth: 300
			SplitView.fillWidth: false
			color: "white"

			// Table
			DS_ComTable {
				id: tableDS_Info

				anchors.fill: parent

				onSigLeftOrRightKey: function() {
					//console.log("DS_Page: Activate DO_Table")
					tableDS_Info.focus = false
					tableDS_Val.focus = true
				}
				onSigForceFocus: function() {
					tableDS_Info.focus = true
					tableDS_Val.focus = false
				}
				onSigSelectedNewDS: function() {
				}
			}
		}

		// Area for DataSet values
		Rectangle {
			SplitView.minimumWidth: 100
			SplitView.fillWidth: true
			color: "white"

			// Table
			DS_StateTable {
				id: tableDS_Val

				anchors.fill: parent

				onSigLeftOrRightKey: function() {
					//console.log("DS_Page: Activate LN_Table")
					tableDS_Info.focus = true
					tableDS_Val.focus = false
				}
				onSigForceFocus: function() {
					tableDS_Info.focus = false
					tableDS_Val.focus = true
				}
			}
		}
	}

	onVisibleChanged: {
	}

	Keys.onPressed: function(event) {
		console.log("DS_Page: Key pressed " + event.key)

		if (event.key == Qt.Key_Left) {
			console.log("DS_Page: Activate LN_Table")
			tableDS_Info.focus = true
			tableDS_Val.focus = false
			event.accepted = true
		}
		if (event.key == Qt.Key_Right) {
			console.log("DS_Page: Activate DO_Table")
			tableDS_Info.focus = false
			tableDS_Val.focus = true
			event.accepted = true
		}
	}
}