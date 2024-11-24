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
import AppStylesModule

Item {
    required property int delegateHeight
    required property bool selected

    property alias text: textFild.text
    property alias textAlign: textFild.horizontalAlignment

    implicitWidth: textFild.implicitWidth + 10
    implicitHeight: delegateHeight

    signal sigClick(int row, int col)
    signal sigDoubleClick(int row, int col)

    Rectangle {
        anchors.fill: parent

        border.color: VisualStyle.table.rowBorderColor2
        color: (selected ? VisualStyle.table.selRowColor : VisualStyle.table.rowColor1)

        Text {
            id: textFild

            anchors.fill: parent

            horizontalAlignment: Text.AlignRight
            verticalAlignment: Text.AlignVCenter
            rightPadding: 8
            leftPadding: 8

            elide: Text.ElideRight
            text: " - "
        }
    }
    MouseArea {
        anchors.fill: parent
        acceptedButtons: Qt.LeftButton | Qt.RightButton

        onClicked: function(mouse) {
            sigClick(row, 1)
        }
        onDoubleClicked: function(mouse) {
            sigDoubleClick(row, 1)
        }
    }
}
