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
	property alias text: msgText.text

	signal sigResult(bool t_user)

	Dialog {
		id: root

		anchors.centerIn: parent

		width: Math.max(400, msgText.implicitWidth + 50)
		height: 120

		modal: true
		closePolicy: Dialog.NoAutoClose
		visible: false

		Label {
			id: msgText

			anchors.centerIn: parent

			text: ""
		}

		footer: DialogButtonBox {
			Button {
				text: "Ok"
				onClicked: {
					console.log("Ok Button Clicked!")
					root.close()

					sigResult(true)
				}
			}

			Button {
				text: "Cancel"
				onClicked: {
					console.log("Cancel Button Clicked!")
					root.close()

					sigResult(false)
				}
			}
		}
	}

	function isActive() {
		return root.visible
	}

	function open(msg) {
		msgText.text = msg
		root.visible = true;
	}

	function close() {
		root.visible = false;
	}
}