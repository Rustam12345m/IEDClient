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

// Header for TableView
HorizontalHeaderView
{
    id: headerID

    property int sortOrder: 0
    property int sortedColumn: 0
    property bool rightTextAlign: false

    boundsBehavior: Flickable.StopAtBounds
    // resizableColumns: true

    focus: false
    clip: true
    syncView: tableID

    delegate: Rectangle {
        property int column: model.column
        property bool sortable: model.display.sortable

        implicitWidth: labelID.implicitWidth + 24 + 24
        // implicitWidth: rowID.implicitWidth + 10
        implicitHeight: 30

        border.color: VisualStyle.table.rowBorderColor2
        color: VisualStyle.table.headerColor
        clip: true

        Row {
            id: rowID

            anchors.fill: parent
            spacing: 8

            layoutDirection: Qt.RightToLeft
            rightPadding: 8
            leftPadding: 8
            focus: false

            Text {
                id: labelID

                anchors.verticalCenter: parent.verticalCenter

                horizontalAlignment: rightTextAlign ? Text.AlignRight : Text.AlignHCenter
                color: VisualStyle.table.headerTextColor
                font.bold: VisualStyle.boldHeaderText
                focus: false

                text: model.display.text
            }
            Image {
                id: imgID

                anchors.verticalCenter: parent.verticalCenter

                visible: (headerID.sortedColumn == column)
                source: (headerID.sortOrder == 0) ? "qrc:/img/icons/keyboard_arrow_down.svg"
                                                    : "qrc:/img/icons/keyboard_arrow_up.svg"

                width: 24
                height: 24
                focus: false
            }
        }
        MouseArea {
            anchors {
                fill: parent
                leftMargin: 5
                rightMargin: 5
            }

            onClicked: function(mouse) {
                mouse.accepted = true

                if (sortable) {
                    if (headerID.sortedColumn != column) {
                        headerID.sortedColumn = column
                        headerID.sortOrder = 0
                    }

                    if (headerID.sortOrder == 0) {
                        headerID.sortOrder = 1
                    } else {
                        headerID.sortOrder = 0
                    }
                    tableID.model.sort(parent.column, headerID.sortOrder)
                }
            }
        }
    }
}
