import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Window 2.15
import QtQuick.Dialogs
import QtCore

// Settings.qml(setting window)
ApplicationWindow {
    id: settingsWindow

    // Set width and height based on content's implicit size plus margins
    width: settingsLayout.implicitWidth + 10 * settingsLayoutLeftRightMargin
    height: settingsLayout.implicitHeight + 2 * settingsLayoutTopBottomMargin

    title: "Settings"
    flags: Qt.Dialog
    color: "#333"

    // Define margins used in the layout for calculation
    readonly property int settingsLayoutLeftRightMargin: 10
    readonly property int settingsLayoutTopBottomMargin: 10

    property var mainWindow: null

    // Signals and Properties
    property int initialFontSize: 11
    property bool showIcon: true
    property bool screenontop: true
    property string originalCopyButtonText: "Copy Date to Clipboard"
    signal fontSizeChanged(int newSize)
    signal fontColorChanged(string newColorName)
    signal exitRequested()
    signal iconVisibilityChanged(bool visible)
    signal screenontopset(bool ontop)

    // The 'platformName' property is now provided by C++ via setContextProperty

    // Hidden TextEdit for clipboard operations
    TextEdit {
        id: hiddenTextEdit
        visible: false
        text: ""
    }

    // Main Layout
    ColumnLayout {
        id: settingsLayout
        anchors.fill: parent
        anchors.margins: settingsLayoutLeftRightMargin

        Label {
            text: "Font Size"
            color: "lightgreen"
            font.pointSize: 12
            Layout.alignment: Qt.AlignHCenter
        }
        Slider {
            id: fontSizeSlider
            Layout.fillWidth: true
            from: 8
            to: 24
            value: initialFontSize
            onValueChanged: fontSizeChanged(value)
        }

        Label {
            text: "Font Color"
            color: "lightgreen"
            font.pointSize: 12
            Layout.alignment: Qt.AlignHCenter
        }

        // Color picker
        Item {
            id: customColorPicker
            width: 250
            height: 150
            Layout.alignment: Qt.AlignHCenter

            property color selectedColor: appSettings.value("fontColor", "white")
            property real currentHue: selectedColor.hsvHue >= 0 ? selectedColor.hsvHue : 0
            property real currentSaturation: selectedColor.hsvSaturation
            property real currentValue: selectedColor.hsvValue
            property bool isInitialized: false

            Component.onCompleted: {
                isInitialized = true;
            }

            onCurrentHueChanged: updateColor()
            onCurrentSaturationChanged: updateColor()
            onCurrentValueChanged: updateColor()

            onSelectedColorChanged: {
                currentHue = selectedColor.hsvHue >= 0 ? selectedColor.hsvHue : 0;
                currentSaturation = selectedColor.hsvSaturation;
                currentValue = selectedColor.hsvValue;

                if (isInitialized) {
                    fontColorChanged(selectedColor);
                }
            }

            function updateColor() {
                var newColor = Qt.hsva(currentHue, currentSaturation, currentValue, 1.0);
                if (newColor.toString() !== selectedColor.toString()) {
                    selectedColor = newColor;
                }
            }

            RowLayout {
                anchors.fill: parent
                spacing: 10
                width: 250
                    height: 180

                Rectangle {
                    id: svArea
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    color: Qt.hsva(customColorPicker.currentHue, 1.0, 1.0, 1.0)

                    Rectangle {
                        anchors.fill: parent
                        gradient: Gradient {
                            GradientStop { position: 0.0; color: "white" }
                            GradientStop { position: 1.0; color: "transparent" }
                        }
                    }
                    Rectangle {
                        anchors.fill: parent
                        gradient: Gradient {
                            orientation: Gradient.Vertical
                            GradientStop { position: 0.0; color: "transparent" }
                            GradientStop { position: 1.0; color: "black" }
                        }
                    }
                    Rectangle {
                        id: svIndicator
                        x: svArea.width * customColorPicker.currentSaturation - (width / 2)
                        y: svArea.height * (1.0 - customColorPicker.currentValue) - (height / 2)
                        width: 12
                        height: 12
                        radius: 6
                        color: "transparent"
                        border.color: customColorPicker.currentValue > 0.5 ? "black" : "white"
                        border.width: 2
                    }
                    MouseArea {
                        anchors.fill: parent
                        onPressed: (mouse) => updateSV(mouse.x, mouse.y)
                        onPositionChanged: (mouse) => { if (pressed) updateSV(mouse.x, mouse.y) }

                        function updateSV(mx, my) {
                            customColorPicker.currentSaturation = Math.max(0, Math.min(1, mx / svArea.width));
                            customColorPicker.currentValue = 1.0 - Math.max(0, Math.min(1, my / svArea.height));
                        }
                    }
                }

                Rectangle {
                    id: hueSlider
                    Layout.preferredWidth: 25
                    Layout.fillHeight: true
                    radius: 4
                    gradient: Gradient {
                        orientation: Gradient.Vertical
                        GradientStop { position: 0.0;     color: "#ff0000" }
                        GradientStop { position: 0.166;   color: "#ffff00" }
                        GradientStop { position: 0.333;   color: "#00ff00" }
                        GradientStop { position: 0.5;     color: "#00ffff" }
                        GradientStop { position: 0.666;   color: "#0000ff" }
                        GradientStop { position: 0.833;   color: "#ff00ff" }
                        GradientStop { position: 1.0;     color: "#ff0000" }
                    }
                    Rectangle {
                        width: parent.width + 4
                        height: 4
                        x: -2
                        y: hueSlider.height * customColorPicker.currentHue - (height / 2)
                        color: "white"
                        border.color: "black"
                    }
                    MouseArea {
                        anchors.fill: parent
                        onPressed: (mouse) => updateHue(mouse.y)
                        onPositionChanged: (mouse) => { if (pressed) updateHue(mouse.y) }

                        function updateHue(my) {
                            customColorPicker.currentHue = Math.max(0, Math.min(1, my / hueSlider.height));
                        }
                    }
                }
            }
        }


        Item { Layout.fillHeight: true }

        Label {
            text: "Show Icon"
            color: "lightgreen"
            font.pointSize: 12
            Layout.alignment: Qt.AlignHCenter
        }

        CheckBox {
            id: iconCheck
            text: "Enable Icon"
            checked: showIcon
            Layout.alignment: Qt.AlignHCenter

            onCheckedChanged: {
                showIcon = checked;
                iconVisibilityChanged(checked);
            }
        }

        Label {
            text: "Stay widget on top"
            color: "lightgreen"
            font.pointSize: 12
            Layout.alignment: Qt.AlignHCenter
        }

        CheckBox {
            id: screentopCheck
            text: "Enable stay on screen top"
            checked: screenontop
            Layout.alignment: Qt.AlignHCenter

            onCheckedChanged: {
                screenontop = checked;
                screenontopset(checked);
            }
        }


        Button {
            id: copyButton
            text: originalCopyButtonText
            Layout.fillWidth: true
            onClicked: {
                if (mainWindow && mainWindow.displayedDate) {
                    copyDateToClipboard(mainWindow.displayedDate);
                } else {
                    showCopyMessage("No date available");
                }
            }
            background: Rectangle { color: "#555"; radius: 3; }
            contentItem: Text {
                text: parent.text; color: "lightgreen"; font.pointSize: 12
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }
        }

        // Autostart Button
        Button {
            id: autostartButton
            Layout.fillWidth: true
            text: autostartManager.isAutostartEnabled() ? "Disable Autostart" : "Enable Autostart"

            onClicked: {
                autostartManager.setAutostart(!autostartManager.isAutostartEnabled());
                autostartButton.text = autostartManager.isAutostartEnabled() ? "Disable Autostart" : "Enable Autostart";
            }

            background: Rectangle { color: "#555"; radius: 3; }
            contentItem: Text {
                text: parent.text; color: "lightgreen"; font.pointSize: 12
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }
        }

        Button {
            text: "Exit Widget"
            Layout.fillWidth: true
            onClicked: exitRequested()
            background: Rectangle { color: "#731d1d"; radius: 3; }
            contentItem: Text {
                text: parent.text; color: "white"; font.pointSize: 12
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }
        }

        Button {
            text: "Close Menu"
            Layout.fillWidth: true
            onClicked: settingsWindow.close()
            background: Rectangle { color: "#555"; radius: 3; }
            contentItem: Text {
                text: parent.text; color: "lightgreen"; font.pointSize: 12
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }
        }
    }

    // Declarative Timer for Copy Message
    Timer {
        id: resetTimer
        interval: 1500
        repeat: false
        onTriggered: {
            if (copyButton) {
                copyButton.text = originalCopyButtonText;
            }
        }
    }

    // Function to copy date to clipboard
    function copyDateToClipboard(dateText) {
        try {
            hiddenTextEdit.text = dateText;
            hiddenTextEdit.selectAll();
            hiddenTextEdit.copy();
            showCopyMessage("Copied!");
        } catch (error) {
            console.log("Error copying to clipboard: " + error.message);
            showCopyMessage("Copy failed");
        }
    }

    // Show message on copy button using the declarative timer
    function showCopyMessage(message) {
        resetTimer.stop(); // Stop previous timer if running
        copyButton.text = message;
        resetTimer.start();
    }

    // Window Closing Handler
    onClosing: {
        resetTimer.stop();
        destroy();
    }
}
