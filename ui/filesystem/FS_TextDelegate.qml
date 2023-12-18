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

import AppStylesModule

Item
{
	required property bool selected
	property int delegateHeight: 30
	property alias text: textFild.text

	signal sigSelectRow(int t_row)
	signal sigDownloadFile(int row)

	implicitWidth: textFild.implicitWidth + 20
	implicitHeight: delegateHeight

	Rectangle {
		anchors.fill: parent

		border.width: 1
		// border.color: (selected ? "black" : "lightgray")
		// color: (selected ? "lightgray" : "white")
		border.color: ColorPalette.tableRowBorderColor2
		color: (selected ? ColorPalette.tableRowColor1 : ColorPalette.tableRowColor2)

		Text {
			id: textFild

			anchors.fill: parent
			
			// horizontalAlignment: Text.AlignHCenter
			horizontalAlignment: Text.AlignRight
			verticalAlignment: Text.AlignVCenter
			
			leftPadding: 10
			rightPadding: 10

			text: " "
		}
	}
	MouseArea {
		anchors.fill: parent
		acceptedButtons: Qt.LeftButton | Qt.RightButton

		onClicked: function(mouse) {
			sigSelectRow(row)
		}
		onDoubleClicked: function(mouse) {
			sigDownloadFile(row)
		}
	}
}