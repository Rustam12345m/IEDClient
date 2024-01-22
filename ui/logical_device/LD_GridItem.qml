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
import QtQuick.Layouts
import QtQuick.Controls

import AppStylesModule

import "qrc:/common/"

// LogicalDevice block in GridView
Item {
	id: rootID

	required property int 		index
	required property bool 		selected
	required property var 		ld_object

	property ListModel ldValsModel: ListModel {}

	// Geometry
	property int cellEdge: 28
	property int borderWidth: 2
	width: 200
	height: (5 / 6 * width)

	signal sigLeftClicked()
	signal sigDLeftClicked()
	signal sigLN_Page()
	signal sigDS_Page()
	signal sigRCB_Page()

	Rectangle {
		anchors.fill: parent

		border.color: (selected ? "gray" : "lightgray")
		border.width: rootID.borderWidth
		color: "pink"

		// Label
		Rectangle {
			id: labelRectID

			anchors {
				top: parent.top
				left: parent.left
				right: parent.right

				margins: rootID.borderWidth
				bottomMargin: 0
			}
			height: rootID.cellEdge

			color: (selected ? "gray" : "lightgray")

			Text {
				id: ldName

				anchors.fill: parent

				horizontalAlignment: Text.AlignLeft
				verticalAlignment: Text.AlignVCenter
				padding: 6

				wrapMode: Text.Wrap
				elide: Text.ElideRight

				font.bold: true
				font.pointSize: 12
				color: "black"

				text: "LD: " + ld_object.name
			}
		}
		// Values
		Rectangle {
			anchors {
				top: labelRectID.bottom
				bottom: parent.bottom
				left: parent.left
				right: parent.right

				margins: rootID.borderWidth
				topMargin: 0
			}
			// color: "red"

			ListView {
				anchors.fill: parent

				model: rootID.ldValsModel

				delegate: Rectangle {
					height: 20

					RowLayout {
						anchors.fill: parent

						Text {
							Layout.preferredWidth: 50
							padding: 6

							text: model.param

							font.pointSize: 10
						}
						Text {
							padding: 6

							text: model.value

							font.pointSize: 10
						}
					}
				}

				Component.onCompleted: {
					rootID.ldValsModel.append({ param: "Mod:", 		value: ld_object.mod })
					rootID.ldValsModel.append({ param: "Beh:", 		value: ld_object.beh })
					rootID.ldValsModel.append({ param: "Health:", 	value: ld_object.health })
					rootID.ldValsModel.append({ param: "", 			value: "" })
					rootID.ldValsModel.append({ param: "Sim:", 		value: ld_object.sim })
					rootID.ldValsModel.append({ param: "Blk:", 		value: ld_object.blk })
				}
			}
		}

		MouseArea {
			anchors.fill: parent
			acceptedButtons: Qt.LeftButton | Qt.RightButton
			preventStealing: true

			onClicked: function(msx) {
				if (msx.button === Qt.RightButton) {
					contextMenu.popup()
				}
				sigLeftClicked()
			}
			onDoubleClicked: function(msx) {
				sigDLeftClicked()
			}
			onPressAndHold: function(msx) {
				if (msx.source === Qt.MouseEventNotSynthesized) {
					contextMenu.popup()
				}
			}

			Menu {
				id: contextMenu

				MenuItem {
					text: "Logical nodes"
					onTriggered: {
						sigLN_Page()
					}
				}
				MenuItem {
					text: "Datasets"
					onTriggered: {
						sigDS_Page()
					}
				}
				MenuItem {
					text: "Report control blocks"
					onTriggered: {
						sigRCB_Page()
					}
				}
			}
		}
	}
}