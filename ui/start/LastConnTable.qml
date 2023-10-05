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
import Qt.labs.qmlmodels

import "qrc:/global/"

Item {
	readonly property int defDelegateHeight: 30

	signal sigDeviceSelected(string t_ip, int t_port)

	HorizontalHeaderView {
		id: headerID

		anchors {
			//leftMargin: 5
			left: parent.left
			top: parent.top
			right: parent.right
		}
		boundsBehavior: Flickable.StopAtBounds
		clip: true

		syncView: tableID

		delegate: Rectangle {
			implicitWidth: textID.implicitWidth + 10
			implicitHeight: defDelegateHeight

			color: "#f6f6f6"
			border.color: "#e4e4e4"

			Label {
				id: textID

				anchors.fill: parent
				horizontalAlignment: Text.AlignHCenter
				verticalAlignment: Text.AlignVCenter

				text: model[headerID.textRole]
				color: "#ff26282a"
			}
		}
	}

	// Table of last used IED
	TableView {
		id: tableID

		anchors {
			//leftMargin: 5
			left: parent.left
			right: parent.right
			top: headerID.bottom
			bottom: parent.bottom
		}
		boundsBehavior: Flickable.StopAtBounds
		clip: true

		model: mainPres.lastConnList
		interactive: true

		selectionBehavior: TableView.SelectRows
		selectionModel: ItemSelectionModel {
			model: tableID.model

			onCurrentChanged: {
				//console.log("Select current changed: " + currentIndex)
			}
		}

		function setGoodColumnsWidth() {
			const iw = []
			let sum = 0, i = 0
			for (i=0;i<columns;i++) {
				iw[i] = Math.max(headerID.implicitColumnWidth(i), implicitColumnWidth(i))
				sum = sum + iw[i]
			}
			if (sum === 0) {
				sum = 1
			}
			for (i=0;i<columns;i++) {
				setColumnWidth(i, width * iw[i] / sum)
			}
		}

		onWidthChanged: function() {
			tableID.forceLayout()
			setGoodColumnsWidth()
		}

		function getValue(row, col) {
			let idx = tableID.model.index(row, col)
			return tableID.model.data(idx, "display")
		}

		delegate: TextDelegate {
			delegateHeight: defDelegateHeight
			text: model.display

			onSigClick: function(row, col) {
				let ip = tableID.getValue(row, 1)
				let port = tableID.getValue(row, 2)

				sigDeviceSelected(ip, port)
			}
			onSigDoubleClick: function(row, col) {
				console.log("On double click: row = " + row + ", col = " + col)
			}
		}
	}
}