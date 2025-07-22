// BikramCalendar.qml
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "PancangaCalculator.js" as Logic

Frame {
    id: calendarRoot

    property var calculator: new Logic.PancangaCalculator()
    property int currentBsYear: 2081
    property int currentBsMonth: 0 // 0 for Baisakh
    property var calendarData: ({})

    property var todayInfo: calculator.getTodayBsInfo()

    background: Rectangle { color: "transparent" }

    Component.onCompleted: updateCalendar()

    function setDate(bsYear, bsMonthIndex) {
        calendarRoot.currentBsYear = bsYear;
        calendarRoot.currentBsMonth = bsMonthIndex;
        calendarRoot.updateCalendar();
    }

    function updateCalendar() {
        calendarData = calculator.getBikramMonthInfo(currentBsYear, currentBsMonth);
        monthLabel.text = calendarData.monthName + " " + calculator.toDevanagari(calendarData.year);
    }

    ColumnLayout {
        anchors.fill: parent
        spacing: 10

        // --- Navigation Header ---
        RowLayout {
            Layout.fillWidth: true
            Button {
                text: "<"
                onClicked: {
                    if(--calendarRoot.currentBsMonth < 0) {
                        calendarRoot.currentBsMonth = 11;
                        calendarRoot.currentBsYear--;
                    }
                    calendarRoot.updateCalendar();
                }
            }
            Label {
                id: monthLabel
                Layout.fillWidth: true
                horizontalAlignment: Text.AlignHCenter
                font.bold: true
                color: "#ffc107"
            }
            Button {
                text: ">"
                onClicked: {
                    if(++calendarRoot.currentBsMonth > 11) {
                        calendarRoot.currentBsMonth = 0;
                        calendarRoot.currentBsYear++;
                    }
                    calendarRoot.updateCalendar();
                }
            }
        }

        // --- Weekday Header ---
        RowLayout {
            Layout.fillWidth: true
            Repeater {
                model: ["आइत", "सोम", "मंगल", "बुध", "बिहि", "शुक्र", "शनि"]
                Label {
                    text: modelData
                    Layout.fillWidth: true
                    horizontalAlignment: Text.AlignHCenter
                    color: (index === 6) ? "#ff6b6b" : "#a0d2ff"
                    font.pointSize: 9
                }
            }
        }

        // --- Separator Line ---
        Rectangle {
            Layout.fillWidth: true
            Layout.preferredHeight: 1
            color: "#2c4a7a"
        }

        // --- Day Grid ---
        GridLayout {
            id: dayGrid
            columns: 7
            columnSpacing: 0
            rowSpacing: 5

            Repeater {
                id: dayGridRepeater
                model: 42

                delegate: DayCell {
                    Layout.fillWidth: true
                    readonly property int dayOffset: calendarData.startDayOfWeek
                    readonly property int day: index - dayOffset + 1

                    isCurrentMonth: day > 0 && day <= calendarData.totalDays
                    dayNumber: day

                    isToday: (day === todayInfo.day &&
                              calendarRoot.currentBsMonth === todayInfo.monthIndex &&
                              calendarRoot.currentBsYear === todayInfo.year)

                    isSaturday: (index % 7) === 6
                }
            }
        }
    }
}
