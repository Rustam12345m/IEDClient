
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
import QtQuick.Layouts
import Qt.labs.qmlmodels

import "qrc:/global/"

// LLN0 & LPHD info for LD
Item {
	property int defRowHeight: 30
	property int defTextPadding: 5

	ListView {
		id: ldPropertyList
		anchors.fill: parent

		property int selectedIndex: -1

		model: ldBackend.ldPropModel

		section.property: "section"
		section.delegate: Rectangle {
			width: ldPropertyList.width
			height: defRowHeight
			color: "#f6f6f6"

			clip: true

			Text {
				text: section
				anchors.centerIn: parent
				font.bold: true
			}
		}

		delegate: Item {
			width: ldPropertyList.width
			height: defRowHeight

			Rectangle {
				anchors.fill: parent

				RowLayout {
					anchors.fill: parent
					spacing: 0

					Rectangle {
						border.width: 1
						border.color: "lightgray"

						color: (index === ldPropertyList.selectedIndex) ? "lightgray" : "white"
						clip: true

						Layout.preferredWidth: 100//textName.paintedWidth + 10
						width: 100//textName.paintedWidth + 10
						height: defRowHeight

						Text {
							id: textName
							anchors.fill: parent

							horizontalAlignment: Text.AlignLeft//HCenter
							verticalAlignment: Text.AlignVCenter
							elide: Text.ElideRight
							leftPadding: defTextPadding
							rightPadding: defTextPadding

							text: model.name
						}
					}
					Rectangle {
						border.width: 1
						border.color: "lightgray"

						color: (index === ldPropertyList.selectedIndex) ? "lightgray" : "white"
						clip: true

						Layout.fillWidth: true
						height: defRowHeight

						Text {
							id: textValue
							anchors.fill: parent

							horizontalAlignment: Text.AlignLeft
							verticalAlignment: Text.AlignVCenter
							elide: Text.ElideRight
							leftPadding: defTextPadding
							rightPadding: defTextPadding

							text: model.value
						}
					}
				}

				MouseArea {
					anchors.fill: parent
					onClicked: {
						ldPropertyList.selectedIndex = index
					}
				}
			}
		}

		focus: true
		Keys.onUpPressed: {
			if (selectedIndex > 0)
				selectedIndex--;
		}
		Keys.onDownPressed: {
			if (selectedIndex < count - 1)
				selectedIndex++;
		}
	}
}