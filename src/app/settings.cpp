/*
 *  main.cpp
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

#include "settings.hpp"

QList<DevConInfo> AppSettings::getDevConList()
{
	QList<DevConInfo> devs;

	QSettings ini;
	int size = ini.beginReadArray("devices");
	for (int i=0;i<size;i++) {
		ini.setArrayIndex(i);

		devs.emplace_back(ini.value("name").toString(), ini.value("ip").toString(), ini.value("port").toInt());
	}
	ini.endArray();

	return devs;
}

void AppSettings::saveNewDevCon(const DevConInfo &t_dev)
{
	QList<DevConInfo> devs = getDevConList();
	for (auto &d : devs) {
		if (d == t_dev) {
			return;
		}
	}
	devs.push_front(t_dev);

	QSettings ini;
	ini.beginWriteArray("devices");
	for (int i=0;(i < devs.size()) && (i < SaveDevsHistoryLen);i++) {
		ini.setArrayIndex(i);

		ini.setValue("name", devs[i].name());
		ini.setValue("ip", devs[i].ip());
		ini.setValue("port", devs[i].port());
	}
	ini.endArray();
}