/*
 *  main.cpp
 *
 *  Copyright 2023-2023 Rustam Mustafin
 *
 *  This file is part of IEDMaster.
 *
 *  IEDMaster is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  IEDMaster is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with IEDMaster.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  See COPYING file for the complete license text.
 * */

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "app_core.h"
#include "main_presenter.h"

int main(int argc, char *argv[])
{
	QGuiApplication app(argc, argv);

	AppCore appCore;
	MainPresenter mainPres(appCore);

	QQmlApplicationEngine engine;
	QQmlContext *context = engine.rootContext();
	context->setContextProperty("appCore", &appCore); // acore
	context->setContextProperty("mainPres", &mainPres); // mpres

	engine.load(QStringLiteral("qrc:/main.qml"));
	return app.exec();
}
