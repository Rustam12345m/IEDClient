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

Window {
	title: qsTr("IEDClient - Open source client for IEC 61850")

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
			statusText.text = Qt.formatDateTime(new Date(), "hh:mm:ss") + " Uptime: " + tick
		}
	}

	GlobalProgressBar {
		anchors.centerIn: parent
		id: globalProgressBar
	}

	Rectangle {
		id: mainBack
		anchors.fill: parent

		// Area for Menu + ToolBar
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
				anchors.fill: parent
				width: parent.width
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
							}
							MenuItem {
								text: qsTr("Disconnect")
							}
							MenuItem {
								text: qsTr("Settings")
							}
							MenuSeparator {
							}
							MenuItem {
								text: qsTr("Exit")
								onTriggered: {
									console.log("Bye - bye. Exit")
									Qt.quit()
								}
							}
						}
						Menu {
							title: qsTr("&Tools")
							MenuItem {
								text: qsTr("Save model")
							}
							MenuItem {
								text: qsTr("Check SCL")
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
							}
						}
						Menu {
							title: qsTr("&Program")
							MenuItem {
								text: qsTr("Documentation")
							}
							MenuSeparator {
							}
							MenuItem {
								text: qsTr("About")
								onTriggered: {
									var aboutComponent = Qt.createComponent("AboutProgram.qml")
									var aboutWindow = aboutComponent.createObject(rootWindow)
									aboutWindow.show()
								}
							}
						}
					}
				}

				// ToolBar
				Item {
					id: toolBar
					width: toolBarRow.implicitWidth
					height: parent.height
					//anchors.verticalCenter: parent.verticalCenter

					Row {
						id: toolBarRow
						anchors.fill: parent

						//anchors.horizontalCenter: parent.horizontalCenter
        				//anchors.verticalCenter: parent.verticalCenter
						spacing: 5

						Button {
							text: "B1"
							width: 30
							height: 30

							anchors.verticalCenter: parent.verticalCenter

							onClicked: function() {
								console.log("Clicked: " + text)
							}
						}
						Rectangle {
							id: myCustonBtn

							width: 30
							height: width

							anchors.verticalCenter: parent.verticalCenter

							border.width: 1
							border.color: "black"
							color: "white"
							radius: 5

							Label {
								anchors.centerIn: parent
								text: "B4"
								color: "black"
							}

							MouseArea {
								anchors.fill: parent
								acceptedButtons: Qt.LeftButton

								onClicked: function(m) {
									console.log("My custom btn: " + m)
								}
								onPressed: function(m) {
									myCustonBtn.color = "gray"
								}
								onReleased: function(m) {
									myCustonBtn.color = "white"
								}
							}
						}
						Button {
							text: "Disconnect"
							anchors.verticalCenter: parent.verticalCenter

							onClicked: function() {
								console.log("Clicked: " + text)
							}
						}
						Button {
							text: "Update"
							anchors.verticalCenter: parent.verticalCenter

							onClicked: function() {
								console.log("Clicked: " + text)
							}
						}
						Item {
							width: 150
							height: toolBar.height

							Row {
								anchors.verticalCenter: parent.verticalCenter

								Label {
									text: "FC: "
								}
								ComboBox {
									width: 150
									enabled: lnPage.visible

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

					width: 10
					height: parent.height
				}

				// Status
				Rectangle {
					color: "black"
					//border.width: 1
					//border.color: "black"

					height: parent.height
					width: Math.max(200, statusText.implicitWidth + 10)

					Text {
						id: statusText
						anchors.centerIn: parent
						color: "white"

						text: " - "
					}
				}
			}
		}

		// WorkArea
		Rectangle {
			id: workArea

			//border.width: 1
			//border.color: "black"
			//color: "lightgray"
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

				// Tabs Area
				Item {
					id: tabsArea
					SplitView.preferredWidth: 750
					SplitView.minimumWidth: 600
					SplitView.fillWidth: true

					// Tabs
					StackLayout {
						id: mainStack

						anchors {
							top: tabsArea.top
							left: tabsArea.left
							right: tabsArea.right
							bottom: mainTabBar.top
						}

						currentIndex: tabBar.currentIndex

						Start_Page {
							focus: true

							onVisibleChanged: {
								if (visible) {
									focus = true
								}
							}
							onNextPageSignal: function(page) {
								//console.log("Connect_Page: " + page)

								mainStack.changePage(page)
							}
						}

						LD_Page {
							id: ldPage

							onVisibleChanged: {
								if (visible) {
									//globalProgressBar.startLoad()
								}
							}
							onSigNextPageSignal: function(page) {
								//console.log("LD: " + page)
								mainStack.changePage(page)
							}
							onSigLDeviceChanged: function(current) {
								console.log("LN: Current LD changed to " + current)
								lnPage.updateLDeviceIndex(current)
							}
						}

						LN_Page {
							id: lnPage

							onVisibleChanged: {
								if (visible) {
									//globalProgressBar.startLoad()
									propertyPanel.SplitView.preferredWidth = 0
								}
							}
						}

						FS_Page {
							onVisibleChanged: {
								if (visible) {
									//globalProgressBar.startLoad()
									mainPres.viewFilesDirectory("/")
								}
							}
						}

						RCB_Page {
							onVisibleChanged: {
								if (visible) {
									//globalProgressBar.startLoad()
								}
							}
						}

						DS_Page {
							onVisibleChanged: {
								if (visible) {
									//globalProgressBar.startLoad()
								}
							}
						}

						function changePage(page) {
							console.log("ActivatePage: new index = " + page)

							switch (page) {
							case Enum.Page.START: {
								tabBar.currentIndex = 0;
								break;
							}
							case Enum.Page.LD: {
								tabBar.currentIndex = 1;
								break;
							}
							case Enum.Page.LN: {
								tabBar.currentIndex = 2;
								break;
							}
							case Enum.Page.FS: {
								tabBar.currentIndex = 3;
								break;
							}
							case Enum.Page.DS: {
								tabBar.currentIndex = 4;
								break;
							}
							case Enum.Page.RCB: {
								tabBar.currentIndex = 5;
								break;
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
								//KeyBlocker.blockKeys(tabBar);
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
								text: qsTr("RCB")
							}
							TabButton {
								text: qsTr("DataSets")
							}
						}
					}
				}

				// Property panel
				PropertyPanel {
					id: propertyPanel
					width: 100

					SplitView.fillWidth: false
					SplitView.preferredWidth: 250

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
					mainStack.changePage(Enum.Page.START)
					event.accepted = true
					break;
				}
				case Qt.Key_2: {
					mainStack.changePage(Enum.Page.LD)
					event.accepted = true
					break;
				}
				case Qt.Key_3: {
					mainStack.changePage(Enum.Page.LN)
					event.accepted = true
					break;
				}
				case Qt.Key_4: {
					mainStack.changePage(Enum.Page.FS)
					event.accepted = true
					break;
				}
				case Qt.Key_5: {
					mainStack.changePage(Enum.Page.DS)
					event.accepted = true
					break;
				}
				case Qt.Key_6: {
					mainStack.changePage(Enum.Page.RCB)
					event.accepted = true
					break;
				}
				}
			}
		}
	}

	function openEventLog() {
		var logsComponent = Qt.createComponent("EventsViewer.qml")
		var logsWindow = logsComponent.createObject(rootWindow)
		logsWindow.show()
	}

	function slotMySignal(msg) {
		console.log(msg)
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
		appCore.mySignal.connect(slotMySignal)
		mainPres.sigProgress.connect(slotOnProgress)
		mainPres.sigFinished.connect(slotOnFinished)
	}
}
