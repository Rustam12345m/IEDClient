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
import QtQuick.Layouts

import GlobalVarsModule
import AppStylesModule

import "qrc:/common/"

// LN page
FocusScope
{
	id: root

	function switchSignalsView() {
		lnSignalsStack.currentIndex = (lnSignalsStack.currentIndex == 0) ? 1 : 0
	}

	// Window for setting a new value for DA/SDA
	DiaChangeValue {
		id: diaChangeValue

		anchors.centerIn: parent
	}

	// LN CommonTable | Delimiter | DataTable
	SplitView {
		id: splitView

		anchors.fill: parent

		// Delimiter
		handle: SplitDelimeter {
			height: splitView.height
			pressed: SplitHandle.pressed
		}

		// Area for LogicalNode's table
		Rectangle {
			SplitView.minimumWidth: 300
			SplitView.fillWidth: false
			color: ColorPalette.backgroundColor2

			// Table
			LN_CommonTable {
				id: tableLN

				anchors {
					fill: parent
				}

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
					// devBackend.updateLN_TreeValues(tableDO.currentLDevice, tableDO.currentLNode)
				}
			}
		}

		// Area for DataObject's table
		Rectangle {
			SplitView.minimumWidth: 100
			SplitView.fillWidth: true
			color: ColorPalette.backgroundColor1

			// Different view pages for the LN
			StackLayout {
				id: lnSignalsStack

				anchors {
					right: rectLnViewTabBar.left
					left: parent.left
					top: parent.top
					bottom: parent.bottom
				}

				// Table DO signals
				LN_StateView {
					id: tableDO

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

					onSigValueClicked: function(t_ref, t_msg, t_value) {
						diaChangeValue.open(t_ref, t_msg, t_value)
					}
				}

				LN_ControlsView {
					id: lnControlsView
					
				}

				LN_SettingsView {
					id: lnSettingsView

				}
			}

			// Vertical TabBar for lnSignalsStack
			Rectangle {
				id: rectLnViewTabBar

				anchors {
					right: parent.right
					top: parent.top
					bottom: parent.bottom
				}

				width: 30

				VerticalTabBar {
					anchors.fill: parent

					model: ListModel {
						ListElement { title: "State" }
						ListElement { title: "Tree" }
						ListElement { title: "Controls" }
						ListElement { title: "Settings" }
					}

					onSigTabSelected: function(index) {
						lnSignalsStack.currentIndex = index
					}
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