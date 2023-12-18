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
import QtQuick.Dialogs

import GlobalVarsModule
import AppStylesModule

import "qrc:/common/"

FocusScope
{
	id: rootID

	signal sigConnectTo(string ip, int port, bool tls, string user, string pass)
	signal sigDumpModel(string dir, string ip, int port, bool tls, string user, string pass)

	function slotSetCurrentDevice(ip, port) {
		ipAddrInput.text = ip
		portInput.text = port
	}

	// Page rectangle (screen)
	Rectangle {
		anchors.fill: parent

		color: ColorPalette.backgroundColor1

		// mini-window
		Rectangle {
			id: miniWindowID

			//anchors.centerIn: parent
			width: 800
			height: 400

			border.width: ColorPalette.borderWidth
			border.color: ColorPalette.modalColor

			// Connect properties
			Rectangle {
				id: connectToDevBarID

				anchors {
					left: parent.left
					right: parent.right
					top: parent.top
					margins: parent.border.width
				}

				height: 50
				color: ColorPalette.modalColor

				MouseArea {
					anchors.fill: parent
					drag.target: miniWindowID
				}

				RowLayout {
					anchors.fill: parent

					property int inputBoxHeight: 28

					spacing: 6
					clip: true

					// IP box
					Text {
						Layout.preferredWidth: 25
						Layout.alignment: Qt.AlignVCenter

						height: parent.height

						text: qsTr("IP")

						font.pixelSize: 14
						font.bold: true

						horizontalAlignment: Text.AlignRight
						verticalAlignment: Text.AlignVCenter
					}
					// Input box for IP address
					Rectangle {
						Layout.preferredWidth: 120
						Layout.alignment: Qt.AlignVCenter

						height: parent.inputBoxHeight
						color: "white"

						TextField {
							id: ipAddrInput

							anchors.fill: parent

							text: qsTr("127.0.0.1")
							placeholderText: "IED address"
							placeholderTextColor: "lightgray"

							font.pixelSize: 14
							verticalAlignment: Text.AlignVCenter
							leftPadding: 4

							focus: true
							color: "black"

							KeyNavigation.backtab: connButton
							KeyNavigation.tab: portInput
						}
					}

					// Port of MMS server
					Text {
						Layout.preferredWidth: 25
						Layout.alignment: Qt.AlignVCenter

						height: parent.height

						text: qsTr("Port")

						font.pixelSize: 14
						font.bold: true

						horizontalAlignment: Text.AlignRight
						verticalAlignment: Text.AlignVCenter
					}
					// Input box for MMS port
					Rectangle {
						Layout.preferredWidth: 60
						Layout.alignment: Qt.AlignVCenter

						height: parent.inputBoxHeight
						color: "white"

						TextField {
							id: portInput
							anchors.fill: parent

							text: qsTr("102")

							placeholderText: "MMS"
							placeholderTextColor: "lightgray"

							font.pixelSize: 14

							verticalAlignment: Text.AlignVCenter
							leftPadding: 4

							color: "black"

							KeyNavigation.backtab: ipAddrInput
							KeyNavigation.tab: tlsSwitcher
						}
					}

					// TLS switcher
					Rectangle {
						Layout.preferredWidth: 100
						Layout.alignment: Qt.AlignVCenter

						height: parent.inputBoxHeight
						color: "transparent"

						Switch {
							id: tlsSwitcher
							anchors.centerIn: parent

							height: parent.height
							text: qsTr("TLS")

							font.pixelSize: 14
							font.bold: true

							KeyNavigation.backtab: portInput
							KeyNavigation.tab: userNameInput

							onCheckedChanged: function() {
								if (tlsSwitcher.checked) {
									portInput.text = "443";
								} else {
									portInput.text = "102"
								}
								userNameInput.enabled = tlsSwitcher.checked
								userPassInput.enabled = tlsSwitcher.checked
							}
						}
					}

					// Input box user name
					Rectangle {
						Layout.preferredWidth: 120
						Layout.alignment: Qt.AlignVCenter

						height: parent.inputBoxHeight
						color: "white"

						TextField {
							id: userNameInput
							anchors.fill: parent

							placeholderText: "Enter username"
							placeholderTextColor: "lightgray"

							text: ""
							font.pixelSize: 14

							verticalAlignment: Text.AlignVCenter
							leftPadding: 4

							color: "black"

							KeyNavigation.backtab: tlsSwitcher
							KeyNavigation.tab: userPassInput
						}
					}

					// Input box user password
					Rectangle {
						Layout.preferredWidth: 120
						Layout.alignment: Qt.AlignVCenter

						height: parent.inputBoxHeight
						color: "white"

						TextField {
							id: userPassInput
							anchors.fill: parent

							placeholderText: "Enter password"
							placeholderTextColor: "lightgray"
							echoMode: TextField.Password

							//text: qsTr("pass")
							font.pixelSize: 14
							
							verticalAlignment: Text.AlignVCenter
							leftPadding: 4

							color: "black"

							KeyNavigation.backtab: userNameInput
							KeyNavigation.tab: connButton
						}
					}

					// Connect
					Button {
						Layout.preferredWidth: 100
						Layout.alignment: Qt.AlignVCenter

						id: connButton

						height: parent.inputBoxHeight

						text: qsTr("Connect")

						font.pixelSize: 14
						font.bold: true

						icon.source: "qrc:/img/icons/call.svg"

						onClicked: {
							sigConnectTo(ipAddrInput.text, portInput.text,
										tlsSwitcher.checked, userNameInput.text, userPassInput.text)
						}

						KeyNavigation.backtab: userPassInput
						KeyNavigation.tab: ipAddrInput
					}
				}
			}
			// History connect table
			Rectangle {
				anchors {
					left: parent.left
					right: parent.right
					top: connectToDevBarID.bottom
					bottom: parent.bottom
					margins: parent.border.width
					topMargin: 0
				}

				ConnectHistoryTable {
					anchors.fill: parent

					onSigDeviceSelected: function(t_ip, t_port) {
						ipAddrInput.text = t_ip
						portInput.text = t_port
					}
				}
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
		miniWindowID.x = (rootID.width - miniWindowID.width) / 2
		miniWindowID.y = (rootID.height - miniWindowID.height) / 2
	}
}