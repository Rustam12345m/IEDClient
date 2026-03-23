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

// Custom TabBar
Rectangle
{
    id: rootID

    color: VisualStyle.toolBarColor
    focus: false

    property bool leftSide: false
    property bool horizontalBar: false
    property alias model: listViewID.model
    property alias currentIndex: listViewID.currentIndex
    property alias spacing: listViewID.spacing
    property int cellWidth: 60
    property int cellHeight: 120

    property color selectedColor: VisualStyle.tabBar.selColor
    property color unselectedColor: VisualStyle.tabBar.unselColor
    property bool showOnlyFirst: false
    property var disabledIndexes: []

    signal sigTabSelected(int index)

    implicitHeight: rootID.horizontalBar ? rootID.height : (listViewID.count * rootID.cellHeight)
    implicitWidth: rootID.horizontalBar ? (listViewID.count * rootID.cellWidth) : rootID.width

    ListView {
        id: listViewID

        anchors {
            fill: parent
        }
        boundsBehavior: Flickable.StopAtBounds

        // model: ListModel {
        //     ListElement { title: "Test 1" }
        //     ListElement { title: "Test 2" }
        // }

        focus: false
        spacing: 0
        orientation: rootID.horizontalBar ? ListView.Horizontal : ListView.Vertical

        delegate: Item {
            id: btnItemID
            required property int index
            required property string title
            property bool selected: (listViewID.currentIndex === index)
            property bool tabVisible: index === 0 || !rootID.showOnlyFirst
            property bool disabled: rootID.disabledIndexes.indexOf(index) >= 0

            visible: tabVisible
            implicitHeight: tabVisible ? (rootID.horizontalBar ? rootID.height : rootID.cellHeight) : 0
            implicitWidth: tabVisible ? (rootID.horizontalBar ? rootID.cellWidth : rootID.width) : 0

            Rectangle {
                id: btnRectID
                width: parent.width
                height: parent.height

                border.width: 0
                border.color: VisualStyle.borderColor
                color: btnItemID.selected ? rootID.selectedColor
                       : (btnItemID.disabled ? VisualStyle.borderColor : rootID.unselectedColor)

                Rectangle {
                    id: hiddenRectID
                    anchors.fill: parent

                    border.width: 0
                    border.color: VisualStyle.borderColor
                    color: VisualStyle.tabBar.hoverColor
                    visible: false
                }

                Text {
                    anchors {
                        centerIn: parent
                    }
                    rotation: rootID.horizontalBar ? 0 : (rootID.leftSide ? -90 : 90)

                    text: title
                    focus: false
                    color: btnItemID.selected ? VisualStyle.tabBar.selTextColor
                           : (hiddenRectID.visible ? VisualStyle.tabBar.hoverTextColor
                                                   : VisualStyle.tabBar.unselTextColor)
                    font.bold: VisualStyle.boldHeaderText
                }
                MouseArea {
                    anchors.fill: parent
                    acceptedButtons: Qt.LeftButton

                    /*
                    hoverEnabled: true
                    onEntered: {
                        hiddenRectID.visible = !btnItemID.selected
                    }

                    onExited: {
                        hiddenRectID.visible = false
                    }
                    */

                    onClicked: function(mouse) {
                        if (btnItemID.disabled) {
                            mouse.accepted = true
                            return
                        }
                        hiddenRectID.visible = false
                        listViewID.currentIndex = index
                        sigTabSelected(index)

                        mouse.accepted = true
                    }
                }
            }
        }
    }
}
