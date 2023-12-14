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
	id: rootID

	property string doReference: "" 
	property alias text: msgText.text
	property alias value: valueBox.text

	signal sigResult(bool t_user, string t_ref, string t_value)

	Dialog {
		id: diaID

		anchors.centerIn: parent

		width: Math.max(400, msgText.implicitWidth + 50)
		height: 150

		modal: true
		closePolicy: Dialog.NoAutoClose
		visible: false

		ColumnLayout {
			anchors.fill: parent

			Label {
				id: msgText

				text: ""
			}

			TextField {
				id: valueBox

				width: 100
				height: 30

				verticalAlignment: Text.AlignVCenter
				focus: true

				font.pixelSize: 14

				text: qsTr("100500.100500")
			}
		}

		footer: DialogButtonBox {
			Button {
				text: "Ok"
				onClicked: {
					console.log("Ok Button Clicked!")
					diaID.close()

					sigResult(true)
				}
			}

			Button {
				text: "Cancel"
				onClicked: {
					console.log("Cancel Button Clicked!")
					diaID.close()
				}
			}
		}
	}

	function isActive() {
		return diaID.visible
	}

	function open(t_ref, t_msg, t_value) {
		rootID.doReference = t_ref
		msgText.text = t_msg
		valueBox.text = t_value
		diaID.visible = true;
	}

	function close() {
		diaID.visible = false;
	}
}