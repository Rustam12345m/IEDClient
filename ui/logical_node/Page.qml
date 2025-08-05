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

// LN page
FocusScope
{
    id: rootID

    function resizeColumnsOnPage() {
        console.log("LN_Page: resizeColumnsOnPage")
        tableDO.resizeColumnsToContent()
    }

    // Window for setting a new value for DA/SDA
    DiaChangeValue {
        id: diaChangeValue

        anchors.centerIn: parent
    }

    // LN CommonTable | Delimiter | DataTable
    SplitView {
        id: splitView

        anchors.fill: parent

        // Delimiter
        handle: SplitDelimeter {
            height: splitView.height
            pressed: SplitHandle.pressed
        }

        // Area for LogicalNode's table
        Rectangle {
            SplitView.preferredWidth: 300
            SplitView.fillWidth: false

            color: VisualStyle.backgroundColor2

            // Table
            LN_OverviewTable {
                id: tableLN

                anchors {
                    fill: parent
                }

                onSigSelectedNewLN: function() {
                    // console.log("LN_Page: Resize columns")
                    // Qt.callLater(rootID.resizeColumnsOnPage)
                }
            }

            onWidthChanged: {
                if (width < 80) {
                    width = 0
                }
            }
        }

        // Area for DataObject's table
        Rectangle {
            SplitView.minimumWidth: 200
            SplitView.fillWidth: true

            color: VisualStyle.backgroundColor1

            // Different view pages for the LN
            StackLayout {
                id: lnSignalsStack

                anchors {
                    top: parent.top
                    bottom: parent.bottom
                    left: parent.left
                    // right: parent.right
                    // bottom: rectLnViewTabBar.top
                    right: rectLnViewTabBar.left
                }

                // Table DO signals
                LN_StateView {
                    id: tableDO
                }

                LN_ControlsView {
                    id: lnControlsView
                }

                LN_SettingsView {
                    id: lnSettingsView
                }

                // Tree DO signals
                LN_CommonTree {
                    id: treeDO

                    onSigValueClicked: function(t_ref, t_msg, t_value) {
                        diaChangeValue.open(t_ref, t_msg, t_value)
                    }
                }
            }

            // TabBar for lnSignalsStack
            Rectangle {
                id: rectLnViewTabBar

                anchors {
                    top: parent.top
                    bottom: parent.bottom
                    right: parent.right
                }
                width: 30

                CustomTabBar {
                    id: lnTabBar

                    anchors.fill: parent

                    cellWidth: 30
                    color: VisualStyle.toolBarColor

                    //selectedColor: "white"
                    //unselectedColor: VisualStyle.toolBarColor

                    model: ListModel {
                        ListElement { title: "State" }
                        ListElement { title: "Controls" }
                        ListElement { title: "Settings" }
                        ListElement { title: "Tree" }
                    }

                    onSigTabSelected: function(index) {
                        lnSignalsStack.currentIndex = index
                    }
                }
            }
        }

        // Update data
        Timer {
            id: lnPageTimer

            interval: 1000
            running: false
            repeat: true

            onTriggered: function() {
            }
        }
    }

    onVisibleChanged: {
        if (visible) {
            tableLN.focus = true
            tableDO.focus = false

            lnPageTimer.start()
        } else {
            tableLN.focus = false
            tableDO.focus = false

            lnPageTimer.stop()
        }
    }
}
