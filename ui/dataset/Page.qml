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
import QtQuick.Layouts

import AppStylesModule

import "qrc:/common/"

// DataSets page
FocusScope
{
    id: rootID

    function resizeColumnsOnPage() {
        tableDS_Val.resizeColumnsToContent()
    }

    SplitView {
        id: splitView

        focus: true
        anchors.fill: parent

        // Delimiter
        handle: SplitDelimeter {
            height: splitView.height
            pressed: SplitHandle.pressed
        }

        // Area for TableView of all DS
        Rectangle {
            SplitView.minimumWidth: 300
            SplitView.fillWidth: false

            color: VisualStyle.input.bg

            // Table
            DS_OverviewTable {
                id: tableDS_Info

                anchors.fill: parent

                onSigLeftOrRightKey: function() {
                    tableDS_Info.focus = false
                    tableDS_Val.focus = true
                }
            }
        }

        // Area for DataSet values
        Rectangle {
            SplitView.minimumWidth: 100
            SplitView.fillWidth: true

            color: VisualStyle.input.bg

            // Table
            DS_StateTable {
                id: tableDS_Val

                anchors.fill: parent

                onSigLeftOrRightKey: function() {
                    tableDS_Info.focus = true
                    tableDS_Val.focus = false
                }
            }
        }
    }

    onVisibleChanged: {
        if (visible) {
            tableDS_Info.focus = true
        }
    }

    Keys.onPressed: function(event) {
        if (event.key == Qt.Key_Left) {
            tableDS_Info.focus = true
            tableDS_Val.focus = false
            event.accepted = true
        }
        if (event.key == Qt.Key_Right) {
            tableDS_Info.focus = false
            tableDS_Val.focus = true
            event.accepted = true
        }
    }
}