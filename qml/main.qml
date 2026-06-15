/*
 * Copyright (C) 2024 khumnath
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls.Material 2.15
import QtCore as Core
import com.calendar.printer 1.0
import "qrc:/qml/"

// main.qml(main calendar window)
ApplicationWindow {
    id: window
    visible: true
    width: 590
    height: 780
    objectName: "printableRoot"
    minimumWidth: 588
    minimumHeight: 720
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
        
        if (activeTabIndex === 0) {
            calendarLogic.renderCalendarByBs(calendarLogic.currentBsYear, calendarLogic.currentBsMonthIndex);
        } else {
            calendarLogic.renderCalendarByAd(calendarLogic.currentAdYear, calendarLogic.currentAdMonth);
        }
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
        if (window.width < window.minimumWidth) window.width = window.minimumWidth;
        if (window.height < window.minimumHeight) window.height = window.minimumHeight;
        calendarLogic.initializeApp()
    }

    property bool isPreparingPrint: false

    function printCalendar() {
        console.log("Preparing to print...");
        isPreparingPrint = true;
        printTimer.start();
    }

    Timer {
        id: printTimer
        interval: 100
        repeat: false
        onTriggered: {
            Printer.print(window);
            isPreparingPrint = false;
        }
    }

    Shortcut {
        sequence: "Ctrl+P"
        onActivated: printCalendar()
    }

    Item {
        anchors.fill: parent

        ColumnLayout {
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.horizontalCenter: parent.horizontalCenter
            width: Math.min(parent.width, 850)
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
                        model: PanchangaNative.solarMonths || []
                        currentIndex: calendarLogic.currentBsMonthIndex || 0
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
                        text: PanchangaNative.getLocalDate().getFullYear().toString()
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
                        model: PanchangaNative.nepaliGregorianMonths || ["जनवरी", "फेब्रुअरी", "मार्च", "अप्रिल", "मे", "जून", "जुलाई", "अगस्ट", "सेप्टेम्बर", "अक्टोबर", "नोभेम्बर", "डिसेम्बर"]
                        currentIndex: PanchangaNative.getLocalDate().getMonth()
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
            Layout.maximumWidth: parent.width
            horizontalAlignment: Text.AlignHCenter
            color: theme.secondaryText
            font.pixelSize: 11
            font.italic: true
            Layout.bottomMargin: 5
        }

        Label {
            visible: calendarLogic.isCurrentMonthUnverified && !calendarLogic.isCurrentMonthComputed
            text: "This future date is based on available data but not officially verified."
            Layout.fillWidth: true
            Layout.maximumWidth: parent.width
            horizontalAlignment: Text.AlignHCenter
            color: theme.secondaryText
            font.pixelSize: 11
            font.italic: true
            Layout.bottomMargin: 5
        }

        CalendarGrid {
            id: calendar
            Layout.fillWidth: true
            theme: theme
            isAdMode: window.activeTabIndex === 1
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
                        text: "पञ्चाङ्ग आधुनिक दृक्-गणित र वैदिक विधिमा आधारित | स्थान: काठमाण्डौ | समय: नेपाल मानक समय क्षेत्र।"
                        font.pixelSize: 12
                        color: theme.secondaryText
                        horizontalAlignment: Text.AlignHCenter
                        anchors.horizontalCenter: parent.horizontalCenter
                        width: parent.width
                        wrapMode: Text.WordWrap
                    }

                    Label {
                        textFormat: Text.RichText
                        text: "<a href='https://opensource.org/licenses/GPL-3.0' style='color:" + theme.accentText + "; text-decoration:none;'>License: GPL-3.0</a> or later. || <a href='https://github.com/khumnath/nepdate' style='color:" + theme.accentText + "; text-decoration:none;'>Source Code</a>"
                        font.pixelSize: 12
                        color: theme.secondaryText
                        horizontalAlignment: Text.AlignHCenter
                        anchors.horizontalCenter: parent.horizontalCenter
                        width: parent.width
                        wrapMode: Text.WordWrap
                        onLinkActivated: function(link) { Qt.openUrlExternally(link) }
                    }
                }

                Rectangle {
                    id: info
                    visible: !window.isPrintMode
                    width: 30
                    height: 30
                    radius: width / 2
                    color: infoMouseArea.containsMouse ? theme.accent : theme.tertiaryBg
                    border.color: infoMouseArea.containsMouse ? theme.accent : "gray"
                    border.width: infoMouseArea.containsMouse ? 2 : 1

                    anchors.right: parent.right
                    anchors.rightMargin: 10
                    anchors.bottom: parent.bottom
                    anchors.bottomMargin: 10

                    Text {
                        text: "i"
                        font.pixelSize: 18
                        font.bold: true
                        font.family: "serif"
                        color: infoMouseArea.containsMouse ? "white" : theme.primaryText
                        anchors.centerIn: parent
                    }

                    MouseArea {
                        id: infoMouseArea
                        anchors.fill: parent
                        hoverEnabled: true
                        cursorShape: Qt.PointingHandCursor
                        onClicked: infoDialog.open()
                    }
                }
            }
        }
    }
}

// Preparing to Print Overlay
    Rectangle {
        id: preparingOverlay
        anchors.fill: parent
        color: theme.isDark ? Qt.rgba(0, 0, 0, 0.7) : Qt.rgba(1, 1, 1, 0.7)
        visible: isPreparingPrint && !window.isPrintMode
        z: 9999

        Column {
            anchors.centerIn: parent
            spacing: 20

            BusyIndicator {
                anchors.horizontalCenter: parent.horizontalCenter
                running: parent.visible
            }

            Label {
                text: "Preparing to print..."
                color: theme.primaryText
                font.pixelSize: 18
                font.bold: true
            }
        }

        // Block mouse events while preparing
        MouseArea {
            anchors.fill: parent
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
