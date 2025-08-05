import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Window 2.15
import "qrc:PanchangaCalculator.js" as Panchanga
import QtCore

// This is the main window for the small desktop widget.
ApplicationWindow {
    id: widgetWindow
    title: "नेपाली क्यालेण्डर"
    visible: false
    width: dateButton.implicitWidth + 20
    height: dateButton.implicitHeight + 10
    // Initial flags, StayOnTopHint will be managed dynamically
   flags: Qt.FramelessWindowHint | Qt.Window | Qt.WindowDoesNotAcceptFocus | Qt.Notification | Qt.BypassWindowManagerHint
    color: "transparent"

    // --- Settings Component ---
    Settings {
        id: appSettings
        category: "NepdateDesktopWidget"
    }

    // --- App Settings Properties ---
    property int fontSize: 12
    property color fontColor: "green"
    property real latitude: 27.71
    property real longitude: 85.32
    property bool showIcon: true
    property bool screenontop: true

    onScreenontopChanged: {
        updateStayOnTopFlag(screenontop);
    }

    // --- Component and Window Properties ---
    property var calendarComponent: null
    property var calendarWindow: null
    property var settingsComponent: null
    property var settingsWindow: null

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
            anchors.verticalCenter: parent.verticalCenter
            Image {
                id: dateIcon
                visible: showIcon
                source: "qrc:/resources/flag.png"
                width: dateText.font.pointSize * 1.3
                height: dateText.font.pointSize * 1.3
                fillMode: Image.PreserveAspectFit
                anchors.verticalCenter: parent.verticalCenter
            }

            Text {
                id: dateText
                text: displayedDate
                color: fontColor
                font.pointSize: fontSize
                verticalAlignment: Text.AlignVCenter
                anchors.verticalCenter: parent.verticalCenter
            }
        }
    }

    // --- Drag and Click Logic ---
    MouseArea {
        id: dragArea
        anchors.fill: parent
        acceptedButtons: Qt.LeftButton | Qt.RightButton
        hoverEnabled: true

        property int dragStartX: 0
        property int dragStartY: 0
        property int windowStartX: 0
        property int windowStartY: 0
        property bool wasDragged: false

        Timer {
            id: resetDragTimer
            interval: 300
            repeat: false
            onTriggered: dragArea.wasDragged = false
        }

        Timer {
            id: tooltipTimer
            interval: 500
            repeat: false
            onTriggered: showTooltip()
        }

        onEntered: tooltipTimer.start()
        onExited: {
            tooltipTimer.stop()
            hideTooltip()
        }

        Item { id: globalMapper; anchors.fill: parent; visible: false }

        onPressed: function(mouse) {
            if (mouse.button === Qt.LeftButton) {
                wasDragged = false;
                if (platformName === "wayland") {
                    widgetWindow.startSystemMove()
                    wasDragged = true; // Assume a drag will happen
                } else { // X11 and other platforms
                    // Use the direct screen coordinates from the mouse event for reliability.
                    var globalPos = globalMapper.mapToGlobal(Qt.point(mouse.x, mouse.y))
                                        dragStartX = globalPos.x
                                        dragStartY = globalPos.y
                                        windowStartX = widgetWindow.x
                                        windowStartY = widgetWindow.y

                }
            }
        }

        onPositionChanged: function(mouse) {
            if (platformName !== "wayland" && (mouse.buttons & Qt.LeftButton)) {
                var globalPos = globalMapper.mapToGlobal(Qt.point(mouse.x, mouse.y))
                               widgetWindow.x = windowStartX + (globalPos.x - dragStartX)
                               widgetWindow.y = windowStartY + (globalPos.y - dragStartY)

                wasDragged = true
                resetDragTimer.stop()
            }
        }

        onReleased: function(mouse) {
            if (mouse.button === Qt.LeftButton && wasDragged) {
                if (mouse.button === Qt.LeftButton && wasDragged) {
                                appSettings.setValue("widgetPositionX", widgetWindow.x)
                                appSettings.setValue("widgetPositionY", widgetWindow.y)
                                resetDragTimer.start()
                }
            }
        }

        onClicked: function(mouse) {
            if (wasDragged) {
                wasDragged = false;
                return;
            }
            if (mouse.button === Qt.LeftButton) {
                openCalendar()
            } else if (mouse.button === Qt.RightButton) {
                openSettingsWindow()
            }
        }
    }

    // --- Custom Tooltip Functions ---
       function showTooltip() {
           tooltipManager.showAboveOrBelow(dragArea, tooltipDate);
       }

       function hideTooltip() {
           tooltipManager.hide();
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
                    Panchanga.clearCache();
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

    // --- Function to update the window's stay on top flag ---
        function updateStayOnTopFlag(isOnTop) {
            const wasVisible = widgetWindow.visible;
            if (wasVisible) {
                widgetWindow.visible = false;
            }

            Qt.callLater(function() {
                if (isOnTop) {
                    // Add the flags
                    widgetWindow.flags |= (Qt.BypassWindowManagerHint | Qt.WindowStaysOnTopHint| Qt.Window | Qt.WindowDoesNotAcceptFocus | Qt.Notification | Qt.Tool);
                } else {
                    // Remove the flags
                    widgetWindow.flags &= ~(Qt.WindowStaysOnTopHint | Qt.BypassWindowManagerHint | Qt.Tool);
                }

                // Make window visible again if it was visible before the flag change.
                if (wasVisible) {
                    widgetWindow.visible = true;
                }
            });
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
                    "showIcon": showIcon,
                    "screenontop": screenontop
                });

                if (settingsWindow) {
                    settingsWindow.mainWindow = widgetWindow;

                    settingsWindow.screenontopset.connect(function(ontop) {
                        screenontop = ontop;
                        appSettings.setValue("screenontop", ontop);
                    });
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

                    if (widgetWindow.screen) {
                        const screen = widgetWindow.screen;
                        const margin = 5;
                        const titleBarOffset = 30;

                        const screenLeft = screen.virtualX;
                        const screenTop = screen.virtualY;
                        const screenRight = screenLeft + screen.width;
                        const screenBottom = screenTop + screen.height;

                        const fallbackWidth = 200;
                        const fallbackHeight = 150;
                        const settingsWidth = settingsWindow.width || fallbackWidth;
                        const settingsHeight = settingsWindow.height || fallbackHeight;

                        let idealX, idealY;

                        const totalVerticalMargin = margin + titleBarOffset;
                        const hasSpaceBelow = (widgetWindow.y + widgetWindow.height + totalVerticalMargin + settingsHeight) <= screenBottom;
                        const hasSpaceAbove = (widgetWindow.y - totalVerticalMargin - settingsHeight) >= screenTop;
                        const hasSpaceRight = (widgetWindow.x + widgetWindow.width + margin + settingsWidth) <= screenRight;

                        if (hasSpaceBelow) {
                            idealY = widgetWindow.y + widgetWindow.height + totalVerticalMargin;
                            idealX = widgetWindow.x + (widgetWindow.width / 2) - (settingsWidth / 2);
                        } else if (hasSpaceAbove) {
                            idealY = widgetWindow.y - settingsHeight - totalVerticalMargin;
                            idealX = widgetWindow.x + (widgetWindow.width / 2) - (settingsWidth / 2);
                        } else if (hasSpaceRight) {
                            idealX = widgetWindow.x + widgetWindow.width + margin;
                            idealY = widgetWindow.y;
                        } else {
                            idealX = widgetWindow.x - settingsWidth - margin;
                            idealY = widgetWindow.y;
                        }

                        settingsWindow.x = Math.max(screenLeft, Math.min(idealX, screenRight - settingsWidth));
                        settingsWindow.y = Math.max(screenTop, Math.min(idealY, screenBottom - settingsHeight));

                    } else {
                        settingsWindow.x = widgetWindow.x + widgetWindow.width + 5;
                        settingsWindow.y = widgetWindow.y;
                    }

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


    // --- Settings Validation and Loading ---
        function resetAndSaveDefaultSettings() {
            const defaultFontSize = 14;
            const defaultFontColor = "magenta";
            const defaultShowIcon = true;
            const defaultScreenOnTop = true;

            updateFontSize(defaultFontSize);
            fontColor = defaultFontColor;
            showIcon = defaultShowIcon;
            screenontop = defaultScreenOnTop;

            appSettings.setValue("fontSize", defaultFontSize);
            appSettings.setValue("fontColor", defaultFontColor);
            appSettings.setValue("showIcon", defaultShowIcon);
            appSettings.setValue("screenontop", defaultScreenOnTop);
        }

        function loadAndValidateSettings() {
            let settingsValid = true;

            const savedFontSize = appSettings.value("fontSize");
            if (savedFontSize === undefined || isNaN(parseInt(savedFontSize, 10)) || parseInt(savedFontSize, 10) < 8 || parseInt(savedFontSize, 10) > 30) {
                settingsValid = false;
            }

            const savedColor = appSettings.value("fontColor");
            if (settingsValid && (savedColor === undefined || typeof savedColor.toString() !== 'string' || savedColor.toString().length === 0)) {
                settingsValid = false;
            }

            const savedShowIcon = appSettings.value("showIcon");
            if (settingsValid && (savedShowIcon === undefined || (savedShowIcon.toString() !== "true" && savedShowIcon.toString() !== "false"))) {
                settingsValid = false;
            }

            const savedScreenOnTop = appSettings.value("screenontop");
            if (settingsValid && (savedScreenOnTop === undefined || (savedScreenOnTop.toString() !== "true" && savedScreenOnTop.toString() !== "false"))) {
                settingsValid = false;
            }

            if (settingsValid) {
                updateFontSize(appSettings.value("fontSize", 14));
                fontColor = appSettings.value("fontColor", "magenta");
                showIcon = appSettings.value("showIcon", true).toString() === "true";
                screenontop = appSettings.value("screenontop", true).toString() === "true";
            } else {
                resetAndSaveDefaultSettings();
            }

            updateDate();
            positioningTimer.start();
        }


        // --- Load settings on startup ---
        Component.onCompleted: {
            Qt.callLater(function() {
                console.log("Platform detected:", platformName);
                loadAndValidateSettings();
            });
        }
    }
