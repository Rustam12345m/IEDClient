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

Item {
	required property bool selected
	property int delegateHeight: 30
	property alias text: textFild.text

	implicitWidth: textFild.implicitWidth + 20
	implicitHeight: delegateHeight

	Rectangle {
		anchors.fill: parent

		border.width: 1
		border.color: (selected ? "black" : "lightgray")
		color: (selected ? "lightgray" : "white")

		Text {
			id: textFild

			anchors.fill: parent
			horizontalAlignment: Text.AlignHCenter
			verticalAlignment: Text.AlignVCenter
			leftPadding: 5

			text: " "
		}
	}
	MouseArea {
		anchors.fill: parent
		acceptedButtons: Qt.LeftButton | Qt.RightButton

		onClicked: function(mouse) {
			if (mouse.button === Qt.RightButton) {
				contextMenu.popup()
			}

			let idx = tableID.model.index(row, 0);
			tableID.selectionModel.setCurrentIndex(idx, ItemSelectionModel.Clear
														| ItemSelectionModel.Select
														| ItemSelectionModel.Rows);
		}
		onPressAndHold: function(mouse) {
			if (mouse.source === Qt.MouseEventNotSynthesized) {
				contextMenu.popup()
			}
		}
		onDoubleClicked: function(mouse) {
			mainPres.downloadFile(tableID.getFilename(row))
		}

		Menu {
			id: contextMenu

			MenuItem {
				text: "Download"
				onTriggered: {
					console.log(text + " " + row)
					mainPres.downloadFile(tableID.getFilename(row))
				}
			}
			MenuItem {
				text: "Remove"
				onTriggered: {
					console.log(text + " " + row)
					mainPres.removeFile(tableID.getFilename(row))
				}
			}
			MenuItem {
				enabled: false
				text: "Rename"
				onTriggered: {
					console.log(text + " " + row)
				}
			}
		}
	}
}