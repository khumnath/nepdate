#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QUrl>
#include <QDir>
#include <QFile>
#include <QCoreApplication>
#include <QDebug>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    QUrl url;

#ifdef QT_DEBUG
    QString sourceDirQmlPath = QString(QML_SOURCES_DIR) + "/main.qml";

    if (QFile::exists(sourceDirQmlPath)) {
        url = QUrl::fromLocalFile(sourceDirQmlPath);
        qDebug() << "Loading QML from local file:" << url.toString();
    } else {
        url = QUrl(QStringLiteral("qrc:/qml/main.qml"));
        qDebug() << "Could not find local QML file at:" << sourceDirQmlPath;
        qDebug() << "Loading QML from embedded resource:" << url.toString();
    }
#else
    url = QUrl(QStringLiteral("qrc:/qml/main.qml"));
#endif

    const QUrl finalUrl = url;

    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
        &app, [finalUrl](QObject *obj, const QUrl &objUrl) {
            if (!obj && finalUrl == objUrl)
                QCoreApplication::exit(-1);
        }, Qt::QueuedConnection);

    engine.load(finalUrl);

    return app.exec();
}
