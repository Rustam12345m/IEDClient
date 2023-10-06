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

	signal sigRemoveFile(int row)
	signal sigDownloadFile(int row)

	id: root
	implicitWidth: downloadBtn.width + progressBar.width + removeBtn.width + 5

	Rectangle {
		anchors.fill: parent

		border.width: 1
		border.color: (selected ? "black" : "lightgray")
		color: (selected ? "lightgray" : "white")

		Row {
			anchors {
				leftMargin: 5
				rightMargin: 5
				fill: parent
			}

			Button {
				id: downloadBtn

				width: root.height
				height: root.height

				icon.source: "qrc:/img/icons/download.svg"

				onClicked: function() {
					sigDownloadFile(row)
				}
			}

			ProgressBar {
				id: progressBar

				width: 120
				height: root.height

				from: 0.0
				to: 100.0
				value: 0.0

				Text {
					anchors.centerIn: parent
					text: progressBar.value + " %"

					z: 1
				}
			}

			Button {
				id: removeBtn

				width: root.height
				height: root.height

				icon.source: "qrc:/img/icons/clear.svg"

				onClicked: function() {
					sigRemoveFile(row)
				}
			}
		}
	}
}