/*
 *  main.qml
 *
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

import "qrc:/global/"
import "qrc:/main/" as Main
import "qrc:/logical_device/" as LD
import "qrc:/logical_node/" as LN
import "qrc:/filesystem/" as FS
import "qrc:/dataset/" as DS
import "qrc:/reports/" as RCB

Window {
	title: qsTr("IEDClient - An Open-Source Client for IEC 61850 Protocols")

	id: rootWindow
	width: 1000
	height: 650
	visible: true
	color: "white"

	property var globals: Globals {}

	// Status timer
	Timer {
		property int tick: 0
		interval: 1000 // every 1 second
		running: true
		repeat: true
		onTriggered: {
			tick++
			//statusTextBox.text = Qt.formatDateTime(new Date(), "hh:mm:ss") + " Uptime: " + tick
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

		anchors.centerIn: parent
	}

	QuestionForUser {
		id: globalQuestionWindow

		anchors.centerIn: parent
	}

	// Main area
	Rectangle {
		id: mainBack
		anchors.fill: parent

		// Menu + ToolBar
		Rectangle {
			id: menuBarRect

			y: 0
			z: 100500
			border.width: 1
			border.color: "black"
			color: "lightgray"

			height: myMenuBar.implicitHeight + 2
			anchors {
				//top: mainBack.top
				left: mainBack.left
				right: mainBack.right
			}

			// Menu + ToolBar
			RowLayout {
				anchors {
					fill: parent
					leftMargin: 0
					rightMargin: 2
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
							}
							MenuItem {
								text: qsTr("Disconnect")
								icon.source: "qrc:/img/icons/call_end.svg"
							}
							MenuItem {
								text: qsTr("Settings")
								icon.source: "qrc:/img/icons/build.svg"
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
						Menu {
							title: qsTr("&Tools")
							MenuItem {
								text: qsTr("Save model")
								icon.source: "qrc:/img/icons/save.svg"
							}
							MenuItem {
								text: qsTr("Check SCL")
								icon.source: "qrc:/img/icons/task_alt.svg"
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
							}
							MenuSeparator {
							}
							MenuItem {
								text: qsTr("About")
								onTriggered: {
									var aboutComponent = Qt.createComponent("global/AboutProgram.qml")
									var aboutWindow = aboutComponent.createObject(rootWindow)
									aboutWindow.show()
								}
								icon.source: "qrc:/img/icons/info.svg"
							}
						}
					}
				}

				// ToolBar
				Item {
					Layout.preferredWidth: toolBarRow.implicitWidth

					id: toolBar
					readonly property int btnHeight: 30

					//width: toolBarRow.implicitWidth
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

							onSigClicked: function() {
								globalQuestionWindow.open("Disconnect from ... ?")
							}
						}
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
					}
				}

				// Spacer
				Item {
					Layout.fillWidth: true

					width: 20
					height: toolBar.height

					/*
					Text {
						id: conMessage
						anchors.centerIn: parent

						horizontalAlignment: Text.AlignHCenter
						verticalAlignment: Text.AlignVCenter

						clip: true
						font.bold: true
						color: "black"

						text: "Disconnected"
					}
					*/
				}

				// Status message
				Rectangle {
					Layout.preferredWidth: Math.max(250, statusTextBox.implicitWidth + toolBar.btnHeight)

					height: toolBar.btnHeight

					border.width: 1
					border.color: "gray"
					clip: true
					color: "white"

					RowLayout {
						anchors.fill: parent

						Led {
							height: toolBar.btnHeight
							width: toolBar.btnHeight
							color: presenter.isConnected ? "green" : "red"
						}
						Text {
							Layout.fillWidth: true

							id: statusTextBox

							height: toolBar.btnHeight

							horizontalAlignment: Text.AlignHCenter
							verticalAlignment: Text.AlignVCenter

							font.bold: true
							color: "black"
							text: ""
						}
					}
				}
			}
		}

		// Work Area
		Rectangle {
			id: workArea
			color: "white"

			anchors {
				top: menuBarRect.bottom
				left: mainBack.left
				right: mainBack.right
				bottom: mainBack.bottom
			}

			// Pages and Property panel
			SplitView {
				id: splitView
				anchors.fill: parent

				// Delimiter
				handle: Rectangle {
					id: handleDelegate
					implicitWidth: 4
					implicitHeight: 4
					color: SplitHandle.pressed ? "gray" : "lightgray"

					containmentMask: Item {
						x: (handleDelegate.width - width) / 2
						width: 20
						height: splitView.height
					}
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
							bottom: mainTabBar.top
						}

						clip: true
						currentIndex: tabBar.currentIndex

						Main.Page {
							id: startPage
							focus: true

							onVisibleChanged: {
								if (visible) {
									focus = true

									setStatusText(presenter.getAppVersion())
									setActivePanel(Globals.Panel.LAST_CONN)
								} else {
									focus = false
								}
							}
							onSigConnectTo: function(ip, port, tls, user, pass) {
								globalProgressBar.startLoad()
								presenter.connectTo(ip, port, tls, user, pass)
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
									setStatusText(ldBackend.getLD_TextStatus())
								} else {
									focus = false
								}
							}
							onSigActivatePage: function(page) {
								rootWindow.setActivePage(page)
							}
						}

						LN.Page {
							id: lnPage

							onVisibleChanged: {
								if (visible) {
									focus = true

									setActivePanel(Globals.Panel.HIDE)
									setStatusText(ldBackend.getLN_TextStatus())
								} else {
									focus = false
								}
							}
						}

						FS.Page {
							onVisibleChanged: {
								if (visible) {
									focus = true

									fsBackend.updateFilesDirectory("/")
									setActivePanel(Globals.Panel.HIDE)

									setStatusText(fsBackend.getFS_TextStatus())
								} else {
									focus = false
								}
							}
						}

						DS.Page {
							onVisibleChanged: {
								if (visible) {
									focus = true

									setActivePanel(Globals.Panel.HIDE)
								} else {
									focus = false
								}
							}
						}

						RCB.Page {
							onVisibleChanged: {
								if (visible) {
									focus = true
									setActivePanel(Globals.Panel.RCB_PROPERTIES)
								} else {
									focus = false
								}
							}
						}
					}

					// TabBar in the bottom of Tabs
					Item {
						id: mainTabBar
						height: tabBar.implicitHeight

						anchors {
							left: tabsArea.left
							right: tabsArea.right
							bottom: tabsArea.bottom
						}

						TabBar {
							id: tabBar
							anchors.fill: parent

							currentIndex: 0
							focus: false
							focusPolicy: Qt.NoFocus

							TabButton {
								text: qsTr("Main")
							}
							TabButton {
								text: qsTr("LD")
							}
							TabButton {
								text: qsTr("LN")
							}
							TabButton {
								text: qsTr("FS")
							}
							TabButton {
								text: qsTr("DS")
							}
							TabButton {
								text: qsTr("RCB")
							}
						}
					}
				}

				// Property panel
				Item {
					id: propertyPanel
					width: 100

					SplitView.fillWidth: false
					SplitView.preferredWidth: 250

					// Stack for Panels
					StackLayout {
						id: panelStack

						anchors.fill: parent

						// Start panel with last connections
						Main.LastConn_Panel {
							id: lastConnPanel
						}

						// Selected LD's properties
						LD.LD_PropPanel {
							id: ldPropPanel
						}

						// Selected RCB's settings & controls
						RCB.RCB_PropPanel {
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
		}
	}

	// Common functions
	function openEventLog() {
		var logsComponent = Qt.createComponent("global/EventsViewer.qml")
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

	// History page list
	property var pageHistoryList: [ 0 ]
	function switchToPreviousPage() {
		console.log("Switch to previous page: " + pageHistoryList)

		//var cur = pageHistoryList.pop()
		var prev = pageHistoryList.pop()
		if (prev >= 0) {
			//setActivePage(prev)
			tabBar.currentIndex = prev
			console.log("Switch to previous page: " + prev)
		}
	}
	function pushPageToHistoryList(page) {
		pageHistoryList.push(page)
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
			case Globals.Panel.LAST_CONN: {
				panelStack.currentIndex = 0
				break
			}
			case Globals.Panel.LD_INFO: {
				panelStack.currentIndex = 1
				break
			}
			case Globals.Panel.RCB_PROPERTIES: {
				panelStack.currentIndex = 2
				break
			}
			}
		}
	}
	function setStatusText(msg) {
		statusTextBox.text = msg
	}

	// Process
	function updateActivePage() {
		console.log("F5: Update active page")

		switch (tabBar.currentIndex) {
		case Globals.Page.START: {
			break;
		}
		case Globals.Page.LD: {
			//ldPage.updatePage()
			setStatusText(ldBackend.getLD_TextStatus())
			break;
		}
		case Globals.Page.LN: {
			ldBackend.updateDO_Table()
			setStatusText(ldBackend.getLN_TextStatus())
			break;
		}
		case Globals.Page.FS: {
			fsBackend.updateFilesDirectory("/")
			setStatusText(fsBackend.getFS_TextStatus())
			break;
		}
		case Globals.Page.DS: {
			break;
		}
		case Globals.Page.RCB: {
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
		ldBackend.sigCmdProgress.connect(slotOnProgress)
		ldBackend.sigCmdFinished.connect(slotOnFinished)

		fsBackend.sigCmdProgress.connect(slotOnProgress)
		fsBackend.sigCmdFinished.connect(slotOnFinished)

		// GUI
		lastConnPanel.sigDeviceSelected.connect(startPage.slotSetCurrentDevice)

		// Start status
		setStatusText(presenter.getAppVersion())
	}
}