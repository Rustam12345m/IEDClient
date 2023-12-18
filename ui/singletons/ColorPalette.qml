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
pragma Singleton

import QtQuick

QtObject
{
	// Window
	property color backgroundColor1: 		"white" //#F0F0F0"
	property color backgroundColor2: 		"white" //EFEFEF"
	property color statusBarColor:	 		"#F0F0F0" //EFEFEF"
	property color borderColor: 			"#F0F0F0"//"#D9D9D9"//"#595959"
	property color textColor: 				"#0D0D0D"
	property color modalColor: 				"#A9A9A9"//"#D9D9D9"
	property int   borderWidth: 			4
	property int   delimeterWidth: 			6

	// ToolBar & TabBar
	property color toolBarColor: 			"#F0F0F0"//"#D9D9D9"

	// Tables
	property color tableHeaderColor: 		"white"//"#D9D9D9"//"#A9A9A9"
	property color tableRowBorderColor1: 	"#707070"
	property color tableRowBorderColor2: 	"#D9D9D9"
	property color selectedRowColor: 		"#A9A9A9"
	property color tableTextColor:			"#0D0D0D"
	property color tableRowColor1: 			"#EFEFEF"// "white"// #CCCCCC"
	property color tableRowColor2: 			"white"//#EFEFEF"

	// LD grid
	property color ldBorderColor: 			"lightgray"
	property color ldBorderSelectColor: 	"black"
	property color ldColor: 				"lightgray"
	property color ldColorSelect: 			"gray"
}