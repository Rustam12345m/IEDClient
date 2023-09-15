/*
 *  main.qml
 *
 *  Copyright 2023 Rustam Mustafin
 *
 *  This file is part of IEDMaster.
 *
 *  IEDMaster is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  IEDMaster is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with IEDMaster.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  See COPYING file for the complete license text.
 * */

import QtQuick

Item {
	required property int delegateHeight
	required property bool selected
	property alias text: textFild.text
	property alias textAlign: textFild.horizontalAlignment

	implicitWidth: textFild.implicitWidth + 10
	implicitHeight: delegateHeight

	Rectangle {
		anchors.fill: parent

		color: (selected ? "lightgray" : "white")
		border.color: (selected ? "black" : "lightgray")

		Text {
			id: textFild

			anchors.fill: parent
			horizontalAlignment: Text.AlignHCenter
			verticalAlignment: Text.AlignVCenter
			leftPadding: 5
			text: " - "
		}
	}
	MouseArea {
		anchors.fill: parent
		acceptedButtons: Qt.LeftButton | Qt.RightButton

		onClicked: function(mouse) {
			let idx = tableID.model.index(row, 0);
			tableID.selectionModel.setCurrentIndex(idx, ItemSelectionModel.Clear
														| ItemSelectionModel.Select
														| ItemSelectionModel.Rows);
		}
	}
}
