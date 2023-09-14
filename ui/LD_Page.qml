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
import QtQuick.Layouts
import QtQuick.Controls

// LD page
Item {
	readonly property int blkSpace: 30
	readonly property int blkBorder: 4
	readonly property int blkWidth: 240
	readonly property int blkHeight: 200
	readonly property int blkBottomH: 40
	readonly property int ledRadius: 20

	signal sigNextPageSignal(int page)
	signal sigLDeviceChanged(int current)

	Component {
		id: lDeviceBlock

		Rectangle {
			required property var name
			required property int index

			width: blkWidth
			height: blkHeight

			border.color: GridView.isCurrentItem ? "black" : "gray"
			border.width: blkBorder

			Rectangle {
				id: textRect

				x: parent.border.width
				y: parent.border.width

				width: blkWidth - 2 * parent.border.width
				height: blkHeight - blkBottomH - parent.border.width
				color: "white"

				Text {
					id: ldName
					//anchors.centerIn: parent
					anchors.fill: parent
					horizontalAlignment: Text.AlignHCenter
					verticalAlignment: Text.AlignVCenter
					padding: 10

					width: parent.width
					wrapMode: Text.WordWrap

					font.bold: true
					font.pointSize: 16
					text: name
				}
			}
			Rectangle {
				id: separator

				x: parent.border.width
				y: textRect.height - parent.border.width

				width: blkWidth - 2 * parent.border.width
				height: parent.border.width

				color: "gray"
			}
			Rectangle {
				x: parent.border.width
				y: textRect.height + separator.height - parent.border.width

				width: blkWidth - 2 * parent.border.width
				height: blkBottomH

				color: "white"

				RowLayout {
					anchors.fill: parent
					Layout.alignment: Qt.AlignVCenter

					RowLayout {
						Layout.alignment: Qt.AlignVCenter
						Layout.leftMargin: 10
						Layout.preferredWidth: parent.width / 3

						Rectangle {
							id: ledMode
							width: ledRadius
							height: ledRadius
							radius: ledRadius

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

						Rectangle {
							id: ledBeh
							width: ledRadius
							height: ledRadius
							radius: ledRadius

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

						Rectangle {
							id: ledHealth
							width: ledRadius
							height: ledRadius
							radius: ledRadius

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

				onClicked: function(msx) {
					if (msx.button === Qt.RightButton) {
						contextMenu.popup()
					}

					gridView.focus = true
					gridView.currentIndex = index

					sigLDeviceChanged(gridView.currentIndex);
				}
				onDoubleClicked: function(msx) {
					if (gridView.currentIndex === index) {
						//gridView.currentIndex = -1
					} else {
						gridView.currentIndex = index
					}
					//console.log("Double click on: " + index)

					sigLDeviceChanged(gridView.currentIndex);
					sigNextPageSignal(Enum.Page.LN)
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
							sigNextPageSignal(Enum.Page.LN)
						}
					}
					MenuItem {
						text: "Datasets"
						onTriggered: {
							sigNextPageSignal(Enum.Page.DS)
						}
					}
					MenuItem {
						text: "Report control blocks"
						onTriggered: {
							sigNextPageSignal(Enum.Page.RCB)
						}
					}
				}
			}
		}
	}

	GridView {
		id: gridView
		anchors.fill: parent
		anchors.margins: blkSpace

		cellWidth: blkWidth + 2*blkBorder + blkSpace
		cellHeight: blkHeight + 3*blkBorder + blkSpace

		focus: true
		keyNavigationEnabled: true

		model: mainPres.ldModel

		delegate: lDeviceBlock

		onCurrentIndexChanged: function() {
			//sigLDeviceChanged(gridView.currentIndex);
		}
	}
}
