#include "tooltipmanager.h"
#include <QToolTip>

TooltipManager::TooltipManager(QObject *parent) : QObject(parent)
{
}

void TooltipManager::showText(const QPoint &pos, const QString &text)
{
    // Use the static QToolTip function to show the text at the global screen position.
    QToolTip::showText(pos, text);
}

void TooltipManager::hide()
{
    // Use the static QToolTip function to hide any visible tooltip.
    QToolTip::hideText();
}
