import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Window 2.15
import QtCore

ApplicationWindow {
    id: settingsWindow

    // Set width and height based on content's implicit size plus margins
    width: settingsLayout.implicitWidth + 2 * settingsLayoutLeftRightMargin
    height: settingsLayout.implicitHeight + 2 * settingsLayoutTopBottomMargin

    title: "Settings"
    flags: Qt.Dialog
    color: "#333"

    // Define margins used in the layout for calculation
    readonly property int settingsLayoutLeftRightMargin: 10
    readonly property int settingsLayoutTopBottomMargin: 10

    property var mainWindow: null

    // --- Signals and Properties ---
    property int initialFontSize: 11
    property bool showIcon: true // Default
    property string originalCopyButtonText: "Copy Date to Clipboard"
    signal fontSizeChanged(int newSize)
    signal fontColorChanged(string newColorName)
    signal exitRequested()
    signal iconVisibilityChanged(bool visible)

    // Hidden TextEdit for clipboard operations
    TextEdit {
        id: hiddenTextEdit
        visible: false
        text: ""
    }

    // --- Main Layout ---
    ColumnLayout {
        id: settingsLayout
        anchors.fill: parent
        anchors.margins: settingsLayoutLeftRightMargin
        spacing: 10

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

        GridLayout {
            Layout.alignment: Qt.AlignHCenter
            columns: 4
            columnSpacing: 15
            rowSpacing: 15

            Repeater {
                model: [
                    { name: "Green", colorValue: "lightgreen" },
                    { name: "Cyan", colorValue: "cyan" },
                    { name: "Yellow", colorValue: "yellow" },
                    { name: "White", colorValue: "white" },
                    { name: "Orange", colorValue: "orange" },
                    { name: "Magenta", colorValue: "magenta" },
                    { name: "Red", colorValue: "red" },
                    { name: "Gray", colorValue: "gray" },
                    { name: "Teal", colorValue: "teal" },
                    { name: "Black", colorValue: "black" },
                    { name: "Blue", colorValue: "blue" },
                    { name: "Light Blue", colorValue: "lightblue" }
                ]
                delegate: Rectangle {
                    width: 40
                    height: 40
                    color: modelData.colorValue
                    radius: 4
                    border.color: "#444"
                    border.width: 1

                    ToolTip.visible: mouseArea.containsMouse
                    ToolTip.text: modelData.name

                    MouseArea {
                        id: mouseArea
                        anchors.fill: parent
                        hoverEnabled: true
                        onClicked: fontColorChanged(modelData.colorValue)
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

        // --- Autostart Button ---
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

    // --- Declarative Timer for Copy Message ---
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

    // --- Window Closing Handler ---
    onClosing: {
        resetTimer.stop();
        destroy();
    }
}
