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

// Data object detail panel — styled like LD_PropertiesPanel
Item
{
    id: rootID

    property int defRowHeight: 30
    property int defTextPadding: 5
    property int defNameWidth: 100

    property string doReference: ""

    function showDetail(ref, detail) {
        doReference = ref
        detailModel.clear()
        var maxWidth = 80
        for (var i = 0; i < detail.length; i++) {
            detailModel.append(detail[i])
            nameMetrics.text = detail[i].name
            var w = nameMetrics.advanceWidth + defTextPadding * 2 + 10
            if (w > maxWidth) {
                maxWidth = w
            }
        }
        defNameWidth = Math.min(maxWidth, rootID.width * 0.6)
    }

    function clear() {
        doReference = ""
        detailModel.clear()
    }

    TextMetrics {
        id: nameMetrics
    }

    // Section header with DO reference
    Rectangle {
        id: headerID
        width: parent.width
        height: defRowHeight
        color: VisualStyle.table.headerColor
        border.color: VisualStyle.table.rowBorderColor2

        clip: true

        Text {
            anchors.centerIn: parent
            font.bold: VisualStyle.boldHeaderText
            text: rootID.doReference || qsTr("Data object detail")
        }
    }

    ListView {
        id: detailListID

        anchors {
            top: headerID.bottom
            left: parent.left
            right: parent.right
            bottom: parent.bottom
        }

        boundsBehavior: Flickable.StopAtBounds
        clip: true

        property int selectedIndex: -1

        model: ListModel { id: detailModel }

        delegate: Item {
            width: detailListID.width
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
                        color: (index === detailListID.selectedIndex)
                               ? VisualStyle.table.selRowColor
                               : VisualStyle.table.rowColor1

                        Text {
                            anchors.fill: parent

                            horizontalAlignment: Text.AlignLeft
                            verticalAlignment: Text.AlignVCenter
                            elide: Text.ElideRight
                            leftPadding: defTextPadding
                            rightPadding: defTextPadding

                            text: name
                        }
                    }
                    Rectangle {
                        border.width: 1
                        border.color: VisualStyle.table.rowBorderColor2

                        color: (index === detailListID.selectedIndex)
                               ? VisualStyle.table.selRowColor
                               : VisualStyle.table.rowColor1
                        clip: true

                        Layout.fillWidth: true
                        height: defRowHeight

                        Text {
                            anchors.fill: parent

                            horizontalAlignment: Text.AlignLeft
                            verticalAlignment: Text.AlignVCenter
                            elide: Text.ElideRight
                            leftPadding: defTextPadding
                            rightPadding: defTextPadding

                            text: value
                        }
                    }
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        detailListID.selectedIndex = index
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

        ScrollBar.vertical: ScrollBar {
            policy: ScrollBar.AsNeeded
            active: true
        }
    }
}
