import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "PancangaCalculator.js" as Logic

ApplicationWindow {
    id: root
    width: 620
    height: 850
    minimumWidth: 600
    minimumHeight: 800
    visible: true
    title: "Bikram Date Utility"
    color: "#0c1a2d"

    font.family: "Segoe UI"

    property var calculator: new Logic.PancangaCalculator()
    property var lastResults: ({})

    // Component and property for creating the calendar window dynamically
    property var calendarComponent: Qt.createComponent("CalendarWindow.qml")
    property var calendarWindow: null

    // --- Component attached to the root to run code on startup ---
    Component.onCompleted: {
        performCalculation();
    }

    // --- Reusable Component Definitions ---
    Component {
        id: resultCardComponent
        Frame {
            property alias title: titleLabel.text
            default property alias content: contentColumn.data
            padding: 10
            background: Rectangle {
                color: "#142d50"
                radius: 8
                border.color: "#ff9800"
                border.width: 1
            }
            ColumnLayout {
                spacing: 10
                Label {
                    id: titleLabel
                    font.bold: true
                    color: "#ffc107"
                    font.pointSize: 14
                }
                Column {
                    id: contentColumn
                    spacing: 10
                    Layout.fillWidth: true
                }
            }
        }
    }

    Component {
        id: resultItemComponent
        RowLayout {
            property alias label: labelText.text
            property alias value: valueText.text
            property bool isHighlight: false
            Layout.alignment: Qt.AlignHCenter
            Label {
                id: labelText
                color: "#a0d2ff"
            }
            Label {
                id: valueText
                text: "-"
                color: isHighlight ? "#ffd54f" : "white"
                font.bold: isHighlight
                Layout.alignment: Qt.AlignRight
                Layout.fillWidth: true
                wrapMode: Text.Wrap
            }
        }
    }

    // --- Main Content Area ---
    ScrollView {
        id: scrollView
        anchors.fill: parent
        clip: true
        leftPadding: 15
        rightPadding: 15
        contentWidth: -1
        contentHeight: contentLayout.height

        ColumnLayout {
            id: contentLayout
            anchors.fill: parent
            spacing: 15

            // --- Header ---
            Label {
                text: "Bikram Date Calculator"
                color: "#ffc107"; font.pointSize: 14; font.bold: true
                Layout.alignment: Qt.AlignHCenter; topPadding: 10; wrapMode: Text.WordWrap
            }
            Label {
                text: "Hindu nepali calendar calculations based on SuryaSiddhanta"
                color: "#a0d2ff"; font.italic: true; font.pointSize: 10
                Layout.alignment: Qt.AlignHCenter; bottomPadding: 5; wrapMode: Text.WordWrap
            }

            // --- Section 1: Main Input Container ---
            Frame {
                id: inputFrame
                Layout.fillWidth: true
                Layout.alignment: Qt.AlignHCenter
                width: - 1
                padding: 5
                background: Rectangle {
                    color: "#081229E0"
                    border.color: "#2c4a7a"
                    radius: 15
                }
                ColumnLayout {
                    width: parent.width
                    spacing: 5
                    RowLayout {
                        Layout.alignment: Qt.AlignHCenter
                        spacing: 15
                        ButtonGroup { id: calendarGroup }
                        RadioButton { id: gregorianRadio; text: "Gregorian Date"; checked: true; ButtonGroup.group: calendarGroup }
                        RadioButton { id: bikramRadio; text: "Bikram Sambat"; ButtonGroup.group: calendarGroup }
                    }
                    RowLayout {
                        id: responsiveInputRow
                        Layout.fillWidth: true
                        spacing: 0
                        Item { Layout.fillWidth: true }
                        Frame {
                            Layout.preferredWidth: 260
                            padding: 15
                            background: Rectangle { color: "#0f1e3c"; radius: 8 }
                            StackLayout {
                                id: inputStack
                                currentIndex: gregorianRadio.checked ? 0 : 1
                                ColumnLayout {
                                    Label { text: "Gregorian Date"; font.bold: true; color: "#ffc107"; Layout.alignment: Qt.AlignHCenter }
                                    Label { text: "Year" }
                                    TextField { id: yearInput; text: new Date().getFullYear(); Layout.fillWidth: true; validator: IntValidator { bottom: 1000; top: 3000 } }
                                    Label { text: "Month" }
                                    ComboBox {
                                        id: monthInput
                                        model: ["January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"]
                                        currentIndex: new Date().getMonth()
                                        Layout.fillWidth: true
                                        popup: Popup {
                                            y: monthInput.height
                                            width: monthInput.width
                                            padding: 1
                                            contentItem: ListView {
                                                clip: true
                                                model: monthInput.model
                                                implicitHeight: 240
                                                delegate: ItemDelegate {
                                                    width: parent.width
                                                    text: modelData
                                                    highlighted: ListView.isCurrentItem
                                                    background: Rectangle { color: highlighted ? "#3a5f9e" : "#142d50" }
                                                    MouseArea {
                                                        anchors.fill: parent
                                                        onClicked: {
                                                            monthInput.currentIndex = index
                                                            monthInput.popup.close()
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                    Label { text: "Day" }
                                    TextField { id: dayInput; text: new Date().getDate(); Layout.fillWidth: true; validator: IntValidator { bottom: 1; top: 31 } }
                                }
                                ColumnLayout {
                                    Label { text: "Bikram Sambat Date"; font.bold: true; color: "#ffc107"; Layout.alignment: Qt.AlignHCenter }
                                    Label { text: "Year" }
                                    TextField { id: bikramYearInput; text: "2081"; Layout.fillWidth: true; validator: IntValidator { bottom: 1000; top: 3000 } }
                                    Label { text: "Month" }
                                    ComboBox {
                                        id: bikramMonthInput
                                        model: calculator.solarMonths
                                        Layout.fillWidth: true
                                        popup: Popup {
                                            y: bikramMonthInput.height
                                            width: bikramMonthInput.width
                                            padding: 1
                                            contentItem: ListView {
                                                clip: true
                                                implicitHeight: 240
                                                model: bikramMonthInput.model
                                                delegate: ItemDelegate {
                                                    width: parent.width
                                                    text: modelData
                                                    highlighted: ListView.isCurrentItem
                                                    background: Rectangle { color: highlighted ? "#3a5f9e" : "#142d50" }
                                                    MouseArea {
                                                        anchors.fill: parent
                                                        onClicked: {
                                                            bikramMonthInput.currentIndex = index
                                                            bikramMonthInput.popup.close()
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                    Label { text: "Day (Gatē)" }
                                    TextField { id: bikramDayInput; text: "1"; Layout.fillWidth: true; validator: IntValidator { bottom: 1; top: 32 } }
                                }
                            }
                        }
                        Item {
                            Layout.fillWidth: true
                            Layout.maximumWidth: 70
                        }
                        Frame {
                            Layout.preferredWidth: 260
                            padding: 15
                            background: Rectangle { color: "#0f1e3c"; radius: 8 }
                            ColumnLayout {
                                Label { text: "Location"; font.bold: true; color: "#ffc107"; Layout.alignment: Qt.AlignHCenter }
                                Label { text: "Latitude" }
                                TextField { id: latitudeInput; text: "27.7"; Layout.fillWidth: true; validator: DoubleValidator { bottom: -90; top: 90; decimals: 4; notation: DoubleValidator.StandardNotation } }
                                Label { text: "Longitude" }
                                TextField { id: longitudeInput; text: "85.3"; Layout.fillWidth: true; validator: DoubleValidator { bottom: -180; top: 180; decimals: 4; notation: DoubleValidator.StandardNotation } }
                                Label { text: "Timezone" }
                                TextField { id: timezoneInput; text: "5.75"; Layout.fillWidth: true; validator: DoubleValidator { bottom: -12; top: 14; decimals: 2; notation: DoubleValidator.StandardNotation } }
                            }
                        }
                        Item { Layout.fillWidth: true }
                    }
                }
            }

            // --- Section 2: Action Buttons ---
            Frame {
                Layout.fillWidth: true
                background: Rectangle { color: "transparent" }
                RowLayout {
                    anchors.centerIn: parent
                    spacing: 15
                    Button {
                        id: calculateBtn
                        text: "Calculate Pancanga"
                        onClicked: performCalculation()
                    }
                    Button {
                        text: "View Calendar"
                        onClicked: {
                            if (calendarWindow) {
                                calendarWindow.raise();
                                calendarWindow.requestActivate();
                                return;
                            }
                            if (lastResults.bsYear && calendarComponent.status === Component.Ready) {
                                calendarWindow = calendarComponent.createObject(root, {
                                    "initialYear": lastResults.bsYear,
                                    "initialMonth": lastResults.bsMonthIndex
                                });
                                calendarWindow.closing.connect(function() { calendarWindow = null; });
                                calendarWindow.show();
                            } else if (calendarComponent.status === Component.Error) {
                                console.log("Error creating calendar window:", calendarComponent.errorString());
                            }
                        }
                    }
                }
            }

            // --- Section 3: Results Section (Fully Responsive) ---
            Frame {
                id: resultsContainer
                Layout.fillWidth: true
                Layout.alignment: Qt.AlignHCenter
                padding: 5
                background: Rectangle {
                    color: "#0a1932E0"
                    border.color: "#2c4a7a"
                    radius: 15
                }
                ColumnLayout {
                    spacing: 20
                    anchors.centerIn: parent
                    Label {
                        id: resultsTitle
                        text: "Pancanga Results"
                        font.pointSize: 16
                        font.bold: true
                        color: "#ffc107"
                        Layout.alignment: Qt.AlignHCenter
                    }
                    RowLayout {
                        id: responsiveResultsRow
                        Layout.fillWidth: true
                        spacing: 20
                        property bool isStacked: root.width < 600
                        onIsStackedChanged: {
                            LayoutMirroring.enabled = isStacked
                            LayoutMirroring.childrenMirroring = isStacked
                        }
                        Frame {
                            Layout.fillWidth: true
                            Layout.minimumWidth: 280
                            padding: 15
                            background: Rectangle {
                                color: "#142d50"
                                radius: 8
                                border.color: "#ff9800"
                                border.width: 1
                            }
                            ColumnLayout {
                                spacing: 10
                                Label {
                                    text: "Date & Time"
                                    font.bold: true
                                    color: "#ffc107"
                                    font.pointSize: 14
                                    Layout.alignment: Qt.AlignHCenter
                                }
                                ResultItem { id: gregorianDateItem; label: "Gregorian Date" }
                                ResultItem { id: bikramSambatItem; label: "Bikram Sambat"; isHighlight: true }
                                ResultItem { id: weekdayItem; label: "Weekday" }
                                ResultItem { id: sunriseItem; label: "Sunrise"; isHighlight: true }
                                ResultItem { id: sunsetItem; label: "Sunset"; isHighlight: true }
                                ResultItem { id: sunRashiItem; label: "Sun Rashi"; isHighlight: true }
                                ResultItem { id: adhikaMasaItem; label: "Adhika/Kshaya Masa" }
                            }
                        }
                        Frame {
                            Layout.fillWidth: true
                            Layout.minimumWidth: 280
                            padding: 30
                            background: Rectangle {
                                color: "#142d50"
                                radius: 8
                                border.color: "#ff9800"
                                border.width: 1
                            }
                            ColumnLayout {
                                spacing: 10
                                Label {
                                    text: "Lunar Elements"
                                    font.bold: true
                                    color: "#ffc107"
                                    font.pointSize: 14
                                    Layout.alignment: Qt.AlignHCenter
                                }
                                ResultItem { id: tithiItem; label: "Tithi"; isHighlight: true }
                                ResultItem { id: pakshaItem; label: "Paksha" }
                                ResultItem { id: nakshatraItem; label: "Nakshatra"; isHighlight: true }
                                ResultItem { id: yogaItem; label: "Yoga"; isHighlight: true }
                                ResultItem { id: karanaItem; label: "Karana" }
                                ResultItem { id: moonRashiItem; label: "Moon Rashi"; isHighlight: true }
                            }
                        }
                    }
                }
            }
        }
    }
    // --- JavaScript Logic Function ---
    function performCalculation() {
        var date;
        if (gregorianRadio.checked) {
            date = new Date(parseInt(yearInput.text), monthInput.currentIndex, parseInt(dayInput.text));
        } else {
            date = calculator.fromBikramSambat(parseInt(bikramYearInput.text), bikramMonthInput.currentIndex, parseInt(bikramDayInput.text));
        }
        var latitude = parseFloat(latitudeInput.text);
        var longitude = parseFloat(longitudeInput.text);
        var timezone = parseFloat(timezoneInput.text);

        lastResults = calculator.calculate(date, latitude, longitude, timezone);

        var days = ["Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"];
        var weekdayName = days[date.getDay()];
        var dateD = [
            ["January", "February", "March", "April", "May", "June",
                "July", "August", "September", "October", "November", "December"][date.getMonth()],
            date.getDate(),
            date.getFullYear()
        ].join(" ");

        resultsTitle.text = "Results for " + dateD;
        gregorianDateItem.value = dateD;
        var bsYearDev = calculator.toDevanagari(lastResults.bsYear);
               var bsDayDev = calculator.toDevanagari(lastResults.bsDay);
               var bsMonthName = calculator.solarMonths[lastResults.bsMonthIndex];
               bikramSambatItem.value = bsMonthName + " " + bsDayDev + ", " + bsYearDev;
        weekdayItem.value = weekdayName;
        sunriseItem.value = lastResults.sunrise;
        sunsetItem.value = lastResults.sunset;
        sunRashiItem.value = lastResults.sunRashi;
        adhikaMasaItem.value = lastResults.adhikaMasa;
        tithiItem.value = lastResults.tithi;
        pakshaItem.value = lastResults.paksha;
        nakshatraItem.value = lastResults.nakshatra;
        yogaItem.value = lastResults.yoga;
        karanaItem.value = lastResults.karana;
        moonRashiItem.value = lastResults.moonRashi;
    }
}
