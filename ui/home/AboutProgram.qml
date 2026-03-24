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

Window
{
    id: rootID

    title: qsTr("About this application")
    minimumWidth: 630
    minimumHeight: infoColumnID.implicitHeight + 20

    modality: Qt.ApplicationModal
    flags: Qt.Dialog

    property ListModel appInfoListModel: ListModel {}

    ColumnLayout {
        id: infoColumnID

        anchors {
            fill: parent
            margins: 10
        }

        spacing: 10

        Item {
            Layout.preferredWidth: parent.width
            Layout.preferredHeight: 30

            TextEdit {
                anchors.centerIn: parent

                focus: false
                textFormat: Text.RichText
                readOnly: true
                cursorVisible: false
                selectByMouse: true

                text: qsTr("IEDClient - An Open-Source Client for IEC 61850 Protocols")
                padding: 5
                font.bold: true
                font.pixelSize: VisualStyle.fontSizeMedium
            }
        }
        Rectangle {
            Layout.preferredWidth: parent.width - 4
            Layout.preferredHeight: 1
            color: "white"
        }
        Item {
            Layout.preferredWidth: parent.width
            Layout.preferredHeight: 180

            ListView {
                id: infoListID

                anchors.fill: parent

                clip: true
                focus: false
                boundsBehavior: Flickable.StopAtBounds

                model: rootID.appInfoListModel

                delegate: Rectangle {
                    width: parent.width
                    height: 30
                    color: "white"

                    RowLayout {
                        anchors.fill: parent

                        TextEdit {
                            Layout.preferredWidth: 120

                            textFormat: Text.RichText
                            readOnly: true
                            cursorVisible: false
                            selectByMouse: true

                            text: name
                            padding: 5
                            font.pixelSize: VisualStyle.fontSizeSmall

                            horizontalAlignment: TextEdit.AlignRight
                        }
                        TextEdit {
                            Layout.preferredWidth: 100
                            Layout.alignment: Qt.AlignLeft

                            textFormat: Text.RichText
                            readOnly: true
                            cursorVisible: false
                            selectByMouse: true

                            text: desc
                            padding: 5
                            font.pixelSize: VisualStyle.fontSizeSmall
                            font.bold: true

                            horizontalAlignment: TextEdit.AlignLeft    
                        }
                        TextEdit {
                            Layout.preferredWidth: 100
                            Layout.fillWidth: true

                            focus: true
                            textFormat: Text.RichText
                            readOnly: true
                            cursorVisible: false
                            selectByMouse: false

                            text: "<a href='" + web + "'>" + web + "</a>"
                            padding: 5
                            font.pixelSize: VisualStyle.fontSizeSmall

                            horizontalAlignment: TextEdit.AlignLeft    

                            onLinkActivated: {
                                Qt.openUrlExternally(web)
                            }
                        }
                    }
                }
            }
        }
        Item {
            Layout.preferredWidth: parent.width
            Layout.preferredHeight: closeButtonID.implicitHeight

            Button {
                id: closeButtonID

                anchors {
                    right: parent.right
                    bottom: parent.bottom
                }
                focus: true

                text: "Ok"

                onClicked: {
                    close()
                }
            }
        }

        Keys.onPressed: function(event) {
            if (event.key == Qt.Key_Return || event.key == Qt.Key_Enter || event.key == Qt.Key_Escape) {
                rootID.close()
            }
        }
    }

    Component.onCompleted: {
        rootID.appInfoListModel.append({ name: "Author:",         desc: "Rustam Mustafin",          web: "" })
        rootID.appInfoListModel.append({ name: "Application:",    desc: appBackend.getAppVersion(), web: "https://github.com/Rustam12345m/IEDClient" })
        rootID.appInfoListModel.append({ name: "License:",        desc: "GPL-3.0",                  web: "https://www.gnu.org/licenses/gpl-3.0.html" })
        rootID.appInfoListModel.append({ name: "Qt library:",     desc: appBackend.getQtVersion(),  web: "https://www.qt.io/" })
        rootID.appInfoListModel.append({ name: "libiec61850:",    desc: appBackend.getLibVersion(), web: "https://github.com/mz-automation/libiec61850" })
        rootID.appInfoListModel.append({ name: "Material Icons:", desc: "0.14.13",                  web: "https://github.com/google/material-design-icons" })
    }
}
