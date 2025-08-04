// helper.cpp

#include "helper.h"
#include <QPrinter>
#include <QPainter>
#include <QPrintDialog>
#include <QPixmap>
#include <QDebug>
#include <QImage>
#include <QQuickItemGrabResult>
#include <QSharedPointer>
Printer::Printer(QObject *parent) : QObject(parent)
{

}

void Printer::print(QQuickWindow* window)
{
    if (!window) {
        qWarning() << "Error: Window is null.";
        return;
    }

    window->setProperty("isPrintMode", true);
    window->contentItem()->update();
    window->update();
    QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
    QImage image = window->grabWindow();
    window->setProperty("isPrintMode", false);

    if (image.isNull()) {
        qWarning() << "Error: Failed to grab window content.";
        return;
    }

    QPrinter printer(QPrinter::HighResolution);
    QPrintDialog dialog(&printer, nullptr);
    if (dialog.exec() != QDialog::Accepted) {
        return;
    }

    QPainter painter(&printer);
    if (!painter.isActive()) {
        qWarning() << "Error: Failed to activate painter for printer.";
        return;
    }

    QRectF pageRect = printer.pageRect(QPrinter::DevicePixel);
    QPixmap pixmap = QPixmap::fromImage(image);
    QSize imageSize = pixmap.size();
    imageSize.scale(pageRect.size().toSize(), Qt::KeepAspectRatio);

    int x = pageRect.x() + (pageRect.width() - imageSize.width()) / 2;
    int y = pageRect.y() + (pageRect.height() - imageSize.height()) / 2;

    painter.drawPixmap(QRect(x, y, imageSize.width(), imageSize.height()), pixmap);
    painter.end();
}
