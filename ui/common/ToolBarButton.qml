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

Item {
	property alias icon: btn.icon.source
	property alias prompt: toolTip.text
	property alias text: btn.text

	signal sigClicked()

	anchors.verticalCenter: parent.verticalCenter

	Button {
		id: btn
		anchors.fill: parent
		focus: false
		focusPolicy: Qt.NoFocus

		onClicked: function() {
			sigClicked()
		}
	}
	ToolTip {
		id: toolTip
		text: "Information isn't found"
		delay: 150
		timeout: 1500
		visible: btn.hovered
	}
}