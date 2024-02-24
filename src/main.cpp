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

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include <QQuickStyle>

#include "app/main_presenter.hpp"

int main(int argc, char *argv[])
{
	QGuiApplication app(argc, argv);

	QCoreApplication::setOrganizationName("OSI");
	QCoreApplication::setOrganizationDomain("opensource.org");
	QCoreApplication::setApplicationName("IEDClient");

	// Singletons
	qmlRegisterSingletonType(QUrl("qrc:/singletons/Globals.qml"), "GlobalVarsModule", 1, 0, "Globals");
	qmlRegisterSingletonType(QUrl("qrc:/singletons/ColorPalette.qml"), "AppStylesModule", 1, 0, "ColorPalette");

	App::MainPresenter presenter; // have to be created befor engine
	QQmlApplicationEngine engine;

	presenter.setQmlContextMembers(engine.rootContext());

	engine.load(QStringLiteral("qrc:/main.qml"));

	QThread::currentThread()->setObjectName("GUI");
	return app.exec();
}