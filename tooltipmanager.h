#ifndef TOOLTIPMANAGER_H
#define TOOLTIPMANAGER_H

#include <QObject>
#include <QPoint>

class TooltipManager : public QObject
{
    Q_OBJECT
public:
    explicit TooltipManager(QObject *parent = nullptr);

    // This function will be callable from QML.
    // It takes screen coordinates (QPoint) and the text to display.
    Q_INVOKABLE void showText(const QPoint &pos, const QString &text);

    // This function will be used to hide the tooltip.
    Q_INVOKABLE void hide();
};

#endif // TOOLTIPMANAGER_H
