import QtQuick 2.15
import QtQuick.Controls 2.15

// This is a separate, frameless window that acts as a custom tooltip.
ApplicationWindow {
    id: tooltipWindow

    // --- Window Properties ---
    visible: false
    width: tooltipText.implicitWidth + 20
    height: tooltipText.implicitHeight + 10

    flags: Qt.FramelessWindowHint | Qt.WindowStaysOnTopHint | Qt.Tool | Qt.BypassWindowManagerHint
    color: "transparent"

    // --- Public Properties ---
    property string text: ""

    // --- UI ---
    Rectangle {
        anchors.fill: parent
        color: "#333"
        border.color: "#555"
        radius: 4

        Text {
            id: tooltipText
            text: tooltipWindow.text
            color: "white"
            anchors.centerIn: parent
        }
    }

    // --- Public Functions ---
    function showAt(widgetX, widgetY, widgetWidth, widgetHeight) {
        tooltipWindow.x = widgetX + (widgetWidth / 2) - (tooltipWindow.width / 2);

        var topPosition = widgetY - tooltipWindow.height - 5;

        if (topPosition < 0) {
            tooltipWindow.y = widgetY + widgetHeight + 5;
        } else {
            tooltipWindow.y = topPosition;
        }

        tooltipWindow.visible = true;
    }
}
