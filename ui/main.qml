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
import "qrc:/start/" as Start
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

			z: 100500
			border.width: 1
			border.color: "black"
			color: "lightgray"

			height: myMenuBar.implicitHeight + 2
			anchors {
				top: mainBack.top
				left: mainBack.left
				right: mainBack.right
			}

			// Menu + ToolBar
			RowLayout {
				anchors {
					fill: parent
					leftMargin: 0
					rightMargin: 5
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
					id: toolBar
					readonly property int btnHeight: 30

					width: toolBarRow.implicitWidth
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

						// FC selector
						Item {
							width: 150
							height: toolBar.height

							Row {
								anchors.fill: parent

								Label {
									anchors.verticalCenter: parent.verticalCenter

									text: "FC: "
								}
								ComboBox {
									id: comboBox
									anchors.verticalCenter: parent.verticalCenter
									height: toolBar.btnHeight

									enabled: lnPage.visible
									implicitContentWidthPolicy: ComboBox.WidestText

									model: ListModel {
										ListElement { text: "State(ST, MX)" }
										ListElement { text: "Control(CO)" }
										ListElement { text: "Description(DO)" }
									}

									onActivated: function(index) {
										console.log("ComboBox: Activate inx = " + index);
									}
								}
							}
						}
					}
				}

				// Spacer
				Item {
					Layout.fillWidth: true

					width: 20
					height: toolBar.height
				}

				// Status
				Rectangle {
					height: toolBar.btnHeight
					width: Math.max(250, statusTextBox.implicitWidth + 20)

					border.width: 1
					border.color: "gray"

					color: "white"// "lightgray" //"black"
					clip: true

					Text {
						id: statusTextBox
						anchors.fill: parent

						horizontalAlignment: Text.AlignHCenter
						verticalAlignment: Text.AlignVCenter

						color: "black" //"white"
						text: ""
						font.bold: true
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
						clip: true

						anchors {
							top: tabsArea.top
							left: tabsArea.left
							right: tabsArea.right
							bottom: mainTabBar.top
						}

						currentIndex: tabBar.currentIndex

						Start.Page {
							id: startPage
							focus: true

							onVisibleChanged: {
								if (visible) {
									focus = true
								}
							}
							onNextPageSignal: function(page) {
								rootWindow.setActivePage(page)
							}
						}

						LD.Page {
							id: ldPage

							onVisibleChanged: {
								if (visible) {
									hidePropertyPanel()
								}
							}
							onSigNextPageSignal: function(page) {
								rootWindow.setActivePage(page)
							}
							onSigLDeviceChanged: function(index, name) {
								//console.log("LN: Current LD changed to " + current)
								lnPage.updateLDeviceIndex(index)
								setStatusText(name)
							}
						}

						LN.Page {
							id: lnPage

							onVisibleChanged: {
								if (visible) {
									hidePropertyPanel()
								}
							}
						}

						FS.Page {
							onVisibleChanged: {
								if (visible) {
									mainPres.updateFilesDirectory("/")
									hidePropertyPanel()
								}
							}
						}

						RCB.Page {
							onVisibleChanged: {
								if (visible) {
								}
							}
						}

						DS.Page {
							onVisibleChanged: {
								if (visible) {
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

							Component.onCompleted: {
							}

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
								text: qsTr("File Browser")
							}
							TabButton {
								text: qsTr("DataSets")
								enabled: false
							}
							TabButton {
								text: qsTr("RCB")
								enabled: false
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
						anchors.fill: parent

						Start.LastConnTable {
							id: lastConnPanel
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
			if (event.key == Qt.Key_F5) {
				console.log("Update data by F5")
			}

			// Alt
			if (event.modifiers & Qt.AltModifier) {
				switch (event.key) {
				case Qt.Key_1: {
					rootWindow.setActivePage(Globals.Page.START)
					event.accepted = true
					break;
				}
				case Qt.Key_2: {
					rootWindow.setActivePage(Globals.Page.LD)
					event.accepted = true
					break;
				}
				case Qt.Key_3: {
					rootWindow.setActivePage(Globals.Page.LN)
					event.accepted = true
					break;
				}
				case Qt.Key_4: {
					rootWindow.setActivePage(Globals.Page.FS)
					event.accepted = true
					break;
				}
				case Qt.Key_5: {
					rootWindow.setActivePage(Globals.Page.DS)
					event.accepted = true
					break;
				}
				case Qt.Key_6: {
					rootWindow.setActivePage(Globals.Page.RCB)
					event.accepted = true
					break;
				}
				}
			}
		}
	}

	// Common functions
	function hidePropertyPanel() {
		propertyPanel.SplitView.preferredWidth = 0
	}

	function openEventLog() {
		var logsComponent = Qt.createComponent("global/EventsViewer.qml")
		var logsWindow = logsComponent.createObject(rootWindow)
		logsWindow.show()
	}

	function setStatusText(msg) {
		statusTextBox.text = msg
	}

	function setActivePage(page) {
		//console.log("ActivatePage: new index = " + page)

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
		}
	}

	function updateActivePage() {
		console.log("F5: Update active page")

		switch (tabBar.currentIndex) {
		case Globals.Page.START: {
			break;
		}
		case Globals.Page.LD: {
			break;
		}
		case Globals.Page.LN: {
			break;
		}
		case Globals.Page.FS: {
			mainPres.updateFilesDirectory("/")
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

	function slotOnProgress(t_perc, t_msg) {
		if (!globalProgressBar.isActive()) {
			globalProgressBar.startLoad()
		}
		globalProgressBar.updateLoad(t_perc, t_msg)
	}

	function slotOnFinished() {
		globalProgressBar.finishLoad()
	}

	Component.onCompleted: function() {
		// App to GUI
		mainPres.sigProgress.connect(slotOnProgress)
		mainPres.sigFinished.connect(slotOnFinished)

		// GUI
		lastConnPanel.sigDeviceSelected.connect(startPage.slotSetCurrentDevice)
	}
}