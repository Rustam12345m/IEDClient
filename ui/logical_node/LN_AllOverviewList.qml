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

import "qrc:/common/"

// Flat list of all LNs across all LDs, with LD section headers
FocusScope
{
    id: rootID

    readonly property int defRowHeight: 30
    readonly property int defStatusWidth: 70
    readonly property int defTextPadding: 5

    signal sigSelectedNewLN()

    ListView {
        id: listView

        anchors.fill: parent
        boundsBehavior: Flickable.StopAtBounds
        clip: true
        focus: true

        model: iedBackend.getLN_AllModel()

        currentIndex: -1

        Connections {
            target: listView.model
            function onSigSelectRow(row) {
                listView.currentIndex = row
            }
        }

        onCurrentIndexChanged: {
            if (currentIndex >= 0) {
                model.selectLN(currentIndex)
                rootID.sigSelectedNewLN()
            }
        }

        section.property: "section"
        section.delegate: Rectangle {
            width: listView.width
            height: defRowHeight
            color: VisualStyle.table.headerColor
            border.color: VisualStyle.table.rowBorderColor2

            clip: true

            Text {
                text: section
                anchors.centerIn: parent
                font.bold: VisualStyle.boldHeaderText
                color: VisualStyle.table.headerTextColor
            }
        }

        delegate: Rectangle {
            id: delegateRoot

            width: listView.width
            height: defRowHeight
            color: (listView.currentIndex === index)
                   ? VisualStyle.table.selRowColor
                   : VisualStyle.table.rowColor1
            border.color: VisualStyle.table.rowBorderColor2

            RowLayout {
                anchors.fill: parent
                spacing: 0

                // LN Name
                Rectangle {
                    Layout.fillWidth: true
                    height: defRowHeight
                    color: "transparent"

                    Text {
                        anchors.fill: parent
                        horizontalAlignment: Text.AlignRight
                        verticalAlignment: Text.AlignVCenter
                        elide: Text.ElideRight
                        leftPadding: defTextPadding
                        rightPadding: defTextPadding
                        text: model.name
                        color: VisualStyle.textColor
                    }
                }

                // Mod
                Rectangle {
                    Layout.preferredWidth: defStatusWidth
                    height: defRowHeight
                    color: "transparent"
                    border.color: VisualStyle.table.rowBorderColor2

                    Row {
                        anchors.centerIn: parent
                        spacing: 5

                        Rectangle {
                            anchors.verticalCenter: parent.verticalCenter
                            width: 12; height: 12; radius: 6
                            color: model.mod ? model.mod.color : "gray"
                        }
                        Text {
                            anchors.verticalCenter: parent.verticalCenter
                            text: model.mod ? model.mod.text : "?"
                            color: VisualStyle.textColor
                        }
                    }
                }

                // Beh
                Rectangle {
                    Layout.preferredWidth: defStatusWidth
                    height: defRowHeight
                    color: "transparent"
                    border.color: VisualStyle.table.rowBorderColor2

                    Row {
                        anchors.centerIn: parent
                        spacing: 5

                        Rectangle {
                            anchors.verticalCenter: parent.verticalCenter
                            width: 12; height: 12; radius: 6
                            color: model.beh ? model.beh.color : "gray"
                        }
                        Text {
                            anchors.verticalCenter: parent.verticalCenter
                            text: model.beh ? model.beh.text : "?"
                            color: VisualStyle.textColor
                        }
                    }
                }

                // Health
                Rectangle {
                    Layout.preferredWidth: defStatusWidth
                    height: defRowHeight
                    color: "transparent"
                    border.color: VisualStyle.table.rowBorderColor2

                    Row {
                        anchors.centerIn: parent
                        spacing: 5

                        Rectangle {
                            anchors.verticalCenter: parent.verticalCenter
                            width: 12; height: 12; radius: 6
                            color: model.health ? model.health.color : "gray"
                        }
                        Text {
                            anchors.verticalCenter: parent.verticalCenter
                            text: model.health ? model.health.text : "?"
                            color: VisualStyle.textColor
                        }
                    }
                }
            }

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    listView.currentIndex = index
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

        Keys.onPressed: function(event) {
            if (event.key === Qt.Key_C && (event.modifiers & Qt.ControlModifier)) {
                event.accepted = true
            }
        }
    }
}
