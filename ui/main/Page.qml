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
import QtQuick.Dialogs

import "qrc:/global/"

FocusScope {
	id: root

	signal sigConnectTo(string ip, int port, bool tls, string user, string pass)
	signal sigDumpModel(string dir, string ip, int port, bool tls, string user, string pass)

	function slotSetCurrentDevice(ip, port) {
		ipAddrInput.text = ip
		portInput.text = port
	}

	// Page rectangle (screen)
	Rectangle {
		anchors.fill: parent

		color: "white" //"lightgray"

		// Folder for the model dump
		FolderDialog {
			id: dumpFolderDialog

			title: "Please, select folder for the model dump file"
			currentFolder: "~/"
			options: FolderDialog.ShowDirsOnly

			acceptLabel: "Select"
			rejectLabel: "Cansel"

			onAccepted: {
				sigDumpModel(dumpFolderDialog.selectedFolder, ipAddrInput.text, portInput.text,
							tlsSwitcher.checked, userNameInput.text, userPassInput.text)
			}
		}

		// mini-window
		Rectangle {
			id: window
			//anchors.centerIn: parent

			width: 460
			height: 250
			color: "lightgray" //"white"

			border.width: 2
			smooth: false

			MouseArea {
				anchors.fill: parent
				drag.target: window
			}

			// Label
			Text {
				text: qsTr("Connection to IED")
				x: 150
				y: 14
				width: 150

				font.pixelSize: 16
				font.bold: true

				horizontalAlignment: Text.AlignHCenter
				verticalAlignment: Text.AlignVCenter
			}

			// IP box
			Text {
				text: qsTr("IP address")
				x: 16
				y: 67
				width: 100
				height: 25

				font.pixelSize: 14
				horizontalAlignment: Text.AlignRight
				verticalAlignment: Text.AlignVCenter
			}
			// Input box for IP address
			TextField {
				id: ipAddrInput
				x: 135
				y: 67
				width: 180
				height: 25

				text: qsTr("127.0.0.1")
				font.pixelSize: 14
				verticalAlignment: Text.AlignVCenter
				leftPadding: 5
				clip: true
				focus: true

				KeyNavigation.backtab: connButton
				KeyNavigation.tab: portInput
			}

			// Port of MMS server
			Text {
				x: 16
				y: 101
				width: 100
				height: 25
				text: qsTr("MMS port")
				font.pixelSize: 14
				horizontalAlignment: Text.AlignRight
				verticalAlignment: Text.AlignVCenter
			}
			// Input box for MMS port
			TextField {
				id: portInput
				x: 135
				y: 101
				width: 180
				height: 25

				text: qsTr("102")
				font.pixelSize: 14
				verticalAlignment: Text.AlignVCenter
				leftPadding: 5
				clip: true

				KeyNavigation.backtab: ipAddrInput
				KeyNavigation.tab: tlsSwitcher
			}

			// Dump model
			Button {
				id: dumpButton
				x: 336
				y: 162
				height: 25
				width: 100

				text: qsTr("Dump")

				icon.source: "qrc:/img/icons/download.svg"

				onClicked: {
					dumpFolderDialog.open()
				}

				KeyNavigation.backtab: connButton
				KeyNavigation.tab: ipAddrInput
			}

			// Connect
			Button {
				id: connButton
				x: 336
				y: 82
				height: 25
				width: 100

				text: qsTr("Connect")

				icon.source: "qrc:/img/icons/call.svg"

				onClicked: {
					sigConnectTo(ipAddrInput.text, portInput.text,
								tlsSwitcher.checked, userNameInput.text, userPassInput.text)
				}

				KeyNavigation.backtab: userPassInput
				KeyNavigation.tab: dumpButton
			}

			// Input box user name
			TextField {
				id: userNameInput
				x: 135
				y: 147
				width: 180
				height: 25

				placeholderText: "Enter username"
				placeholderTextColor: "lightgray"

				text: ""
				font.pixelSize: 14
				verticalAlignment: Text.AlignVCenter
				leftPadding: 5
				clip: true

				KeyNavigation.backtab: tlsSwitcher
				KeyNavigation.tab: userPassInput
			}

			// Input box user password
			TextField {
				id: userPassInput

				x: 135
				y: 182
				width: 180
				height: 25

				placeholderText: "Enter password"
				placeholderTextColor: "lightgray"
				echoMode: TextField.Password

				//text: qsTr("pass")
				font.pixelSize: 14
				verticalAlignment: Text.AlignVCenter
				leftPadding: 5
				clip: true
				smooth: false

				KeyNavigation.backtab: userNameInput
				KeyNavigation.tab: connButton
			}

			// TLS switcher
			Switch {
				id: tlsSwitcher
				x: 35//334
				y: 162
				height: 30
				text: qsTr("TLS")

				KeyNavigation.backtab: portInput
				KeyNavigation.tab: userNameInput
			}
		}
	}

	Keys.onPressed: function(event) {
		console.log("StartPage: Key pressed " + event.key)
		if (event.key == Qt.Key_Return || event.key == Qt.Key_Enter) {
			connButton.onClicked()
		}
	}

	onWidthChanged: moveToCenter()
	onHeightChanged: moveToCenter()

	function moveToCenter() {
		window.x = (root.width - window.width) / 2
		window.y = (root.height - window.height) / 2
	}
}