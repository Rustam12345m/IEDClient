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

// Table of all Logical Nodes for one Logical Device
Item
{
	id: rootID

	readonly property int defDelegateHeight: 30
	readonly property int defDelegateWidth: 70

	signal sigSelectedNewLN()

	// Header of LN table below
	TableHeader {
		id: headerID

		defDelegateWidth: rootID.defDelegateWidth
		defDelegateHeight: rootID.defDelegateHeight

		anchors {
			left: parent.left
			right: parent.right
			top: parent.top
		}
	}

	// Table of all LN for selected LD with status columns
	TableView {
		id: tableID

		anchors {
			left: parent.left
			right: parent.right
			top: headerID.bottom
			bottom: parent.bottom
		}
		model: iedBackend.getLN_ComModel()

		focus: true
		clip: true
		interactive: true
		boundsBehavior: Flickable.StopAtBounds

		selectionBehavior: TableView.SelectRows
		selectionModel: ItemSelectionModel {
			model: tableID.model
		}

		onCurrentRowChanged: {
			tableID.model.setSelectedLN(tableID.currentRow)
			sigSelectedNewLN()
		}

		columnWidthProvider: function(column) {
			switch (column) {
			case 0: {
				let res = width - 3 * defDelegateWidth
				return res
			}
			default: {
				let v1 = header.implicitColumnWidth(column)
				let v2 = implicitColumnWidth(column)
				let res = (v1 > v2) ? v1 : v2
				return Math.max(res, defDelegateWidth)
			}
			}
		}

		delegate: DelegateChooser {
			// Name
			DelegateChoice {
				column: 0

				delegate: TextDelegate {
					delegateHeight: defDelegateHeight
					selected: (tableID.currentRow == row)

					textAlign: Text.AlignRight// HCenter
					text: model.value

					onSigClick: function(row, col) {
						Globals.setSelectedRow(tableID, row)
					}
				}
			}
			// Mode
			DelegateChoice {
				column: 1

				delegate: ModBehHealthDelegate {
					delegateHeight: defDelegateHeight
					delegateWidth: defDelegateWidth
					selected: (tableID.currentRow == row)

					value: model.value

					onSigClick: function(row, col) {
						Globals.setSelectedRow(tableID, row)
					}
				}
			}
			// Beh
			DelegateChoice {
				column: 2

				delegate: ModBehHealthDelegate {
					delegateHeight: defDelegateHeight
					delegateWidth: defDelegateWidth
					selected: (tableID.currentRow == row)

					value: model.value

					onSigClick: function(row, col) {
						Globals.setSelectedRow(tableID, row)
					}
				}
			}
			// Health
			DelegateChoice {
				column: 3

				delegate: ModBehHealthDelegate {
					delegateHeight: defDelegateHeight
					delegateWidth: defDelegateWidth
					selected: (tableID.currentRow == row)

					value: model.value

					onSigClick: function(row, col) {
						Globals.setSelectedRow(tableID, row)
					}
				}
			}
		}

		ScrollBar.vertical: ScrollBar {
			policy: ScrollBar.AsNeeded
			active: true
			onActiveChanged: {
				if (!active) {
					active = true;
				}
			}
		}

		onVisibleChanged: {
			if (tableID.rows > 0 && tableID.currentRow < 0) {
				Globals.setSelectedRow(tableID, 0)
			}
		}
	}
}