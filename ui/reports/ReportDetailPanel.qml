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

// Report detail panel — styled like LD_PropertiesPanel
Item
{
    id: rootID

    property int defRowHeight: 30
    property int defTextPadding: 5
    property int defNameWidth: 100

    function showReport(detail) {
        reportModel.clear()

        var headerFields = [
            { section: "Report info",  name: "Seq #",   value: detail.seqNum !== undefined ? String(detail.seqNum) : "" },
            { section: "Report info",  name: "Time",    value: detail.timestamp || "" },
            { section: "Report info",  name: "RCB",     value: detail.rcbRef || "" },
            { section: "Report info",  name: "DataSet", value: detail.dataSetRef || "" },
            { section: "Report info",  name: "Reason",  value: detail.reason || "" }
        ]

        var maxWidth = 80
        for (var i = 0; i < headerFields.length; i++) {
            reportModel.append(headerFields[i])
            nameMetrics.text = headerFields[i].name
            var w = nameMetrics.advanceWidth + defTextPadding * 2 + 4
            if (w > maxWidth) maxWidth = w
        }

        if (detail.entries) {
            for (var j = 0; j < detail.entries.length; j++) {
                var entry = detail.entries[j]
                var val = entry.value || ""
                if (entry.reason && entry.reason.length > 0) {
                    val += " [" + entry.reason + "]"
                }
                var item = { section: "Data entries", name: entry.name || "", value: val }
                reportModel.append(item)

                nameMetrics.text = item.name
                var nw = nameMetrics.advanceWidth + defTextPadding * 2 + 4
                if (nw > maxWidth) maxWidth = nw
            }
        }

        defNameWidth = Math.min(maxWidth, rootID.width * 0.6)
    }

    function clear() {
        reportModel.clear()
    }

    TextMetrics {
        id: nameMetrics
        font.pixelSize: 13
    }

    ListView {
        id: reportListID

        anchors.fill: parent
        boundsBehavior: Flickable.StopAtBounds
        clip: true

        property int selectedIndex: -1

        model: ListModel { id: reportModel }

        section.property: "section"
        section.delegate: Rectangle {
            width: reportListID.width
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
            width: reportListID.width
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
                        color: (index === reportListID.selectedIndex)
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

                        color: (index === reportListID.selectedIndex)
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
                        reportListID.selectedIndex = index
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
