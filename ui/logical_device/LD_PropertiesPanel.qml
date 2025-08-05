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

import "qrc:/common/"

import GlobalVarsModule
import AppStylesModule

// LLN0 & LPHD info for LD
Item
{
    property int defRowHeight: 30
    property int defTextPadding: 5
    property int defNameWidth: 100

    ListView {
        id: ldPropertyList

        anchors.fill: parent
        boundsBehavior: Flickable.StopAtBounds

        property int selectedIndex: -1

        model: iedBackend.getLD_PropModel()

        section.property: "section"
        section.delegate: Rectangle {
            width: ldPropertyList.width
            height: defRowHeight
            color: VisualStyle.table.headerColor
            border.color: VisualStyle.table.rowBorderColor2

            clip: true

            Text {
                text: section
                anchors.centerIn: parent
                font.bold: VisualStyle.boldHeaderText
            }
        }

        delegate: Item {
            width: ldPropertyList.width
            height: defRowHeight

            Rectangle {
                anchors.fill: parent
                clip: true

                RowLayout {
                    anchors.fill: parent
                    spacing: 0

                    Rectangle {
                        Layout.preferredWidth: defNameWidth
                        width: defNameWidth
                        height: defRowHeight

                        border.width: 1
                        border.color: VisualStyle.table.rowBorderColor2
                        color: (index === ldPropertyList.selectedIndex)
                               ? VisualStyle.table.selRowColor
                               : VisualStyle.table.rowColor1

                        Text {
                            id: textName
                            anchors.fill: parent

                            horizontalAlignment: Text.AlignLeft
                            verticalAlignment: Text.AlignVCenter
                            elide: Text.ElideRight
                            leftPadding: defTextPadding
                            rightPadding: defTextPadding

                            text: model.name
                        }
                    }
                    Rectangle {
                        border.width: 1
                        border.color: VisualStyle.table.rowBorderColor2

                        color: (index === ldPropertyList.selectedIndex)
                               ? VisualStyle.table.selRowColor
                               : VisualStyle.table.rowColor1
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
