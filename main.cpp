#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QtCore>
#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QStandardPaths>
#include <QIcon>
#include "autostartmanager.h"

void ensureDesktopFile(const QString &desktopFileName, const QString &startupWMClass) {
    QString dirPath = QStandardPaths::writableLocation(QStandardPaths::ApplicationsLocation);
    QDir().mkpath(dirPath);

    // Hack to show calendar icon for calendar window
    QString desktopFilePath = dirPath + "/" + desktopFileName;
    QFile desktopFile(desktopFilePath);

    if (desktopFile.exists()) {
        if (desktopFile.open(QIODevice::ReadWrite | QIODevice::Text)) {
            QString content = desktopFile.readAll();
            desktopFile.resize(0); // clear contents
            QTextStream out(&desktopFile);

            QString updatedContent;
            for (const QString &line : content.split('\n')) {
                if (line.startsWith("Exec=")) {
                    updatedContent += "Exec=" + QCoreApplication::applicationFilePath() + "\n";
                } else {
                    updatedContent += line + "\n";
                }
            }

            out << updatedContent.trimmed();
            desktopFile.close();
        } else {
            qWarning() << "Couldn't update desktop file:" << desktopFilePath;
        }
    } else {
        if (desktopFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&desktopFile);
            out << "[Desktop Entry]\n";
            out << "Categories=Utility;Calendar;\n";
            out << "Comment=Nepali Calendar Application\n";
            out << "Icon=calendar\n";
            out << "Exec=" << QCoreApplication::applicationFilePath() << "\n";
            out << "Name=Nepdate Calendar\n";
            out << "StartupNotify=true\n";
            out << "StartupWMClass=" << startupWMClass << "\n";
            out << "Terminal=false\n";
            out << "Type=Application\n";
            out << "NoDisplay=true\n";
            desktopFile.close();
        } else {
            qWarning() << "Couldn't create desktop file:" << desktopFilePath;
        }
    }
}

int main(int argc, char *argv[]) {
    QCoreApplication::setOrganizationName("Nepdate");
    QCoreApplication::setOrganizationDomain("com.nepdate.calendar");
    QCoreApplication::setApplicationName("NepaliCalendar");

    QGuiApplication app(argc, argv);
    app.setWindowIcon(QIcon(":/resources/flag.png"));
    app.setDesktopFileName("nepdate-calendar");

    // Ensure .desktop file is created or Exec= line updated
    ensureDesktopFile("nepdate-calendar.desktop", "NepaliCalendar");

    AutostartManager autostartManager;
    QQmlApplicationEngine engine;

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
