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
import "qrc:/filesystem/" as FS
import "qrc:/dataset/" as DS
import "qrc:/reports/" as RCB

Window
{
	title: qsTr("IEDClient - An Open-Source Client for IEC 61850 Protocols")

	id: rootWindow
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
			z: 100500
			height: 30 + 4

			color: ColorPalette.toolBarColor
			clip: true

			// Menu + ToolBar
			RowLayout {
				anchors {
					fill: parent
					leftMargin: 0
					rightMargin: 10
				}
				spacing: 10

				// Menu
				Item {
					width: myMenuBar.implicitWidth + 2
					height: myMenuBar.implicitHeight + 2

					MenuBar {
						id: myMenuBar
						anchors.centerIn: parent

						Menu {
							title: qsTr("&Menu")

							MenuItem {
								text: qsTr("Connect")
								icon.source: "qrc:/img/icons/call.svg"
								enabled: false
							}
							MenuItem {
								text: qsTr("Disconnect")
								icon.source: "qrc:/img/icons/call_end.svg"
								enabled: false
							}
							MenuItem {
								text: qsTr("Settings")
								icon.source: "qrc:/img/icons/build.svg"
								enabled: false
							}
							MenuSeparator {
							}
							MenuItem {
								text: qsTr("Exit")
								onTriggered: {
									console.log("Bye - bye. Exit")
									Qt.quit()
								}
								icon.source: "qrc:/img/icons/close.svg"
							}
						}
						/*
						Menu {
							title: qsTr("&View")

							MenuItem {
								text: qsTr("[LD] Logical devices")
							}
							MenuItem {
								text: qsTr("[LN] Logical nodes")
							}
							MenuItem {
								text: qsTr("[DS] Datasets")
							}
							MenuItem {
								text: qsTr("[CB] Report blocks")
							}
							MenuItem {
								text: qsTr("[FS] Filesystem")
							}
						}
						*/
						Menu {
							title: qsTr("&Tools")

							MenuItem {
								text: qsTr("Save model")
								icon.source: "qrc:/img/icons/save.svg"
								enabled: false
							}
							MenuItem {
								text: qsTr("Check SCL")
								icon.source: "qrc:/img/icons/task_alt.svg"
								enabled: false
							}
							MenuSeparator {
							}
							MenuItem {
								text: qsTr("Event logger")
								Shortcut {
									sequence: "Ctrl+L"
									onActivated: openEventLog()
								}
								onTriggered: openEventLog()
								icon.source: "qrc:/img/icons/terminal.svg"
							}
						}
						Menu {
							title: qsTr("&Program")
							
							MenuItem {
								text: qsTr("Documentation")
								icon.source: "qrc:/img/icons/school.svg"
								enabled: false
							}
							MenuSeparator {
							}
							MenuItem {
								text: qsTr("About")
								onTriggered: {
									rootWindow.showAbotProgramWindow()
								}
								icon.source: "qrc:/img/icons/info.svg"
							}
						}
					}
				}

				// ToolBar
				Item {
					Layout.preferredWidth: toolBarRow.implicitWidth
					readonly property int btnHeight: 30

					id: toolBar
					height: parent.height

					// ToolBar buttons
					Row {
						id: toolBarRow

						anchors.fill: parent
						spacing: 5

						// Home
						ToolBarButton {
							icon: "qrc:/img/icons/home.svg"
							prompt: "Go to start page"
							width: toolBar.btnHeight
							height: toolBar.btnHeight

							onSigClicked: function() {
								rootWindow.setActivePage(Globals.Page.START)
							}
						}
						// Full-screen
						ToolBarButton {
							id: fullScreenBtn
							icon: "qrc:/img/icons/fullscreen.svg"
							prompt: "Full screen mode"
							width: toolBar.btnHeight
							height: toolBar.btnHeight

							onSigClicked: function() {
								toFullscreenMode()
							}
						}
						// Disconnect
						ToolBarButton {
							icon: "qrc:/img/icons/call_end.svg"
							prompt: "Close the connection"
							width: toolBar.btnHeight
							height: toolBar.btnHeight
							visible: presenter.isConnected

							onSigClicked: function() {
								presenter.disconnectFrom()
							}
						}
						/*
						// Back
						ToolBarButton {
							icon: "qrc:/img/icons/arrow_back.svg"
							prompt: "Back to previous page"
							width: toolBar.btnHeight
							height: toolBar.btnHeight

							onSigClicked: function() {
								switchToPreviousPage()
							}
						}
						// Forward
						ToolBarButton {
							icon: "qrc:/img/icons/arrow_forward.svg"
							prompt: "Go to next page"
							width: toolBar.btnHeight
							height: toolBar.btnHeight

							onSigClicked: function() {
							}

							enabled: false
						}
						*/
						// Update
						ToolBarButton {
							icon: "qrc:/img/icons/refresh.svg"
							prompt: "Update data on this page"
							width: toolBar.btnHeight
							height: toolBar.btnHeight

							onSigClicked: function() {
								rootWindow.updateActivePage()
							}
						}
						Rectangle {
							width: 1
							height: parent.height// - 8
							color: ColorPalette.modalColor
						}
						// Table's columns to content size
						ToolBarButton {
							icon: "qrc:/img/icons/code.svg"
							prompt: "Set appropriate width for columns"
							width: toolBar.btnHeight
							height: toolBar.btnHeight

							onSigClicked: function() {
								console.log("Clicked: " + prompt)
								rootWindow.resizeColumnsOnPage()
							}
						}
					}
				}

				// Spacer
				Item {
					Layout.fillWidth: true
					height: toolBar.btnHeight
				}

				// TabBar for StackLayout
				Item {
					Layout.fillWidth: false
					Layout.preferredWidth: tabBar.implicitWidth

					id: mainTabBar
					// height: toolBar.btnHeight
					height: 26

					CustomTabBar {
						id: tabBar
						anchors.fill: parent

						horizontalBar: true

						model: ListModel {
							ListElement { title: "Home" }
							ListElement { title: "LD" }
							ListElement { title: "LN" }
							ListElement { title: "DS" }
							ListElement { title: "CB" }
							ListElement { title: "FS" }
						}
					}
				}
			}
		}

		// Work Area
		Rectangle {
			id: workArea

			anchors {
				top: menuBarRect.bottom
				left: mainBack.left
				right: mainBack.right
				bottom: statusBarID.top

				margins: ColorPalette.borderWidth
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
						id: mainStack

						anchors {
							top: tabsArea.top
							left: tabsArea.left
							right: tabsArea.right
							// bottom: mainTabBar.top
							bottom: tabsArea.bottom
						}

						clip: true
						currentIndex: tabBar.currentIndex

						Home.Page {
							id: startPage
							focus: true

							onVisibleChanged: {
								if (visible) {
									focus = true

									setStatusText(appBackend.getAppVersion())
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
									setStatusText(devBackend.ldsPageStatus())
								} else {
									focus = false
								}
							}
							onSigActivatePage: function(page) {
								rootWindow.setActivePage(page)
							}
						}

						LN.Page {
							id: lnPageID

							onVisibleChanged: {
								if (visible) {
									focus = true

									setActivePanel(Globals.Panel.HIDE)
									setStatusText(devBackend.lnsPageStatus())
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
									setStatusText(devBackend.dsPageStatus())
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
									setStatusText(devBackend.rcbPageStatus())
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

									setStatusText(fsBackend.fsPageStatus())
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
					width: 100

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
						if (width < 50) {
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
		}

		Keys.onPressed: function(event) {
			console.log("Window: Key pressed " + event.key)

			// Update
			if (event.key == Qt.Key_F5) {
				console.log("Update data by F5")
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
					rootWindow.setActivePage(Globals.Page.START)
					event.accepted = true
					return
				}
				case Qt.Key_2: {
					rootWindow.setActivePage(Globals.Page.LD)
					event.accepted = true
					return
				}
				case Qt.Key_3: {
					rootWindow.setActivePage(Globals.Page.LN)
					event.accepted = true
					return
				}
				case Qt.Key_4: {
					rootWindow.setActivePage(Globals.Page.FS)
					event.accepted = true
					return
				}
				case Qt.Key_5: {
					rootWindow.setActivePage(Globals.Page.DS)
					event.accepted = true
					return
				}
				case Qt.Key_6: {
					rootWindow.setActivePage(Globals.Page.RCB)
					event.accepted = true
					return
				}
				}
			}
			// Ctrl
			if (event.modifiers & Qt.ControlModifier) {
				if (event.key == Qt.Key_H) {
					rootWindow.showAbotProgramWindow()
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
			console.log("Hide modal window by timeout")
			globalProgressBar.finishLoad()
		}
	}
	ModalProgressBar {
		id: globalProgressBar
	}

	// Common functions
	function openEventLog() {
		var logsComponent = Qt.createComponent("home/AppEventViewer.qml")
		var logsWindow = logsComponent.createObject(rootWindow)
		logsWindow.show()
	}
	function toFullscreenMode() {
		if (rootWindow.visibility === Window.Windowed) {
			rootWindow.visibility = Window.FullScreen;
			fullScreenBtn.icon = "qrc:/img/icons/close_fullscreen.svg"
		} else {
			rootWindow.visibility = Window.Windowed;
			fullScreenBtn.icon = "qrc:/img/icons/fullscreen.svg"
		}
	}
	function showAbotProgramWindow() {
		var aboutComponent = Qt.createComponent("common/AboutProgram.qml")
		var aboutWindow = aboutComponent.createObject(rootWindow)
		aboutWindow.show()
	}

	// History page list
	property var pageHistoryList: [ 0 ]
	function pushPageToHistoryList(page) {
		pageHistoryList.push(page)
	}
	function switchToPreviousPage() {
		console.log("Switch to previous page: " + pageHistoryList)

		switch (tabBar.currentIndex) {
		case Globals.Page.LD: {
			setActivePage(Globals.Page.Main)
			break;
		}
		case Globals.Page.LN:
		case Globals.Page.FS:
		case Globals.Page.DS:
		case Globals.Page.RCB: {
			setActivePage(Globals.Page.LD)
			break;
		}
		}
	}

	// Active Page + Panel
	function setActivePage(page) {
		//console.log("ActivatePage: new index = " + page)
		pushPageToHistoryList(mainStack.currentIndex)

		switch (page) {
		case Globals.Page.START: {
			tabBar.currentIndex = 0;
			break;
		}
		case Globals.Page.LD: {
			tabBar.currentIndex = 1;
			break;
		}
		case Globals.Page.LN: {
			tabBar.currentIndex = 2;
			break;
		}
		case Globals.Page.FS: {
			tabBar.currentIndex = 3;
			break;
		}
		case Globals.Page.DS: {
			tabBar.currentIndex = 4;
			break;
		}
		case Globals.Page.RCB: {
			tabBar.currentIndex = 5;
			break;
		}
		default: {
			return;
		}
		}
	}
	function setActivePanel(index) {
		//console.log("ActivatePanel: new index = " + index)
		if (index == Globals.Panel.HIDE) {
			propertyPanel.visible = false
		} else {
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
	function setStatusText(msg) {
		statusBarID.pageStatusText = msg
	}
	function resizeColumnsOnPage() {
		switch (tabBar.currentIndex) {
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

		switch (tabBar.currentIndex) {
		case Globals.Page.START: {
			break;
		}
		case Globals.Page.LD: {
			devBackend.updateLDs_Status()
			setStatusText(devBackend.ldsPageStatus())
			break;
		}
		case Globals.Page.LN: {
			devBackend.updateLNs_Status()
			devBackend.updateLN_TreeValues()
			setStatusText(devBackend.lnsPageStatus())
			break;
		}
		case Globals.Page.FS: {
			fsBackend.updateFilesDirectory("/")
			setStatusText(fsBackend.fsPageStatus())
			break;
		}
		case Globals.Page.DS: {
			devBackend.updateDS_Values()
			setStatusText(devBackend.dsPageStatus())
			break;
		}
		case Globals.Page.RCB: {
			devBackend.updateRCBs_Status()
			setStatusText(devBackend.rcbPageStatus())
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
			rootWindow.setActivePage(Globals.Page.LD)
		} else {
			timerModalWindow.running = true
		}
	}

	Component.onCompleted: function() {
		// App
		presenter.sigConnected.connect(slotOnConnected)
		presenter.sigCmdProgress.connect(slotOnProgress)
		presenter.sigCmdFinished.connect(slotOnFinished)

		// Backends to GUI
		devBackend.sigCmdProgress.connect(slotOnProgress)
		devBackend.sigCmdFinished.connect(slotOnFinished)

		fsBackend.sigCmdProgress.connect(slotOnProgress)
		fsBackend.sigCmdFinished.connect(slotOnFinished)

		// Start status
		setStatusText(appBackend.getAppVersion())
		setActivePanel(Globals.Panel.HIDE)
	}
}