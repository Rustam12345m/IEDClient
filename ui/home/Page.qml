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

Item {
    id: rootID

    signal sigConnectTo(var con)

    function slotSetCurrentDevice(ip, port) {
        ipAddrInput.text = ip
        portInput.text = port
    }

    function connectNow() {
        var con = {
            "ip":       ipAddrInput.text,
            "port":     portInput.text,
            "tls":      tlsSwitcher.checked,
            "login":    userNameInput.text,
            "password": userPassInput.text
        }
        sigConnectTo(con)
    }

    // Access a specific column from the QAbstractTableModel by row index
    function getField(row, col) {
        return appBackend.lastConnList.data(appBackend.lastConnList.index(row, col))
    }

    width:  parent.width
    height: parent.height

    // ── Root layout: left list | right controls ───────────────────────────
    RowLayout {
        anchors {
            fill:    parent
            margins: VisualStyle.borderWidth
        }
        spacing: VisualStyle.borderWidth

        // ── LEFT PANEL: Recent connections ────────────────────────────────
        Rectangle {
            Layout.fillHeight:     true
            Layout.preferredWidth: Math.min(380, parent.width * 0.42)
            Layout.minimumWidth:   240

            color:        VisualStyle.input.bg
            border.color: VisualStyle.borderColor
            border.width: VisualStyle.borderWidth

            ColumnLayout {
                anchors {
                    fill:    parent
                    margins: VisualStyle.borderWidth
                }
                spacing: 0

                // Section header
                Rectangle {
                    Layout.fillWidth: true
                    height:           26
                    color:            VisualStyle.toolBarColor

                    Text {
                        anchors.fill: parent
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        text:             "RECENT CONNECTIONS"
                        color:            VisualStyle.statusBar.textColor
                        font.pixelSize:   14
                        font.bold:        true
                        font.family:      VisualStyle.fontFamily
                    }
                }

                // Card list
                ListView {
                    id: historyListID

                    Layout.fillWidth:  true
                    Layout.fillHeight: true

                    clip:             true
                    focus:            true
                    boundsBehavior:   Flickable.StopAtBounds
                    model:            appBackend.lastConnList
                    ScrollBar.vertical: ScrollBar { policy: ScrollBar.AsNeeded }

                    delegate: Rectangle {
                        id: cardID

                        required property int index

                        width:  historyListID.width
                        height: 56

                        property bool isSelected: historyListID.currentIndex === index

                        color: isSelected ? VisualStyle.table.selRowColor
                                          : (index % 2 === 0 ? VisualStyle.table.rowColor1
                                                             : VisualStyle.table.rowColor2)

                        // Row separator
                        Rectangle {
                            anchors { left: parent.left; right: parent.right; bottom: parent.bottom }
                            height:  1
                            color:   VisualStyle.borderColor
                            opacity: 0.35
                        }

                        // Card content
                        Column {
                            anchors {
                                verticalCenter: parent.verticalCenter
                                left:           parent.left
                                right:          parent.right
                                leftMargin:     10
                                rightMargin:    8
                            }
                            spacing: 3

                            // IP : Port
                            Text {
                                width:          parent.width
                                text:           "▶  " + rootID.getField(cardID.index, 2) +
                                                " : " + rootID.getField(cardID.index, 3)
                                color:          VisualStyle.textColor
                                font.pixelSize: 13
                                font.family:    VisualStyle.fontFamily
                                elide:          Text.ElideRight
                            }

                            // IED name · last date
                            Text {
                                width:          parent.width
                                text:           "    " + rootID.getField(cardID.index, 1) +
                                                "  ·  " + rootID.getField(cardID.index, 4)
                                color:          "#666666"
                                font.pixelSize: 11
                                font.family:    VisualStyle.fontFamily
                                elide:          Text.ElideRight
                            }
                        }

                        MouseArea {
                            anchors.fill: parent

                            onClicked: function() {
                                historyListID.currentIndex = cardID.index
                                ipAddrInput.text = rootID.getField(cardID.index, 2)
                                portInput.text   = rootID.getField(cardID.index, 3)
                            }
                            onDoubleClicked: function() {
                                historyListID.currentIndex = cardID.index
                                ipAddrInput.text = rootID.getField(cardID.index, 2)
                                portInput.text   = rootID.getField(cardID.index, 3)
                                connectNow()
                            }
                        }
                    }

                    Keys.onPressed: function(event) {
                        if (event.key === Qt.Key_Delete && currentIndex >= 0) {
                            appBackend.lastConnList.removeFromHistory(currentIndex)
                            event.accepted = true
                        } else if ((event.key === Qt.Key_Return || event.key === Qt.Key_Enter)
                                   && currentIndex >= 0) {
                            ipAddrInput.text = rootID.getField(currentIndex, 2)
                            portInput.text   = rootID.getField(currentIndex, 3)
                            connectNow()
                            event.accepted = true
                        }
                    }
                }
            }
        }

        // ── RIGHT PANEL ───────────────────────────────────────────────────
        ColumnLayout {
            Layout.fillHeight: true
            Layout.fillWidth:  true

            spacing: VisualStyle.borderWidth

            // ── QUICK CONNECT ─────────────────────────────────────────────
            Rectangle {
                Layout.fillWidth:       true
                Layout.preferredHeight: quickConnectLayout.implicitHeight +
                                        VisualStyle.borderWidth * 2

                color:        VisualStyle.input.bg
                border.color: VisualStyle.borderColor
                border.width: VisualStyle.borderWidth

                ColumnLayout {
                    id: quickConnectLayout

                    anchors {
                        left:    parent.left
                        right:   parent.right
                        top:     parent.top
                        margins: VisualStyle.borderWidth
                    }
                    spacing: 0

                    Rectangle {
                        Layout.fillWidth: true
                        height:           26
                        color:            VisualStyle.toolBarColor

                        Text {
                            anchors.fill: parent
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                            text:             "QUICK CONNECT"
                            color:            VisualStyle.statusBar.textColor
                            font.pixelSize:   14
                            font.bold:        true
                            font.family:      VisualStyle.fontFamily
                        }
                    }

                    // Form rows
                    ColumnLayout {
                        Layout.fillWidth:   true
                        Layout.topMargin:   8
                        Layout.leftMargin:  4
                        Layout.rightMargin: 4
                        spacing:            6

                        readonly property int labelW:  56
                        readonly property int fieldH:  26

                        // IP
                        RowLayout {
                            Layout.fillWidth: true
                            spacing: 8

                            Text {
                                Layout.preferredWidth: parent.parent.labelW
                                Layout.alignment:      Qt.AlignVCenter | Qt.AlignRight
                                text:                  "IP"
                                color:                 VisualStyle.textColor
                                font.pixelSize:        13
                                font.family:           VisualStyle.fontFamily
                            }
                            Rectangle {
                                Layout.fillWidth: true
                                height:           parent.parent.fieldH
                                color:            VisualStyle.input.bg
                                border.color:     VisualStyle.input.border
                                border.width:     1

                                TextField {
                                    id:                   ipAddrInput
                                    anchors.fill:         parent
                                    anchors.margins:      1
                                    text:                 "127.0.0.1"
                                    placeholderText:      "IED address"
                                    placeholderTextColor: VisualStyle.input.placeholder
                                    font.pixelSize:       13
                                    font.family:          VisualStyle.fontFamily
                                    verticalAlignment:    Text.AlignVCenter
                                    leftPadding:          4
                                    focus:                true
                                    color:                VisualStyle.input.text
                                    KeyNavigation.tab:    portInput
                                    KeyNavigation.backtab: connButton
                                }
                            }
                        }

                        // Port
                        RowLayout {
                            Layout.fillWidth: true
                            spacing: 8

                            Text {
                                Layout.preferredWidth: parent.parent.labelW
                                Layout.alignment:      Qt.AlignVCenter | Qt.AlignRight
                                text:                  "Port"
                                color:                 VisualStyle.textColor
                                font.pixelSize:        13
                                font.family:           VisualStyle.fontFamily
                            }
                            Rectangle {
                                Layout.preferredWidth: 80
                                height:                parent.parent.fieldH
                                color:                 VisualStyle.input.bg
                                border.color:          VisualStyle.input.border
                                border.width:          1

                                TextField {
                                    id:                   portInput
                                    anchors.fill:         parent
                                    anchors.margins:      1
                                    text:                 "102"
                                    placeholderText:      "MMS"
                                    placeholderTextColor: VisualStyle.input.placeholder
                                    font.pixelSize:       13
                                    font.family:          VisualStyle.fontFamily
                                    verticalAlignment:    Text.AlignVCenter
                                    leftPadding:          4
                                    color:                VisualStyle.input.text
                                    KeyNavigation.tab:    tlsSwitcher
                                    KeyNavigation.backtab: ipAddrInput
                                }
                            }
                        }

                        // TLS
                        RowLayout {
                            Layout.fillWidth: true
                            spacing: 8

                            Text {
                                Layout.preferredWidth: parent.parent.labelW
                                Layout.alignment:      Qt.AlignVCenter | Qt.AlignRight
                                text:                  "TLS"
                                color:                 VisualStyle.textColor
                                font.pixelSize:        13
                                font.family:           VisualStyle.fontFamily
                            }
                            Switch {
                                id:                    tlsSwitcher
                                height:                parent.parent.fieldH
                                KeyNavigation.tab:    tlsSwitcher.checked ? userNameInput : connButton
                                KeyNavigation.backtab: portInput

                                onCheckedChanged: function() {
                                    portInput.text = tlsSwitcher.checked ? "443" : "102"
                                }
                            }
                        }

                        // Username (TLS only)
                        RowLayout {
                            Layout.fillWidth: true
                            spacing:          8
                            visible:          tlsSwitcher.checked

                            Text {
                                Layout.preferredWidth: parent.parent.labelW
                                Layout.alignment:      Qt.AlignVCenter | Qt.AlignRight
                                text:                  "User"
                                color:                 VisualStyle.textColor
                                font.pixelSize:        13
                                font.family:           VisualStyle.fontFamily
                            }
                            Rectangle {
                                Layout.fillWidth: true
                                height:           parent.parent.fieldH
                                color:            VisualStyle.input.bg
                                border.color:     VisualStyle.input.border
                                border.width:     1

                                TextField {
                                    id:                   userNameInput
                                    anchors.fill:         parent
                                    anchors.margins:      1
                                    enabled:              tlsSwitcher.checked
                                    placeholderText:      "Username"
                                    placeholderTextColor: VisualStyle.input.placeholder
                                    font.pixelSize:       13
                                    font.family:          VisualStyle.fontFamily
                                    verticalAlignment:    Text.AlignVCenter
                                    leftPadding:          4
                                    color:                VisualStyle.input.text
                                    KeyNavigation.tab:    userPassInput
                                    KeyNavigation.backtab: tlsSwitcher
                                }
                            }
                        }

                        // Password (TLS only)
                        RowLayout {
                            Layout.fillWidth: true
                            spacing:          8
                            visible:          tlsSwitcher.checked

                            Text {
                                Layout.preferredWidth: parent.parent.labelW
                                Layout.alignment:      Qt.AlignVCenter | Qt.AlignRight
                                text:                  "Pass"
                                color:                 VisualStyle.textColor
                                font.pixelSize:        13
                                font.family:           VisualStyle.fontFamily
                            }
                            Rectangle {
                                Layout.fillWidth: true
                                height:           parent.parent.fieldH
                                color:            VisualStyle.input.bg
                                border.color:     VisualStyle.input.border
                                border.width:     1

                                TextField {
                                    id:                   userPassInput
                                    anchors.fill:         parent
                                    anchors.margins:      1
                                    enabled:              tlsSwitcher.checked
                                    echoMode:             TextField.Password
                                    placeholderText:      "Password"
                                    placeholderTextColor: VisualStyle.input.placeholder
                                    font.pixelSize:       13
                                    font.family:          VisualStyle.fontFamily
                                    verticalAlignment:    Text.AlignVCenter
                                    leftPadding:          4
                                    color:                VisualStyle.input.text
                                    KeyNavigation.tab:    connButton
                                    KeyNavigation.backtab: userNameInput
                                }
                            }
                        }

                        // Connect and MMS Dump buttons
                        RowLayout {
                            Layout.fillWidth:    true
                            Layout.topMargin:    4
                            Layout.bottomMargin: 4

                            Item { Layout.fillWidth: true }

                            Button {
                                id:             connButton
                                implicitWidth:  140
                                text:           qsTr("Connect")
                                font.pixelSize: 13
                                font.bold:      VisualStyle.boldHeaderText
                                icon.source:    "qrc:/img/icons/call.svg"

                                KeyNavigation.tab:    ipAddrInput
                                KeyNavigation.backtab: tlsSwitcher.checked ? userPassInput : tlsSwitcher

                                onClicked: connectNow()

                                Keys.onPressed: function(event) {
                                    if (event.key === Qt.Key_Return || event.key === Qt.Key_Enter) {
                                        connectNow()
                                        event.accepted = true
                                    }
                                }
                            }

                            Button {
                                implicitWidth:  140
                                text:           qsTr("MMS Dump")
                                font.pixelSize: 13
                                font.bold:      VisualStyle.boldHeaderText
                                icon.source:    "qrc:/img/icons/code.svg"

                                onClicked: {
                                    iedBackend.dumpMmsModel(ipAddrInput.text,
                                                            parseInt(portInput.text) || 102)
                                }
                            }

                            Item { Layout.fillWidth: true }
                        }
                    }
                }
            }

            // ── SHORTCUTS ─────────────────────────────────────────────────
            Rectangle {
                Layout.fillWidth:       true
                Layout.fillHeight:      true

                color:        VisualStyle.input.bg
                border.color: VisualStyle.borderColor
                border.width: VisualStyle.borderWidth

                ColumnLayout {
                    id: shortcutsLayout

                    anchors {
                        left:    parent.left
                        right:   parent.right
                        top:     parent.top
                        margins: VisualStyle.borderWidth
                    }
                    spacing: 0

                    Rectangle {
                        Layout.fillWidth: true
                        height:           26
                        color:            VisualStyle.toolBarColor

                        Text {
                            anchors.fill: parent
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                            text:             "SHORTCUTS"
                            color:            VisualStyle.statusBar.textColor
                            font.pixelSize:   14
                            font.bold:        true
                            font.family:      VisualStyle.fontFamily
                        }
                    }

                    // Shortcut rows
                    Repeater {
                        model: [
                            { key: "F5",     desc: "Refresh current page" },
                            { key: "F11",    desc: "Fullscreen"           },
                            { key: "Alt+1",  desc: "Home"                 },
                            { key: "Alt+2",  desc: "Logical devices"      },
                            { key: "Alt+3",  desc: "Logical nodes"        },
                            { key: "Alt+4",  desc: "DataSets"             },
                            { key: "Alt+5",  desc: "Report blocks"        },
                            { key: "Alt+6",  desc: "Tree"                 },
                            { key: "Alt+7",  desc: "Files"                },
                            { key: "Ctrl+C", desc: "Copy selected row"    }
                        ]

                        delegate: RowLayout {
                            Layout.fillWidth: true
                            spacing:          0

                            Text {
                                Layout.preferredWidth: 68
                                Layout.topMargin:      3
                                Layout.bottomMargin:   3
                                Layout.leftMargin:     6
                                text:                  modelData.key
                                color:                 VisualStyle.borderColor
                                font.pixelSize:        12
                                font.family:           VisualStyle.fontFamily
                                font.bold:             true
                            }
                            Text {
                                Layout.fillWidth:    true
                                Layout.topMargin:    3
                                Layout.bottomMargin: 3
                                text:                modelData.desc
                                color:               VisualStyle.textColor
                                font.pixelSize:      12
                                font.family:         VisualStyle.fontFamily
                            }
                        }
                    }
                }
            }

        }
    }
}
