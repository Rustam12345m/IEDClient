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

import GlobalVarsModule
import AppStylesModule

import "qrc:/common/"

FocusScope
{
	id: rootID

	// Vertical TabBar: CB
	VerticalTabBar {
		id: cbViewTabBar

		anchors {
			left: parent.left
			top: parent.top
			bottom: parent.bottom
		}
		leftSide: true

		model: ListModel {
			ListElement { title: "Buffered" }
			ListElement { title: "Unbuffered" }
			ListElement { title: "GOOSE" }
			ListElement { title: "SV" }
		}

		onSigTabSelected: function(index) {
			console.log("Selected TAB: " + index)
		}
	}

	// Header for Table below
	TableHeader {
		id: headerID

		defDelegateWidth: 60
		defDelegateHeight: 30

		anchors {
			left: cbViewTabBar.right
			right: parent.right
			top: parent.top
		}
	}

	// Table of RCB on the IED
	TableView {
		id: tableID

		anchors {
			left: cbViewTabBar.right
			right: parent.right
			top: headerID.bottom
			bottom: parent.bottom
		}

		model: devBackend.getRCB_ComModel()

		focus: true
		reuseItems: true
		keyNavigationEnabled: true

		clip: true
		interactive: true
		boundsBehavior: Flickable.StopAtBounds

		columnWidthProvider: function(t_column) {
			return Globals.calcColumnsWidth(headerID, tableID, t_column)
		}

		selectionBehavior: TableView.SelectRows
		selectionModel: ItemSelectionModel {
			model: tableID.model
			/*
			onCurrentChanged: {
				console.log(currentIndex)
			}
			*/
		}

		delegate: TextDelegate {
			delegateHeight: 30
			selected: (tableID.currentRow == row)

			textAlign: Text.AlignHCenter
			text: model.display

			onSigClick: function(row, col) {
				Globals.setSelectedRow(tableID, row)
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

		Keys.onPressed: function(event) {
		}
	}

	onVisibleChanged: {
		tableID.focus = visible
	}
}