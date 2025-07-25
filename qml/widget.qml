import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Window 2.15
import "PanchangaCalculator.js" as Panchanga
import QtCore

// This is the main window for the small desktop widget.
ApplicationWindow {
    id: widgetWindow

    // Start invisible to prevent flicker during initialization
    visible: false

    // --- Bind the window size directly to the button's content size ---
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

    property var calendarComponent: null
    property var calendarWindow: null
    property var settingsComponent: null
    property var settingsWindow: null
    // --- Properties for the custom tooltip ---
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
        contentItem: Text {
            text: parent.text
            color: fontColor
            font.pointSize: fontSize
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }
    }

    // --- Drag and Click Logic ---
    MouseArea {
        id: dragArea
        anchors.fill: parent
        acceptedButtons: Qt.LeftButton | Qt.RightButton
        hoverEnabled: true
        property point lastMousePos
        property bool wasDragged: false // Flag to distinguish click from drag

        // Timer to control when the tooltip appears
        Timer {
            id: tooltipTimer
            interval: 0
            onTriggered: showTooltip()
        }

        onEntered: tooltipTimer.start()
        onExited: {
            tooltipTimer.stop()
            hideTooltip()
        }

        onPositionChanged: function(mouse) {
            tooltipTimer.restart();
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
            if (mouse.button === Qt.LeftButton) {
                appSettings.setValue("widgetPositionX", parseInt(widgetWindow.x));
                appSettings.setValue("widgetPositionY", parseInt(widgetWindow.y));
            }
        }
        onClicked: function(mouse) {
            if (mouse.button === Qt.LeftButton && !wasDragged) {
                openCalendar();
            } else if (mouse.button === Qt.RightButton) {
                openSettingsWindow();
            }
        }
    }

    // --- Custom Tooltip Functions ---
    function showTooltip() {
        if (!tooltipComponent) {
            tooltipComponent = Qt.createComponent("Tooltip.qml");
        }
        if (tooltipComponent.status === Component.Ready) {
            if (!tooltipWindow) {
                tooltipWindow = tooltipComponent.createObject(null); // Create as top-level
                 if (tooltipWindow) {
                     tooltipWindow.closing.connect(function() { tooltipWindow = null; });
                 }
            }
            if (tooltipWindow) {
                tooltipWindow.text = tooltipDate;
                tooltipWindow.showAt(widgetWindow.x, widgetWindow.y, widgetWindow.width, widgetWindow.height);
            }
        }
    }

    function hideTooltip() {
        if (tooltipWindow) {
            tooltipWindow.close();
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
                calendarWindow.closing.connect(() => { calendarWindow = null; });
                calendarWindow.show();
            } else { console.error("Failed to create calendar window."); }
        } else { console.error("Error loading component:", calendarComponent.errorString()); }
    }

    function updateFontSize(newSize) {
        var size = parseInt(newSize);
        fontSize = size;
        dateButton.contentItem.font.pointSize = size;
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
                "initialFontSize": fontSize
            });
            if (settingsWindow) {
                settingsWindow.mainWindow = widgetWindow;
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

                settingsWindow.closing.connect(() => { settingsWindow = null; });

                // --- Smart Positioning Logic ---
                var idealX = widgetWindow.x + widgetWindow.width + 5;
                var idealY = widgetWindow.y;

                if (widgetWindow.screen && (idealX + settingsWindow.width) > (widgetWindow.screen.virtualX + widgetWindow.screen.virtualWidth)) {
                    idealX = widgetWindow.x - settingsWindow.width - 5;
                }

                if (widgetWindow.screen && idealX < widgetWindow.screen.virtualX) {
                    idealX = widgetWindow.screen.virtualX + 5;
                }

                if (widgetWindow.screen && (idealY + settingsWindow.height) > (widgetWindow.screen.virtualY + widgetWindow.screen.virtualHeight)) {
                    idealY = widgetWindow.screen.virtualY + widgetWindow.screen.virtualHeight - settingsWindow.height - 5;
                }
                if (widgetWindow.screen && idealY < widgetWindow.screen.virtualY) {
                    idealY = widgetWindow.screen.virtualY + 5;
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
            // First make window visible so size calculations work correctly
            widgetWindow.visible = true;

            // Small delay to ensure rendering is complete before positioning
            Qt.callLater(function() {
                var savedX = appSettings.value("widgetPositionX", -1);
                var savedY = appSettings.value("widgetPositionY", -1);

                if (savedX !== -1 && savedY !== -1) {
                    widgetWindow.x = parseInt(savedX);
                    widgetWindow.y = parseInt(savedY);
                } else {
                    // Default to a specified position if no saved position
                    widgetWindow.x = 969;
                    widgetWindow.y = 979;
                }
            });
        }
    }

    // --- Load settings on startup ---
    Component.onCompleted: {
        var savedFontSize = appSettings.value("fontSize", 14);
        updateFontSize(savedFontSize);

        var savedColor = appSettings.value("fontColor", "green");
        fontColor = savedColor;

        updateDate();
        positioningTimer.start();
    }
}
