#include "tooltipmanager.h"
#include <QToolTip>
#include <QGuiApplication>
#include <QTimer>
#include <QQuickWindow>
#include <QFontMetrics>
#include <QQmlContext>
#include <QQuickView>

TooltipManager::TooltipManager(QObject *parent) : QObject(parent) {}

void TooltipManager::showText(const QPoint &pos, const QString &text)
{
    QQuickWindow *window = qobject_cast<QQuickWindow *>(QGuiApplication::focusWindow());
    if (!window) return;
    QQuickItem *tooltipItem = new QQuickItem(window->contentItem());
    tooltipItem->setParentItem(window->contentItem());
    tooltipItem->setWidth(200);
    tooltipItem->setHeight(50);

    QQuickItem *textItem = new QQuickItem(tooltipItem);
    textItem->setParentItem(tooltipItem);
    textItem->setWidth(tooltipItem->width());
    textItem->setHeight(tooltipItem->height());
    textItem->setProperty("text", text);
    tooltipItem->setPosition(pos);
    tooltipItem->setParentItem(window->contentItem());
    tooltipItem->setProperty("transientParent", QVariant::fromValue(window));
}

void TooltipManager::hide()
{
    QToolTip::hideText();
}

void TooltipManager::showAboveOrBelow(QQuickItem *item, const QString &text)
{
    if (!item || !item->window() || text.isEmpty()) {
        return;
    }

    QQuickWindow *window = item->window();
    QQuickItem *content = window->contentItem();

    QScreen *screen = window->screen();
    if (!screen) {
        screen = QGuiApplication::primaryScreen();
    }
    if (!screen) return;

    QFontMetrics metrics(QToolTip::font());
    QRect textRect = metrics.boundingRect(QRect(), Qt::AlignJustify, text);

    const qreal itemCenterY = window->y() + item->mapToItem(content, 0, item->height() / 2.0).y();
    const qreal screenCenterY = screen->geometry().y() + screen->geometry().height() / 2.0;

    QPoint screenPos;

    if (itemCenterY < screenCenterY) {
        QPointF localPoint(item->width() / 2.0, item->height());
        QPointF windowPoint = item->mapToItem(content, localPoint);
        screenPos = window->position() + windowPoint.toPoint();
        screenPos.ry() -= 10;
        screenPos.rx() -= (textRect.width() / 2);
    } else {
        QPointF localPoint(item->width() / 2.0, 0);
        QPointF windowPoint = item->mapToItem(content, localPoint);
        screenPos = window->position() + windowPoint.toPoint();
        screenPos.ry() -= (textRect.height() + 10);
        screenPos.rx() -= (textRect.width() / 2);
    }

    QToolTip::showText(screenPos, text);
}
