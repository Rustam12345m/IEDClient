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

import "qrc:/common/"

import GlobalVarsModule
import AppStylesModule

// Special delegate for Mod-Beh-Health cells in LN_Table
Item
{
	id: rootID

	required property int delegateHeight
	required property int delegateWidth
	required property bool selected
	required property int value

	property alias prompt: toolTip.text
	property alias text: textFild.text

	implicitHeight: delegateHeight
	implicitWidth: delegateWidth

	signal sigClick(int row, int col)
	signal sigDoubleClick(int row, int col)

	Rectangle {
		anchors.fill: parent

		clip: true
		// border.color: (selected ? "black" : "lightgray")
		border.color: ColorPalette.tableRowBorderColor2
		color: (selected ? ColorPalette.tableRowColor1 : ColorPalette.tableRowColor2)

		Rectangle {
			id: led

			anchors.centerIn: parent

			color: {
				switch (rootID.value) {
				case 0: return "red";
				case 1: return "green";
				case 2: return "yellow";
				case 3: return "blue";
				case 4: return "darkred";
				}
				return "lightgray";
			}

			height: delegateHeight - 4
			width: height
			radius: height/2

			Text {
				id: textFild

				anchors.centerIn: parent
				horizontalAlignment: Text.AlignHCenter
				verticalAlignment: Text.AlignVCenter

				text: rootID.value
			}
		}
		ToolTip {
			id: toolTip
			text: ""//Information isn't found"
			delay: 200
			visible: mouseArea.containsMouse && (text != "")
		}
	}

	MouseArea {
		id: mouseArea

		anchors.fill: parent
		acceptedButtons: Qt.LeftButton | Qt.RightButton
		//hoverEnabled: true

		onClicked: function(mouse) {
			sigClick(row, 1)
		}
		onDoubleClicked: function(mouse) {
			sigDoubleClick(row, 1)
		}
	}
}