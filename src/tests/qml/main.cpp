#include <QtQuickTest/quicktest.h>

#include <QObject>
#include <QQmlComponent>
#include <QQmlContext>
#include <QQmlEngine>
#include <QUrl>

class TestSetup : public QObject
{
    Q_OBJECT
public slots:
    void qmlEngineAvailable(QQmlEngine *engine)
    {
        // Mirror production singleton registration so `import AppStylesModule`
        // and `import GlobalVarsModule` resolve the same way they do in
        // src/main.cpp.
        qmlRegisterSingletonType(
            QUrl("qrc:/singletons/Globals.qml"),
            "GlobalVarsModule", 1, 0, "Globals");
        qmlRegisterSingletonType(
            QUrl("qrc:/singletons/VisualStyle.qml"),
            "AppStylesModule", 1, 0, "VisualStyle");

        auto inject = [engine](const char *name, const QString &qrcPath) {
            auto *comp = new QQmlComponent(engine, QUrl(qrcPath));
            QObject *obj = comp->create();
            if (!obj) {
                qFatal("Mock load failed: %s -- %s",
                       qrcPath.toUtf8().constData(),
                       qPrintable(comp->errorString()));
            }
            // Parent to engine so LSan in --check doesn't flag a leak at exit.
            obj->setParent(engine);
            engine->rootContext()->setContextProperty(name, obj);
        };
        inject("iedBackend", "qrc:/mocks/MockIedBackend.qml");
        inject("appBackend", "qrc:/mocks/MockAppBackend.qml");
        inject("presenter",  "qrc:/mocks/MockPresenter.qml");
        inject("fsBackend",  "qrc:/mocks/MockFsBackend.qml");
    }
};

QUICK_TEST_MAIN_WITH_SETUP(qml_tests, TestSetup)

// TestSetup carries Q_OBJECT but is defined in a .cpp, so the moc output
// has to be pulled in here.
#include "main.moc"
