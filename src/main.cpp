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
#include <QStandardPaths>
#include <QDir>
#include <QFont>
#include <QFontDatabase>

#include <QQuickStyle>
#include <iostream>

#include "app/main_presenter.hpp"

#if defined(BREAKPAD_LINUX)
#include "client/linux/handler/exception_handler.h"
namespace {
    bool crashCallback(const google_breakpad::MinidumpDescriptor &descriptor,
                       void *, bool succeeded) {
        fprintf(stderr, "Crash dump written to: %s\n", descriptor.path());
        return succeeded;
    }
}
#elif defined(BREAKPAD_WINDOWS)
#include "client/windows/handler/exception_handler.h"
namespace {
    bool crashCallback(const wchar_t *dumpPath, const wchar_t *minidumpId,
                       void *, EXCEPTION_POINTERS *, MDRawAssertionInfo *, bool succeeded) {
        fwprintf(stderr, L"Crash dump written to: %s\\%s.dmp\n", dumpPath, minidumpId);
        return succeeded;
    }
}
#endif

int main(int argc, char *argv[])
{
#ifdef BREAKPAD_ENABLED
    QString crashDir = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation)
                     + "/crashes";
    QDir().mkpath(crashDir);
#endif

#if defined(BREAKPAD_LINUX)
    google_breakpad::MinidumpDescriptor descriptor(crashDir.toStdString());
    google_breakpad::ExceptionHandler eh(descriptor, nullptr, crashCallback, nullptr,
                                          true, -1);
#elif defined(BREAKPAD_WINDOWS)
    google_breakpad::ExceptionHandler eh(crashDir.toStdWString(), nullptr, crashCallback,
                                          nullptr,
                                          google_breakpad::ExceptionHandler::HANDLER_ALL);
#endif

    QGuiApplication app(argc, argv);

    // Embed Noto Sans Mono as the application-wide font
    QFontDatabase::addApplicationFont(":/fonts/NotoSansMono-Regular.ttf");
    QFontDatabase::addApplicationFont(":/fonts/NotoSansMono-Bold.ttf");
    app.setFont(QFont("Noto Sans Mono", 10));

    QCoreApplication::setOrganizationName("OSI");
    QCoreApplication::setOrganizationDomain("opensource.org");
    QCoreApplication::setApplicationName("IEDClient");
    QThread::currentThread()->setObjectName("GUI");

    // Singletons
    qmlRegisterSingletonType(QUrl("qrc:/singletons/Globals.qml"), "GlobalVarsModule", 1, 0, "Globals");
    qmlRegisterSingletonType(QUrl("qrc:/singletons/VisualStyle.qml"), "AppStylesModule", 1, 0, "VisualStyle");

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
