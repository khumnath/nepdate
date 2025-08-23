/**
 * Bikram Calculator - Hindu Astrological Calendar with panchanga
 * Copyright (C) 2025 Khumnath Cg <nath.khum@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

.pragma library

    .import "qrc:/resources/PreCalculated_Data.js" as Bsdata
        .import "qrc:/resources/EventData.js" as EventsData

// Caching
var calculationCache = {};
function clearCache() {
    calculationCache = {};
}

// Surya Siddhanta Constants
var YugaRotation = {
    'star': 1582237828, 'sun': 4320000, 'moon': 57753336,
    'mercury': 17937060, 'venus': 7022376, 'mars': 2296832,
    'jupiter': 364220, 'saturn': 146568, 'Candrocca': 488203,
    'Rahu': -232238
};
var YugaCivilDays = 1577917828; // YugaRotation.star - YugaRotation.sun
var KaliEpoch = 588465.5;
var PlanetApogee = { 'sun': 77 + 17 / 60 };
var PlanetCircumm = { 'sun': 13 + 50 / 60, 'moon': 31 + 50 / 60 };
var rad = 180 / Math.PI;

// Panchanga Names
var tithiNamesList = [
    "प्रतिपदा", "द्वितीया", "तृतीया", "चतुर्थी", "पञ्चमी", "षष्ठी", "सप्तमी", "अष्टमी", "नवमी", "दशमी",
    "एकादशी", "द्वादशी", "त्रयोदशी", "चतुर्दशी", "पूर्णिमा", "अमावस्या"
];
var nakshatras = [
    "अश्विनी", "भरणी", "कृत्तिका", "रोहिणी", "मृगशिरा", "आर्द्रा", "पुनर्वसु", "पुष्य", "अश्लेषा",
    "मघा", "पूर्व फाल्गुनी", "उत्तर फाल्गुनी", "हस्त", "चित्रा", "स्वाती", "विशाखा", "अनुराधा",
    "ज्येष्ठा", "मूल", "पूर्वाषाढा", "उत्तराषाढा", "श्रवण", "धनिष्ठा", "शतभिषा", "पूर्व भाद्रपद",
    "उत्तर भाद्रपद", "रेवती"
];
var yogas = [
    "विष्कम्भ", "प्रीति", "आयुष्मान्", "सौभाग्य", "शोभन", "अतिगण्ड", "सुकर्म", "धृति", "शूल", "गण्ड",
    "वृद्धि", "ध्रुव", "व्याघात", "हर्षण", "वज्र", "सिद्धि", "व्यतिपात", "वरीयान्", "परिघ", "शिव",
    "सिद्ध", "साध्य", "शुभ", "शुक्ल", "ब्रह्म", "इन्द्र", "वैधृति"
];
var karanas = [
    "किंस्तुघ्न", "बव", "बालव", "कौलव", "तैतिल", "गर", "वणिज", "विष्टि", "शकुनि", "चतुष्पाद", "नाग"
];
var rashis = [
    "मेष", "वृषभ", "मिथुन", "कर्क", "सिंह", "कन्या",
    "तुला", "वृश्चिक", "धनु", "मकर", "कुम्भ", "मीन"
];
var solarMonths = [
    "वैशाख", "ज्येष्ठ", "आषाढ", "श्रावण", "भाद्रपद", "आश्विन",
    "कार्तिक", "मार्गशीर्ष", "पौष", "माघ", "फाल्गुन", "चैत्र"
];
var weekdays = ["आइतबार", "सोमबार", "मङ्गलबार", "बुधबार", "बिहीबार", "शुक्रबार", "शनिबार"];
var nepaliGregorianMonths = [
    "जनवरी", "फेब्रुअरी", "मार्च", "अप्रिल", "मे", "जून",
    "जुलाई", "अगस्ट", "सेप्टेम्बर", "अक्टोबर", "नोभेम्बर", "डिसेम्बर"
];


// Helper Functions
function zero360(x) { return x - Math.floor(x / 360) * 360; }
function sinDeg(deg) { return Math.sin(deg / rad); }
function cosDeg(deg) { return Math.cos(deg / rad); }
function arcsinDeg(x) { return Math.asin(x) * rad; }

function toDevanagari(num) {
    if (num <= 0) return "";
    var devanagariNumerals = ["०", "१", "२", "३", "४", "५", "६", "७", "८", "९"];
    return String(num).split('').map(function (digit) {
        return devanagariNumerals[parseInt(digit, 10)];
    }).join('');
}

function formatMonthDay(month, day) {
    return (month < 10 ? '0' : '') + month + '/' + (day < 10 ? '0' : '') + day;
}

function toJulianDay(year, month, day) {
    var m = month + 1;
    var y = year;
    if (m <= 2) { y--; m += 12; }
    var a = Math.floor(y / 100);
    var b = 2 - a + Math.floor(a / 4);
    return Math.floor(365.25 * (y + 4716)) + Math.floor(30.6001 * (m + 1)) + day + b - 1524.5;
}

function fromJulianDay(jd) {
    jd += 0.5;
    var z = Math.floor(jd);
    var f = jd - z;
    var a;
    if (z < 2299161) {
        a = z;
    } else {
        var alpha = Math.floor((z - 1867216.25) / 36524.25);
        a = z + 1 + alpha - Math.floor(alpha / 4);
    }
    var b = a + 1524;
    var c = Math.floor((b - 122.1) / 365.25);
    var d = Math.floor(365.25 * c);
    var e = Math.floor((b - d) / 30.6001);
    var day = Math.floor(b - d - Math.floor(30.6001 * e) + f);
    var month = (e < 14) ? e - 1 : e - 13;
    var year = (month > 2) ? c - 4716 : c - 4715;
    return new Date(Date.UTC(year, month - 1, day));
}

// Core Surya Siddhanta Calculations
function meanLongitude(ahar, rotation) {
    return zero360(rotation * ahar * 360 / YugaCivilDays);
}

function mandaEquation(meanLong, apogee, circ) {
    var arg = meanLong - apogee;
    return arcsinDeg(circ / 360 * sinDeg(arg));
}

function trueLongitudeSun(ahar) {
    var meanLong = meanLongitude(ahar, YugaRotation.sun);
    var manda = mandaEquation(meanLong, PlanetApogee.sun, PlanetCircumm.sun);
    return zero360(meanLong - manda);
}

function trueLongitudeMoon(ahar) {
    var meanLong = meanLongitude(ahar, YugaRotation.moon);
    var apogee = meanLongitude(ahar, YugaRotation.Candrocca) + 90;
    var manda = mandaEquation(meanLong, apogee, PlanetCircumm.moon);
    return zero360(meanLong - manda);
}

function getTithi(sunLong, moonLong) {
    return zero360(moonLong - sunLong) / 12;
}

function findNewMoon(ahar) {
    var getElongation = function (a) { return zero360(trueLongitudeMoon(a) - trueLongitudeSun(a)); };
    var guess = ahar;
    for (var i = 0; i < 10; i++) {
        var elong = getElongation(guess);
        if (elong < 5 || elong > 355) break;
        var correction = (elong < 180 ? -elong : 360 - elong) / 12.19;
        guess += correction;
    }
    var lo = guess - 2, hi = guess + 2;
    for (var j = 0; j < 30; j++) {
        var mid = (lo + hi) / 2;
        var em = getElongation(mid);
        if (em < 180) { hi = mid; } else { lo = mid; }
    }
    return (lo + hi) / 2;
}

function findPurnima(ahar) {
    var getElongation = function (a) { return zero360(trueLongitudeMoon(a) - trueLongitudeSun(a)); };
    var guess = ahar;
    for (var i = 0; i < 10; i++) {
        var elong = getElongation(guess);
        if (Math.abs(elong - 180) < 5) break;
        var correction = (180 - elong) / 12.19;
        guess += correction;
    }
    var lo = guess - 2, hi = guess + 2;
    for (var j = 0; j < 30; j++) {
        var mid = (lo + hi) / 2;
        var em = getElongation(mid);
        if (em < 180) { lo = mid; } else { hi = mid; }
    }
    return (lo + hi) / 2;
}

function getSunriseSunset(date, lat, lon, tz) {
    lat = lat || 27.7172;
    lon = lon || 85.3240;
    tz = tz || 5.75;
    var dayOfYear = Math.floor((date - new Date(date.getUTCFullYear(), 0, 0)) / 86400000);
    var B = (360 / 365) * (dayOfYear - 81) / rad;
    var eot = 9.87 * Math.sin(2 * B) - 7.53 * Math.cos(B) - 1.5 * Math.sin(B);
    var lstm = 15 * tz;
    var tc = (4 * (lon - lstm) + eot) / 60;
    var declination = -23.45 * cosDeg(360 / 365 * (dayOfYear + 10));
    var hourAngleRad = Math.acos((sinDeg(-0.833) - sinDeg(lat) * sinDeg(declination)) / (cosDeg(lat) * cosDeg(declination)));
    var hourAngle = hourAngleRad * rad;
    var sunrise = 12 - hourAngle / 15 - tc;
    var sunset = 12 + hourAngle / 15 - tc;
    var formatTime = function (h) {
        if (!isFinite(h)) return "N/A";
        var hr = Math.floor(h);
        var min = Math.round((h - hr) * 60);
        if (min === 60) { hr++; min = 0; }
        return (hr < 10 ? '0' : '') + hr + ":" + (min < 10 ? '0' : '') + min;
    };
    return { sunrise: formatTime(sunrise), sunset: formatTime(sunset) };
}

function fromBikramSambat(bsYear, monthIndex, day) {
    if (bsYear >= Bsdata.BS_START_YEAR && bsYear <= Bsdata.BS_END_YEAR) {
        var daysOffset = 0;
        for (var y = Bsdata.BS_START_YEAR; y < bsYear; y++) {
            var yearData = Bsdata.NP_MONTHS_DATA[y - Bsdata.BS_START_YEAR];
            var totalDaysInYear = 0;
            for (var m = 0; m < 12; m++) {
                totalDaysInYear += yearData[m];
            }
            daysOffset += totalDaysInYear;
        }
        var targetYearData = Bsdata.NP_MONTHS_DATA[bsYear - Bsdata.BS_START_YEAR];
        for (let m = 0; m < monthIndex; m++) {
            daysOffset += targetYearData[m];
        }
        daysOffset += (day - 1);
        var resultDate = new Date(Bsdata.BS_START_DATE_AD.getTime());
        resultDate.setUTCDate(resultDate.getUTCDate() + daysOffset);
        return resultDate;
    } else {
        // Fallback for out-of-range years
        var YearSaka = bsYear - 135;
        var YearKali = YearSaka + 3179;
        var ahar = Math.floor((YearKali * YugaCivilDays) / YugaRotation.sun);
        var currentDay = getSauraMasaDay(ahar);
        while (currentDay.m !== monthIndex || currentDay.d !== day) {
            if (currentDay.m < monthIndex || (currentDay.m === monthIndex && currentDay.d < day)) {
                ahar += 1;
            } else {
                ahar -= 1;
            }
            currentDay = getSauraMasaDay(ahar);
        }
        var julian_date = ahar + KaliEpoch;
        return fromJulianDay(julian_date);
    }
}

function getBikramMonthInfo(bsYear, monthIndex) {
    if (bsYear >= Bsdata.BS_START_YEAR && bsYear <= Bsdata.BS_END_YEAR) {
        var firstDayAd = fromBikramSambat(bsYear, monthIndex, 1);
        if (!firstDayAd) return null;
        var monthData = Bsdata.NP_MONTHS_DATA[bsYear - Bsdata.BS_START_YEAR];
        return {
            totalDays: monthData[monthIndex],
            startDayOfWeek: firstDayAd.getUTCDay(),
            monthName: solarMonths[monthIndex],
            year: bsYear
        };
    } else {
        // Fallback for out-of-range years
        var first = fromBikramSambat(bsYear, monthIndex, 1);
        var nextMon = monthIndex === 11 ? 0 : monthIndex + 1;
        var nextYear = monthIndex === 11 ? bsYear + 1 : bsYear;
        var nextFirst = fromBikramSambat(nextYear, nextMon, 1);
        var jd1 = toJulianDay(first.getUTCFullYear(), first.getUTCMonth(), first.getUTCDate());
        var jd2 = toJulianDay(nextFirst.getUTCFullYear(), nextFirst.getUTCMonth(), nextFirst.getUTCDate());
        return {
            totalDays: Math.round(jd2 - jd1),
            startDayOfWeek: first.getUTCDay(),
            monthName: solarMonths[monthIndex],
            year: bsYear
        };
    }
}

function toBikramSambat(gregorianDate) {
    var isDataAvailable = false;
    if (Bsdata && Bsdata.BS_START_DATE_AD) {
        var targetUtcDate = new Date(Date.UTC(gregorianDate.getFullYear(), gregorianDate.getMonth(), gregorianDate.getDate()));
        var startDate = new Date(Date.UTC(Bsdata.BS_START_DATE_AD.getFullYear(), Bsdata.BS_START_DATE_AD.getMonth(), Bsdata.BS_START_DATE_AD.getDate()));
        if (targetUtcDate >= startDate && gregorianDate.getFullYear() <= (Bsdata.BS_END_YEAR - 56)) {
            isDataAvailable = true;
        }
    }

    if (isDataAvailable) {
        var daysOffset = Math.floor((targetUtcDate.getTime() - startDate.getTime()) / 86400000);
        for (var y = 0; y < Bsdata.NP_MONTHS_DATA.length; y++) {
            var currentBsYear = Bsdata.BS_START_YEAR + y;
            var yearData = Bsdata.NP_MONTHS_DATA[y];
            var daysInYear = 0;
            for (var m_idx = 0; m_idx < 12; m_idx++) { daysInYear += yearData[m_idx]; }
            if (daysOffset < daysInYear) {
                for (var m = 0; m < 12; m++) {
                    var daysInMonth = yearData[m];
                    if (daysOffset < daysInMonth) {
                        return { year: currentBsYear, monthIndex: m, day: daysOffset + 1, monthName: solarMonths[m], isComputed: false };
                    }
                    daysOffset -= daysInMonth;
                }
            }
            daysOffset -= daysInYear;
        }
    }
    // Fallback to astronomical calculation
    var result = fromGregorianAstronomical(
        gregorianDate.getUTCFullYear(),
        gregorianDate.getUTCMonth() + 1,
        gregorianDate.getUTCDate()
    );
    result.isComputed = true;
    return result;
}

function resolveTithiName(tithiDay, paksha) {
    if (paksha === "कृष्ण पक्ष" && tithiDay === 15) return tithiNamesList[15];
    if (paksha === "शुक्ल पक्ष" && tithiDay === 15) return tithiNamesList[14];
    return tithiNamesList[tithiDay - 1];
}

function getLunarMonthNameWithAdhik(ahar) {
    var lunarMonthStart = findNewMoon(ahar);
    if (lunarMonthStart > ahar) {
        lunarMonthStart = findNewMoon(lunarMonthStart - 29.53);
    }
    var sunLongAtAmavasya = trueLongitudeSun(lunarMonthStart);
    var nameSign = Math.floor(sunLongAtAmavasya / 30);
    var adhikaMasaStatus = calculateAdhikaMasa(ahar);
    var isAdhika = adhikaMasaStatus.startsWith("अधिक");
    var result = {
        monthName: solarMonths[nameSign],
        isAdhika: isAdhika
    };
    return result;
}

function _getPanchangaBasics(date) {
    var jd = toJulianDay(date.getUTCFullYear(), date.getUTCMonth(), date.getUTCDate());
    var ahar = jd - KaliEpoch + 0.25 + ((85.3240 / 15 - 5.75) / 24);
    var purnimantaInfo = calculate(date);
    return {
        ahar: ahar,
        lunarMonthName: purnimantaInfo.lunarMonth.replace("अधिक ", ""),
        isAdhika: purnimantaInfo.adhikaMasa.startsWith("अधिक"),
        paksha: purnimantaInfo.paksha,
        tithiName: purnimantaInfo.tithi,
        tithiDay: purnimantaInfo.tithiDay
    };
}


function getEventsForDate(date, bsInfo, lunarInfo) {
    var events = [];
    var gregorianYear = date.getUTCFullYear();
    var gregorianMonth = date.getUTCMonth() + 1;
    var gregorianDay = date.getUTCDate();
    var formattedGregorianDate = formatMonthDay(gregorianMonth, gregorianDay);
    var formattedBikramRecurringDate = formatMonthDay(bsInfo.monthIndex + 1, bsInfo.day);

    // Handle Gregorian events
    if (EventsData.gregorianEvents) {
        for (var i = 0; i < EventsData.gregorianEvents.length; i++) {
            var event = EventsData.gregorianEvents[i];
            if (event.startYear && gregorianYear < event.startYear) continue;
            if (event.endYear && gregorianYear > event.endYear) continue;
            if (event.date === formattedGregorianDate) {
                events.push({ name: event.event, detail: event.detail, holiday: event.holiday });
            }
        }
    }

    // Handle Bikram recurring events
    if (EventsData.bikramRecurringEvents) {
        for (var j = 0; j < EventsData.bikramRecurringEvents.length; j++) {
            let event = EventsData.bikramRecurringEvents[j];
            if (event.startYear && bsInfo.year < event.startYear) continue;
            if (event.endYear && bsInfo.year > event.endYear) continue;
            if (event.date === formattedBikramRecurringDate) {
                events.push({ name: event.event, detail: event.detail, holiday: event.holiday });
            }
        }
    }

    // Handle Bikram fixed events
    if (EventsData.bikramFixedEvents) {
        var formattedFixedDate = bsInfo.year + "/" + formatMonthDay(bsInfo.monthIndex + 1, bsInfo.day);
        for (let i = 0; i < EventsData.bikramFixedEvents.length; i++) {
            let event = EventsData.bikramFixedEvents[i];
            if (event.date === formattedFixedDate) {
                events.push({ name: event.event, detail: event.detail, holiday: event.holiday });
            }
        }
    }

    // Handle Lunar events
    if (EventsData.lunarEvents && lunarInfo) {
        if (lunarInfo.isAdhika) {
            return events; // Assuming no lunar events in Adhika masa
        }

        for (var k = 0; k < EventsData.lunarEvents.length; k++) {
            var lunarEvent = EventsData.lunarEvents[k];
            if (lunarEvent.startYear && bsInfo.year < lunarEvent.startYear) continue;
            if (lunarEvent.endYear && bsInfo.year > lunarEvent.endYear) continue;

            if (lunarEvent.lunarMonth === lunarInfo.lunarMonthName &&
                lunarEvent.paksha === lunarInfo.paksha &&
                lunarEvent.tithi === lunarInfo.tithiName) {
                events.push({ name: lunarEvent.event, detail: lunarEvent.detail, holiday: lunarEvent.holiday });
            }
        }
    }
    return events;
}

// Main Calculation Function
function calculate(date, lat, lon, tz) {
    var cacheKey = "panchanga_" + date.getTime();
    if (calculationCache[cacheKey]) return calculationCache[cacheKey];
    var jd = toJulianDay(date.getUTCFullYear(), date.getUTCMonth(), date.getUTCDate());
    var ahar = jd - KaliEpoch + 0.25 + (((lon || 85.3240) / 15 - (tz || 5.75)) / 24);
    var sunLong = trueLongitudeSun(ahar);
    var moonLong = trueLongitudeMoon(ahar);
    var tithiVal = getTithi(sunLong, moonLong);
    var tithiNum = Math.floor(tithiVal) + 1;
    var paksha = tithiNum <= 15 ? "शुक्ल पक्ष" : "कृष्ण पक्ष";
    var tithiDay = tithiNum > 15 ? tithiNum - 15 : tithiNum;
    var tithiName = resolveTithiName(tithiDay, paksha);
    var purnima_end_of_month = findPurnima(ahar);
    if (purnima_end_of_month < ahar) {
        purnima_end_of_month = findPurnima(ahar + 29.53);
    }

    var sunLongAtPurnima = trueLongitudeSun(purnima_end_of_month);
    var nameSign = Math.floor(sunLongAtPurnima / 30);
    var purnimantaMonthName = solarMonths[nameSign];

    var adhikaMasaStatus = calculateAdhikaMasa(ahar);
    var isAdhika = adhikaMasaStatus.startsWith("अधिक");

    var lunarMonthDisplayName = isAdhika ? "अधिक " + purnimantaMonthName : purnimantaMonthName;
    var bsInfo = toBikramSambat(date);
    if (!bsInfo) return { error: "Date out of range" };

    var lunarInfo = {
        lunarMonthName: purnimantaMonthName,
        paksha: paksha,
        tithiName: tithiName,
        isAdhika: isAdhika
    };
    var events = getEventsForDate(date, bsInfo, lunarInfo);

    var karanaIdx = Math.floor(2 * tithiVal);
    var karanaName = karanaIdx > 0 ? (karanaIdx < 57 ? karanas[(karanaIdx - 1) % 7 + 1] : karanas[karanaIdx - 57 + 8]) : karanas[0];
    var sunriseSunset = getSunriseSunset(date, lat, lon, tz);
    var isComputed = bsInfo.isComputed;

    var result = {
        gregorianDate: Qt.formatDateTime(date, "dddd, MMMM d, yyyy"),
        bikramSambat: toDevanagari(bsInfo.year) + " " + bsInfo.monthName + " " + toDevanagari(bsInfo.day),
        bsYear: bsInfo.year,
        bsMonthIndex: bsInfo.monthIndex,
        bsDay: bsInfo.day,
        weekday: weekdays[date.getUTCDay()],
        sunrise: sunriseSunset.sunrise,
        sunset: sunriseSunset.sunset,
        tithi: tithiName,
        tithiDay: tithiDay, // Added lunar month day
        paksha: paksha,
        lunarMonth: lunarMonthDisplayName,
        nakshatra: nakshatras[Math.floor(moonLong / (360 / 27))],
        yoga: yogas[Math.floor(zero360(sunLong + moonLong) / (360 / 27))],
        karana: karanaName,
        sunRashi: rashis[Math.floor(sunLong / 30)],
        moonRashi: rashis[Math.floor(moonLong / 30)],
        events: events,
        isComputed: isComputed,
        adhikaMasa: adhikaMasaStatus
    };
    calculationCache[cacheKey] = result;
    return result;
}


function getTslong(ahar) {
    var t1 = (YugaRotation.sun * ahar / YugaCivilDays);
    t1 -= Math.floor(t1);
    var mslong = 360 * t1;
    var x1 = mslong - PlanetApogee.sun;
    var y1 = PlanetCircumm.sun / 360;
    var y2 = sinDeg(x1);
    var y3 = y1 * y2;
    var x2 = arcsinDeg(y3);
    var x3 = mslong - x2;
    return x3;
}

function todaySauraMasaFirstP(ahar) {
    var tslong_today = getTslong(ahar);
    var tslong_tomorrow = getTslong(ahar + 1);
    tslong_today -= Math.floor(tslong_today / 30) * 30;
    tslong_tomorrow -= Math.floor(tslong_tomorrow / 30) * 30;
    return (25 < tslong_today && tslong_tomorrow < 5);
}

function getSauraMasaDay(ahar) {
    try {
        if (todaySauraMasaFirstP(ahar)) {
            var day = 1;
            var tslong_tomorrow = getTslong(ahar + 1);
            var month = Math.floor(tslong_tomorrow / 30) % 12;
            month = (month + 12) % 12;
            return { m: month, d: day };
        } else {
            var yesterday = getSauraMasaDay(ahar - 1);
            return { m: yesterday.m, d: yesterday.d + 1 };
        }
    } catch (e) {
        // Recursive functions can hit stack limits, provide a fallback.
        return { m: 0, d: 1 };
    }
}

function fromGregorianAstronomical(gYear, gMonth, gDay) {
    var julian = toJulianDay(gYear, gMonth - 1, gDay);
    var ahar = julian - KaliEpoch;
    var sauraMasaResult = getSauraMasaDay(ahar);
    var saura_masa_num = sauraMasaResult.m;
    var saura_masa_day = sauraMasaResult.d;
    var YearKali = Math.floor(ahar * YugaRotation.sun / YugaCivilDays);
    var YearSaka = YearKali - 3179;
    var nepalimonth = saura_masa_num % 12;
    var year = YearSaka + 135 + Math.floor((saura_masa_num - nepalimonth) / 12);
    var month = (saura_masa_num + 12) % 12 + 1;
    return {
        year: year,
        monthIndex: month - 1,
        day: saura_masa_day,
        monthName: solarMonths[month - 1]
    };
}

function calculateAdhikaMasa(ahar) {
    var lunarMonthStart = findNewMoon(ahar);
    if (lunarMonthStart > ahar) {
        lunarMonthStart = findNewMoon(lunarMonthStart - 29.530588853);
    }
    var lunarMonthEnd = findNewMoon(lunarMonthStart + 29.530588853);
    var sunLongStart = trueLongitudeSun(lunarMonthStart);
    var sunLongEnd = trueLongitudeSun(lunarMonthEnd);
    var startSign = Math.floor(sunLongStart / 30);
    var endSign = Math.floor(sunLongEnd / 30);

    if (endSign < startSign) endSign += 12; // Handle wrap-around from Pisces to Aries

    if (endSign === startSign) {
        return "अधिक " + solarMonths[startSign % 12];
    }
    if (endSign > startSign + 1) {
        var skippedSign = (startSign + 1) % 12;
        return "क्षय " + solarMonths[skippedSign];
    }
    return "छैन";
}


// Debug calculations
function generateDebugInfo(date, lat, lon, tz) {
    lat = lat || 27.7172;
    lon = lon || 85.3240;
    tz = tz || 5.75;
    var cacheKey = "debug_" + date.getTime();
    if (calculationCache[cacheKey]) return calculationCache[cacheKey];
    var bsInfoData = toBikramSambat(date, lon, tz);

    if (!bsInfoData) {
        return { debug: "Date out of pre-calculated range." };
    }

    var jd = toJulianDay(
        date.getUTCFullYear(),
        date.getUTCMonth(),
        date.getUTCDate()
    );
    var ahar = jd - KaliEpoch + 0.25 + ((lon / 15 - tz) / 24);
    var sunLong = trueLongitudeSun(ahar);
    var moonLong = trueLongitudeMoon(ahar);
    var tithiVal = getTithi(sunLong, moonLong);
    var tithiNum = Math.floor(tithiVal) + 1;
    var paksha = tithiNum <= 15 ? "शुक्ल पक्ष" : "कृष्ण पक्ष";
    var tithiDay = tithiNum > 15 ? tithiNum - 15 : tithiNum;
    var tithiName = resolveTithiName(tithiDay, paksha);

    var karanaIdx = Math.floor(2 * tithiVal);
    var karanaName = karanaIdx > 0 ? (karanaIdx < 57 ? karanas[(karanaIdx - 1) % 7 + 1] : karanas[karanaIdx - 57 + 8]) : karanas[0];

    var bsInfoCalc = fromGregorianAstronomical(
        date.getUTCFullYear(),
        date.getUTCMonth() + 1,
        date.getUTCDate()
    );
    var dayDifference = 0;
    var gregFromAstronomical_Today = fromBikramSambat(
        bsInfoCalc.year,
        bsInfoCalc.monthIndex,
        bsInfoCalc.day
    );

    if (gregFromAstronomical_Today) {
        var timeDiff = date.getTime() - gregFromAstronomical_Today.getTime();
        dayDifference = Math.round(timeDiff / (1000 * 60 * 60 * 24));
    }

    if (dayDifference === 0 && !bsInfoData.isComputed) {
        var isMismatch = (bsInfoData.day !== bsInfoCalc.day || bsInfoData.monthIndex !== bsInfoCalc.monthIndex);
        if (isMismatch) {
            var tomorrow = new Date(date.getTime());
            tomorrow.setUTCDate(date.getUTCDate() + 1);
            var bsInfoCalc_Tomorrow = fromGregorianAstronomical(
                tomorrow.getUTCFullYear(),
                tomorrow.getUTCMonth() + 1,
                tomorrow.getUTCDate()
            );

            var gregFromAstronomical_Tomorrow = fromBikramSambat(
                bsInfoCalc_Tomorrow.year,
                bsInfoCalc_Tomorrow.monthIndex,
                bsInfoCalc_Tomorrow.day
            );

            if (gregFromAstronomical_Tomorrow) {
                var timeDiff_Tomorrow = tomorrow.getTime() - gregFromAstronomical_Tomorrow.getTime();
                var dayDifference_Tomorrow = Math.round(timeDiff_Tomorrow / (1000 * 60 * 60 * 24));

                if (dayDifference_Tomorrow !== 0) {
                    dayDifference = dayDifference_Tomorrow;
                }
            }
        }
    }

    var dayDifferenceDisplay = dayDifference;
    if (Math.abs(dayDifference) > 2) {
        dayDifferenceDisplay = '<font color="#ff0000">' + dayDifference + '</font>';
    } else if (Math.abs(dayDifference) > 0) {
        dayDifferenceDisplay = '<font color="yellow">' + dayDifference + '</font>';
    }

    var sunriseSunset = getSunriseSunset(date, lat, lon, tz);
    var isComputed = bsInfoData.isComputed;
    var computedbsdate = toDevanagari(bsInfoCalc.year) + " " + bsInfoCalc.monthName + " " + toDevanagari(bsInfoCalc.day);
    var dataDrivenBsDateString = "N/A";
    var acceptedBsDate = computedbsdate;
    var dataDrivenInfo = "";

    if (bsInfoData && !isComputed) {
        dataDrivenBsDateString = toDevanagari(bsInfoData.year) + " " + bsInfoData.monthName + " " + toDevanagari(bsInfoData.day);
        acceptedBsDate = dataDrivenBsDateString;
        dataDrivenInfo = `<span style="font-size: 10pt; color: #00CED1;">Debug Information\n(Data-Driven for date conversion):</span>\n` + "Gregorian Date: " + date + "\n" + "accepted BS Date: " + acceptedBsDate + "\n" + "(panchanga is based on data-driven conversion)\n";
    } else {
        dataDrivenInfo = `<span style="font-size: 10pt; color: #FF6347;">Debug Information\n(Astronomical Calculation - outside data range):</span>\n` + "Gregorian Date: " + date + "\n" + "accepted BS Date: " + acceptedBsDate + "\n" + "(panchanga is based on astronomical computation)\n";
    }

    var fullPanchanga = calculate(date, lat, lon, tz);
    var lunarMonthDisplay = fullPanchanga.lunarMonth + " " + fullPanchanga.paksha;

    var debugOutput =
        '<pre style="font-family: monospace; font-size: 8pt; color: white; white-space: pre; line-height: 1.2;">' +
        dataDrivenInfo +
        `<span style="color: orange;">Consistency Check:</span>\n` +
        "Data-Driven BS Date: " + dataDrivenBsDateString + "\n" +
        "Astronomical BS Date (Computed): " + computedbsdate + "\n" +
        "Day Difference: " + dayDifferenceDisplay + " " + (Math.abs(dayDifference) === 1 ? 'day' : 'days') + "\n" +
        `<span style="color: red;">Note: Positive = Astronomical date is behind;\n` +
        `Negative = Astronomical date is ahead</span>\n` +
        " " + `<span style="color: yellow;">Lunar Month (Purnimanta): ` + lunarMonthDisplay + "</span>\n" +
        `<span style="color: #B118E7;">--- Solar Outputs ---</span>\n` +
        "gregorianDate: " + Qt.formatDateTime(date, "dddd, MMMM d, yyyy") + "\n" +
        "sunrise: " + sunriseSunset.sunrise + "\n" + "sunset: " + sunriseSunset.sunset + "\n" +
        "sunRashi: " + rashis[Math.floor(sunLong / 30) % 12] + " | index: " + (Math.floor(sunLong / 30) % 12 + 1) + "\n" +
        " " + `<span style="color: #B118E7;">--- Lunar Outputs ---</span>\n` +
        "tithi: " + tithiName + " | index: " + tithiDay + "\n" +
        "tithiAngle: " + (tithiVal * 12).toFixed(4) + "°\n" + "paksha: " + paksha + "\n" +
        "nakshatra: " + nakshatras[Math.floor(moonLong / (360 / 27))] + " | index: " + (Math.floor(moonLong / (360 / 27)) + 1) + "\n" +
        "yoga: " + yogas[Math.floor(zero360(sunLong + moonLong) / (360 / 27))] + " | index: " + (Math.floor(zero360(sunLong + moonLong) / (360 / 27)) + 1) + "\n" +
        "yogaAngle: " + zero360(sunLong + moonLong).toFixed(4) + "°\n" +
        "karana: " + karanaName + " | index: " + karanaIdx + "\n" +
        "karanaAngle: " + (2 * tithiVal).toFixed(4) + "\n" +
        "moonRashi: " + rashis[Math.floor(moonLong / 30) % 12] + " | index: " + (Math.floor(moonLong / 30) % 12 + 1) + "\n" +
        "adhikaMasa: " + calculateAdhikaMasa(ahar) + " (computed)\n" +

        " " + `<span style="color: #B118E7;">--- Metadata ---</span>\n` +
        "Julian Day: " + jd.toFixed(4) + "\n" +
        "Ahar: " + ahar.toFixed(4) + "\n" +
        "weekday (UTC): " + weekdays[date.getUTCDay()] + "\n" +
        "isComputed: " + isComputed +
        '</pre>';

    var result = { debug: debugOutput.trim().replace(/^\s*[\r\n]/gm, "") };
    calculationCache[cacheKey] = result;
    return result;
}
