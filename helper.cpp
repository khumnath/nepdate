// helper.cpp

#include "helper.h"
#include <QDebug>
#include <QGuiApplication>
#include <QImage>
#include <QPainter>
#include <QPalette>
#include <QPixmap>
#include <QPrintDialog>
#include <QPrinter>
#include <QQuickItemGrabResult>
#include <QSharedPointer>
#include <QPointer>
#include <QTimer>

Printer::Printer(QObject *parent) : QObject(parent) {}

void Printer::print(QQuickWindow *window) {
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

  // We purposefully do NOT set isPreparingPrint to false here yet!
  // We want the overlay to stay visible while the OS is loading the print
  // dialog.

  window->contentItem()->update();
  window->update();
  QCoreApplication::processEvents(QEventLoop::AllEvents, 100);

  if (image.isNull()) {
    window->setProperty("isPreparingPrint", false);
    qWarning() << "Error: Failed to grab window content.";
    return;
  }

  QPrinter printer(QPrinter::HighResolution);
  QPrintDialog dialog(&printer, nullptr);

  // This 500-ms timer gets queued in the event loop. As soon as dialog.exec()
  // starts its own nested event loop, this timer fires after 0.5s, which is
  // exactly enough time for the dialog to visually appear on the screen, then
  // hides the overlay!
  QPointer<QQuickWindow> windowGuard(window);
  QTimer::singleShot(500, window, [windowGuard]() {
    if (windowGuard)
      windowGuard->setProperty("isPreparingPrint", false);
  });

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

  painter.drawPixmap(QRect(x, y, imageSize.width(), imageSize.height()),
                     pixmap);
  painter.end();
}

bool Printer::isSystemDark() const {
  QColor windowColor = QGuiApplication::palette().color(QPalette::Window);
  // Relative luminance formula: Y = 0.2126 * R + 0.7152 * G + 0.0722 * B
  double luminance = 0.2126 * windowColor.redF() +
                     0.7152 * windowColor.greenF() +
                     0.0722 * windowColor.blueF();
  return luminance < 0.5;
}
