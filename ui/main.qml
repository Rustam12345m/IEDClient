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

    property bool hasConnected: false
    minimumWidth: 800
    minimumHeight: 600

    visible: true
    color: VisualStyle.backgroundColor1

    // Main area
    Rectangle {
        id: mainBack

        anchors {
            fill: parent
        }
        color: VisualStyle.borderColor

        // Menu + ToolBar
        Rectangle {
            id: menuBarRect

            anchors {
                top: mainBack.top
                left: mainBack.left
                right: mainBack.right
            }
            visible: true
            height: 30 + VisualStyle.borderWidth * 2

            color: VisualStyle.toolBarColor
            clip: true

            // ToolBar
            RowLayout {
                readonly property int btnHeight: 30
                id: toolBar

                anchors {
                    fill: parent
                    topMargin: VisualStyle.borderWidth
                    bottomMargin: VisualStyle.borderWidth
                    leftMargin: VisualStyle.borderWidth
                    rightMargin: VisualStyle.borderWidth
                }
                spacing: 4

                // Delimiter
                // Rectangle {
                //     Layout.alignment: Qt.AlignVCenter

                //     width: 1
                //     height: toolBar.btnHeight - 4
                //     color: VisualStyle.modalColor
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
                    color: VisualStyle.modalColor
                }
                // Table's columns to content size
                // ToolBarButton {
                //     Layout.alignment: Qt.AlignVCenter
                //     width: toolBar.btnHeight
                //     height: toolBar.btnHeight

                //     icon: "qrc:/img/icons/code.svg"
                //     prompt: "Set appropriate width for columns"

                //     onSigClicked: function() {
                //         console.log("Clicked: " + prompt)
                //         rootWindowID.resizeColumnsOnPage()
                //     }
                // }
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
                    color: VisualStyle.modalColor
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

                    onSigClicked: function() {
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

                    onSigClicked: function() {
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
            height: VisualStyle.tabBar.horizontalHeight + 1 * VisualStyle.borderWidth
            color: VisualStyle.toolBarColor

            CustomTabBar {
                id: mainTabBarID

                anchors {
                    fill: parent
                    margins: 0 //VisualStyle.borderWidth
                    // topMargin: 0
                    bottomMargin: 0
                }

                horizontalBar: true
                cellHeight: VisualStyle.tabBar.horizontalHeight
                cellWidth: VisualStyle.tabBar.horizontalWidth
                spacing: VisualStyle.tabBar.horizontalSpacing

                // color: VisualStyle.toolBarColor
                // selectedColor: VisualStyle.tabBar.selColor
                // unselectedColor: VisualStyle.tabBar.unselColor
                showOnlyFirst: !rootWindowID.hasConnected

                model: ListModel {
                    ListElement { title: "HOME" }
                    ListElement { title: "LOGICAL DEVICES" }
                    ListElement { title: "LOGICAL NODES" }
                    ListElement { title: "DATASETS" }
                    ListElement { title: "REPORT BLOCKS" }
                    ListElement { title: "TREE" }
                    ListElement { title: "FILES" }
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

                margins: 0 //VisualStyle.borderWidth
                topMargin: 0
                bottomMargin: 0
            }
            color: VisualStyle.input.bg

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

                            onSigLNSelectionChanged: {
                                setPageStatusText(iedBackend.lnsPageStatus())
                            }

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

                            property int lastPanel: Globals.Panel.HIDE

                            onVisibleChanged: {
                                if (visible) {
                                    focus = true
                                    setActivePanel(lastPanel)
                                    setPageStatusText(iedBackend.dsPageStatus())
                                } else {
                                    focus = false
                                }
                            }

                            onSigItemDetail: function(doRef, detail) {
                                if (detail.length > 0) {
                                    dsDetailPanel.showDetail(doRef, detail)
                                    lastPanel = Globals.Panel.DS_DETAIL
                                    setActivePanel(lastPanel)
                                } else {
                                    lastPanel = Globals.Panel.HIDE
                                    setActivePanel(lastPanel)
                                }
                            }
                        }

                        RCB.Page {
                            id: rcbPageID

                            property int lastPanel: Globals.Panel.HIDE

                            onVisibleChanged: {
                                if (visible) {
                                    focus = true
                                    setActivePanel(lastPanel)
                                    setPageStatusText(iedBackend.rcbPageStatus())
                                } else {
                                    focus = false
                                }
                            }

                            onSigRCBRowSelected: function(isRCB) {
                                if (isRCB) {
                                    rcbPropPanel.rcbModel = rcbPageID.activeModel
                                    rcbPropPanel.loadFromModel()
                                    lastPanel = Globals.Panel.RCB_PROPERTIES
                                    setActivePanel(lastPanel)
                                } else {
                                    lastPanel = Globals.Panel.HIDE
                                    setActivePanel(lastPanel)
                                }
                            }

                            onSigGOOSERowSelected: function(isGOOSE) {
                                if (isGOOSE) {
                                    goosePropPanel.gooseModel = rcbPageID.activeModel
                                    goosePropPanel.loadFromModel()
                                    lastPanel = Globals.Panel.GOOSE_PROPERTIES
                                    setActivePanel(lastPanel)
                                } else {
                                    lastPanel = Globals.Panel.HIDE
                                    setActivePanel(lastPanel)
                                }
                            }

                            onSigSVRowSelected: function(isSV) {
                                if (isSV) {
                                    svPropPanel.svModel = rcbPageID.activeModel
                                    svPropPanel.loadFromModel()
                                    lastPanel = Globals.Panel.SV_PROPERTIES
                                    setActivePanel(lastPanel)
                                } else {
                                    lastPanel = Globals.Panel.HIDE
                                    setActivePanel(lastPanel)
                                }
                            }

                            onSigReportDetail: function(detail) {
                                if (detail.entries) {
                                    reportDetailPanel.showReport(detail)
                                    lastPanel = Globals.Panel.REPORT_DETAIL
                                    setActivePanel(lastPanel)
                                } else {
                                    lastPanel = Globals.Panel.HIDE
                                    setActivePanel(lastPanel)
                                }
                            }
                        }

                        IEDTREE.Page {
                            id: iedTreeID

                            onVisibleChanged: {
                                if (visible) {
                                    focus = true
                                    setActivePanel(Globals.Panel.HIDE)
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
                                } else {
                                    focus = false
                                }
                            }

                            Connections {
                                target: fsBackend
                                function onSigCmdFinished() {
                                    if (fsPageID.visible) {
                                        setPageStatusText(fsBackend.fsPageStatus())
                                    }
                                }
                            }
                        }
                    }
                }

                // Property panel
                Rectangle {
                    id: propertyPanel
                    width: 480
                    clip: true

                    property int lastWidth: 480

                    SplitView.fillWidth: false
                    SplitView.preferredWidth: 480

                    color: VisualStyle.backgroundColor1

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

                            onSigEnable: function(trgOps, bufTm, intgPd, rptId, datSet) {
                                iedBackend.setRCBEnable(rcbPageID.isBuffered,
                                    rcbPageID.selectedRCBIndex, true, trgOps, bufTm, intgPd,
                                    rptId, datSet)
                            }
                            onSigDisable: {
                                iedBackend.setRCBEnable(rcbPageID.isBuffered,
                                    rcbPageID.selectedRCBIndex, false, 0, 0, 0, "", "")
                            }
                            onSigGoToDataSet: function(dsRef) {
                                setActivePage(Globals.Page.DS)
                                Qt.callLater(function() {
                                    iedBackend.selectDataSetByRef(dsRef)
                                })
                            }

                            Connections {
                                target: iedBackend
                                function onSigRCBUpdated() {
                                    rcbPropPanel.loadFromModel()
                                }
                            }
                        }

                        // Report detail panel
                        RCB.ReportDetailPanel {
                            id: reportDetailPanel
                        }

                        // Data object detail panel
                        DS.DS_DetailPanel {
                            id: dsDetailPanel
                        }

                        // GOOSE control block properties
                        RCB.GOOSE_PropertiesPanel {
                            id: goosePropPanel

                            onSigEnable: function(goId, datSet) {
                                iedBackend.setGOOSEEnable(rcbPageID.selectedRCBIndex, true)
                            }
                            onSigDisable: {
                                iedBackend.setGOOSEEnable(rcbPageID.selectedRCBIndex, false)
                            }

                            Connections {
                                target: iedBackend
                                function onSigGOOSE_SVUpdated() {
                                    goosePropPanel.loadFromModel()
                                }
                            }
                        }

                        // SV control block properties
                        RCB.SV_PropertiesPanel {
                            id: svPropPanel

                            onSigEnable: {
                                iedBackend.setSVEnable(rcbPageID.selectedRCBIndex, true)
                            }
                            onSigDisable: {
                                iedBackend.setSVEnable(rcbPageID.selectedRCBIndex, false)
                            }

                            Connections {
                                target: iedBackend
                                function onSigGOOSE_SVUpdated() {
                                    svPropPanel.loadFromModel()
                                }
                            }
                        }
                    }

                    onWidthChanged: function() {
                        if (width < 80) {
                            panelStack.visible = false
                            width = 0
                        } else {
                            if (!panelStack.visible) {
                                panelStack.visible = true
                            }
                            lastWidth = width
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
        }
    }

    // Window-level shortcuts — fire regardless of which item holds focus.
    // Keys.onPressed on mainBack is bypassed when a TextField/CheckBox/Button
    // inside the property panel holds activeFocus (they accept all key events
    // at the C++ level). Shortcut items are not part of the focus chain and
    // always work at the ApplicationWindow scope.
    Shortcut { sequence: "F1";    onActivated: showAbotProgramWindow() }
    Shortcut { sequence: "F5";    onActivated: updateActivePage()      }
    Shortcut { sequence: "F11";   onActivated: toFullscreenMode()      }
    Shortcut { sequence: "Alt+1"; onActivated: setActivePage(Globals.Page.START) }
    Shortcut { sequence: "Alt+2"; enabled: presenter.isConnected; onActivated: setActivePage(Globals.Page.LD)    }
    Shortcut { sequence: "Alt+3"; enabled: presenter.isConnected; onActivated: setActivePage(Globals.Page.LN)    }
    Shortcut { sequence: "Alt+4"; enabled: presenter.isConnected; onActivated: setActivePage(Globals.Page.DS)    }
    Shortcut { sequence: "Alt+5"; enabled: presenter.isConnected; onActivated: setActivePage(Globals.Page.RCB)   }
    Shortcut { sequence: "Alt+6"; enabled: presenter.isConnected; onActivated: setActivePage(Globals.Page.IED_TREE) }
    Shortcut { sequence: "Alt+7"; enabled: presenter.isConnected; onActivated: setActivePage(Globals.Page.FS)       }
    Shortcut { sequence: "Ctrl+H"; onActivated: showAbotProgramWindow()          }

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
    }

    ModalDialog {
        id: eventLogSubWindowID

        title: "Event Log"
        dialogWidth: rootWindowID.width * 0.8
        dialogHeight: rootWindowID.height * 0.8

        Home.AppEventViewer {
            anchors.fill: parent

            onSigClose: function() {
                eventLogSubWindowID.close()
            }
        }
    }

    ModalDialog {
        id: aboutSubWindowID

        title: "About IEDClient"
        dialogWidth: 600
        dialogHeight: 300

        Column {
            anchors {
                fill: parent
                margins: 12
            }
            spacing: 6

            Repeater {
                model: [
                    { key: "Application:", desc: "IEDClient — IEC 61850 Client"  },
                    { key: "Version:",     desc: appBackend.getAppVersion()       },
                    { key: "Qt:",          desc: appBackend.getQtVersion()        },
                    { key: "libiec61850:", desc: appBackend.getLibVersion()       },
                    { key: "License:",     desc: "GNU General Public License v3"  },
                    { key: "Author:",      desc: "Rustam Mustafin"                }
                ]

                delegate: Row {
                    spacing: 8
                    leftPadding: 4

                    Text {
                        width:                 120
                        text:                  modelData.key
                        color:                 VisualStyle.statusBar.textColor
                        font.pixelSize:        14
                        font.family:           VisualStyle.fontFamily
                        font.bold:             true
                        horizontalAlignment:   Text.AlignRight
                        verticalAlignment:     Text.AlignVCenter
                    }
                    Text {
                        text:           modelData.desc
                        color:          VisualStyle.statusBar.textColor
                        font.pixelSize: 14
                        font.family:    VisualStyle.fontFamily
                        verticalAlignment: Text.AlignVCenter
                    }
                }
            }
        }
    }

    ModalDialog {
        id: connectionErrorDialog

        title: "Connection Error"
        dialogWidth: 500
        dialogHeight: 200
        bgColor: VisualStyle.errorModalColor

        Text {
            id: errorMessageText

            anchors.fill: parent
            anchors.margins: 10

            wrapMode: Text.Wrap
            color: VisualStyle.statusBar.textColor
            font.pixelSize: 14
            text: ""
        }

        function showError(msg) {
            errorMessageText.text = msg
            open()
        }
    }

    // Common functions
    function openEventLog() {
        eventLogSubWindowID.open()
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
        aboutSubWindowID.open()
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
            if (!propertyPanel.visible) {
                propertyPanel.SplitView.preferredWidth = propertyPanel.lastWidth
                propertyPanel.visible = true
            }

            switch (index) {
            case Globals.Panel.LD_INFO: {
                panelStack.currentIndex = 0
                break
            }
            case Globals.Panel.RCB_PROPERTIES: {
                panelStack.currentIndex = 1
                break
            }
            case Globals.Panel.REPORT_DETAIL: {
                panelStack.currentIndex = 2
                break
            }
            case Globals.Panel.DS_DETAIL: {
                panelStack.currentIndex = 3
                break
            }
            case Globals.Panel.GOOSE_PROPERTIES: {
                panelStack.currentIndex = 4
                break
            }
            case Globals.Panel.SV_PROPERTIES: {
                panelStack.currentIndex = 5
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
        // console.log("F5: Update active page")

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
        case Globals.Page.IED_TREE: {
            iedBackend.updateWatchlistValues()
            break;
        }
        }
    }

    // Slots from backend
    function slotOnProgress(perc, msg) {
        if (!globalProgressBar.isActive()) {
            globalProgressBar.startLoad()
        }
        globalProgressBar.updateLoad(perc, msg)
    }
    function slotOnFinished() {
        globalProgressBar.finishLoad()
    }
    function slotOnConnected(done) {
        globalProgressBar.finishLoad()
        if (done) {
            rootWindowID.hasConnected = true
            rootWindowID.setActivePage(Globals.Page.LD)
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
        presenter.sigConnectionError.connect(function(msg) {
            connectionErrorDialog.showError(msg)
        })
        appBackend.sigNewStatusMsg.connect(slotStatusMessage)

        // Backends to GUI
        iedBackend.sigCmdProgress.connect(slotOnProgress)
        iedBackend.sigCmdFinished.connect(slotOnFinished)
        iedBackend.sigCmdError.connect(function(msg) {
            connectionErrorDialog.showError(msg)
        })

        fsBackend.sigCmdProgress.connect(slotOnProgress)
        fsBackend.sigCmdFinished.connect(slotOnFinished)
        fsBackend.sigCmdError.connect(function(msg) {
            connectionErrorDialog.showError(msg)
        })

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
            // console.log("Currently focused item:", rootWindowID.activeFocusItem)
        }
    }
}
