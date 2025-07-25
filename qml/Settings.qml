import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

ApplicationWindow {
    id: settingsWindow
    width: 280
    height: 415
    title: "Widget Settings"
    flags: Qt.Dialog
    color: "#333"

    property var mainWindow: null

    // --- Signals and Properties ---
    property int initialFontSize: 11
    signal fontSizeChanged(int newSize)
    signal fontColorChanged(string newColorName)
    signal exitRequested()

    // Hidden TextEdit for clipboard operations
    TextEdit {
        id: hiddenTextEdit
        visible: false
        text: ""
    }

    // --- Main Layout ---
    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 10
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

            onValueChanged: {
                fontSizeChanged(value)
            }
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

        Button {
            id: copyButton
            text: "Copy Date to Clipboard"
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

    // Function to copy date to clipboard using hidden TextEdit
    function copyDateToClipboard(dateText) {
        try {
            hiddenTextEdit.text = dateText;
            hiddenTextEdit.selectAll();
            hiddenTextEdit.copy();
            showCopyMessage("Copied!");
            console.log("Successfully copied date to clipboard: " + dateText);
        } catch (error) {
            console.log("Error copying to clipboard: " + error.message);
            showCopyMessage("Copy failed");
        }
    }

    // Show message on copy button using Qt Timer
    function showCopyMessage(message) {
        var originalText = copyButton.text;
        copyButton.text = message;

        var resetTimer = Qt.createQmlObject("import QtQuick 2.15; Timer {}", settingsWindow);
        resetTimer.interval = 1500;
        resetTimer.repeat = false;
        resetTimer.triggered.connect(function() {
            if (copyButton) {
                copyButton.text = originalText;
            }
            resetTimer.destroy();
        });
        resetTimer.start();
    }
}
