import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls.Material 2.15
import QtCore
import "qrc:PanchangaCalculator.js" as Panchanga

ApplicationWindow {
    id: window
    visible: true
    width: 800
    height: 700
    title: "नेपाली क्यालेण्डर"
    color: theme.primaryBg
    Material.theme: theme.isDark ? Material.Dark : Material.Light
    Material.accent: theme.accent
    flags: Qt.Window


    Settings {
        id: settings
        category: "Appearance"
        property alias isDarkMode: window.isDarkMode
    }

    Settings {
        id: windowSettings
        category: "WindowGeometry"
        property alias x: window.x
        property alias y: window.y
        property alias width: window.width
        property alias height: window.height
    }
    property bool isDarkMode: false
    property bool debugVisible: false
    property string currentDebugInfo: ""
    QtObject {
        id: theme
        readonly property bool isDark: window.isDarkMode
        readonly property color primaryBg: isDark ? "#1e293b" : "#f8fafc"
        readonly property color secondaryBg: isDark ? "#334155" : "white"
        readonly property color tertiaryBg: isDark ? "#475569" : "#f1f5f9"
        readonly property color inputBg: isDark ? "#475569" : "#f8fafc"
        readonly property color primaryText: isDark ? "#f1f5f9" : "#1e293b"
        readonly property color secondaryText: isDark ? "#cbd5e1" : "#64748b"
        readonly property color accentText: isDark ? "#5eead4" : "#0f766e"
        readonly property color saturdayText: isDark ? "#fca5a5" : "#dc2626"
        readonly property color headerBg: isDark ? "#1e293b" : "white"
        readonly property color headerPrimaryText: isDark ? "white" : "#1e293b"
        readonly property color headerSecondaryText: isDark ? "#e0f2fe" : "#475569"
        readonly property color headerDivider: isDark ? "#94a3b8" : "#cbd5e1"
        readonly property color accent: isDark ? "#2dd4bf" : "#0d9488"
        readonly property color accentHover: isDark ? "#5eead4" : "#14b8a6"
        readonly property color accentDark: isDark ? "#14b8a6" : "#0f766e"
        readonly property color borderColor: isDark ? "#475569" : "#e2e8f0"
        readonly property color todayBorder: isDark ? "#2dd4bf" : "#0d9488"
        readonly property color saturdayBorder: isDark ? "#f472b6" : "#fda4af"
        readonly property color todayBg: isDark ? "#115e59" : "#f0fdfa"
        readonly property color saturdayBg: isDark ? "#831843" : "#fff1f2"
        readonly property color purnimaText: isDark ? "#fcd34d" : "#b45309"
        readonly property color purnimaIconBg: isDark ? "#78350f" : "#fef3c7"
        readonly property color amavasyaIconBg: isDark ? "#334155" : "#e2e8f0"
        readonly property color modalHeaderText: isDark ? theme.headerPrimaryText : theme.headerPrimaryText
        readonly property color modalButtonBg: isDark ? theme.headerBg : theme.headerBg
        readonly property color modalButtonText: isDark ? theme.headerPrimaryText : theme.headerPrimaryText
    }

    onClosing: {
       //  console.log("Closing application, cleaning up resources...");
        windowSettings.sync();
        settings.sync();
        calendarModel = [];
        clearPanchangaDetails();
        // console.log("Cleanup complete. Exiting.");
        destroy();
    }

    Component.onCompleted: {
        if (window.width < 400) window.width = 400;
        if (window.height < 300) window.height = 300;
        initializeApp()
    }

    property int currentBsYear: 2081
    property int currentBsMonthIndex: 0
    readonly property date todayDate: new Date()
    property var calendarModel: []
    property string currentBsLabelStr: ""
    property string currentAdLabelStr: ""
    property string prevMonthName: ""
    property string nextMonthName: ""
    property bool isCurrentMonthComputed: false

    function toDevanagari(num) {
        return Panchanga.toDevanagari(num)
    }

    function initializeApp() {
        var todayInfo = Panchanga.getTodayBsInfo()
        currentBsYear = todayInfo.year
        currentBsMonthIndex = todayInfo.monthIndex
        renderCalendarByBs(currentBsYear, currentBsMonthIndex)
    }

    function getGregorianRange(bsYear, monthIndex) {
        var first = Panchanga.fromBikramSambat(bsYear, monthIndex, 1)
        var info = Panchanga.getBikramMonthInfo(bsYear, monthIndex)
        var last = Panchanga.fromBikramSambat(bsYear, monthIndex, info.totalDays)
        var firstMonth = Panchanga.nepaliGregorianMonths[first.getUTCMonth()]
        var lastMonth = Panchanga.nepaliGregorianMonths[last.getUTCMonth()]
        var firstYear = first.getUTCFullYear()
        var lastYear = last.getUTCFullYear()
        if (firstMonth !== lastMonth || firstYear !== lastYear) {
            return firstMonth + " " + firstYear + " - " + lastMonth + " " + lastYear
        } else {
            return firstMonth + " " + firstYear
        }
    }

    function renderCalendarByBs(year, monthIndex) {
        calendarModel = [];
        currentBsYear = year
        currentBsMonthIndex = monthIndex
        var info = Panchanga.getBikramMonthInfo(year, monthIndex)
        if (!info) {
            console.error("Failed to get Bikram month info for", year, monthIndex)
            return
        }
        var daysInMonth = info.totalDays
        var startDay = info.startDayOfWeek
        var model = []
        var weekdaysNe = ["आइतबार", "सोमबार", "मङ्गलबार", "बुधबार", "बिहीबार", "शुक्रबार", "शनिबार"]
        for (var i = 0; i < 7; ++i) {
            model.push({ type: "header", text: weekdaysNe[i] })
        }
        for (i = 0; i < startDay; ++i) {
            model.push({ type: "empty" })
        }
        for (var day = 1; day <= daysInMonth; ++day) {
            var adDate = Panchanga.fromBikramSambat(year, monthIndex, day)
            var result = Panchanga.calculate(adDate)
            if (day === 1) {
                           window.isCurrentMonthComputed = result.isComputed;
                       }
            var isToday = adDate.toDateString() === todayDate.toDateString()
            var isSaturday = (startDay + day - 1) % 7 === 6
            result.monthName = info.monthName;
            model.push({
                type: "day",
                bsDay: day,
                adDay: adDate.getDate(),
                tithi: result.tithi,
                isToday: isToday,
                isSaturday: isSaturday,
                gregorianDate: result.gregorianDate,
                panchanga: result
            })
        }
        calendarModel = model
        currentBsLabelStr = toDevanagari(year) + " " + info.monthName
        currentAdLabelStr = getGregorianRange(year, monthIndex)
        var prevMonthIndex = monthIndex - 1;
        var prevYear = year;
        if (prevMonthIndex < 0) {
            prevMonthIndex = 11;
            prevYear--;
        }
        prevMonthName = Panchanga.solarMonths[prevMonthIndex] || "";
        var nextMonthIndex = monthIndex + 1;
        var nextYear = year;
        if (nextMonthIndex > 11) {
            nextMonthIndex = 0;
            nextYear++;
        }
        nextMonthName = Panchanga.solarMonths[nextMonthIndex] || "";
        // console.log("✅ Rendered:", year, info.monthName, "| Days:", daysInMonth, "| Start weekday:", startDay)
    }

    function fromDevanagari(devanagariStr) {
        // Return an empty string if the input isn't a string to avoid errors.
        if (typeof devanagariStr !== 'string') {
            return "";
        }

        const digitMap = {
            '०': '0', '१': '1', '२': '2', '३': '3', '४': '4',
            '५': '5', '६': '6', '७': '7', '८': '8', '९': '9'
        };

        // Find all Devanagari digits globally (/g) and replace them using the map.
        return devanagariStr.replace(/[०-९]/g, (match) => digitMap[match]);
    }

    function renderCalendarByAd(year, monthIndex) {
        var d = 1;
        var date = new Date(Date.UTC(year, monthIndex, d));
        var jd = Panchanga.toJulianDay(year, monthIndex + 1, d);
        var ahar = jd - Panchanga.KaliEpoch;
        var sunLong = Panchanga.trueLongitudeSun(ahar);
        var bsInfo = Panchanga.getBikramSambatInfo(ahar, sunLong);
        renderCalendarByBs(bsInfo.year, bsInfo.monthIndex);
    }

    function navigateMonth(direction) {
            var newMonth = currentBsMonthIndex + direction;
            var newYear = currentBsYear;
            if (newMonth > 11) {
                newMonth = 0;
                newYear++;
            } else if (newMonth < 0) {
                newMonth = 11;
                newYear--;
            }
            bsYearInput.internalAsciiValue = newYear.toString();
            renderCalendarByBs(newYear, newMonth);
        }


    function clearPanchangaDetails() {
        for (var i = panchangaDetails.children.length - 1; i >= 0; i--) {
            panchangaDetails.children[i].destroy();
        }
    }

    function showDetail(panchanga) {
        clearPanchangaDetails();
        modalTitle.text = panchanga.gregorianDate;
        panchangaDetails.data = [
            createDetailRow("बिक्रम संवत", toDevanagari(panchanga.bsYear) + " " + panchanga.monthName + " " + toDevanagari(panchanga.bsDay)),
            createDetailRow("वार", panchanga.weekday),
            createDetailRow("तिथि", panchanga.tithi + " (" + panchanga.paksha + ")"),
            createDetailRow("नक्षत्र", panchanga.nakshatra),
            createDetailRow("योग", panchanga.yoga),
            createDetailRow("करण", panchanga.karana),
            createDetailRow("सूर्य राशि", panchanga.sunRashi),
            createDetailRow("चन्द्र राशि", panchanga.moonRashi),
            createDetailRow("उदयास्त", "सूर्योदय " + panchanga.sunrise + " | सूर्यास्त " + panchanga.sunset),
            createDetailRow("अधिक/क्षय मास", panchanga.adhikaMasa)
        ];

        // Generate debug info for the selected date
        try {
            // Extract date components from gregorianDate string
            var dateParts = panchanga.gregorianDate.split(" ");
            var monthNames = ["January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"];
            var monthIndex = monthNames.indexOf(dateParts[1]);
            var day = parseInt(dateParts[2]);
            var year = parseInt(dateParts[3]);

            if (monthIndex !== -1) {
                var debugDate = new Date(Date.UTC(year, monthIndex, day));
                var debugInfo = Panchanga.generateDebugInfo(debugDate);
                window.currentDebugInfo = debugInfo.debug || "Debug information not available";
            } else {
                window.currentDebugInfo = "Debug Information:\n    Error: Could not parse date for debug information";
            }
        } catch (e) {
            console.error("Error generating debug info:", e);
            window.currentDebugInfo = "Debug Information:\n    Error: " + e.message;
        }

        window.debugVisible = false;
        detailModal.open();
    }


    function createDetailRow(label, value) {
        var rowItem = Qt.createQmlObject('import QtQuick.Layouts 1.15; RowLayout { spacing: 10; Layout.fillWidth: true }', panchangaDetails);
        if (rowItem) {
            var label1 = Qt.createQmlObject('import QtQuick.Controls 2.15; Label {}', rowItem);
            if (label1) {
                label1.text = "<b>" + label + ":</b>";
                label1.color = theme.accentText;
                label1.font.bold = true;
                label1.width = 150;
                label1.font.pixelSize = 14;
            }
            var label2 = Qt.createQmlObject('import QtQuick.Controls 2.15; import QtQuick 2.15; Label {}', rowItem);
            if (label2) {
                label2.text = value;
                label2.color = theme.primaryText;
                label2.wrapMode = Text.WordWrap;
                label2.font.pixelSize = 14;
                label2.Layout.fillWidth = true;
            }
            return rowItem;
        }
        console.error("Failed to create detail row item.");
        return null;
    }

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        Rectangle {
            id: headerRect
            Layout.fillWidth: true
            Layout.preferredHeight: 80
            color: theme.headerBg
            border.width: 1
            border.color: theme.isDark ? "transparent" : theme.borderColor
            radius: 0

            ColumnLayout {
                anchors.centerIn: parent
                spacing: 4

                Label {
                    text: "नेपाली क्यालेण्डर"
                    font.pixelSize: 24
                    font.bold: true
                    color: theme.headerPrimaryText
                    horizontalAlignment: Text.AlignHCenter
                    Layout.alignment: Qt.AlignHCenter
                }

                RowLayout {
                    spacing: 20
                    Layout.alignment: Qt.AlignHCenter

                    Label {
                        text: "<b>बि.सं.</b> " + currentBsLabelStr
                        renderType: Text.NativeRendering
                        color: theme.headerSecondaryText
                        font.pixelSize: 16
                    }

                    Rectangle {
                        width: 1
                        height: 20
                        color: theme.headerDivider
                    }

                    Label {
                        text: "<b>ई.सं.</b> " + currentAdLabelStr
                        renderType: Text.NativeRendering
                        color: theme.headerSecondaryText
                        font.pixelSize: 16
                    }
                }
            }

            Button {
                id: themeToggleBtn
                anchors.right: parent.right
                anchors.verticalCenter: parent.verticalCenter
                anchors.rightMargin: 15
                text: theme.isDark ? "☀️" : "🌙"
                font.pixelSize: 20
                flat: true
                onClicked: window.isDarkMode = !window.isDarkMode
                background: Rectangle {
                    color: "transparent"
                }
                contentItem: Text {
                    text: parent.text
                    font: parent.font
                    color: theme.headerPrimaryText
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
            }
        }

        RowLayout {
            Layout.fillWidth: true
            Layout.leftMargin: 20
            Layout.rightMargin: 20
            Layout.topMargin: 5
            Layout.bottomMargin: 2
            spacing: 10

            Button {
                text: "‹ " + prevMonthName
                font.pixelSize: 13
                onClicked: navigateMonth(-1)
                visible: tabBar.currentIndex === 0 && prevMonthName
                padding: 8
                background: Rectangle {
                    radius: 8
                    color: theme.tertiaryBg
                    border.color: theme.borderColor
                    border.width: 1
                }
                contentItem: Text {
                    text: parent.text
                    font: parent.font
                    color: theme.primaryText
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
            }

            Item { Layout.fillWidth: true }

            RowLayout {
                Layout.alignment: Qt.AlignHCenter
                spacing: 10

                TabBar {
                    id: tabBar
                    currentIndex: 0
                    background: Rectangle { color: "transparent" }

                    TabButton {
                        text: "बिक्रम संवत"
                        width: implicitWidth + 15
                        font.pixelSize: 13
                        contentItem: Text {
                            text: parent.text; font: parent.font; color: parent.checked ? theme.accentText : theme.secondaryText
                            horizontalAlignment: Text.AlignHCenter; verticalAlignment: Text.AlignVCenter
                        }
                        background: Rectangle {
                            radius: 8; color: parent.checked ? theme.accentText + "33" : "transparent"
                            border.color: parent.checked ? theme.accent : "transparent"; border.width: 2
                        }
                    }

                    TabButton {
                        text: "ग्रेगोरियन"
                        width: implicitWidth + 15
                        font.pixelSize: 13
                        contentItem: Text {
                            text: parent.text; font: parent.font; color: parent.checked ? theme.accentText : theme.secondaryText
                            horizontalAlignment: Text.AlignHCenter; verticalAlignment: Text.AlignVCenter
                        }
                        background: Rectangle {
                            radius: 8; color: parent.checked ? theme.accentText + "33" : "transparent"
                            border.color: parent.checked ? theme.accent : "transparent"; border.width: 2
                        }
                    }
                }

                Button {
                    text: "आज"
                    font.pixelSize: 13
                    onClicked: initializeApp()
                    background: Rectangle {
                        radius: 8
                        color: theme.tertiaryBg
                        border.color: theme.borderColor
                        border.width: 1
                    }
                    contentItem: Text {
                        text: parent.text
                        font: parent.font
                        color: theme.primaryText
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }
                }
            }

            Item { Layout.fillWidth: true }

            Button {
                text: nextMonthName + " ›"
                font.pixelSize: 13
                onClicked: navigateMonth(1)
                visible: tabBar.currentIndex === 0 && nextMonthName
                padding: 8
                background: Rectangle {
                    radius: 8
                    color: theme.tertiaryBg
                    border.color: theme.borderColor
                    border.width: 1
                }
                contentItem: Text {
                    text: parent.text
                    font: parent.font
                    color: theme.primaryText
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
            }
        }

        Rectangle {
            Layout.fillWidth: true
            Layout.leftMargin: 20
            Layout.rightMargin: 20
            color: theme.secondaryBg
            radius: 12
            border.color: theme.borderColor
            border.width: 1
            Layout.preferredHeight: 60

            Item {
                anchors.fill: parent
                anchors.margins: 10

                RowLayout {
                    id: bsNavGroup
                    anchors.fill: parent
                    spacing: 10
                    visible: tabBar.currentIndex === 0

                    TextField {
                        id: bsYearInput
                        Layout.fillWidth: true
                        Layout.preferredWidth: 1
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

                        property string internalAsciiValue: currentBsYear.toString()

                        onInternalAsciiValueChanged: {
                            const newDevanagariText = toDevanagari(internalAsciiValue)
                            if (text !== newDevanagariText) {
                                text = newDevanagariText
                            }
                        }

                        onTextEdited: {
                            const asciiValue = fromDevanagari(text)
                            const sanitizedAscii = asciiValue.replace(/[^\d]/g, "")
                            if (internalAsciiValue !== sanitizedAscii) {
                                internalAsciiValue = sanitizedAscii
                                // Position cursor at the end for a better editing experience
                                cursorPosition = text.length
                            }
                        }

                        onAccepted: {
                            const parsedValue = parseInt(internalAsciiValue) || currentBsYear
                            renderCalendarByBs(parsedValue, bsMonthSelect.currentIndex)
                        }
                    }

                    ComboBox {
                        id: bsMonthSelect
                        model: Panchanga.solarMonths || []
                        currentIndex: currentBsMonthIndex
                        Layout.fillWidth: true
                        Layout.preferredWidth: 1
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
                            anchors.leftMargin: 0
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
                            const year = parseInt(bsYearInput.internalAsciiValue) || currentBsYear;
                            renderCalendarByBs(year, currentIndex);
                        }
                    }
                }

                RowLayout {
                    id: adNavGroup
                    anchors.fill: parent
                    spacing: 10
                    visible: tabBar.currentIndex === 1

                    TextField {
                        id: adYearInput
                        text: todayDate.getFullYear().toString()
                        color: theme.primaryText
                        inputMethodHints: Qt.ImhDigitsOnly
                        font.pixelSize: 14; Layout.fillWidth: true
                        Layout.preferredWidth: 1
                        horizontalAlignment: TextInput.AlignHCenter
                        background: Rectangle { radius: 8; border.color: theme.borderColor; border.width: 1; color: theme.inputBg }
                        padding: 6
                        onAccepted: renderCalendarByAd(parseInt(text), adMonthSelect.currentIndex)
                    }

                    ComboBox {
                        id: adMonthSelect
                        model: Panchanga.nepaliGregorianMonths || ["जनवरी", "फेब्रुअरी", "मार्च", "अप्रिल", "मे", "जून", "जुलाई", "अगस्ट", "सेप्टेम्बर", "अक्टोबर", "नोभेम्बर", "डिसेम्बर"]
                        currentIndex: todayDate.getMonth()
                        Layout.fillWidth: true; font.pixelSize: 14
                        Layout.preferredWidth: 1
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

                        onActivated: renderCalendarByAd(parseInt(adYearInput.text), currentIndex)
                    }
                }
            }
        }

        Label {
                    visible: isCurrentMonthComputed
                    text: "This date is out of available data. Generated using computation."
                    Layout.fillWidth: true
                    horizontalAlignment: Text.AlignHCenter
                    color: theme.secondaryText
                    font.pixelSize: 11
                    font.italic: true
                    Layout.bottomMargin: 5
                }

        GridLayout {
            id: calendarGrid
            columns: 7
            rowSpacing: 8
            columnSpacing: 8
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.margins: 20
            Layout.topMargin: 10

            Repeater {
                model: calendarModel
                delegate: Item {
                    Layout.fillWidth: true
                    Layout.fillHeight: modelData.type === "day" || modelData.type === "empty"
                    Layout.preferredHeight: modelData.type === "header" ? 35 : -1

                    Rectangle {
                        visible: modelData.type === "header"
                        anchors.fill: parent
                        radius: 8
                        color: theme.isDark ? theme.secondaryBg : theme.tertiaryBg
                        border.width: 0

                        Label {
                            text: modelData.text || ""
                            color: theme.isDark ? theme.secondaryText : theme.accentText
                            font.bold: true
                            font.pixelSize: 12
                            anchors.fill: parent
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                        }
                    }

                    Item {
                        visible: modelData.type === "empty"; anchors.fill: parent
                    }

                    Rectangle {
                        id: dayCell
                        visible: modelData.type === "day"; anchors.fill: parent; radius: 12
                        color: modelData.isToday ? theme.todayBg : (modelData.isSaturday ? theme.saturdayBg : theme.secondaryBg)
                        border.color: modelData.isToday ? theme.todayBorder : (modelData.isSaturday ? theme.saturdayBorder : theme.borderColor)
                        border.width: modelData.isToday ? 2 : 1

                        //FontLoader {
                        //    id: laila
                        //    source: "https://fonts.gstatic.com/s/laila/v20/LYjMdG_8nE8jDLRagCY.woff2"
                        //}

                        Label {
                            text: toDevanagari(modelData.bsDay || 0)
                          //font.family: lailalfont
                            font.bold: true
                            font.pixelSize: 26

                            color: {
                                if (modelData.isToday) return theme.accentText;
                                if (modelData.isSaturday) return theme.saturdayText;
                                return theme.primaryText;
                            }
                            anchors.centerIn: parent
                        }

                        Label {
                            text: modelData.tithi || ""
                            font.pixelSize: 11
                            color: (modelData.tithi === "पूर्णिमा" || modelData.tithi === "अमावस्या") ? theme.purnimaText : theme.secondaryText
                            elide: Text.ElideRight
                            font.bold: modelData.tithi === "पूर्णिमा" || modelData.tithi === "अमावस्या"
                            anchors.left: parent.left
                            anchors.bottom: parent.bottom
                            anchors.leftMargin: 8
                            anchors.bottomMargin: 8
                        }

                        Rectangle {
                            width: 24; height: 24; radius: 12; color: theme.tertiaryBg
                            anchors.right: parent.right; anchors.top: parent.top; anchors.margins: 2
                            Label { anchors.centerIn: parent; text: modelData.adDay || 0; font.pixelSize: 12; color: theme.secondaryText }
                        }

                        Text {
                            visible: modelData.tithi === "पूर्णिमा" || modelData.tithi === "अमावस्या"
                            text: modelData.tithi === "पूर्णिमा" ? "🌕" : "🌑"
                            font.pixelSize: 14
                            anchors.right: parent.right
                            anchors.bottom: parent.bottom
                            anchors.rightMargin: 8
                            anchors.bottomMargin: 8
                        }

                        MouseArea {
                            anchors.fill: parent; cursorShape: Qt.PointingHandCursor; hoverEnabled: true
                            onClicked: showDetail(modelData.panchanga)
                        }
                    }
                }
            }
        }

        Rectangle {
            Layout.fillWidth: true
            Layout.preferredHeight: 50
            color: theme.tertiaryBg
            border.color: theme.borderColor
            border.width: 1

            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 10
                spacing: 2
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter

                Label {
                    text: "पञ्चाङ्ग  सूर्य सिद्धान्तमा आधारित | स्थान: काठमाण्डौ |  समय: नेपाल मानक समय क्षेत्र। "
                    font.pixelSize: 12
                    color: theme.secondaryText
                    horizontalAlignment: Text.AlignHCenter
                    Layout.alignment: Qt.AlignHCenter
                    wrapMode: Text.WordWrap
                }

                Label {
                    textFormat: Text.RichText
                    text: "<a href='https://opensource.org/licenses/GPL-3.0' style='color:" + theme.accentText + "; text-decoration:none;'>License: GPL-3.0</a> or later. || <a href='https://github.com/khumnath/nepdate/tree/qml' style='color:" + theme.accentText + "; text-decoration:none;'>Source Code</a>"
                    font.pixelSize: 12
                    Layout.alignment: Qt.AlignHCenter
                    horizontalAlignment: Text.AlignHCenter

                    onLinkActivated: Qt.openUrlExternally(link)
                }
            }
        }
    }

    Dialog {
        id: detailModal
        width: Math.min(parent.width * 0.9, 600)
        height: Math.min(parent.height * 0.8, 550)
        modal: true
        closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside
        x: (parent.width - width) / 2
        y: (parent.height - height) / 2
        padding: 0

        background: Rectangle {
            color: theme.secondaryBg
            radius: 12
            border.color: theme.borderColor
            border.width: 1
            clip: true
        }

        contentItem: Item {
            width: detailModal.width
            height: detailModal.height

            Rectangle {
                id: modalHeader
                width: parent.width
                height: 60
                color: theme.secondaryBg

                Label {
                    id: modalTitle
                    text: "दिनको विवरण"
                    font.pixelSize: 20
                    font.bold: true
                    color: theme.modalHeaderText
                    anchors.centerIn: parent
                    z: 1
                }
            }

            Rectangle {
                id: modalFooter
                width: parent.width
                height: 65
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 12
                color: theme.secondaryBg

                Button {
                    text: "बन्द गर्नुहोस्"
                    anchors.centerIn: parent
                    onClicked: {
                        detailModal.close()
                    }

                    background: Rectangle {
                        radius: 8
                        color: theme.secondaryBg
                        border.color: theme.accent
                        border.width: 1
                    }

                    contentItem: Text {
                        text: parent.text
                        font: parent.font
                        color: theme.modalButtonText
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        padding: 10
                    }
                }
            }

            Flickable {
                id: flickableArea
                anchors {
                    left: parent.left
                    right: parent.right
                    top: modalHeader.bottom
                    bottom: modalFooter.top
                    leftMargin: 30
                    rightMargin: 20
                }
                clip: false
                contentHeight: scrollContent.height

                ColumnLayout {
                    id: scrollContent
                    width: parent.width
                    spacing: 15

                    // Button to SHOW debug info (visible by default)
                    Button {
                        id: showDebugButton
                        visible: !window.debugVisible
                        text: "डिबग जानकारी देखाउनुहोस्"
                        Layout.fillWidth: true
                        height: 40
                        onClicked: window.debugVisible = true

                        background: Rectangle {
                            radius: 8
                            color: theme.tertiaryBg
                            border.color: theme.accent
                            border.width: 1
                        }
                        contentItem: Text {
                            text: parent.text
                            font: parent.font
                            color: theme.modalButtonText
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                        }
                    }

                    // Panchanga Details (visible by default)
                    ColumnLayout {
                        id: panchangaDetails
                        visible: !window.debugVisible
                        property var data: []
                        spacing: 15
                        Layout.fillWidth: true
                    }

                    //  Debug info area (hidden by default)
                    Rectangle {
                        id: debugInfoArea
                        visible: window.debugVisible
                        z: 99
                        Layout.fillWidth: true
                        implicitHeight: debugInfoText.paintedHeight + 10
                        color: "black"
                        border.color: theme.accent
                        border.width: 1
                        radius: 8

                        Flickable {
                            anchors.fill: parent
                            contentWidth: debugInfoText.paintedWidth
                            contentHeight: debugInfoText.paintedHeight + 10
                            clip: true

                            TextEdit {
                                id: debugInfoText
                                text: window.currentDebugInfo
                                color: "white"
                                font.family: "monospace"
                                font.pointSize: 9
                                readOnly: true
                                wrapMode: TextEdit.NoWrap
                                selectByMouse: true
                                padding: 10
                            }
                        }
                    }

                    //  Button to HIDE debug info (hidden by default)
                    Button {
                        id: hideDebugButton
                        visible: window.debugVisible
                        text: "डिबग लुकाउनुहोस्"
                        Layout.fillWidth: true
                        height: 40
                        onClicked: window.debugVisible = false

                        background: Rectangle {
                            radius: 8
                            color: theme.tertiaryBg
                            border.color: theme.accent
                            border.width: 1
                        }
                        contentItem: Text {
                            text: parent.text
                            font: parent.font
                            color: theme.modalButtonText
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                        }
                    }
                }
            }
        }

        onClosed: {
            Panchanga.clearCache()
            clearPanchangaDetails()
            window.debugVisible = false
            // console.log("Closed detail modal and cleared its dynamic contents.")
        }
    }

}
