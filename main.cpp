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
    QCoreApplication::setOrganizationName("Nepdate");
    QCoreApplication::setApplicationName("Nepali Calendar");
    // ---------------------------------------------------------

    QQmlApplicationEngine engine;

    QUrl url;
    url = QUrl(QStringLiteral("qrc:/qml/main.qml"));
    engine.load(url);

    return app.exec();
}
