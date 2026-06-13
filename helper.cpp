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
#include <QGuiApplication>
#include <QPalette>
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

bool Printer::isSystemDark() const
{
    QColor windowColor = QGuiApplication::palette().color(QPalette::Window);
    // Relative luminance formula: Y = 0.2126 * R + 0.7152 * G + 0.0722 * B
    double luminance = 0.2126 * windowColor.redF() + 
                       0.7152 * windowColor.greenF() + 
                       0.0722 * windowColor.blueF();
    return luminance < 0.5;
}
