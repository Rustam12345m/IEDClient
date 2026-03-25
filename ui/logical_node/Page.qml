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

    signal sigLNSelectionChanged()

    property int modelVer: 0

    function resizeColumnsOnPage() {
        tableDO.resizeColumnsToContent()
    }

    // LN CommonTable | Delimiter | DataTable
    SplitView {
        id: splitView

        anchors.fill: parent

        handle: Rectangle {
            implicitWidth: VisualStyle.delimeterWidth
            height: splitView.height
            color: SplitHandle.pressed ? VisualStyle.delimiterPressedColor : VisualStyle.borderColor

            Rectangle {
                anchors.centerIn: parent
                width: 2
                height: 40
                radius: 1
                color: SplitHandle.pressed ? "white" : VisualStyle.delimiterPressedColor
            }
        }

        // Area for LogicalNode's list (all LDs)
        Rectangle {
            SplitView.preferredWidth: 300
            SplitView.fillWidth: false

            color: VisualStyle.input.bg

            LN_AllOverviewList {
                id: tableLN

                anchors.fill: parent

                onSigSelectedNewLN: function() {
                    rootID.sigLNSelectionChanged()
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

            color: VisualStyle.input.bg

            // Different view pages for the LN
            StackLayout {
                id: lnSignalsStack

                anchors {
                    top: parent.top
                    bottom: parent.bottom
                    left: parent.left
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
                    spacing: 2
                    color: VisualStyle.toolBarColor

                    model: ListModel {
                        ListElement { title: "STATE" }
                        ListElement { title: "CONTROLS" }
                        ListElement { title: "SETTINGS" }
                        ListElement { title: "TREE" }
                    }

                    disabledIndexes: {
                        rootID.modelVer
                        var arr = []
                        if (iedBackend.getLN_ControlsModel().rowCount() === 0) arr.push(1)
                        if (iedBackend.getLN_SettingsModel().rowCount() === 0 &&
                            iedBackend.getLD_SettingsModel().rowCount() === 0) arr.push(2)
                        return arr
                    }

                    Connections {
                        target: iedBackend.getLN_ControlsModel()
                        function onModelReset() { rootID.modelVer++ }
                        function onRowsInserted() { rootID.modelVer++ }
                        function onRowsRemoved() { rootID.modelVer++ }
                    }
                    Connections {
                        target: iedBackend.getLN_SettingsModel()
                        function onModelReset() { rootID.modelVer++ }
                        function onRowsInserted() { rootID.modelVer++ }
                        function onRowsRemoved() { rootID.modelVer++ }
                    }
                    Connections {
                        target: iedBackend.getLD_SettingsModel()
                        function onModelReset() { rootID.modelVer++ }
                        function onRowsInserted() { rootID.modelVer++ }
                        function onRowsRemoved() { rootID.modelVer++ }
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
