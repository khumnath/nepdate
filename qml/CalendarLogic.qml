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

// CalendarLogic.qml
Item {
    id: calendarLogic

    // State Properties
    property int currentAdYear: PanchangaNative.getLocalDate().getFullYear()
    property int currentAdMonth: PanchangaNative.getLocalDate().getMonth()
    property string prevAdMonthName: ""
    property string nextAdMonthName: ""

    property int currentBsYear: 2081
    property int currentBsMonthIndex: 1
    property var calendarModel: []
    property var currentMonthEvents: [] // Holds events for the current month
    property string currentBsLabelStr: ""
    property string currentAdLabelStr: ""
    property string prevMonthName: ""
    property string nextMonthName: ""
    property bool isCurrentMonthComputed: false
    property bool isCurrentMonthUnverified: false

    // Async loader properties
    property var _asyncModel: []
    property var _asyncEvents: ({})
    property int _asyncCurrentDay: 1
    property int _asyncDaysInMonth: 0
    property int _asyncYear: 0
    property int _asyncMonthIndex: 0
    property int _asyncStartDay: 0
    property bool isAdRenderMode: false

    function getBsRangeForAd(adYear, adMonthIndex) {
        var first = new Date(Date.UTC(adYear, adMonthIndex, 1));
        var last = new Date(Date.UTC(adYear, adMonthIndex + 1, 0));
        if (adYear >= 0 && adYear < 100) {
            first.setUTCFullYear(adYear);
            last.setUTCFullYear(adYear);
        }
        var isBikramSambat = !isAdRenderMode;
        var infoFirst = PanchangaNative.calculate(first, 27.7172, 85.3240, 5.75, isBikramSambat);
        var infoLast = PanchangaNative.calculate(last, 27.7172, 85.3240, 5.75, isBikramSambat);
        
        var firstBsMonth = PanchangaNative.solarMonths[infoFirst.bsMonthIndex];
        var lastBsMonth = PanchangaNative.solarMonths[infoLast.bsMonthIndex];
        var firstBsYear = infoFirst.bsYear;
        var lastBsYear = infoLast.bsYear;
        
        if (firstBsYear !== lastBsYear) {
            return firstBsMonth + " " + toDevanagari(firstBsYear) + " – " + lastBsMonth + " " + toDevanagari(lastBsYear);
        } else if (firstBsMonth !== lastBsMonth) {
            return firstBsMonth + " - " + lastBsMonth + " " + toDevanagari(firstBsYear);
        } else {
            return firstBsMonth + " " + toDevanagari(firstBsYear);
        }
    }

    Timer {
        id: asyncLoader
        interval: 1
        repeat: true
        onTriggered: calendarLogic.processAsyncDays()
    }


    function toDevanagari(num) {
        return PanchangaNative.toDevanagari(String(num))
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
        var today = PanchangaNative.getLocalDate();
        var isBikramSambat = !isAdRenderMode;
        var bsInfo = PanchangaNative.calculate(today, 27.7172, 85.3240, 5.75, isBikramSambat);
        renderCalendarByBs(bsInfo.bsYear, bsInfo.bsMonthIndex);
    }

    function getGregorianRange(bsYear, monthIndex) {
        var first = PanchangaNative.fromBikramSambat(bsYear, monthIndex, 1);
        var info = PanchangaNative.getBikramMonthInfo(bsYear, monthIndex);
        var last = PanchangaNative.fromBikramSambat(bsYear, monthIndex, info.totalDays);
        var firstMonth = PanchangaNative.nepaliGregorianMonths[first.getUTCMonth()];
        var lastMonth = PanchangaNative.nepaliGregorianMonths[last.getUTCMonth()];
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
        isAdRenderMode = false;
        var newCalendarModel = [];
        var eventsByDay = {}; // Use an object to group events by day

        currentBsYear = year;
        currentBsMonthIndex = monthIndex;
        var info = PanchangaNative.getBikramMonthInfo(year, monthIndex);
        if (!info) {
            console.error("Failed to get Bikram month info for", year, monthIndex);
            return;
        }

        if (!preserveAdState) {
            var bsMonthStartDate = PanchangaNative.fromBikramSambat(year, monthIndex, 1);
            var startAdYear = bsMonthStartDate.getUTCFullYear();
            var startAdMonth = bsMonthStartDate.getUTCMonth();
            var nextAdMonth = (startAdMonth + 1) % 12;
            var nextAdYear = (startAdMonth === 11) ? startAdYear + 1 : startAdYear;
            var firstOfNextAdMonth = new Date(0);
            firstOfNextAdMonth.setUTCFullYear(nextAdYear, nextAdMonth, 1);
            firstOfNextAdMonth.setUTCHours(0, 0, 0, 0);
            var bsMonthEndDate = PanchangaNative.fromBikramSambat(year, monthIndex, info.totalDays);

            if (firstOfNextAdMonth <= bsMonthEndDate) {
                currentAdYear = nextAdYear;
                currentAdMonth = nextAdMonth;
            } else {
                currentAdYear = startAdYear;
                currentAdMonth = startAdMonth;
            }
        }

        var prevAdMonthIndex = (currentAdMonth - 1 + 12) % 12;
        var nextAdMonthIndex = (currentAdMonth + 1) % 12;
        prevAdMonthName = PanchangaNative.nepaliGregorianMonths[prevAdMonthIndex];
        nextAdMonthName = PanchangaNative.nepaliGregorianMonths[nextAdMonthIndex];

        var daysInMonth = info.totalDays;
        var startDay = info.startDayOfWeek;
        var weekdaysNe = ["आइतबार", "सोमबार", "मङ्गलबार", "बुधबार", "बिहीबार", "शुक्रबार", "शनिबार"];

        // Build and render skeleton
        for (var i = 0; i < 7; ++i) {
            newCalendarModel.push({ type: "header", text: weekdaysNe[i] });
        }
        for (i = 0; i < startDay; ++i) {
            newCalendarModel.push({ type: "empty" });
        }
        for (var day = 1; day <= daysInMonth; ++day) {
            var adDate = PanchangaNative.fromBikramSambat(year, monthIndex, day);
            var isToday = adDate.toDateString() === PanchangaNative.getLocalDate().toDateString();
            var isSaturday = (startDay + day - 1) % 7 === 6;
            newCalendarModel.push({
                type: "day", bsDay: day, adDay: adDate.getDate(),
                tithi: "...", isToday: isToday, isSaturday: isSaturday,
                hasEvent: false, gregorianDate: "", panchanga: null
            });
        }

        var totalDaySlots = startDay + daysInMonth;
        var targetSlots = totalDaySlots > 35 ? 42 : 35;
        for (var p = totalDaySlots; p < targetSlots; ++p) {
            newCalendarModel.push({ type: "empty" });
        }

        calendarModel = newCalendarModel;

        currentBsLabelStr = toDevanagari(year) + " " + info.monthName;
        currentAdLabelStr = getGregorianRange(year, monthIndex);
        var prevMonthIndex = (monthIndex - 1 + 12) % 12;
        var nextMonthIndex = (monthIndex + 1) % 12;

        prevMonthName = PanchangaNative.solarMonths[prevMonthIndex] || "";
        nextMonthName = PanchangaNative.solarMonths[nextMonthIndex] || "";

        // 2. Start Async Loader
        _asyncModel = newCalendarModel;
        _asyncEvents = {};
        _asyncCurrentDay = 1;
        _asyncDaysInMonth = daysInMonth;
        _asyncYear = year;
        _asyncMonthIndex = monthIndex;
        _asyncStartDay = startDay;
        asyncLoader.start();
    }

    function processAsyncDays() {
        var daysToProcessPerTick = 5; // process 5 days per frame
        for (var k = 0; k < daysToProcessPerTick; k++) {
            if (_asyncCurrentDay > _asyncDaysInMonth) {
                asyncLoader.stop();
                finishAsyncLoading();
                return;
            }

            var day = _asyncCurrentDay;
            var adDate;
            if (isAdRenderMode) {
                adDate = new Date(Date.UTC(_asyncYear, _asyncMonthIndex, day));
                if (_asyncYear >= 0 && _asyncYear < 100) adDate.setUTCFullYear(_asyncYear);
            } else {
                adDate = PanchangaNative.fromBikramSambat(_asyncYear, _asyncMonthIndex, day);
            }
            
            var result = PanchangaNative.calculate(adDate, 27.7172, 85.3240, 5.75, !isAdRenderMode);
            if (day === 1) {
                isCurrentMonthComputed = result.isComputed;
                isCurrentMonthUnverified = result.isUnverified;
            }
            var hasEventForDay = result.events ? (result.events.length > 0) : false;

            if (hasEventForDay) {
                var displayDayLabel = isAdRenderMode ? day.toString() : toDevanagari(day);
                if (!_asyncEvents[displayDayLabel]) {
                    _asyncEvents[displayDayLabel] = [];
                }
                for (var j = 0; j < result.events.length; j++) {
                    _asyncEvents[displayDayLabel].push(result.events[j].name);
                }
            }

            var modelIndex = 7 + _asyncStartDay + day - 1;
            if (isAdRenderMode) {
                _asyncModel[modelIndex].bsDay = result.bsDay;
                result.monthName = PanchangaNative.solarMonths[result.bsMonthIndex] || "";
            } else {
                result.monthName = currentBsLabelStr.split(" ")[1];
            }
            
            _asyncModel[modelIndex].tithi = result.tithi;
            _asyncModel[modelIndex].hasEvent = hasEventForDay;
            _asyncModel[modelIndex].gregorianDate = result.gregorianDate;
            _asyncModel[modelIndex].panchanga = result;

            _asyncCurrentDay++;
        }
    }

    function finishAsyncLoading() {
        var newMonthEvents = [];
        var sortedDays = Object.keys(_asyncEvents).sort(function(a, b) {
            if (isAdRenderMode) {
                return parseInt(a) - parseInt(b);
            } else {
                return parseInt(fromDevanagari(a)) - parseInt(fromDevanagari(b));
            }
        });

        for (var i = 0; i < sortedDays.length; i++) {
            var dayLabel = sortedDays[i];
            newMonthEvents.push({
                bsDay: dayLabel,
                eventName: _asyncEvents[dayLabel].join(", ")
            });
        }

        calendarModel = _asyncModel;
        currentMonthEvents = newMonthEvents;
    }

    function renderCalendarByAd(year, monthIndex) {
        isAdRenderMode = true;
        var newCalendarModel = [];
        currentAdYear = year;
        currentAdMonth = monthIndex;
        
        var firstDate = new Date(Date.UTC(year, monthIndex, 1));
        if (year >= 0 && year < 100) firstDate.setUTCFullYear(year);
        
        var lastDate = new Date(Date.UTC(year, monthIndex + 1, 0));
        if (year >= 0 && year < 100) lastDate.setUTCFullYear(year);
        
        var daysInMonth = lastDate.getUTCDate();
        var startDay = firstDate.getUTCDay();
        var weekdaysAd = ["Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"];
        
        for (var i = 0; i < 7; ++i) {
            newCalendarModel.push({ type: "header", text: weekdaysAd[i] });
        }
        for (i = 0; i < startDay; ++i) {
            newCalendarModel.push({ type: "empty" });
        }
        for (var day = 1; day <= daysInMonth; ++day) {
            var iterDate = new Date(Date.UTC(year, monthIndex, day));
            if (year >= 0 && year < 100) iterDate.setUTCFullYear(year);
            var isToday = iterDate.toDateString() === PanchangaNative.getLocalDate().toDateString();
            var isSaturday = (startDay + day - 1) % 7 === 6;
            
            newCalendarModel.push({
                type: "day", bsDay: 0, adDay: day,
                tithi: "...", isToday: isToday, isSaturday: isSaturday,
                hasEvent: false, gregorianDate: "", panchanga: null
            });
        }
        
        var totalDaySlots = startDay + daysInMonth;
        var targetSlots = totalDaySlots > 35 ? 42 : 35;
        for (var p = totalDaySlots; p < targetSlots; ++p) {
            newCalendarModel.push({ type: "empty" });
        }
        
        calendarModel = newCalendarModel;
        
        currentAdLabelStr = PanchangaNative.nepaliGregorianMonths[monthIndex] + " " + year;
        currentBsLabelStr = getBsRangeForAd(year, monthIndex);
        
        _asyncModel = newCalendarModel;
        _asyncEvents = {};
        _asyncCurrentDay = 1;
        _asyncDaysInMonth = daysInMonth;
        _asyncYear = year;
        _asyncMonthIndex = monthIndex;
        _asyncStartDay = startDay;
        asyncLoader.start();
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

    function navigateBsYear(direction) {
        renderCalendarByBs(currentBsYear + direction, currentBsMonthIndex);
    }

    function navigateAdYear(direction) {
        renderCalendarByAd(currentAdYear + direction, currentAdMonth);
    }
}
