import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls.Material 2.15
import QtCore as Core
import com.calendar.printer 1.0
import "qrc:/PanchangaCalculator.js" as Panchanga
import "qrc:/qml/"

// main.qml(main calendar window)
ApplicationWindow {
    id: window
    visible: true
    width: 800
    height: 720
    objectName: "printableRoot"
    minimumWidth: 540
    minimumHeight: 700
    title: "नेपाली क्यालेण्डर"
    color: theme.primaryBg
    Material.theme: theme.isDark ? Material.Dark : Material.Light
    Material.accent: theme.accent
    flags: Qt.Window

    property bool isPrintMode: false
    property int activeTabIndex: 0

    onActiveTabIndexChanged: {
        adYearInput.text = calendarLogic.currentAdYear.toString();
        adMonthSelect.currentIndex = calendarLogic.currentAdMonth;
        bsYearInput.internalAsciiValue = calendarLogic.currentBsYear.toString();
        bsMonthSelect.currentIndex = calendarLogic.currentBsMonthIndex;
    }

    Theme { id: theme }
    CalendarLogic { id: calendarLogic }

    // State handlers
    Connections {
        target: calendarLogic

        function onCurrentAdYearChanged() {
            adYearInput.text = calendarLogic.currentAdYear.toString();
        }
        function onCurrentAdMonthChanged() {
            adMonthSelect.currentIndex = calendarLogic.currentAdMonth;
        }
        function onCurrentBsYearChanged() {
            bsYearInput.internalAsciiValue = calendarLogic.currentBsYear.toString();
        }
        function onCurrentBsMonthIndexChanged() {
            bsMonthSelect.currentIndex = calendarLogic.currentBsMonthIndex;
        }
    }

    Core.Settings {
        id: settings
        category: "Appearance"
        property alias isDarkMode: theme.isDark
    }

    Core.Settings {
        id: windowSettings
        category: "WindowGeometry"
        property alias x: window.x
        property alias y: window.y
        property alias width: window.width
        property alias height: window.height
    }

    onClosing: {
        windowSettings.sync();
        settings.sync();
        calendarLogic.calendarModel = [];
        destroy();
    }

    Component.onCompleted: {
        if (window.width < 400) window.width = 400;
        if (window.height < 300) window.height = 300;
        calendarLogic.initializeApp()
    }

    function printCalendar() {
        console.log("Calling print function...");
        Printer.print(window);
    }

    Shortcut {
        sequence: "Ctrl+P"
        onActivated: printCalendar()
    }

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        Header {
            id: header
            theme: theme
            currentBsLabelStr: calendarLogic.currentBsLabelStr
            currentAdLabelStr: calendarLogic.currentAdLabelStr
            isPrintMode: window.isPrintMode
            onPrintClicked: printCalendar()
            onThemeToggleClicked: theme.isDark = !theme.isDark
        }

        NavigationBar {
            id: navBar
            theme: theme
            Layout.bottomMargin: 8
            calendarLogic: calendarLogic
            isPrintMode: window.isPrintMode
            onTabChanged: function(newIndex) { activeTabIndex = newIndex }
            onTodayClicked: {
                calendarLogic.initializeApp(); // Reset the logic state

                // Now, guarantee the UI inputs are updated
                adYearInput.text = calendarLogic.currentAdYear.toString();
                adMonthSelect.currentIndex = calendarLogic.currentAdMonth;
                bsYearInput.internalAsciiValue = calendarLogic.currentBsYear.toString();
                bsMonthSelect.currentIndex = calendarLogic.currentBsMonthIndex;
            }
        }

        Rectangle {
            visible: !window.isPrintMode
            Layout.fillWidth: true
            Layout.leftMargin: 20
            Layout.rightMargin: 20
            Layout.bottomMargin: 8
            color: theme.primaryBg
            radius: 12
            border.color: theme.borderColor
            border.width: 1
            Layout.preferredHeight: 50

            Item {
                anchors.fill: parent
                anchors.margins: 10

                // BS Date Navigation
                RowLayout {
                    id: bsNavGroup
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: parent.left
                    anchors.right: parent.right
                    spacing: 10
                    visible: activeTabIndex === 0

                    TextField {
                        id: bsYearInput
                        Layout.fillWidth: true
                        Layout.preferredWidth: parent.width / 2 - parent.spacing / 2
                        horizontalAlignment: TextInput.AlignHCenter
                        font.pixelSize: 14
                        color: theme.primaryText
                        inputMethodHints: Qt.ImhDigitsOnly
                        padding: 6
                        background: Rectangle {
                            radius: 8
                            border.color: theme.borderColor
                            border.width: 1
                            color: theme.inputBg
                        }
                        property string internalAsciiValue: calendarLogic.currentBsYear.toString()
                        onInternalAsciiValueChanged: {
                            const newDevanagariText = calendarLogic.toDevanagari(internalAsciiValue)
                            if (text !== newDevanagariText) {
                                text = newDevanagariText
                            }
                        }
                        onTextEdited: {
                            const asciiValue = calendarLogic.fromDevanagari(text)
                            const sanitizedAscii = asciiValue.replace(/[^\d]/g, "")
                            if (internalAsciiValue !== sanitizedAscii) {
                                internalAsciiValue = sanitizedAscii
                                cursorPosition = text.length
                            }
                        }
                        onAccepted: {
                            const parsedValue = parseInt(internalAsciiValue) || calendarLogic.currentBsYear
                            calendarLogic.renderCalendarByBs(parsedValue, bsMonthSelect.currentIndex)
                        }
                    }

                    ComboBox {
                        id: bsMonthSelect
                        model: Panchanga.solarMonths || []
                        currentIndex: calendarLogic.currentBsMonth || 0
                        Layout.fillWidth: true
                        Layout.preferredWidth: parent.width / 2 - parent.spacing / 2
                        font.pixelSize: 14
                        padding: 6
                        background: Rectangle {
                            radius: 8
                            border.color: theme.borderColor
                            border.width: 1
                            color: theme.inputBg
                        }
                        contentItem: Text {
                            text: bsMonthSelect.currentText
                            color: theme.primaryText
                            font.pixelSize: 14
                            verticalAlignment: Text.AlignVCenter
                            elide: Text.ElideRight
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.left: parent.left
                            anchors.leftMargin: 10
                        }
                        popup.background: Rectangle { color: theme.secondaryBg; border.color: theme.borderColor; radius: 8 }
                        delegate: ItemDelegate {
                            width: parent.width
                            hoverEnabled: true
                            height: 35
                            background: Rectangle {
                                color: highlighted || hovered ? theme.tertiaryBg : "transparent"
                            }
                            contentItem: Rectangle {
                                color: "transparent"
                                anchors.fill: parent
                                Text {
                                    text: modelData
                                    color: theme.primaryText
                                    font.pixelSize: 14
                                    anchors.verticalCenter: parent.verticalCenter
                                    anchors.left: parent.left
                                    anchors.leftMargin: 10
                                    elide: Text.ElideRight
                                }
                            }
                        }
                        onActivated: {
                            const year = parseInt(bsYearInput.internalAsciiValue) || calendarLogic.currentBsYear;
                            calendarLogic.renderCalendarByBs(year, currentIndex);
                        }
                    }
                }

                // AD Date Navigation
                RowLayout {
                    id: adNavGroup
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: parent.left
                    anchors.right: parent.right
                    spacing: 10
                    visible: activeTabIndex === 1

                    TextField {
                        id: adYearInput
                        text: new Date().getFullYear().toString()
                        color: theme.primaryText
                        inputMethodHints: Qt.ImhDigitsOnly
                        font.pixelSize: 14
                        Layout.fillWidth: true
                        Layout.preferredWidth: parent.width / 2 - parent.spacing / 2
                        horizontalAlignment: TextInput.AlignHCenter
                        background: Rectangle { radius: 8; border.color: theme.borderColor; border.width: 1; color: theme.inputBg }
                        padding: 6
                        onAccepted: calendarLogic.renderCalendarByAd(parseInt(text), adMonthSelect.currentIndex)
                    }

                    ComboBox {
                        id: adMonthSelect
                        model: Panchanga.nepaliGregorianMonths || ["जनवरी", "फेब्रुअरी", "मार्च", "अप्रिल", "मे", "जून", "जुलाई", "अगस्ट", "सेप्टेम्बर", "अक्टोबर", "नोभेम्बर", "डिसेम्बर"]
                        currentIndex: new Date().getMonth()
                        Layout.fillWidth: true
                        Layout.preferredWidth: parent.width / 2 - parent.spacing / 2
                        font.pixelSize: 14
                        padding: 6
                        background: Rectangle { radius: 8; border.color: theme.borderColor; border.width: 1; color: theme.inputBg }
                        contentItem: Text {
                            text: adMonthSelect.currentText; color: theme.primaryText; font.pixelSize: 14
                            verticalAlignment: Text.AlignVCenter; elide: Text.ElideRight
                            anchors.verticalCenter: parent.verticalCenter; anchors.left: parent.left; anchors.leftMargin: 10
                        }
                        popup.background: Rectangle { color: theme.secondaryBg; border.color: theme.borderColor; radius: 8 }
                        delegate: ItemDelegate {
                            width: parent.width
                            hoverEnabled: true
                            height: 35
                            background: Rectangle {
                                color: highlighted || hovered ? theme.tertiaryBg : "transparent"
                            }
                            contentItem: Rectangle {
                                color: "transparent"
                                anchors.fill: parent
                                Text {
                                    text: modelData
                                    color: theme.primaryText
                                    font.pixelSize: 14
                                    anchors.verticalCenter: parent.verticalCenter
                                    anchors.left: parent.left
                                    anchors.leftMargin: 10
                                    elide: Text.ElideRight
                                }
                            }
                        }
                        onActivated: calendarLogic.renderCalendarByAd(parseInt(adYearInput.text), currentIndex)
                    }
                }
            }
        }


        Label {
            visible: calendarLogic.isCurrentMonthComputed
            text: "This date is out of available data. Generated using computation."
            Layout.fillWidth: true
            horizontalAlignment: Text.AlignHCenter
            color: theme.secondaryText
            font.pixelSize: 11
            font.italic: true
            Layout.bottomMargin: 5
        }

        CalendarGrid {
            id: calendar
            theme: theme
            calendarModel: calendarLogic.calendarModel
            onDayClicked: function(panchanga) {
                panchangaDetailDialog.panchangaData = panchanga
                panchangaDetailDialog.open()
            }
            eventListModel: calendarLogic.currentMonthEvents
        }


        Rectangle {
            Layout.fillWidth: true
            Layout.preferredHeight: 100
            color: theme.tertiaryBg
            border.color: theme.borderColor
            border.width: 1

            Item {
                anchors.fill: parent
                anchors.margins: 10

                Column {
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.verticalCenter: parent.verticalCenter
                    spacing: 4
                    width: parent.width

                    Label {
                        text: "पञ्चाङ्ग सूर्य सिद्धान्तमा आधारित | स्थान: काठमाण्डौ | समय: नेपाल मानक समय क्षेत्र।"
                        font.pixelSize: 12
                        color: theme.secondaryText
                        horizontalAlignment: Text.AlignHCenter
                        anchors.horizontalCenter: parent.horizontalCenter
                    }

                    Label {
                        textFormat: Text.RichText
                        text: "<a href='https://opensource.org/licenses/GPL-3.0' style='color:" + theme.accentText + "; text-decoration:none;'>License: GPL-3.0</a> or later. || <a href='https://github.com/khumnath/nepdate' style='color:" + theme.accentText + "; text-decoration:none;'>Source Code</a>"
                        font.pixelSize: 12
                        color: theme.secondaryText
                        horizontalAlignment: Text.AlignHCenter
                        anchors.horizontalCenter: parent.horizontalCenter
                        onLinkActivated: Qt.openUrlExternally(link)
                    }
                }

                Button {
                    id: info
                    visible: !window.isPrintMode
                    text: "ⓘ"
                    font.pixelSize: 15
                    width: 40
                    height: 40
                    anchors.right: parent.right
                    anchors.rightMargin: 10
                    anchors.verticalCenter: parent.verticalCenter

                    onClicked: infoDialog.open()

                    background: Rectangle {
                        radius: 8
                        color: theme.tertiaryBg
                        border.color: parent.hovered ?  theme.accent : "gray"
                        border.width: parent.hovered ? 2 : 1
                    }

                    contentItem: Text {
                        text: parent.text
                        font: parent.font
                        color: theme.primaryText
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        anchors.fill: parent
                        scale: parent.hovered ? 1.2 : 1.0
                        z: parent.hovered ? 1 : 0
                        Behavior on scale {
                            NumberAnimation { duration: 150; easing.type: Easing.InOutQuad }
                        }
                    }
                }
            }
        }
    }

    InfoDialog {
        id: infoDialog
        theme: theme
    }

    PanchangaDetailDialog {
        id: panchangaDetailDialog
        theme: theme
    }
}
