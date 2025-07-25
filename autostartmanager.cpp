#include "autostartmanager.h"
#include <QTextStream>

AutostartManager::AutostartManager(QObject *parent) : QObject(parent)
{
}

bool AutostartManager::isAutostartEnabled() const
{
#ifdef Q_OS_WIN
    QSettings registrySettings(m_autostartKey, QSettings::NativeFormat);
    return registrySettings.contains(m_appName);
#else
    QFile desktopFile(m_desktopFile);
    return desktopFile.exists();
#endif
}

void AutostartManager::setAutostart(bool enabled)
{
    if (enabled) {
        QDir().mkpath(m_autostartDir);
        QFile desktopFile(m_desktopFile);
        if (desktopFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&desktopFile);
            out << "[Desktop Entry]\n";
            out << "Type=Application\n";
            out << "Name=Nepali Calendar Widget\n";
            out << "Exec=" << QCoreApplication::applicationFilePath() << "\n";
            out << "Icon=calendar\n";
            out << "X-GNOME-Autostart-enabled=true\n";
            desktopFile.close();
        }
    } else {
        QFile::remove(m_desktopFile);
    }
}
