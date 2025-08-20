#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QtCore>
#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QStandardPaths>
#include <QCommandLineParser>
#include <QIcon>
#include "autostartmanager.h"
#include "tooltipmanager.h"
#include "helper.h"
#include <QStyleHints>

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

QString readStringFromResource(const QString& resourcePath) {
    QFile file(resourcePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Could not open resource file:" << resourcePath;
        return QString();
    }
    QTextStream in(&file);
    return in.readLine().trimmed();
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

    // C++ Platform Detection
    // Get the platform name (e.g., "xcb" or "wayland").
    QString platform = QGuiApplication::platformName();
    engine.rootContext()->setContextProperty("platformName", platform);

    // Pass Version and Build Info to QML
    QString baseVersion = readStringFromResource(":/resources/version.conf");
    QString buildInfo = readStringFromResource(":/resources/build_info.conf");
    engine.rootContext()->setContextProperty("appVersion", baseVersion);
    engine.rootContext()->setContextProperty("appBuildInfo", buildInfo);

    // Register the Printer class as a QML singleton
    qmlRegisterSingletonType<Printer>("com.calendar.printer", 1, 0, "Printer", [](QQmlEngine *engine, QJSEngine *scriptEngine) -> QObject * {
        Q_UNUSED(engine)
        Q_UNUSED(scriptEngine)
        return new Printer();
    });

    engine.rootContext()->setContextProperty("autostartManager", &autostartManager);
    engine.rootContext()->setContextProperty("tooltipManager", &tooltipManager);

    const QUrl url(QStringLiteral("qrc:/qml/widget.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
                         if (!obj && url == objUrl)
                             QCoreApplication::exit(-1);
                     }, Qt::QueuedConnection);

    engine.load(url);
    // Tooltip style.  dark light mode detection removed for compatibility with smaller Qt6 versions.
    app.setStyleSheet(
        "QToolTip {"
        "    color: #000000;"
        "    background-color: #ffffff;"
        "}"
        );
    QCoreApplication::setApplicationVersion(baseVersion);

    // Command-line parsing
    QCommandLineParser parser;
    parser.setApplicationDescription("Nepali Calendar Application");
    parser.addHelpOption();
    parser.addVersionOption();   // --version and -v
    parser.process(app);
    return app.exec();
}
