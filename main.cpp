#include "autostartmanager.h"
#include "helper.h"
#include "tooltipmanager.h"
#include "src/panchangacalculator.h"
#include <QApplication>
#include <QCommandLineParser>
#include <QDir>
#include <QFile>
#include <QIcon>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickWindow>
#include <QStandardPaths>
#include <QStyleHints>
#include <QTextStream>
#include <QtCore>
#include <QLibrary>

void ensureDesktopFile(const QString &desktopFileName,
                       const QString &startupWMClass) {
  QString dirPath =
      QStandardPaths::writableLocation(QStandardPaths::ApplicationsLocation);
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
          updatedContent +=
              "Exec=\"" + QCoreApplication::applicationFilePath() + "\"\n";
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
      out << "Exec=\"" << QCoreApplication::applicationFilePath() << "\"\n";
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

QString readStringFromResource(const QString &resourcePath) {
  QFile file(resourcePath);
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
    qDebug() << "Could not open resource file:" << resourcePath;
    return QString();
  }
  QTextStream in(&file);
  return in.readLine().trimmed();
}

bool checkX11Compositor() {
  QLibrary x11Lib("X11");
  if (!x11Lib.load()) {
    x11Lib.setFileName("libX11.so.6");
    if (!x11Lib.load()) {
      x11Lib.setFileName("libX11.so");
      if (!x11Lib.load()) {
        return false;
      }
    }
  }

  typedef void* (*XOpenDisplay_fn)(const char*);
  typedef int (*DefaultScreen_fn)(void*);
  typedef unsigned long (*XInternAtom_fn)(void*, const char*, int);
  typedef unsigned long (*XGetSelectionOwner_fn)(void*, unsigned long);
  typedef int (*XCloseDisplay_fn)(void*);

  auto xOpenDisplay = (XOpenDisplay_fn)x11Lib.resolve("XOpenDisplay");
  auto defaultScreen = (DefaultScreen_fn)x11Lib.resolve("DefaultScreen");
  auto xInternAtom = (XInternAtom_fn)x11Lib.resolve("XInternAtom");
  auto xGetSelectionOwner = (XGetSelectionOwner_fn)x11Lib.resolve("XGetSelectionOwner");
  auto xCloseDisplay = (XCloseDisplay_fn)x11Lib.resolve("XCloseDisplay");

  if (!xOpenDisplay || !defaultScreen || !xInternAtom || !xGetSelectionOwner || !xCloseDisplay) {
    return false;
  }

  void* display = xOpenDisplay(nullptr);
  if (!display) {
    return false;
  }

  int screen = defaultScreen(display);
  QString atomName = QString("_NET_WM_CM_S%1").arg(screen);
  unsigned long managerAtom = xInternAtom(display, atomName.toUtf8().constData(), 1); // True means only if exists

  bool hasCompositor = false;
  if (managerAtom != 0) { // None in X11 is 0
    unsigned long owner = xGetSelectionOwner(display, managerAtom);
    if (owner != 0) {
      hasCompositor = true;
    }
  }

  xCloseDisplay(display);
  return hasCompositor;
}

bool isCompositorInProc() {
  QDir procDir("/proc");
  QStringList filters;
  filters << "[0-9]*";
  procDir.setNameFilters(filters);
  procDir.setFilter(QDir::Dirs);

  for (const QString &pidStr : procDir.entryList()) {
    QFile commFile(QString("/proc/%1/comm").arg(pidStr));
    if (commFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
      QString name = commFile.readAll().trimmed();
      commFile.close();
      if (name == "kwin_x11" || name == "kwin" || name == "picom" ||
          name == "compton" || name == "mutter" || name == "xfwm4" ||
          name == "compiz" || name == "deepin-wm" || name == "deepin-kwin" ||
          name == "dde-kwin") {
        return true;
      }
    }
  }
  return false;
}

bool checkCompositorSupportsTransparency() {
  QString platform = QGuiApplication::platformName();
  if (platform == "wayland" || platform == "wayland-egl") {
    return true;
  }
  if (platform == "cocoa" || platform == "windows") {
    return true;
  }
  if (platform == "xcb" || platform == "dxcb") {
    return checkX11Compositor() || isCompositorInProc();
  }
  return true; // Default fallback
}

int main(int argc, char *argv[]) {
  QCoreApplication::setOrganizationName("Nepdate");
  QCoreApplication::setOrganizationDomain("com.nepdate.calendar");
  QCoreApplication::setApplicationName("NepaliCalendar");

  QQuickWindow::setDefaultAlphaBuffer(true);

  QApplication app(argc, argv);
  app.setWindowIcon(QIcon(":/resources/flag.png"));
  app.setDesktopFileName("nepdate-calendar");

  // Ensure .desktop file is created or Exec= line updated
  ensureDesktopFile("nepdate-calendar.desktop", "NepaliCalendar");

  AutostartManager autostartManager;
  TooltipManager tooltipManager;
  // Command-line parsing must happen before QML engine load so --help works quickly
  QCommandLineParser parser;
  parser.setApplicationDescription("Nepali Calendar Application");
  parser.addHelpOption();
  parser.addVersionOption(); // --version and -v
  parser.process(app);

  QQmlApplicationEngine engine;

  // C++ Platform Detection
  // Get the platform name (e.g., "xcb" or "wayland").
  QString platform = QGuiApplication::platformName();
  engine.rootContext()->setContextProperty("platformName", platform);

  bool compositorSupportsTransparency = checkCompositorSupportsTransparency();
  engine.rootContext()->setContextProperty("compositorSupportsTransparency", compositorSupportsTransparency);

  // Pass Version and Build Info to QML
  QString baseVersion = readStringFromResource(":/resources/version.conf");
  QString buildInfo = readStringFromResource(":/resources/build_info.conf");
  engine.rootContext()->setContextProperty("appVersion", baseVersion);
  engine.rootContext()->setContextProperty("appBuildInfo", buildInfo);

  // Register the Printer class as a QML singleton
  qmlRegisterSingletonType<Printer>(
      "com.calendar.printer", 1, 0, "Printer",
      [](QQmlEngine *engine, QJSEngine *scriptEngine) -> QObject * {
        Q_UNUSED(engine)
        Q_UNUSED(scriptEngine)
        return new Printer();
      });

  engine.rootContext()->setContextProperty("autostartManager",
                                           &autostartManager);
  engine.rootContext()->setContextProperty("tooltipManager", &tooltipManager);

  PanchangaCalculator panchangaCalculator;
  engine.rootContext()->setContextProperty("PanchangaNative", &panchangaCalculator);

  const QUrl url(QStringLiteral("qrc:/qml/widget.qml"));
  QObject::connect(
      &engine, &QQmlApplicationEngine::objectCreated, &app,
      [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
          QCoreApplication::exit(-1);
      },
      Qt::QueuedConnection);

  engine.load(url);

  // Set translucent background on the widget window for platforms (like Deepin's
  // dxcb) that require the widget attribute to be set from C++.
  for (QObject *obj : engine.rootObjects()) {
      QQuickWindow *window = qobject_cast<QQuickWindow *>(obj);
      if (window) {
          window->setColor(Qt::transparent);
      }
  }
  // Tooltip style.  dark light mode detection removed for compatibility with
  // smaller Qt6 versions.
  app.setStyleSheet("QToolTip {"
                    "    color: #000000;"
                    "    background-color: #ffffff;"
                    "}");
  QCoreApplication::setApplicationVersion(baseVersion);

  return app.exec();
}
