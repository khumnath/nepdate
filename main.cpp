#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QtCore>
#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QStandardPaths>
#include <QIcon>
#include "autostartmanager.h"
#include "tooltipmanager.h"
#include <cstdlib>

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

// Read version.txt from project root or known relative path
QString readVersionText() {
    QFile versionFile(":/version.txt");
    if (!versionFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Could not open version.txt";
        return "Unknown";
    }
    QTextStream in(&versionFile);
    QString version = in.readLine().trimmed();
    versionFile.close();
    return version;
}


int main(int argc, char *argv[]) {
    QCoreApplication::setOrganizationName("Nepdate");
    QCoreApplication::setOrganizationDomain("com.nepdate.calendar");
    QCoreApplication::setApplicationName("NepaliCalendar");

    QApplication app(argc, argv);
    app.setWindowIcon(QIcon(":/resources/flag.png"));
    app.setDesktopFileName("nepdate-calendar");

    // Ensure .desktop file is created or Exec= line updated
    ensureDesktopFile("nepdate-calendar.desktop", "NepaliCalendar");

    AutostartManager autostartManager;
    TooltipManager tooltipManager;
    QQmlApplicationEngine engine;

    // --- C++ Platform Detection ---
    // Grt the platform name (e.g., "xcb" or "wayland").
    QString platform = QGuiApplication::platformName();
    engine.rootContext()->setContextProperty("platformName", platform);

    // --- App version pass to qml
    QString appVersion = readVersionText();
    engine.rootContext()->setContextProperty("appVersion", appVersion);



    engine.rootContext()->setContextProperty("autostartManager", &autostartManager);
    engine.rootContext()->setContextProperty("tooltipManager", &tooltipManager);

    const QUrl url(QStringLiteral("qrc:/qml/widget.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
                         if (!obj && url == objUrl)
                             QCoreApplication::exit(-1);
                     }, Qt::QueuedConnection);

    engine.load(url);
    return app.exec();
}
