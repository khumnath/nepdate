#ifndef AUTOSTARTMANAGER_H
#define AUTOSTARTMANAGER_H

#include <QObject>
#include <QSettings>
#include <QCoreApplication>
#include <QFile>
#include <QDir>

class AutostartManager : public QObject
{
    Q_OBJECT
public:
    explicit AutostartManager(QObject *parent = nullptr);
    Q_INVOKABLE bool isAutostartEnabled() const;
    Q_INVOKABLE void setAutostart(bool enabled);

private:
    const QString m_appName = "Nepali Calendar Widget";
    const QString m_autostartDir = QDir::homePath() + "/.config/autostart/";
    const QString m_desktopFile = m_autostartDir + m_appName + ".desktop";
};

#endif // AUTOSTARTMANAGER_H
