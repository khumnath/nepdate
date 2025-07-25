import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Window 2.15
import "qrc:PanchangaCalculator.js" as Panchanga
import QtCore

// This is the main window for the small desktop widget.
ApplicationWindow {
    id: widgetWindow
    visible: false
    width: dateButton.implicitWidth + 20
    height: dateButton.implicitHeight + 10
    flags: Qt.FramelessWindowHint | Qt.WindowStaysOnTopHint | Qt.Tool
    color: "transparent"

    // --- Settings Component ---
    Settings {
        id: appSettings
        category: "NepdateDesktopWidget"
    }

    // --- App Settings Properties ---
    property int fontSize: 12
    property color fontColor: "green"
    property real latitude: 27.6000
    property real longitude: 82.9000
    property bool showIcon: true

    // --- Component and Window Properties ---
    property var calendarComponent: null
    property var calendarWindow: null
    property var settingsComponent: null
    property var settingsWindow: null
    property var tooltipComponent: null
    property var tooltipWindow: null

    // --- Date and Timer Logic ---
    property string displayedDate: "Loading..."
    property string tooltipDate: "Loading date..."

    Timer {
        interval: 1000
        running: true
        repeat: true
        onTriggered: updateDate()
    }

    function updateDate() {
        const now = new Date();
        const result = Panchanga.calculate(now, latitude, longitude);
        const nepaliYear = Panchanga.toDevanagari(result.bsYear);
        const nepaliDay = Panchanga.toDevanagari(result.bsDay);
        const nepaliMonthName = Panchanga.solarMonths[result.bsMonthIndex];
        const nepaliDate = `${nepaliYear} ${nepaliMonthName} ${nepaliDay} गते`;
        const nepaliWeekday = result.weekday;
        displayedDate = `${nepaliDate} ${nepaliWeekday}`;
        tooltipDate = `${displayedDate}\n${result.paksha} ${result.tithi}`;
    }

    // --- Main UI Button ---
    Button {
        id: dateButton
        anchors.centerIn: parent
        text: displayedDate
        background: Rectangle { color: "transparent" }
        contentItem: Row {
            spacing: 3
            anchors.centerIn: parent
            Image {
                id: dateIcon
                visible: showIcon
                source: "qrc:/resources/flag.png"
                width: dateText.font.pointSize * 1.2
                height: dateText.font.pointSize * 1.2
                fillMode: Image.PreserveAspectFit
                anchors.verticalCenter: parent.verticalCenter
            }
            Text {
                id: dateText
                text: displayedDate
                color: fontColor
                font.pointSize: fontSize
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }
        }
    }

    // --- Drag and Click Logic ---
    MouseArea {
        id: dragArea
        anchors.fill: parent
        acceptedButtons: Qt.LeftButton | Qt.RightButton
        hoverEnabled: true
        property point lastMousePos
        property bool wasDragged: false

        Timer {
            id: tooltipTimer
            interval: 500 // A slight delay before showing the tooltip
            onTriggered: showTooltip()
        }

        onEntered: tooltipTimer.start()
        onExited: {
            tooltipTimer.stop()
            hideTooltip()
        }
        onPositionChanged: function(mouse) {
            tooltipTimer.restart(); // Restart timer if mouse moves while inside
            if (mouse.buttons & Qt.LeftButton) {
                const delta = Qt.point(mouse.x - lastMousePos.x, mouse.y - lastMousePos.y);
                widgetWindow.x += delta.x;
                widgetWindow.y += delta.y;
                wasDragged = true;
            }
        }
        onPressed: function(mouse) {
            tooltipTimer.stop()
            hideTooltip()
            if (mouse.button === Qt.LeftButton) {
                lastMousePos = Qt.point(mouse.x, mouse.y);
                wasDragged = false;
            }
        }
        onReleased: function(mouse) {
            if (mouse.button === Qt.LeftButton && wasDragged) {
                appSettings.setValue("widgetPositionX", parseInt(widgetWindow.x));
                appSettings.setValue("widgetPositionY", parseInt(widgetWindow.y));
            }
        }
        onClicked: function(mouse) {
            if (wasDragged) return; // Do not trigger click after a drag

            if (mouse.button === Qt.LeftButton) {
                openCalendar();
            } else if (mouse.button === Qt.RightButton) {
                openSettingsWindow();
            }
        }
    }

    // --- Custom Tooltip Functions ---
    function showTooltip() {
        if (tooltipWindow) return; // Don't create if one already exists

        if (!tooltipComponent) {
            tooltipComponent = Qt.createComponent("Tooltip.qml");
        }

        if (tooltipComponent.status === Component.Ready) {
            tooltipWindow = tooltipComponent.createObject(null); // Create as top-level
            if (tooltipWindow) {
                tooltipWindow.text = tooltipDate;
                tooltipWindow.showAt(widgetWindow.x, widgetWindow.y, widgetWindow.width, widgetWindow.height);
            }
        } else if (tooltipComponent.status === Component.Error) {
             console.error("Error loading tooltip component:", tooltipComponent.errorString());
        }
    }

    function hideTooltip() {
        if (tooltipWindow) {
            tooltipWindow.destroy();
            tooltipWindow = null;
        }
    }

    // --- Function to launch the main calendar ---
    function openCalendar() {
        if (calendarWindow) {
            calendarWindow.show();
            calendarWindow.raise();
            calendarWindow.requestActivate();
            return;
        }
        if (!calendarComponent) {
            calendarComponent = Qt.createComponent("main.qml");
        }
        if (calendarComponent.status === Component.Ready) {
            calendarWindow = calendarComponent.createObject(widgetWindow);
            if (calendarWindow) {
               calendarWindow.closing.connect(() => {
                    calendarWindow.destroy();
                    calendarWindow = null;
                });
                calendarWindow.show();
            } else { console.error("Failed to create calendar window."); }
        } else { console.error("Error loading component:", calendarComponent.errorString()); }
    }

    function updateFontSize(newSize) {
        var size = parseInt(newSize);
        if (!isNaN(size)) {
            fontSize = size;
            dateText.font.pointSize = size;
        }
    }

    // --- Function to launch the settings window ---
    function openSettingsWindow() {
        if (settingsWindow) {
            settingsWindow.show();
            settingsWindow.raise();
            settingsWindow.requestActivate();
            return;
        }
        if (!settingsComponent) {
            settingsComponent = Qt.createComponent("Settings.qml");
        }
        if (settingsComponent.status === Component.Ready) {
            settingsWindow = settingsComponent.createObject(widgetWindow, {
                "initialFontSize": fontSize,
                "showIcon": showIcon
            });

            if (settingsWindow) {
                settingsWindow.mainWindow = widgetWindow;

                settingsWindow.iconVisibilityChanged.connect(function(visible) {
                    showIcon = visible;
                    appSettings.setValue("showIcon", visible);
                });
                settingsWindow.fontSizeChanged.connect(function(newSize) {
                    updateFontSize(newSize);
                    appSettings.setValue("fontSize", parseInt(newSize));
                });
                settingsWindow.fontColorChanged.connect(function(newColorName) {
                    fontColor = newColorName;
                    appSettings.setValue("fontColor", newColorName);
                });
                settingsWindow.exitRequested.connect(function() {
                    Qt.quit();
                });
                settingsWindow.closing.connect(() => {
                    settingsWindow.destroy();
                    settingsWindow = null;
                });

                // --- Smart Positioning Logic ---
                var idealX = widgetWindow.x + widgetWindow.width + 5;
                var idealY = widgetWindow.y;
                 if (widgetWindow.screen) {
                    if ((idealX + settingsWindow.width) > (widgetWindow.screen.virtualX + widgetWindow.screen.virtualWidth)) {
                        idealX = widgetWindow.x - settingsWindow.width - 5;
                    }
                    if (idealX < widgetWindow.screen.virtualX) {
                        idealX = widgetWindow.screen.virtualX + 5;
                    }
                    if ((idealY + settingsWindow.height) > (widgetWindow.screen.virtualY + widgetWindow.screen.virtualHeight)) {
                        idealY = widgetWindow.screen.virtualY + widgetWindow.screen.virtualHeight - settingsWindow.height - 5;
                    }
                    if (idealY < widgetWindow.screen.virtualY) {
                        idealY = widgetWindow.screen.virtualY + 5;
                    }
                }
                settingsWindow.x = idealX;
                settingsWindow.y = idealY;
                settingsWindow.show();
            } else {
                console.error("Failed to create settings window.");
            }
        } else {
            console.error("Error loading settings component:", settingsComponent.errorString());
        }
    }

    // --- Use a one-shot Timer for reliable initial positioning ---
    Timer {
        id: positioningTimer
        interval: 50
        repeat: false
        onTriggered: {
            widgetWindow.visible = true;
            Qt.callLater(function() {
                var savedX = appSettings.value("widgetPositionX", -1);
                var savedY = appSettings.value("widgetPositionY", -1);

                if (savedX !== -1 && savedY !== -1) {
                    widgetWindow.x = parseInt(savedX);
                    widgetWindow.y = parseInt(savedY);
                } else if (widgetWindow.screen) {
                    // Center on the primary screen if no position is saved
                    widgetWindow.x = widgetWindow.screen.virtualX + (widgetWindow.screen.virtualWidth - widgetWindow.width) / 2;
                    widgetWindow.y = widgetWindow.screen.virtualY + (widgetWindow.screen.virtualHeight - widgetWindow.height) / 2;
                }
            });
        }
    }

    // --- Load settings on startup ---
    Component.onCompleted: {
        var savedFontSize = appSettings.value("fontSize", 14);
        updateFontSize(savedFontSize);

        var savedColor = appSettings.value("fontColor", "magenta");
        fontColor = savedColor;

        // Ensure boolean conversion is robust
        var savedShowIcon = appSettings.value("showIcon", true);
        showIcon = (savedShowIcon === "true" || savedShowIcon === true);

        updateDate();
        positioningTimer.start();
    }
}
