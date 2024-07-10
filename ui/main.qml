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
import "qrc:/home/" as Home
import "qrc:/logical_device/" as LD
import "qrc:/logical_node/" as LN
import "qrc:/dataset/" as DS
import "qrc:/reports/" as RCB
import "qrc:/iedtree/" as IEDTREE
import "qrc:/filesystem/" as FS

ApplicationWindow
{
    title: qsTr("IEDClient - An Open-Source Client for IEC 61850 Protocols")

    id: rootWindowID
    width: 1000
    height: 650
    minimumWidth: 800
    minimumHeight: 600

    visible: true
    color: "white"

    // Main area
    Rectangle {
        id: mainBack

        anchors {
            fill: parent
        }
        color: ColorPalette.borderColor

        // Menu + ToolBar
        Rectangle {
            id: menuBarRect

            anchors {
                top: mainBack.top
                left: mainBack.left
                right: mainBack.right
            }
            // width: parent.width
            height: 30 + 4

            color: ColorPalette.toolBarColor
            clip: true

            // Menu + ToolBar
            RowLayout {
                readonly property int btnHeight: 30
                id: toolBar

                anchors {
                    fill: parent
                    topMargin: ColorPalette.borderWidth
                    leftMargin: ColorPalette.borderWidth
                    rightMargin: ColorPalette.borderWidth
                }
                spacing: 10

                // Delimiter
                // Rectangle {
                //     Layout.alignment: Qt.AlignVCenter

                //     width: 1
                //     height: toolBar.btnHeight - 4
                //     color: ColorPalette.modalColor
                // }
                // Home
                ToolBarButton {
                    Layout.alignment: Qt.AlignVCenter
                    width: toolBar.btnHeight
                    height: toolBar.btnHeight

                    icon: "qrc:/img/icons/home.svg"
                    prompt: "Go to start page"

                    onSigClicked: function() {
                        rootWindowID.setActivePage(Globals.Page.START)
                    }
                }
                // Full-screen
                ToolBarButton {
                    id: fullScreenBtn
                    Layout.alignment: Qt.AlignVCenter
                    width: toolBar.btnHeight
                    height: toolBar.btnHeight

                    icon: "qrc:/img/icons/fullscreen.svg"
                    prompt: "Full screen mode"

                    onSigClicked: function() {
                        toFullscreenMode()
                    }
                }
                // Disconnect
                ToolBarButton {
                    Layout.alignment: Qt.AlignVCenter
                    width: toolBar.btnHeight
                    height: toolBar.btnHeight

                    visible: presenter.isConnected
                    icon: "qrc:/img/icons/call_end.svg"
                    prompt: "Close the connection"

                    onSigClicked: function() {
                        presenter.disconnectFrom()
                    }
                }
                // Navigation screen
                // ToolBarButton {
                //     Layout.alignment: Qt.AlignVCenter
                //     width: toolBar.btnHeight
                //     height: toolBar.btnHeight
                //     icon: "qrc:/img/icons/view_comfy_alt.svg"
                //     prompt: "Navigation window"
                //     onSigClicked: function() {
                //         console.log("Clicked: " + prompt)
                //         rootWindowID.showNavigationGrid()
                //     }
                // }
                // Delimiter
                Rectangle {
                    // anchors.verticalCenter: parent.verticalCenter
                    Layout.alignment: Qt.AlignVCenter

                    width: 1
                    height: toolBar.btnHeight - 4
                    color: ColorPalette.modalColor
                }
                // Table's columns to content size
                ToolBarButton {
                    Layout.alignment: Qt.AlignVCenter
                    width: toolBar.btnHeight
                    height: toolBar.btnHeight

                    icon: "qrc:/img/icons/code.svg"
                    prompt: "Set appropriate width for columns"

                    onSigClicked: function() {
                        console.log("Clicked: " + prompt)
                        rootWindowID.resizeColumnsOnPage()
                    }
                }
                // Update
                ToolBarButton {
                    Layout.alignment: Qt.AlignVCenter
                    width: toolBar.btnHeight
                    height: toolBar.btnHeight

                    icon: "qrc:/img/icons/refresh.svg"
                    prompt: "Update data on this page"

                    onSigClicked: function() {
                        rootWindowID.updateActivePage()
                    }
                }

                // Spacer
                Item {
                    Layout.fillWidth: true
                    height: toolBar.btnHeight
                }

                // Delimiter
                Rectangle {
                    // anchors.verticalCenter: parent.verticalCenter
                    Layout.alignment: Qt.AlignVCenter

                    width: 1
                    height: toolBar.btnHeight - 4
                    color: ColorPalette.modalColor
                }
                // Events
                ToolBarButton {
                    Layout.alignment: Qt.AlignVCenter
                    width: toolBar.btnHeight
                    height: toolBar.btnHeight

                    icon: "qrc:/img/icons/terminal.svg"
                    prompt: "Event logger"
                    visible: presenter.isConnected

                    onSigClicked: function() {
                        console.log("Clickerd: ", prompt)
                        openEventLog()
                    }
                    Shortcut {
                        sequence: "Ctrl+E"
                        onActivated: openEventLog()
                    }
                }
                // About
                ToolBarButton {
                    Layout.alignment: Qt.AlignVCenter
                    width: toolBar.btnHeight
                    height: toolBar.btnHeight

                    icon: "qrc:/img/icons/info.svg"
                    prompt: "About IEDClient"
                    visible: presenter.isConnected

                    onSigClicked: function() {
                        console.log("Clickerd: ", prompt)
                        rootWindowID.showAbotProgramWindow()
                    }
                }
                // Exit, close the app
                ToolBarButton {
                    Layout.alignment: Qt.AlignVCenter
                    width: toolBar.btnHeight
                    height: toolBar.btnHeight

                    icon: "qrc:/img/icons/input.svg"
                    prompt: "Exit"
                    visible: presenter.isConnected

                    onSigClicked: function() {
                        console.log("Clickerd: ", prompt)
                        Qt.quit()
                    }
                }
            }
        }

        // TabBar
        Rectangle {
            id: tabsRectID

            anchors {
                top: menuBarRect.bottom
                left: mainBack.left
                right: mainBack.right
            }
            // width: parent.width
            height: 30 + 2 * ColorPalette.borderWidth
            color: ColorPalette.toolBarColor

            CustomTabBar {
                id: mainTabBarID

                anchors {
                    fill: parent
                    margins: ColorPalette.borderWidth
                }

                horizontalBar: true
                cellHeight: 30
                cellWidth: 125
                spacing: ColorPalette.borderWidth

                color: ColorPalette.toolBarColor
                selectedColor: "white"// "lightgray" //"#595959"
                unselectedColor: "#F0F0F0"

                model: ListModel {
                    ListElement { title: "Home" }
                    ListElement { title: "Logical devices" }
                    ListElement { title: "Logical nodes" }
                    ListElement { title: "DataSets" }
                    ListElement { title: "Report blocks" }
                    ListElement { title: "Tree" }
                    ListElement { title: "Files" }
                }

                onSigTabSelected: function(inx) {
                    setActivePage(inx)
                }
            }
        }

        // Work Area with pages
        Rectangle {
            id: workArea

            anchors {
                top: tabsRectID.bottom
                bottom: statusBarID.top
                left: mainBack.left
                right: mainBack.right

                margins: ColorPalette.borderWidth
                topMargin: 0
                bottomMargin: 0
            }
            color: ColorPalette.backgroundColor1

            // Pages and Property panel
            SplitView {
                id: splitView
                anchors.fill: parent

                // Delimiter
                handle: SplitDelimeter {
                    height: splitView.height
                    pressed: SplitHandle.pressed
                }

                // Pages Area
                Item {
                    id: tabsArea

                    SplitView.preferredWidth: 750
                    SplitView.minimumWidth: 600
                    SplitView.fillWidth: true

                    // Pages
                    StackLayout {
                        id: mainStackID

                        anchors {
                            top: tabsArea.top
                            left: tabsArea.left
                            right: tabsArea.right
                            bottom: tabsArea.bottom
                        }
                        clip: true

                        Home.Page {
                            id: startPage
                            focus: true

                            onVisibleChanged: {
                                if (visible) {
                                    focus = true

                                    setPageStatusText(appBackend.getAppVersion())
                                    setActivePanel(Globals.Panel.HIDE)
                                } else {
                                    focus = false
                                }
                            }
                            onSigConnectTo: function(con) {
                                globalProgressBar.startLoad()
                                presenter.connectTo(con)
                            }
                            onSigDumpModel: function(dir, ip, port, tls, user, pass) {
                                globalProgressBar.startLoad()
                                presenter.toolDumpModel(dir, ip, port, tls, user, pass)
                            }
                        }

                        LD.Page {
                            id: ldPage

                            onVisibleChanged: {
                                if (visible) {
                                    focus = true

                                    setActivePanel(Globals.Panel.LD_INFO)
                                    setPageStatusText(iedBackend.ldsPageStatus())
                                } else {
                                    focus = false
                                }
                            }
                            onSigActivatePage: function(page) {
                                rootWindowID.setActivePage(page)
                            }
                        }

                        LN.Page {
                            id: lnPageID

                            onVisibleChanged: {
                                if (visible) {
                                    focus = true

                                    setActivePanel(Globals.Panel.HIDE)
                                    setPageStatusText(iedBackend.lnsPageStatus())
                                } else {
                                    focus = false
                                }
                            }
                        }

                        DS.Page {
                            id: dsPageID

                            onVisibleChanged: {
                                if (visible) {
                                    focus = true

                                    setActivePanel(Globals.Panel.HIDE)
                                    setPageStatusText(iedBackend.dsPageStatus())
                                } else {
                                    focus = false
                                }
                            }
                        }

                        RCB.Page {
                            id: rcbPageID

                            onVisibleChanged: {
                                if (visible) {
                                    focus = true
                                    setActivePanel(Globals.Panel.RCB_PROPERTIES)
                                    setPageStatusText(iedBackend.rcbPageStatus())
                                } else {
                                    focus = false
                                }
                            }
                        }

                        IEDTREE.Page {
                            id: iedTreeID

                            onVisibleChanged: {
                                if (visible) {
                                    focus = true
                                    // setActivePanel(Globals.Panel.RCB_PROPERTIES)
                                    // setPageStatusText(iedBackend.rcbPageStatus())
                                } else {
                                    focus = false
                                }
                            }
                        }

                        FS.Page {
                            id: fsPageID

                            onVisibleChanged: {
                                if (visible) {
                                    focus = true

                                    fsBackend.updateFilesDirectory("/")
                                    setActivePanel(Globals.Panel.HIDE)

                                    setPageStatusText(fsBackend.fsPageStatus())
                                } else {
                                    focus = false
                                }
                            }
                        }
                    }
                }

                // Property panel
                Rectangle {
                    id: propertyPanel
                    width: 250

                    SplitView.fillWidth: false
                    SplitView.preferredWidth: 250

                    color: ColorPalette.backgroundColor2

                    // Stack for Panels
                    StackLayout {
                        id: panelStack

                        anchors.fill: parent

                        // Selected LD's properties
                        LD.LD_PropertiesPanel {
                            id: ldPropPanel
                        }

                        // Selected RCB's settings & controls
                        RCB.RCB_PropertiesPanel {
                            id: rcbPropPanel
                        }
                    }

                    onWidthChanged: function() {
                        if (width < 80) {
                            width = 0;
                        }
                    }
                }
            }
        }

        // Status line
        AppStatusBar {
            id: statusBarID

            anchors {
                left: mainBack.left
                right: mainBack.right
                bottom: mainBack.bottom
            }
            height: 24

            appConnStatus: presenter.iedConStatus
        }

        Keys.onPressed: function(event) {
            // console.log("Window: Key pressed " + event.key)

            // Update
            if (event.key == Qt.Key_F5) {
                // console.log("Update data by F5")
                updateActivePage()
                event.accepted = true
                return
            }

            // Fullscreen
            if (event.key == Qt.Key_F11) {
                toFullscreenMode()
                event.accepted = true
                return
            }

            // Alt
            if (event.modifiers & Qt.AltModifier) {
                switch (event.key) {
                case Qt.Key_1: {
                    rootWindowID.setActivePage(Globals.Page.START)
                    event.accepted = true
                    return
                }
                case Qt.Key_2: {
                    rootWindowID.setActivePage(Globals.Page.LD)
                    event.accepted = true
                    return
                }
                case Qt.Key_3: {
                    rootWindowID.setActivePage(Globals.Page.LN)
                    event.accepted = true
                    return
                }
                case Qt.Key_4: {
                    rootWindowID.setActivePage(Globals.Page.DS)
                    event.accepted = true
                    return
                }
                case Qt.Key_5: {
                    rootWindowID.setActivePage(Globals.Page.RCB)
                    event.accepted = true
                    return
                }
                case Qt.Key_6: {
                    rootWindowID.setActivePage(Globals.Page.FS)
                    event.accepted = true
                    return
                }
                }
            }

            // Ctrl
            if (event.modifiers & Qt.ControlModifier) {
                if (event.key == Qt.Key_H) {
                    rootWindowID.showAbotProgramWindow()
                }
            }
        }
    }

    // Close modal window timer
    Timer {
        id: timerModalWindow

        interval: 3000
        running: false
        repeat: false

        onTriggered: {
            // console.log("Hide modal window by timeout")
            globalProgressBar.finishLoad()
        }
    }
    Home.AppProgressBar {
        id: globalProgressBar

        windowColor: "#A9A9A9" // ColorPalette.modalColor
        windowShadeColor: "#D9D9D9" //ColorPalette.toolBarColor
    }

    // Common functions
    function openEventLog() {
        var logsComponent = Qt.createComponent("home/AppEventViewer.qml")
        var logsWindow = logsComponent.createObject(rootWindowID)
        logsWindow.show()
    }
    function toFullscreenMode() {
        if (rootWindowID.visibility === Window.Windowed) {
            rootWindowID.visibility = Window.FullScreen;
            fullScreenBtn.icon = "qrc:/img/icons/close_fullscreen.svg"
        } else {
            rootWindowID.visibility = Window.Windowed;
            fullScreenBtn.icon = "qrc:/img/icons/fullscreen.svg"
        }
    }
    function showAbotProgramWindow() {
        var aboutComponent = Qt.createComponent("home/AboutProgram.qml")
        var aboutWindow = aboutComponent.createObject(rootWindowID)
        aboutWindow.show()
    }

    // Active Page + Panel
    function setActivePage(page) {
        mainStackID.currentIndex = page
        mainTabBarID.currentIndex = page
    }
    function setActivePanel(index) {
        //console.log("ActivatePanel: new index = " + index)
        if (index == Globals.Panel.HIDE) {
            propertyPanel.visible = false
        } else {
            propertyPanel.visible = false
            propertyPanel.SplitView.preferredWidth = 250
            propertyPanel.visible = true

            switch (index) {
            case Globals.Panel.LD_INFO: {
                panelStack.currentIndex = 0
                break
            }
            case Globals.Panel.RCB_PROPERTIES: {
                panelStack.currentIndex = 1
                break
            }
            }
        }
    }
    function setPageStatusText(msg) {
        statusBarID.pageStatusText = msg
    }
    function resizeColumnsOnPage() {
        switch (mainStackID.currentIndex) {
        case Globals.Page.START:
        case Globals.Page.LD: {
            break;
        }
        case Globals.Page.LN: {
            lnPageID.resizeColumnsOnPage()
            break;
        }
        case Globals.Page.DS: {
            dsPageID.resizeColumnsOnPage()
            break;
        }
        case Globals.Page.RCB: {
            rcbPageID.resizeColumnsOnPage()
            break;
        }
        case Globals.Page.FS: {
            fsPageID.resizeColumnsOnPage()
            break;
        }
        }
    }

    // Process
    function updateActivePage() {
        console.log("F5: Update active page")

        switch (mainStackID.currentIndex) {
        case Globals.Page.START: {
            break;
        }
        case Globals.Page.LD: {
            iedBackend.updateLDs_Status()
            setPageStatusText(iedBackend.ldsPageStatus())
            break;
        }
        case Globals.Page.LN: {
            iedBackend.updateLNs_Status()
            iedBackend.updateLN_TreeValues()
            setPageStatusText(iedBackend.lnsPageStatus())
            break;
        }
        case Globals.Page.FS: {
            fsBackend.updateFilesDirectory("/")
            setPageStatusText(fsBackend.fsPageStatus())
            break;
        }
        case Globals.Page.DS: {
            iedBackend.updateDS_Values()
            setPageStatusText(iedBackend.dsPageStatus())
            break;
        }
        case Globals.Page.RCB: {
            iedBackend.updateRCBs_Status()
            setPageStatusText(iedBackend.rcbPageStatus())
            break;
        }
        }
    }

    // Slots from backend
    function slotOnProgress(t_perc, t_msg) {
        if (!globalProgressBar.isActive()) {
            globalProgressBar.startLoad()
        }
        globalProgressBar.updateLoad(t_perc, t_msg)
    }
    function slotOnFinished() {
        globalProgressBar.finishLoad()
    }
    function slotOnConnected(t_done) {
        if (t_done) {
            globalProgressBar.finishLoad()
            rootWindowID.setActivePage(Globals.Page.LD)
        } else {
            timerModalWindow.running = true
        }
    }
    function slotStatusMessage() {
        statusBarID.msgStatusText = appBackend.getLastStatusMsg()
    }

    Component.onCompleted: function() {
        // App
        presenter.sigIEDConChanged.connect(slotOnConnected)
        presenter.sigCmdProgress.connect(slotOnProgress)
        presenter.sigCmdFinished.connect(slotOnFinished)
        appBackend.sigNewStatusMsg.connect(slotStatusMessage)

        // Backends to GUI
        iedBackend.sigCmdProgress.connect(slotOnProgress)
        iedBackend.sigCmdFinished.connect(slotOnFinished)

        fsBackend.sigCmdProgress.connect(slotOnProgress)
        fsBackend.sigCmdFinished.connect(slotOnFinished)

        // Start status
        setPageStatusText(appBackend.getAppVersion())
        setActivePanel(Globals.Panel.HIDE)
    }

    // Debug
    Timer {
        id: debugTimerID

        interval: 5000
        //running: true
        repeat: true

        onTriggered: {
            console.log("Currently focused item:", rootWindowID.activeFocusItem)
        }
    }
}