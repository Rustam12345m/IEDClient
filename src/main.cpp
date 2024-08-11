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
#include <iostream>

#include "app/main_presenter.hpp"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QCoreApplication::setOrganizationName("OSI");
    QCoreApplication::setOrganizationDomain("opensource.org");
    QCoreApplication::setApplicationName("IEDClient");
    QThread::currentThread()->setObjectName("GUI");

    // Singletons
    qmlRegisterSingletonType(QUrl("qrc:/singletons/Globals.qml"), "GlobalVarsModule", 1, 0, "Globals");
    qmlRegisterSingletonType(QUrl("qrc:/singletons/ColorPalette.qml"), "AppStylesModule", 1, 0, "ColorPalette");

    App::MainPresenter presenter; // have to be created befor engine
    QQmlApplicationEngine engine;

    presenter.setQmlContextMembers(engine.rootContext());
    engine.load(QStringLiteral("qrc:/main.qml"));
    // Check if the root objects are empty (indicating a failure to load QML)
    if (engine.rootObjects().isEmpty()) {
        std::cerr << "Error: Failed to load QML file. Possible reasons could be missing QML plugins or modules." << std::endl;
        return -1;
    }
    return app.exec();
}