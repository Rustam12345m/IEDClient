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

Window
{
    title: qsTr("IEDClient - System messages")

    id: windowID

    width: 800
    height: 300
    visible: true

    Rectangle {
        id: rootID

        anchors.fill: parent

        HorizontalHeaderView {
            id: header

            anchors.left: tableID.left
            anchors.top: parent.top
            anchors.right: parent.right

            boundsBehavior: Flickable.StopAtBounds
            syncView: tableID

            delegate: Rectangle {
                implicitWidth: text.implicitWidth + 20
                implicitHeight: VisualStyle.rowHeight
                color: VisualStyle.section.bg
                border.color: VisualStyle.section.border

                Label {
                    id: text

                    anchors.centerIn: parent

                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter

                    text: model[header.textRole]
                    color: VisualStyle.section.text
                }
            }
        }

        TableView {
            id: tableID

            anchors {
                left: parent.left
                top: header.bottom
                right: parent.right
                bottom: parent.bottom
            }

            clip: true
            interactive: true
            boundsBehavior: Flickable.StopAtBounds

            model: appBackend.appLogsModel

            selectionBehavior: TableView.SelectRows
            selectionModel: ItemSelectionModel {
                model: tableID.model

                onCurrentChanged: {
                    //console.log("Select current changed: " + currentIndex)
                }
            }

            function calcGoodWidthFoColumn(col) {
                const iw = []
                let sum = 0
                for (let i=0;i<columns;i++) {
                    iw[i] = Math.max(header.implicitColumnWidth(i), implicitColumnWidth(i))
                    sum = sum + iw[i]
                }
                if (sum === 0) sum = 1
                return width * (iw[col] / sum)
            }
            function setGoodColumnsWidth() {
                const iw = []
                let sum = 0, i = 0
                for (i=0;i<columns;i++) {
                    iw[i] = Math.max(header.implicitColumnWidth(i), implicitColumnWidth(i))
                    sum = sum + iw[i]
                }
                if (sum === 0) {
                    sum = 1
                }
                for (i=0;i<columns;i++) {
                    setColumnWidth(i, width * iw[i] / sum)
                }
            }

            onWidthChanged: function() {
                setGoodColumnsWidth()
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

            delegate: Rectangle {
                required property bool selected

                implicitWidth: textArea.implicitWidth + 20
                implicitHeight: 20

                color: selected ? VisualStyle.table.selRowColor : VisualStyle.table.rowColor1
                border.color: selected ? VisualStyle.borderColor : VisualStyle.table.rowBorderColor2
                border.width: 1

                Text {
                    id: textArea
                    text: display
                    anchors.centerIn: parent
                }
                MouseArea {
                    anchors.fill: parent
                    acceptedButtons: Qt.LeftButton | Qt.RightButton

                    onClicked: function(mouse) {
                        let idx = tableID.model.index(row, 0);
                        tableID.selectionModel.setCurrentIndex(idx, ItemSelectionModel.Clear
                                                                    | ItemSelectionModel.Select
                                                                    | ItemSelectionModel.Rows);
                    }
                }
            }
        }
    }
}