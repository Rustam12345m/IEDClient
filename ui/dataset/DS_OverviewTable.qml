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

import GlobalVarsModule
import AppStylesModule

// List of found DataSets in the IED
FocusScope
{
    id: rootID

    readonly property int defDelegateHeight: 30
    readonly property int defDelegateWidth: 60

    property int defRowHeight: 30
    property int defTextPadding: 5
    property int defCountWidth: 50

    signal sigLeftOrRightKey()
    signal sigSelectedNewDS()

    ListView {
        id: listViewID

        anchors.fill: parent

        focus: true
        interactive: true
        keyNavigationEnabled: true
        boundsBehavior: Flickable.StopAtBounds

        model: iedBackend.getDS_ComModel()

        property int selectedIndex: -1
        onSelectedIndexChanged: {
            sigSelectedNewDS()
            listViewID.model.setSelectedDS(listViewID.selectedIndex)
        }

        Connections {
            target: listViewID.model
            function onSigSelectRow(row) {
                listViewID.selectedIndex = row
                listViewID.currentIndex = row
            }
        }

        section.property: "section"
        section.delegate: Rectangle {
            width: listViewID.width
            height: defRowHeight
            color: VisualStyle.table.headerColor
            border.color: VisualStyle.table.rowBorderColor2

            clip: true

            Text {
                anchors.centerIn: parent
                font.bold: VisualStyle.boldHeaderText

                text: section
            }
        }

        delegate: Item {
            width: listViewID.width
            height: defRowHeight

            Rectangle {
                anchors.fill: parent

                RowLayout {
                    anchors.fill: parent
                    spacing: 0

                    Rectangle {
                        Layout.fillWidth: true
                        height: defRowHeight

                        border.width: 1
                        border.color: VisualStyle.table.rowBorderColor2

                        clip: true
                        // color: (index === listViewID.selectedIndex) ? "lightgray" : "white"
                        color: (index === listViewID.selectedIndex) ? VisualStyle.table.selRowColor : VisualStyle.table.rowColor1

                        Text {
                            id: textName
                            anchors.fill: parent

                            horizontalAlignment: Text.AlignLeft //HCenter
                            verticalAlignment: Text.AlignVCenter

                            elide: Text.ElideRight
                            leftPadding: defTextPadding
                            rightPadding: defTextPadding

                            text: model.name
                        }
                    }
                    Rectangle {
                        Layout.preferredWidth: defCountWidth
                        height: defRowHeight

                        border.width: 1
                        border.color: VisualStyle.table.rowBorderColor2

                        clip: true
                        // color: (index === listViewID.selectedIndex) ? "lightgray" : "white"
                        color: (index === listViewID.selectedIndex) ? VisualStyle.table.selRowColor : VisualStyle.table.rowColor1

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
                        listViewID.selectedIndex = index
                        listViewID.focus = true
                    }
                }
            }
        }

        Keys.onUpPressed: {
            if (listViewID.selectedIndex > 0) {
                listViewID.selectedIndex--;
            }
        }
        Keys.onDownPressed: {
            if (listViewID.selectedIndex < count - 1) {
                listViewID.selectedIndex++;
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
            if ((listViewID.selectedIndex < 0) && (listViewID.count > 0)) {
                listViewID.selectedIndex = 0
            }
        }
    }
}
