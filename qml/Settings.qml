import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Window 2.15
import QtQuick.Dialogs
import QtCore
import com.calendar.printer 1.0

// Settings.qml (setting window redesigned to match test.html)
ApplicationWindow {
    id: settingsWindow

    readonly property bool isDarkTheme: Printer.isSystemDark()

    // Theme Colors (matched with test.html styles for light & dark theme)
    readonly property color themeBg: isDarkTheme ? "#242424" : "#f0f0f0"
    readonly property color themeBorder: isDarkTheme ? "#444444" : "#d0d0d0"
    readonly property color themeTitleBarBg: isDarkTheme ? "#2c2c2c" : "#e8e8e8"
    readonly property color themeTitleBarBorder: isDarkTheme ? "#3c3c3c" : "#d0d0d0"
    readonly property color themeTitleText: isDarkTheme ? "#ffffff" : "#333333"

    readonly property color themeTextColor: isDarkTheme ? "#81c784" : "#2e7d32"
    readonly property color themeLabelColor: isDarkTheme ? "#81c784" : "#2e7d32"
    readonly property color themeControlBg: isDarkTheme ? "#3c3c3c" : "#e0e0e0"
    readonly property color themeControlBorder: isDarkTheme ? "#555555" : "#cccccc"

    readonly property color themeExitBtnBg: isDarkTheme ? "#c62828" : "#d32f2f"
    readonly property color themeExitBtnBorder: isDarkTheme ? "#8b1a1a" : "#b71c1c"

    readonly property color themeCheckTextColor: isDarkTheme ? "#ffffff" : "#333333"

    // Colors for Tabs
    readonly property color tabActiveBg: isDarkTheme ? "#2e7d32" : "#a5d6a7"
    readonly property color tabActiveBorder: isDarkTheme ? "#81c784" : "#2e7d32"
    readonly property color tabActiveText: isDarkTheme ? "#ffffff" : "#1b5e20"

    // Frameless window configuration to allow rounded borders
    flags: Qt.Window | Qt.FramelessWindowHint
    color: "transparent"

    width: 400
    height: 580

    // Define margins used in the layout for calculation
    readonly property int settingsLayoutLeftRightMargin: 20
    readonly property int settingsLayoutTopBottomMargin: 20

    property var mainWindow: null

    // Signals and Properties
    property int initialFontSize: 11
    property bool showIcon: true
    property bool screenontop: true
    property string originalCopyButtonText: "Copy Date to Clipboard"
    signal fontSizeChanged(int newSize)
    signal fontColorChanged(string newColorName)
    signal backgroundColorChanged(string newColorName)
    signal exitRequested()
    signal iconVisibilityChanged(bool visible)
    signal screenontopset(bool ontop)

    property string backgroundColorName: "none"
    property color fontColorVal: appSettings.value("fontColor", "magenta")
    property string backColorVal: appSettings.value("backgroundColor", compositorSupportsTransparency ? "none" : "#2c2c2c")
    property color backColorCustomVal: {
        var c = appSettings.value("backgroundColor", "#2c2c2c");
        return c === "none" ? "#2c2c2c" : c;
    }
    property string activeColorTarget: "font" // "font" or "background"

    // Swatch original color tracking
    property color originalFontColor: "magenta"
    property color originalBackColor: "#2c2c2c"

    onActiveColorTargetChanged: {
        if (customColorPicker.isInitialized) {
            if (activeColorTarget === "font") {
                customColorPicker.loadColorToPicker(fontColorVal);
            } else {
                customColorPicker.loadColorToPicker(backColorCustomVal);
            }
        }
    }

    // Settings storage
    Settings {
        id: appSettings
        category: "NepdateDesktopWidget"
    }

    // Hidden TextEdit for clipboard operations
    TextEdit {
        id: hiddenTextEdit
        visible: false
        text: ""
    }

    // Root Container offering Rounded Corners and Window Border
    Rectangle {
        id: rootContainer
        anchors.fill: parent
        color: themeBg
        border.color: themeBorder
        border.width: 1
        radius: 8
        clip: true

        implicitHeight: titleBar.height + contentLayout.implicitHeight

        ColumnLayout {
            anchors.fill: parent
            spacing: 0

            // Title Bar matching test.html style
            Rectangle {
                id: titleBar
                Layout.fillWidth: true
                height: 44
                color: themeTitleBarBg
                radius: 8

                // Inner Rectangle to make bottom corners of title bar sharp
                Rectangle {
                    anchors.bottom: parent.bottom
                    anchors.left: parent.left
                    anchors.right: parent.right
                    height: 20
                    color: themeTitleBarBg
                    radius: 0
                }

                Rectangle {
                    anchors.bottom: parent.bottom
                    width: parent.width
                    height: 1
                    color: themeTitleBarBorder
                }

                // Dragging MouseArea covering the title bar (leaving controls space clickable)
                MouseArea {
                    anchors.fill: parent
                    anchors.rightMargin: 100
                    onPressed: (mouse) => {
                        settingsWindow.startSystemMove();
                    }
                }

                // Empty space to balance controls
                Item {
                    anchors.left: parent.left
                    anchors.leftMargin: 16
                    anchors.verticalCenter: parent.verticalCenter
                    width: 80
                    height: 20
                }

                // Window Title
                Text {
                    text: "Settings"
                    font.pointSize: 11
                    font.bold: true
                    color: themeTitleText
                    anchors.centerIn: parent
                }

                // Window Controls
                Row {
                    anchors.right: parent.right
                    anchors.rightMargin: 16
                    anchors.verticalCenter: parent.verticalCenter
                    spacing: 12

                    // Minimize Button
                    Button {
                        width: 16
                        height: 16
                        flat: true
                        hoverEnabled: true
                        background: Rectangle { color: "transparent" }
                        contentItem: Text {
                            text: "—"
                            font.pointSize: 10
                            color: parent.hovered ? (isDarkTheme ? "#ffffff" : "#333333") : "#666666"
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                        }
                        onClicked: settingsWindow.showMinimized()
                    }

                    // Maximize/Restore Button
                    Button {
                        width: 16
                        height: 16
                        flat: true
                        hoverEnabled: true
                        background: Rectangle { color: "transparent" }
                        contentItem: Text {
                            text: "□"
                            font.pointSize: 12
                            color: parent.hovered ? (isDarkTheme ? "#ffffff" : "#333333") : "#666666"
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                        }
                        onClicked: {
                            if (settingsWindow.visibility === Window.Maximized) {
                                settingsWindow.showNormal();
                            } else {
                                settingsWindow.showMaximized();
                            }
                        }
                    }

                    // Close Button
                    Button {
                        width: 16
                        height: 16
                        flat: true
                        hoverEnabled: true
                        background: Rectangle { color: "transparent" }
                        contentItem: Text {
                            text: "✕"
                            font.pointSize: 10
                            color: parent.hovered ? "#d32f2f" : "#666666"
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                        }
                        onClicked: settingsWindow.close()
                    }
                }
            }

            // Top Tab Navigation Bar
            RowLayout {
                id: topTabBar
                Layout.fillWidth: true
                Layout.leftMargin: 20
                Layout.rightMargin: 20
                Layout.topMargin: 12
                Layout.bottomMargin: 8
                spacing: 8

                property string currentTab: "general" // "general" or "appearance"

                Button {
                    id: generalTabBtn
                    text: "General"
                    Layout.fillWidth: true
                    Layout.preferredWidth: 1
                    hoverEnabled: true
                    checkable: true
                    checked: topTabBar.currentTab === "general"
                    onClicked: topTabBar.currentTab = "general"

                    background: Rectangle {
                        color: generalTabBtn.checked ? tabActiveBg : themeControlBg
                        border.color: generalTabBtn.checked ? tabActiveBorder : themeControlBorder
                        border.width: 2
                        radius: 6
                    }
                    contentItem: Text {
                        text: parent.text
                        color: generalTabBtn.checked ? tabActiveText : (isDarkTheme ? "#bbbbbb" : "#666666")
                        font.pointSize: 10
                        font.bold: true
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }
                }

                Button {
                    id: appearanceTabBtn
                    text: "Appearance"
                    Layout.fillWidth: true
                    Layout.preferredWidth: 1
                    hoverEnabled: true
                    checkable: true
                    checked: topTabBar.currentTab === "appearance"
                    onClicked: topTabBar.currentTab = "appearance"

                    background: Rectangle {
                        color: appearanceTabBtn.checked ? tabActiveBg : themeControlBg
                        border.color: appearanceTabBtn.checked ? tabActiveBorder : themeControlBorder
                        border.width: 2
                        radius: 6
                    }
                    contentItem: Text {
                        text: parent.text
                        color: appearanceTabBtn.checked ? tabActiveText : (isDarkTheme ? "#bbbbbb" : "#666666")
                        font.pointSize: 10
                        font.bold: true
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }
                }
            }

            // Scrollable Main Content Area
            ScrollView {
                id: contentScrollView
                Layout.fillWidth: true
                Layout.fillHeight: true
                clip: true
                ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
                ScrollBar.vertical.policy: ScrollBar.AsNeeded

                padding: 16
                topPadding: 16
                bottomPadding: 16
                leftPadding: 20
                rightPadding: 20

                ColumnLayout {
                    id: contentLayout
                    width: contentScrollView.availableWidth
                    spacing: 24

                // 1. Font Size Section
                ColumnLayout {
                    Layout.fillWidth: true
                    spacing: 8
                    visible: topTabBar.currentTab === "appearance"

                    Label {
                        text: "Font Size"
                        color: themeLabelColor
                        font.pointSize: 11
                        font.bold: true
                        Layout.alignment: Qt.AlignHCenter
                    }

                    Slider {
                        id: fontSizeSlider
                        Layout.fillWidth: true
                        from: 8
                        to: 24
                        value: initialFontSize
                        onValueChanged: fontSizeChanged(value)

                        background: Rectangle {
                            x: fontSizeSlider.leftPadding
                            y: fontSizeSlider.topPadding + fontSizeSlider.availableHeight / 2 - height / 2
                            implicitHeight: 6
                            width: fontSizeSlider.availableWidth
                            height: implicitHeight
                            radius: 3
                            color: isDarkTheme ? "#444444" : "#dddddd"

                            Rectangle {
                                width: fontSizeSlider.visualPosition * parent.width
                                height: parent.height
                                color: isDarkTheme ? "#666666" : "#cccccc"
                                radius: 3
                            }
                        }

                        handle: Rectangle {
                            id: fontSizeHandle
                            x: fontSizeSlider.leftPadding + fontSizeSlider.visualPosition * (fontSizeSlider.availableWidth - width)
                            y: fontSizeSlider.topPadding + fontSizeSlider.availableHeight / 2 - height / 2
                            implicitWidth: 28
                            implicitHeight: 28
                            radius: 14
                            color: "#2b7de1"
                            border.color: "#1a5fb4"
                            border.width: 1

                            // Shadow glow surrounding handle
                            Rectangle {
                                anchors.centerIn: parent
                                width: parent.width + 8
                                height: parent.height + 8
                                radius: width / 2
                                color: "#2b7de1"
                                opacity: fontSizeSlider.pressed ? 0.35 : (fontSizeSlider.hovered ? 0.2 : 0.08)
                                z: -1
                            }
                        }
                    }
                }

                // 2. Color Picker Section
                ColumnLayout {
                    Layout.fillWidth: true
                    spacing: 12
                    visible: topTabBar.currentTab === "appearance"

                    // Color Target Tabs
                    RowLayout {
                        Layout.alignment: Qt.AlignHCenter
                        Layout.fillWidth: true
                        spacing: 8

                        Button {
                            id: fontColorTabBtn
                            text: "Font Color"
                            Layout.fillWidth: true
                            Layout.preferredWidth: 1
                            checkable: true
                            checked: activeColorTarget === "font"
                            onClicked: activeColorTarget = "font"

                            background: Rectangle {
                                color: fontColorTabBtn.checked ? tabActiveBg : themeControlBg
                                border.color: fontColorTabBtn.checked ? tabActiveBorder : themeControlBorder
                                border.width: 2
                                radius: 6
                            }
                            contentItem: Text {
                                text: parent.text
                                color: fontColorTabBtn.checked ? tabActiveText : (isDarkTheme ? "#bbbbbb" : "#666666")
                                font.pointSize: 10
                                font.bold: true
                                horizontalAlignment: Text.AlignHCenter
                                verticalAlignment: Text.AlignVCenter
                            }
                        }

                        Button {
                            id: bgColorTabBtn
                            text: "Background Color"
                            Layout.fillWidth: true
                            Layout.preferredWidth: 1
                            checkable: true
                            checked: activeColorTarget === "background"
                            onClicked: activeColorTarget = "background"

                            background: Rectangle {
                                color: bgColorTabBtn.checked ? tabActiveBg : themeControlBg
                                border.color: bgColorTabBtn.checked ? tabActiveBorder : themeControlBorder
                                border.width: 2
                                radius: 6
                            }
                            contentItem: Text {
                                text: parent.text
                                color: bgColorTabBtn.checked ? tabActiveText : (isDarkTheme ? "#bbbbbb" : "#666666")
                                font.pointSize: 10
                                font.bold: true
                                horizontalAlignment: Text.AlignHCenter
                                verticalAlignment: Text.AlignVCenter
                            }
                        }
                    }

                    // Transparent Background Toggle
                    CheckBox {
                        id: transparentBgCheck
                        text: "Transparent Background"
                        visible: activeColorTarget === "background" && compositorSupportsTransparency
                        checked: backColorVal === "none"
                        Layout.alignment: Qt.AlignHCenter

                        indicator: Rectangle {
                            implicitWidth: 22
                            implicitHeight: 22
                            x: parent.leftPadding
                            y: parent.topPadding + parent.availableHeight / 2 - height / 2
                            radius: 4
                            border.color: parent.checked ? "#2b7de1" : (isDarkTheme ? "#555555" : "#cccccc")
                            border.width: 2
                            color: parent.checked ? "#2b7de1" : "transparent"

                            Text {
                                text: "✓"
                                color: "white"
                                font.bold: true
                                font.pointSize: 10
                                anchors.centerIn: parent
                                visible: parent.parent.checked
                            }
                        }

                        contentItem: Text {
                            text: parent.text
                            font.pointSize: 11
                            color: themeCheckTextColor
                            leftPadding: parent.indicator.width + parent.spacing
                            verticalAlignment: Text.AlignVCenter
                        }

                        onCheckedChanged: {
                            if (checked) {
                                backColorVal = "none";
                                backgroundColorChanged("none");
                            } else {
                                backColorVal = backColorCustomVal.toString();
                                backgroundColorChanged(backColorCustomVal.toString());
                            }
                        }
                    }

                    // Color Picker Canvas & Sliders
                    Item {
                        id: customColorPicker
                        width: 200 + 24 + (alphaSliderContainer.visible ? 24 + 24 : 12)
                        height: 200
                        Layout.alignment: Qt.AlignHCenter
                        Layout.topMargin: 8
                        Layout.bottomMargin: 8
                        opacity: (activeColorTarget === "background" && transparentBgCheck.checked) ? 0.3 : 1.0
                        enabled: !(activeColorTarget === "background" && transparentBgCheck.checked)

                        property color selectedColor: "white"
                        property real currentHue: 0
                        property real currentSaturation: 0
                        property real currentValue: 0
                        property real currentAlpha: 1.0
                        property bool isUpdating: false
                        property bool isInitialized: false

                        function loadColorToPicker(newColor) {
                            isUpdating = true;
                            selectedColor = newColor;
                            currentHue = newColor.hsvHue >= 0 ? newColor.hsvHue : 0;
                            currentSaturation = newColor.hsvSaturation;
                            currentValue = newColor.hsvValue;
                            currentAlpha = compositorSupportsTransparency ? newColor.a : 1.0;
                            isUpdating = false;
                        }

                        Component.onCompleted: {
                            isInitialized = true;
                            settingsWindow.originalFontColor = settingsWindow.fontColorVal;
                            settingsWindow.originalBackColor = settingsWindow.backColorCustomVal;
                            if (activeColorTarget === "font") {
                                loadColorToPicker(fontColorVal);
                            } else {
                                loadColorToPicker(backColorCustomVal);
                            }
                        }

                        onCurrentHueChanged: if (isInitialized && !isUpdating) updateColor()
                        onCurrentSaturationChanged: if (isInitialized && !isUpdating) updateColor()
                        onCurrentValueChanged: if (isInitialized && !isUpdating) updateColor()
                        onCurrentAlphaChanged: if (isInitialized && !isUpdating) updateColor()

                        function updateColor() {
                            var newColor = Qt.hsva(currentHue, currentSaturation, currentValue, currentAlpha);
                            if (newColor.toString() !== selectedColor.toString()) {
                                selectedColor = newColor;
                                if (activeColorTarget === "font") {
                                    fontColorVal = newColor;
                                    fontColorChanged(newColor);
                                } else {
                                    backColorCustomVal = newColor;
                                    if (!transparentBgCheck.checked) {
                                        backColorVal = newColor.toString();
                                        backgroundColorChanged(newColor.toString());
                                    }
                                }
                            }
                        }

                        Row {
                            anchors.fill: parent
                            spacing: 12

                            // Saturation-Value 2D Area
                            Rectangle {
                                id: svArea
                                width: 200
                                height: 200
                                color: Qt.hsva(customColorPicker.currentHue, 1.0, 1.0, 1.0)
                                border.color: isDarkTheme ? "#555555" : "#cccccc"
                                border.width: 1
                                radius: 4
                                clip: true

                                // Horizontal White-to-Transparent Gradient
                                Rectangle {
                                    anchors.fill: parent
                                    radius: 3
                                    gradient: Gradient {
                                        orientation: Gradient.Horizontal
                                        GradientStop { position: 0.0; color: "white" }
                                        GradientStop { position: 1.0; color: "transparent" }
                                    }
                                }

                                // Vertical Transparent-to-Black Gradient
                                Rectangle {
                                    anchors.fill: parent
                                    radius: 3
                                    gradient: Gradient {
                                        orientation: Gradient.Vertical
                                        GradientStop { position: 0.0; color: "transparent" }
                                        GradientStop { position: 1.0; color: "black" }
                                    }
                                }

                                // Interactive Circle Cursor
                                Rectangle {
                                    id: svIndicator
                                    x: svArea.width * customColorPicker.currentSaturation - (width / 2)
                                    y: svArea.height * (1.0 - customColorPicker.currentValue) - (height / 2)
                                    width: 16
                                    height: 16
                                    radius: 8
                                    color: "transparent"
                                    border.color: "white"
                                    border.width: 2

                                    Rectangle {
                                        anchors.fill: parent
                                        anchors.margins: -1
                                        radius: 9
                                        color: "transparent"
                                        border.color: "#4d000000"
                                        border.width: 1
                                    }
                                }

                                MouseArea {
                                    anchors.fill: parent
                                    preventStealing: true
                                    onPressed: (mouse) => updateSV(mouse.x, mouse.y)
                                    onPositionChanged: (mouse) => { if (pressed) updateSV(mouse.x, mouse.y) }

                                    function updateSV(mx, my) {
                                        customColorPicker.currentSaturation = Math.max(0, Math.min(1, mx / svArea.width));
                                        customColorPicker.currentValue = 1.0 - Math.max(0, Math.min(1, my / svArea.height));
                                    }
                                }
                            }

                            // Hue Vertical Slider Container
                            Item {
                                id: hueSliderContainer
                                width: 24
                                height: 200

                                // Track
                                Rectangle {
                                    id: hueSliderTrack
                                    anchors.fill: parent
                                    radius: 12
                                    border.color: isDarkTheme ? "#555555" : "#cccccc"
                                    border.width: 1
                                    clip: true

                                    Rectangle {
                                        anchors.fill: parent
                                        radius: 11
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
                                    }

                                    // Selection line indicator
                                    Rectangle {
                                        width: parent.width
                                        height: 2
                                        color: "white"
                                        border.color: "black"
                                        border.width: 0.5
                                        y: Math.max(0, Math.min(parent.height - 2, parent.height * customColorPicker.currentHue - 1))
                                    }
                                }

                                // Left Pointer (Triangle pointing right)
                                Canvas {
                                    id: hueLeftPointer
                                    width: 6
                                    height: 10
                                    x: -width - 2
                                    y: Math.max(0, Math.min(parent.height - height, parent.height * customColorPicker.currentHue - (height / 2)))
                                    onPaint: {
                                        var ctx = getContext("2d");
                                        ctx.reset();
                                        ctx.fillStyle = settingsWindow.isDarkTheme ? "#ffffff" : "#333333";
                                        ctx.beginPath();
                                        ctx.moveTo(0, 0);
                                        ctx.lineTo(width, height / 2);
                                        ctx.lineTo(0, height);
                                        ctx.closePath();
                                        ctx.fill();
                                    }
                                    Connections {
                                        target: settingsWindow
                                        function onIsDarkThemeChanged() { hueLeftPointer.requestPaint(); }
                                    }
                                }

                                MouseArea {
                                    anchors.fill: parent
                                    preventStealing: true
                                    onPressed: (mouse) => updateHue(mouse.y)
                                    onPositionChanged: (mouse) => { if (pressed) updateHue(mouse.y) }

                                    function updateHue(my) {
                                        customColorPicker.currentHue = Math.max(0, Math.min(1, my / hueSliderContainer.height));
                                    }
                                }
                            }

                            // Alpha Transparency Slider Container (Wayland/X11 Compositor Dependent)
                            Item {
                                id: alphaSliderContainer
                                visible: compositorSupportsTransparency
                                width: 24
                                height: 200

                                // Track
                                Rectangle {
                                    id: alphaSliderTrack
                                    anchors.fill: parent
                                    radius: 12
                                    border.color: isDarkTheme ? "#555555" : "#cccccc"
                                    border.width: 1
                                    clip: true

                                    // Checkerboard pattern representing transparency
                                    Grid {
                                        anchors.fill: parent
                                        columns: 2
                                        rows: 20
                                        clip: true
                                        Repeater {
                                            model: 40
                                            Rectangle {
                                                width: alphaSliderTrack.width / 2
                                                height: alphaSliderTrack.height / 20
                                                color: (index + Math.floor(index / 2)) % 2 === 0 ? "#ffffff" : "#cccccc"
                                            }
                                        }
                                    }

                                    Rectangle {
                                        anchors.fill: parent
                                        radius: 11
                                        gradient: Gradient {
                                            orientation: Gradient.Vertical
                                            GradientStop { position: 0.0; color: Qt.hsva(customColorPicker.currentHue, customColorPicker.currentSaturation, customColorPicker.currentValue, 1.0) }
                                            GradientStop { position: 1.0; color: Qt.hsva(customColorPicker.currentHue, customColorPicker.currentSaturation, customColorPicker.currentValue, 0.0) }
                                        }
                                    }

                                    // Selection line indicator
                                    Rectangle {
                                        width: parent.width
                                        height: 2
                                        color: "white"
                                        border.color: "black"
                                        border.width: 0.5
                                        y: Math.max(0, Math.min(parent.height - 2, parent.height * (1.0 - customColorPicker.currentAlpha) - 1))
                                    }
                                }

                                // Right Pointer (Triangle pointing left)
                                Canvas {
                                    id: alphaRightPointer
                                    width: 6
                                    height: 10
                                    x: parent.width + 2
                                    y: Math.max(0, Math.min(parent.height - height, parent.height * (1.0 - customColorPicker.currentAlpha) - (height / 2)))
                                    onPaint: {
                                        var ctx = getContext("2d");
                                        ctx.reset();
                                        ctx.fillStyle = settingsWindow.isDarkTheme ? "#ffffff" : "#333333";
                                        ctx.beginPath();
                                        ctx.moveTo(width, 0);
                                        ctx.lineTo(0, height / 2);
                                        ctx.lineTo(width, height);
                                        ctx.closePath();
                                        ctx.fill();
                                    }
                                    Connections {
                                        target: settingsWindow
                                        function onIsDarkThemeChanged() { alphaRightPointer.requestPaint(); }
                                    }
                                }

                                MouseArea {
                                    anchors.fill: parent
                                    preventStealing: true
                                    onPressed: (mouse) => updateAlpha(mouse.y)
                                    onPositionChanged: (mouse) => { if (pressed) updateAlpha(mouse.y) }

                                    function updateAlpha(my) {
                                        customColorPicker.currentAlpha = Math.max(0, Math.min(1, 1.0 - my / alphaSliderContainer.height));
                                    }
                                }
                            }
                        }
                    }

                    // Alpha Text Indicator
                    Label {
                        visible: activeColorTarget === "background" && compositorSupportsTransparency && !transparentBgCheck.checked
                        text: "Alpha: " + Math.round(customColorPicker.currentAlpha * 100) + "%"
                        color: isDarkTheme ? "#aaaaaa" : "#666666"
                        font.pointSize: 10
                        Layout.alignment: Qt.AlignHCenter
                        Layout.topMargin: 4
                    }

                    // Color Preview Swatches (Current vs New)
                    RowLayout {
                        Layout.alignment: Qt.AlignHCenter
                        Layout.topMargin: 8
                        spacing: 8

                        // Current Color Swatch
                        Rectangle {
                            id: previewCurrent
                            width: 60
                            height: 12
                            radius: 2
                            border.color: isDarkTheme ? "#555555" : "#cccccc"
                            border.width: 1
                            clip: true

                            Grid {
                                anchors.fill: parent
                                columns: 6
                                rows: 2
                                Repeater {
                                    model: 12
                                    Rectangle {
                                        width: 10
                                        height: 6
                                        color: (index + Math.floor(index / 6)) % 2 === 0 ? "#ffffff" : "#cccccc"
                                    }
                                }
                            }

                            Rectangle {
                                anchors.fill: parent
                                color: activeColorTarget === "font" ? originalFontColor : originalBackColor
                            }

                            ToolTip.visible: currentHover.containsMouse
                            ToolTip.text: "Current Color"
                            MouseArea {
                                id: currentHover
                                anchors.fill: parent
                                hoverEnabled: true
                            }
                        }

                        // New Selected Color Swatch
                        Rectangle {
                            id: previewNew
                            width: 60
                            height: 12
                            radius: 2
                            border.color: isDarkTheme ? "#ffffff" : "#333333"
                            border.width: 2
                            clip: true

                            Grid {
                                anchors.fill: parent
                                columns: 6
                                rows: 2
                                Repeater {
                                    model: 12
                                    Rectangle {
                                        width: 10
                                        height: 6
                                        color: (index + Math.floor(index / 6)) % 2 === 0 ? "#ffffff" : "#cccccc"
                                    }
                                }
                            }

                            Rectangle {
                                anchors.fill: parent
                                color: (activeColorTarget === "background" && transparentBgCheck.checked) ? "transparent" : customColorPicker.selectedColor
                            }

                            ToolTip.visible: newHover.containsMouse
                            ToolTip.text: "New Color"
                            MouseArea {
                                id: newHover
                                anchors.fill: parent
                                hoverEnabled: true
                            }
                        }
                    }
                }

                // 3. Show Icon Checkbox
                ColumnLayout {
                    Layout.fillWidth: true
                    spacing: 4
                    visible: topTabBar.currentTab === "general"

                    Label {
                        text: "Show Icon"
                        color: themeLabelColor
                        font.pointSize: 11
                        font.bold: true
                        Layout.alignment: Qt.AlignHCenter
                    }

                    CheckBox {
                        id: iconCheck
                        text: "Enable Icon"
                        checked: showIcon
                        Layout.alignment: Qt.AlignHCenter

                        indicator: Rectangle {
                            implicitWidth: 22
                            implicitHeight: 22
                            x: parent.leftPadding
                            y: parent.topPadding + parent.availableHeight / 2 - height / 2
                            radius: 4
                            border.color: parent.checked ? "#2b7de1" : (isDarkTheme ? "#555555" : "#cccccc")
                            border.width: 2
                            color: parent.checked ? "#2b7de1" : "transparent"

                            Text {
                                text: "✓"
                                color: "white"
                                font.bold: true
                                font.pointSize: 10
                                anchors.centerIn: parent
                                visible: parent.parent.checked
                            }
                        }

                        contentItem: Text {
                            text: parent.text
                            font.pointSize: 11
                            color: themeCheckTextColor
                            leftPadding: parent.indicator.width + parent.spacing
                            verticalAlignment: Text.AlignVCenter
                        }

                        onCheckedChanged: {
                            showIcon = checked;
                            iconVisibilityChanged(checked);
                        }
                    }
                }

                // 4. Stay on Top Checkbox
                ColumnLayout {
                    Layout.fillWidth: true
                    spacing: 4
                    visible: topTabBar.currentTab === "general"

                    Label {
                        text: "Stay widget on top"
                        color: themeLabelColor
                        font.pointSize: 11
                        font.bold: true
                        Layout.alignment: Qt.AlignHCenter
                    }

                    CheckBox {
                        id: screentopCheck
                        text: "Enable stay on screen top"
                        checked: screenontop
                        Layout.alignment: Qt.AlignHCenter

                        indicator: Rectangle {
                            implicitWidth: 22
                            implicitHeight: 22
                            x: parent.leftPadding
                            y: parent.topPadding + parent.availableHeight / 2 - height / 2
                            radius: 4
                            border.color: parent.checked ? "#2b7de1" : (isDarkTheme ? "#555555" : "#cccccc")
                            border.width: 2
                            color: parent.checked ? "#2b7de1" : "transparent"

                            Text {
                                text: "✓"
                                color: "white"
                                font.bold: true
                                font.pointSize: 10
                                anchors.centerIn: parent
                                visible: parent.parent.checked
                            }
                        }

                        contentItem: Text {
                            text: parent.text
                            font.pointSize: 11
                            color: themeCheckTextColor
                            leftPadding: parent.indicator.width + parent.spacing
                            verticalAlignment: Text.AlignVCenter
                        }

                        onCheckedChanged: {
                            screenontop = checked;
                            screenontopset(checked);
                        }
                    }
                }

                // 5. Action Buttons Group
                ColumnLayout {
                    Layout.fillWidth: true
                    spacing: 10
                    visible: topTabBar.currentTab === "general"

                    Button {
                        id: copyButton
                        text: originalCopyButtonText
                        Layout.fillWidth: true
                        hoverEnabled: true
                        onClicked: {
                            if (mainWindow && mainWindow.displayedDate) {
                                copyDateToClipboard(mainWindow.displayedDate);
                            } else {
                                showCopyMessage("No date available");
                            }
                        }
                        background: Rectangle {
                            color: copyButton.hovered ? (isDarkTheme ? "#4a4a4a" : "#d0d0d0") : (isDarkTheme ? "#3c3c3c" : "#e0e0e0")
                            border.color: isDarkTheme ? "#555" : "#ccc"
                            border.width: 1
                            radius: 6
                        }
                        contentItem: Text {
                            text: parent.text
                            color: isDarkTheme ? "#81c784" : "#2e7d32"
                            font.pointSize: 11
                            font.bold: true
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                        }
                    }

                    Button {
                        id: autostartButton
                        Layout.fillWidth: true
                        hoverEnabled: true
                        text: autostartManager.isAutostartEnabled() ? "Disable Autostart" : "Enable Autostart"

                        onClicked: {
                            autostartManager.setAutostart(!autostartManager.isAutostartEnabled());
                            autostartButton.text = autostartManager.isAutostartEnabled() ? "Disable Autostart" : "Enable Autostart";
                        }

                        background: Rectangle {
                            color: autostartButton.hovered ? (isDarkTheme ? "#4a4a4a" : "#d0d0d0") : (isDarkTheme ? "#3c3c3c" : "#e0e0e0")
                            border.color: isDarkTheme ? "#555" : "#ccc"
                            border.width: 1
                            radius: 6
                        }
                        contentItem: Text {
                            text: parent.text
                            color: isDarkTheme ? "#81c784" : "#2e7d32"
                            font.pointSize: 11
                            font.bold: true
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                        }
                    }
                }
                }
            }

            // Bottom Pinned Actions
            ColumnLayout {
                id: bottomActionsArea
                Layout.fillWidth: true
                Layout.leftMargin: 20
                Layout.rightMargin: 20
                Layout.bottomMargin: 16
                spacing: 8

                Button {
                    id: exitButton
                    text: "Exit Widget"
                    Layout.fillWidth: true
                    hoverEnabled: true
                    onClicked: exitRequested()
                    background: Rectangle {
                        color: exitButton.hovered ? (isDarkTheme ? "#b71c1c" : "#c62828") : (isDarkTheme ? "#731d1d" : "#d32f2f")
                        border.color: isDarkTheme ? "#8b1a1a" : "#b71c1c"
                        border.width: 1
                        radius: 6
                    }
                    contentItem: Text {
                        text: parent.text
                        color: "white"
                        font.pointSize: 11
                        font.bold: true
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }
                }

                Button {
                    id: closeButton
                    text: "Close Menu"
                    Layout.fillWidth: true
                    hoverEnabled: true
                    onClicked: settingsWindow.close()
                    background: Rectangle {
                        color: closeButton.hovered ? (isDarkTheme ? "#4a4a4a" : "#d0d0d0") : (isDarkTheme ? "#3c3c3c" : "#e0e0e0")
                        border.color: isDarkTheme ? "#555" : "#ccc"
                        border.width: 1
                        radius: 6
                    }
                    contentItem: Text {
                        text: parent.text
                        color: isDarkTheme ? "#81c784" : "#2e7d32"
                        font.pointSize: 11
                        font.bold: true
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }
                }
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

