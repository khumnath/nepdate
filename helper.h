#ifndef HELPER_H
#define HELPER_H

#include <QObject>
#include <QQuickItem>
#include <QQuickWindow>

class Printer : public QObject
{
    Q_OBJECT

public:
    explicit Printer(QObject *parent = nullptr);
    Q_INVOKABLE void print(QQuickWindow* window);
    Q_INVOKABLE bool isSystemDark() const;
};

#endif // HELPER_H
