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

import "qrc:/common/"

import GlobalVarsModule
import AppStylesModule

// LogicalDevice block in GridView
Item {
	id: root

	required property string name
	required property int index
	required property bool selected

	// Geometry
	required property int blkWidth
	required property int blkHeight
	required property int blkBorder

	readonly property int blkBottomH: 40
	readonly property int ledWH: 25

	width: blkWidth + 2 * blkBorder
	height: blkHeight + 2 * blkBorder

	signal sigLeftClicked()
	signal sigDLeftClicked()
	signal sigLN_Page()
	signal sigDS_Page()
	signal sigRCB_Page()

	Rectangle {
		anchors.fill: parent

		border.color: selected ? "black" : "gray"
		border.width: blkBorder

		Rectangle {
			id: textRect

			x: blkBorder
			y: blkBorder

			width: blkWidth
			height: blkHeight - blkBottomH - blkBorder
			color: "white"

			Text {
				id: ldName

				anchors.fill: parent
				horizontalAlignment: Text.AlignHCenter
				verticalAlignment: Text.AlignTop
				padding: 10

				width: parent.width
				wrapMode: Text.Wrap

				clip: true
				elide: Text.ElideRight

				font.bold: true
				font.pointSize: 14
				text: root.name
			}
		}
		Rectangle {
			id: separator

			x: blkBorder
			y: blkBorder + textRect.height

			width: blkWidth
			height: blkBorder

			color: "gray"
		}
		Rectangle {
			x: blkBorder
			y: blkBorder + textRect.height + separator.height

			width: blkWidth
			height: blkBottomH

			color: "white"

			RowLayout {
				anchors.fill: parent
				Layout.alignment: Qt.AlignVCenter

				RowLayout {
					Layout.alignment: Qt.AlignVCenter
					Layout.leftMargin: 10
					Layout.preferredWidth: parent.width / 3

					Led {
						id: ledMode
						width: ledWH
						height: ledWH
						color: "green"
					}
					Text {
						font.pointSize: 14
						font.bold: true
						text: "M"
					}
				}
				RowLayout {
					Layout.alignment: Qt.AlignVCenter
					Layout.leftMargin: 10
					Layout.preferredWidth: parent.width / 3

					Led {
						id: ledBeh
						width: ledWH
						height: ledWH
						color: "green"
					}
					Text {
						font.pointSize: 14
						font.bold: true
						text: "B"
					}
				}
				RowLayout {
					Layout.alignment: Qt.AlignVCenter
					Layout.leftMargin: 10
					Layout.preferredWidth: parent.width / 3

					Led {
						id: ledHealth
						width: ledWH
						height: ledWH
						color: "green"
					}
					Text {
						font.pointSize: 14
						font.bold: true
						text: "H"
					}
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