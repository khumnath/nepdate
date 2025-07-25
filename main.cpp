#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QtCore>
#include "autostartmanager.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setOrganizationName("Nepdate");
    QCoreApplication::setApplicationName("Nepali Calendar");
    QGuiApplication app(argc, argv);

    // autostart manager
    AutostartManager autostartManager;
    QQmlApplicationEngine engine;

    // Expose the C++ object to QML under the name "autostartManager"
    engine.rootContext()->setContextProperty("autostartManager", &autostartManager);

    const QUrl url(QStringLiteral("qrc:/qml/widget.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
        &app, [url](QObject *obj, const QUrl &objUrl) {
            if (!obj && url == objUrl)
                QCoreApplication::exit(-1);
        }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
