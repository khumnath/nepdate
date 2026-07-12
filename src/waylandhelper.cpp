#include "waylandhelper.h"
#include <QMargins>
#include <QCursor>

#ifdef HAS_LAYERSHELL_QT
#include <LayerShellQt/Window>
#endif

WaylandHelper::WaylandHelper(QObject *parent) : QObject(parent)
{
}

void WaylandHelper::initLayerShell(QQuickWindow *window)
{
    if (!window) return;

#ifdef HAS_LAYERSHELL_QT
    if (auto layerWindow = LayerShellQt::Window::get(window)) {
        layerWindow->setLayer(LayerShellQt::Window::LayerOverlay);
        layerWindow->setAnchors(LayerShellQt::Window::Anchors(LayerShellQt::Window::AnchorTop | LayerShellQt::Window::AnchorLeft));
        layerWindow->setKeyboardInteractivity(LayerShellQt::Window::KeyboardInteractivityNone);
    }
#endif
}

void WaylandHelper::setLayer(QQuickWindow *window, bool stayOnTop)
{
    if (!window) return;

#ifdef HAS_LAYERSHELL_QT
    if (auto layerWindow = LayerShellQt::Window::get(window)) {
        if (stayOnTop) {
            layerWindow->setLayer(LayerShellQt::Window::LayerOverlay);
        } else {
            layerWindow->setLayer(LayerShellQt::Window::LayerTop);
        }
    }
#endif
}

void WaylandHelper::setPosition(QQuickWindow *window, int x, int y)
{
    if (!window) return;

#ifdef HAS_LAYERSHELL_QT
    if (auto layerWindow = LayerShellQt::Window::get(window)) {
        layerWindow->setMargins(QMargins(x, y, 0, 0));
        window->update();
    }
#endif
}

QPoint WaylandHelper::cursorPos()
{
    return QCursor::pos();
}


