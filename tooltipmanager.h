#ifndef TOOLTIPMANAGER_H
#define TOOLTIPMANAGER_H

#include <QObject>
#include <QPoint>
#include <QQuickItem>

class TooltipManager : public QObject
{
    Q_OBJECT
public:
    explicit TooltipManager(QObject *parent = nullptr);
    Q_INVOKABLE void showText(const QPoint &pos, const QString &text);
    Q_INVOKABLE void hide();

    Q_INVOKABLE void showAboveOrBelow(QQuickItem *item, const QString &text);
};

#endif // TOOLTIPMANAGER_H
