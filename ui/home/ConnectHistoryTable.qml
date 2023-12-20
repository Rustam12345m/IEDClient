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
import Qt.labs.qmlmodels

import GlobalVarsModule
import AppStylesModule

import "qrc:/common/"

FocusScope
{
	id: rootID

	readonly property int defDelegateHeight: 30
	readonly property int defDelegateWidth: 150

	signal sigDeviceSelected(string t_ip, int t_port)

	function getValue(row, col) {
		let idx = tableID.model.index(row, col)
		return tableID.model.data(idx, "display")
	}

	// Header for TableView below
	TableHeader {
		id: headerID

		defDelegateWidth: rootID.defDelegateWidth
		defDelegateHeight: rootID.defDelegateHeight
		rightTextAlign: true

		anchors {
			left: parent.left
			top: parent.top
			right: parent.right
		}
	}

	// Table of last used IED
	TableView {
		id: tableID

		anchors {
			left: parent.left
			right: parent.right
			top: headerID.bottom
			bottom: parent.bottom
		}
		boundsBehavior: Flickable.StopAtBounds
		clip: true

		focus: true
		model: appBackend.lastConnList
		interactive: true

		selectionBehavior: TableView.SelectRows
		selectionModel: ItemSelectionModel {
			model: tableID.model

			/*
			onCurrentChanged: {
			}
			*/
		}

		columnWidthProvider: function(t_column) {
			return Globals.calcColumnsWidth(headerID, tableID, t_column)
		}

		delegate: TextDelegate {
			delegateHeight: defDelegateHeight
			text: model.display

			onSigClick: function(row, col) {
				Globals.setSelectedRow(tableID, row)

				let ip = rootID.getValue(row, 2)
				let port = rootID.getValue(row, 3)

				sigDeviceSelected(ip, port)
			}

			onSigDoubleClick: function(row, col) {
				//console.log("On double click: row = " + row + ", col = " + col)
			}
		}

		Keys.onPressed: function(event) {
			console.log("ConHistoryTable: " + event.key)
			event.accepted = false
		}
	}
}