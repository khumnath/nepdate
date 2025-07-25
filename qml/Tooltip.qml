import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Window 2.15

ApplicationWindow {
    id: tooltipWindow

    visible: false
    width: tooltipText.implicitWidth + 20
    height: tooltipText.implicitHeight + 10
    flags: Qt.FramelessWindowHint | Qt.WindowStaysOnTopHint | Qt.Tool | Qt.BypassWindowManagerHint
    color: "transparent"

    property string text: ""

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
            wrapMode: Text.WordWrap
        }
    }

    function showAt(widgetX, widgetY, widgetWidth, widgetHeight) {
        var idealX = widgetX + (widgetWidth / 2) - (tooltipWindow.width / 2);
        var idealY = widgetY - tooltipWindow.height - 5; // 5px margin

        if (tooltipWindow.screen) {
            const screen = tooltipWindow.screen;
            const screenMargin = 5;
            if (idealY < screen.virtualY) {
                idealY = widgetY + widgetHeight + screenMargin;
            }
            if ((idealY + tooltipWindow.height) > (screen.virtualY + screen.virtualHeight)) {
                idealY = screen.virtualY + screen.virtualHeight - tooltipWindow.height - screenMargin;
            }
            if ((idealX + tooltipWindow.width) > (screen.virtualX + screen.virtualWidth)) {
                idealX = screen.virtualX + screen.virtualWidth - tooltipWindow.width - screenMargin;
            }
            if (idealX < screen.virtualX) {
                idealX = screen.virtualX + screenMargin;
            }
        }

        tooltipWindow.x = idealX;
        tooltipWindow.y = idealY;
        tooltipWindow.visible = true;
    }
}
