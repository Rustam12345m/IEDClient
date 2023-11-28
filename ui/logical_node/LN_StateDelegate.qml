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

// Special delegate for Mod-Beh-Health cells in LN_Table
Item {
	id: root

	required property int delegateHeight
	required property int delegateWidth
	required property bool selected
	required property int value

	// property alias text: textFild.text

	implicitHeight: delegateHeight
	implicitWidth: delegateWidth

	signal sigClick(int row, int col)
	signal sigDoubleClick(int row, int col)

	Rectangle {
		anchors.fill: parent

		color: {
			switch (root.value) {
			case 0: return "red";
			case 1: return "green";
			case 2: return "yellow";
			case 3: return "blue";
			case 4: return "darkred";
			}
			return "lightgray";
		}
		border.color: (selected ? "black" : "lightgray")
		//clip: true

		Text {
			id: textFild

			anchors.centerIn: parent
			horizontalAlignment: Text.AlignHCenter
			verticalAlignment: Text.AlignVCenter

			text: root.value
		}
	}

	MouseArea {
		anchors.fill: parent
		acceptedButtons: Qt.LeftButton | Qt.RightButton

		onClicked: function(mouse) {
			sigClick(row, 1)
		}
		onDoubleClicked: function(mouse) {
			sigDoubleClick(row, 1)
		}
	}
}