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

QtObject {
	enum Page {
		START = 0,
		LD,
		LN,
		FS,
		DS,
		RCB,
		RCB_Reports
	}

	enum Panel {
		HIDE = 0,
		LAST_CONN,
		LD_INFO,
		RCB_PROPERTIES
	}

	function printObjectToConsole(item) {
		for (var p in item) {
			if (typeof item[p] != "function") {
				if (p != "objectName") {
					console.log(p + ":" + item[p]);
				}
			}
		}
	}

	function setSelectedRow(t_tableID, t_row) {
		if (t_tableID.currentRow === t_row) {
			return;
		}

		let idx = t_tableID.model.index(t_row, 0);
		t_tableID.selectionModel.setCurrentIndex(idx, ItemSelectionModel.Clear
													| ItemSelectionModel.Select
													| ItemSelectionModel.Rows);
	}

	function resizeColumnsToContent(t_headerID, t_tableID) {
		var iw = []
		let sum = 0, i = 0
		for (i=0;i<t_tableID.columns;i++) {
			iw[i] = Math.max(t_headerID.implicitColumnWidth(i), t_tableID.implicitColumnWidth(i))
			if (iw[i] < 0 || isNaN(iw[i])) {
				iw[i] = 1
			}
			sum = sum + iw[i]
		}
		if (sum === 0) {
			sum = 1
		}
		for (i=0;i<t_tableID.columns;i++) {
			let w = t_tableID.width * iw[i] / sum
			if (w < 0 || isNaN(w)) {
				w = 1
			}
			t_tableID.setColumnWidth(i, w)
		}
	}

	function calcColumnsWidth(t_headerID, t_tableID, t_column) {
		var iw = []
		let sum = 0, i = 0
		for (i=0;i<t_tableID.columns;i++) {
			iw[i] = Math.max(t_headerID.implicitColumnWidth(i), t_tableID.implicitColumnWidth(i))
			if (iw[i] < 0 || isNaN(iw[i])) {
				iw[i] = 1
			}
			sum = sum + iw[i]
		}
		if (sum === 0) {
			sum = 1
		}
		let w = (t_tableID.width * iw[t_column] / sum)
		if (w < 0 || isNaN(w)) {
			return 1
		}
		return w;
	}
}