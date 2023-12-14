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

Item {
	Popup {
		id: progressOverlay

		anchors.centerIn: parent

		width: Math.max(600, progressText.implicitWidth + 50)
		height: 150

		modal: true
		closePolicy: Dialog.NoAutoClose
		visible: false

		Rectangle {
			color: "lightgray"
			anchors.fill: parent

			ColumnLayout {
				anchors.fill: parent

				Text {
					Layout.alignment: Qt.AlignCenter
					Layout.fillWidth: true
					horizontalAlignment: Text.AlignHCenter

					id: progressText
					font.bold: true
					font.pixelSize: 16

					color: "black"
					text: "Please, wait..."
				}
				ProgressBar {
					Layout.alignment: Qt.AlignBottom
					Layout.fillWidth: true

					id: progressBar
					width: 100
					height: 30

					from: 0.0
					to: 100.0
					value: 0.0

					Text {
						id: progressValue

						anchors.centerIn: parent
						z: 1

						text: ""
						color: "black"
					}
				}
			}
		}
	}

	function isActive() {
		return progressOverlay.visible
	}

	function startLoad() {
		//console.log("ModalProgressBar: startLoad")

		progressBar.value = 0
		progressOverlay.visible = true;
		parent.enabled = false;
	}

	function updateLoad(t_perc, t_msg) {
		progressBar.value = t_perc
		progressValue.text = t_perc + " %"

		progressText.text = t_msg
	}

	function finishLoad() {
		//console.log("ModalProgressBar: finishLoad")

		progressOverlay.visible = false;
		parent.enabled = true;
	}
}