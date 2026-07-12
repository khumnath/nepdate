#ifndef WAYLANDHELPER_H
#define WAYLANDHELPER_H

#include <QObject>
#include <QQuickWindow>

class WaylandHelper : public QObject
{
    Q_OBJECT
public:
    explicit WaylandHelper(QObject *parent = nullptr);

    // Initialize LayerShell for the given window (call before showing)
    Q_INVOKABLE void initLayerShell(QQuickWindow *window);

    // Set whether the window should stay on top (LayerOverlay vs LayerTop)
    Q_INVOKABLE void setLayer(QQuickWindow *window, bool stayOnTop);

    // Update the margins to simulate window positioning
    Q_INVOKABLE void setPosition(QQuickWindow *window, int x, int y);

    // Get global cursor position for Wayland dragging
    Q_INVOKABLE QPoint cursorPos();
};

#endif // WAYLANDHELPER_H
