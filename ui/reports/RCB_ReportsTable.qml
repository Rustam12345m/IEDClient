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

// Received reports for the particular RCB
FocusScope
{
    id: rootID

    signal sigReportDetail(var detail)

    property bool detailPanelOpen: false

    // Header for Table below
    TableHeader {
        id: headerID

        defDelegateWidth: 60
        defDelegateHeight: 30

        anchors {
            left: tableID.left
            top: parent.top
            right: parent.right
        }
    }

    // Table of received reports
    TableView {
        id: tableID

        anchors {
            left: parent.left
            top: headerID.bottom
            right: parent.right
            bottom: parent.bottom
        }

        model: iedBackend.getReportsModel()

        focus: true
        keyNavigationEnabled: true
        reuseItems: true

        interactive: true
        clip: true
        boundsBehavior: Flickable.StopAtBounds

        columnWidthProvider: function(t_column) {
            return Globals.columnWidthCalculator(headerID, tableID, t_column)
        }

        selectionBehavior: TableView.SelectRows
        selectionModel: ItemSelectionModel {
            model: tableID.model
        }

        delegate: TextDelegate {
            delegateHeight: 30
            selected: (tableID.currentRow == row)

            textAlign: Text.AlignHCenter
            text: model.display

            onSigClick: function(row, col) {
                Globals.setSelectedRow(tableID, row)
            }
            onSigDoubleClick: function(row, col) {
                Globals.setSelectedRow(tableID, row)
                detailPanelOpen = !detailPanelOpen
                if (detailPanelOpen) {
                    var detail = tableID.model.getReportDetail(row)
                    if (detail && detail.entries) {
                        sigReportDetail(detail)
                    }
                } else {
                    sigReportDetail({})
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

        onCurrentRowChanged: {
            if (currentRow >= 0 && detailPanelOpen) {
                var detail = tableID.model.getReportDetail(currentRow)
                if (detail && detail.entries) {
                    sigReportDetail(detail)
                }
            }
        }

        Keys.onPressed: function(event) {
            if (event.key === Qt.Key_C && (event.modifiers & Qt.ControlModifier)) {
                Globals.copyRowToClipboard(tableID)
                event.accepted = true
            }
        }
    }

    // Empty state placeholder
    Text {
        anchors.centerIn: parent
        text: "No reports received"
        color: VisualStyle.textColor
        font.pixelSize: 14
        visible: tableID.rows === 0
    }

    onVisibleChanged: {
        tableID.focus = visible
    }
}
