import QtQuick 2.15
import "qrc:/PanchangaCalculator.js" as Panchanga

// CalendarLogic.qml
QtObject {
    id: calendarLogic

    // State Properties
    property int currentAdYear: new Date().getFullYear()
    property int currentAdMonth: new Date().getMonth()
    property string prevAdMonthName: ""
    property string nextAdMonthName: ""

    property int currentBsYear: 2081
    property int currentBsMonthIndex: 1
    property var calendarModel: []
    property string currentBsLabelStr: ""
    property string currentAdLabelStr: ""
    property string prevMonthName: ""
    property string nextMonthName: ""
    property bool isCurrentMonthComputed: false


    function toDevanagari(num) {
        return Panchanga.toDevanagari(String(num))
    }

    function fromDevanagari(devanagariStr) {
        if (typeof devanagariStr !== 'string') return "";
        const digitMap = {
            '०': '0', '१': '1', '२': '2', '३': '3', '४': '4',
            '५': '5', '६': '6', '७': '7', '८': '8', '९': '9'
        };
        return devanagariStr.replace(/[०-९]/g, (match) => digitMap[match]);
    }

    function initializeApp() {
        var today = new Date();
        var bsInfo = Panchanga.calculate(today);
        renderCalendarByBs(bsInfo.bsYear, bsInfo.bsMonthIndex);
    }

    function getGregorianRange(bsYear, monthIndex) {
        var first = Panchanga.fromBikramSambat(bsYear, monthIndex, 1);
        var info = Panchanga.getBikramMonthInfo(bsYear, monthIndex);
        var last = Panchanga.fromBikramSambat(bsYear, monthIndex, info.totalDays);
        var firstMonth = Panchanga.nepaliGregorianMonths[first.getUTCMonth()];
        var lastMonth = Panchanga.nepaliGregorianMonths[last.getUTCMonth()];
        var firstYear = first.getUTCFullYear();
        var lastYear = last.getUTCFullYear();

        if (firstYear !== lastYear) {
            return firstMonth + " " + firstYear + " – " + lastMonth + " " + lastYear;
        } else if (firstMonth !== lastMonth) {
            return firstMonth + " - " + lastMonth + " " + firstYear;
        } else {
            return firstMonth + " " + firstYear;
        }
    }

    function renderCalendarByBs(year, monthIndex, preserveAdState = false) {
        calendarModel = [];
        currentBsYear = year;
        currentBsMonthIndex = monthIndex;
        var info = Panchanga.getBikramMonthInfo(year, monthIndex);
        if (!info) {
            console.error("Failed to get Bikram month info for", year, monthIndex);
            return;
        }

        if (!preserveAdState) {
            // LOGIC to determine the representative AD month
            var bsMonthStartDate = Panchanga.fromBikramSambat(year, monthIndex, 1);
            var startAdYear = bsMonthStartDate.getUTCFullYear();
            var startAdMonth = bsMonthStartDate.getUTCMonth();

            var nextAdMonth = (startAdMonth + 1) % 12;
            var nextAdYear = (startAdMonth === 11) ? startAdYear + 1 : startAdYear;
            var firstOfNextAdMonth = new Date(Date.UTC(nextAdYear, nextAdMonth, 1));

            var bsMonthEndDate = Panchanga.fromBikramSambat(year, monthIndex, info.totalDays);

            if (firstOfNextAdMonth <= bsMonthEndDate) {
                // Prefer the later AD month if its first day is in the BS month.
                currentAdYear = nextAdYear;
                currentAdMonth = nextAdMonth;
            } else {
                // Otherwise, use the starting AD month.
                currentAdYear = startAdYear;
                currentAdMonth = startAdMonth;
            }
        }

        var prevAdMonthIndex = (currentAdMonth - 1 + 12) % 12;
        var nextAdMonthIndex = (currentAdMonth + 1) % 12;
        prevAdMonthName = Panchanga.nepaliGregorianMonths[prevAdMonthIndex];
        nextAdMonthName = Panchanga.nepaliGregorianMonths[nextAdMonthIndex];

        var daysInMonth = info.totalDays;
        var startDay = info.startDayOfWeek;
        var model = [];
        var weekdaysNe = ["आइतबार", "सोमबार", "मङ्गलबार", "बुधबार", "बिहीबार", "शुक्रबार", "शनिबार"];
        for (var i = 0; i < 7; ++i) {
            model.push({ type: "header", text: weekdaysNe[i] });
        }
        for (i = 0; i < startDay; ++i) {
            model.push({ type: "empty" });
        }
        for (var day = 1; day <= daysInMonth; ++day) {
            var adDate = Panchanga.fromBikramSambat(year, monthIndex, day);
            var result = Panchanga.calculate(adDate);
            if (day === 1) {
                isCurrentMonthComputed = result.isComputed;
            }
            var isToday = adDate.toDateString() === new Date().toDateString();
            var isSaturday = (startDay + day - 1) % 7 === 6;
            result.monthName = info.monthName;
            model.push({
                type: "day", bsDay: day, adDay: adDate.getDate(),
                tithi: result.tithi, isToday: isToday, isSaturday: isSaturday,
                gregorianDate: result.gregorianDate, panchanga: result
            });
        }
        calendarModel = model;
        currentBsLabelStr = toDevanagari(year) + " " + info.monthName;
        currentAdLabelStr = getGregorianRange(year, monthIndex);
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
    }

    function renderCalendarByAd(year, monthIndex) {
        currentAdYear = year;
        currentAdMonth = monthIndex;

        var date = new Date(Date.UTC(year, monthIndex, 1));
        var bsInfo = Panchanga.calculate(date);

        renderCalendarByBs(bsInfo.bsYear, bsInfo.bsMonthIndex, true);
    }

    function navigateBsMonth(direction) {
        var newMonth = currentBsMonthIndex + direction;
        var newYear = currentBsYear;
        if (newMonth > 11) {
            newMonth = 0;
            newYear++;
        } else if (newMonth < 0) {
            newMonth = 11;
            newYear--;
        }
        renderCalendarByBs(newYear, newMonth);
    }

    function navigateAdMonth(direction) {
        var newAdMonth = currentAdMonth + direction;
        var newAdYear = currentAdYear;
        if (newAdMonth > 11) {
            newAdMonth = 0;
            newAdYear++;
        } else if (newAdMonth < 0) {
            newAdMonth = 11;
            newAdYear--;
        }
        renderCalendarByAd(newAdYear, newAdMonth);
    }
}
