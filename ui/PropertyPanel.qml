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
import QtQuick.Controls
import QtQuick.Layouts

Item {
	Rectangle {
		id: root
		anchors.fill: parent
		color: "white"//"lightgrey"

		ScrollView {
			anchors.fill: parent

			ListView {
				id: listView
				width: parent.width

				model: ListModel {
					id: propertyModel

					ListElement {
						name: "Name N1"
						value: "1"
					}
					ListElement {
						name: "Name N2"
						value: "2"
					}
					ListElement {
						name: "Name N3"
						value: "3"
					}
				}

				delegate: RowLayout {
					width: listView.width
					height: childrenRect.height

					Label {
						text: model.name
						Layout.fillWidth: true
					}
					TextField {
						text: model.value
						Layout.fillWidth: true
					}
				}

			}
		}
	}
}
