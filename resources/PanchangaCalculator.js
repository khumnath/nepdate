// PanchangaCalculator.js
// QML-compatible version - uses var, no 'class', no 'const/let', no =>

.pragma library

// --- Constants and Global Variables (some are  not used now) ---
var YugaRotation = {
    'star': 1582237828,
    'sun': 4320000,
    'moon': 57753336,
    'mercury': 17937060,
    'venus': 7022376,
    'mars': 2296832,
    'jupiter': 364220,
    'saturn': 146568,
    'Candrocca': 488203,
    'Rahu': -232238
};
var YugaCivilDays = YugaRotation.star - YugaRotation.sun;
var KaliEpoch = 588465.5;

var PlanetApogee = { 'sun': 77 + 17 / 60 };
var PlanetCircumm = { 'sun': 13 + 50 / 60, 'moon': 31 + 50 / 60 };

var rashis = [
    "मेष", "वृषभ", "मिथुन", "कर्क", "सिंह", "कन्या",
    "तुला", "वृश्चिक", "धनु", "मकर", "कुम्भ", "मीन"
];
var solarMonths = [
    "वैशाख", "ज्येष्ठ", "आषाढ़", "श्रावण", "भाद्रपद", "आश्विन",
    "कार्तिक", "मार्गशीर्ष", "पौष", "माघ", "फाल्गुन", "चैत्र"
];
var nepaliGregorianMonths = [
    "जनवरी", "फेब्रुअरी", "मार्च", "अप्रिल", "मे", "जून",
    "जुलाई", "अगस्ट", "सेप्टेम्बर", "अक्टोबर", "नोभेम्बर", "डिसेम्बर"
];

// --- Helper Functions ---
function zero360(x) {
    x = x - Math.floor(x / 360) * 360;
    return x < 0 ? x + 360 : x;
}
function sinDeg(deg) { return Math.sin(deg * Math.PI / 180); }
function cosDeg(deg) { return Math.cos(deg * Math.PI / 180); }
function arcsinDeg(x) { return Math.asin(x) * 180 / Math.PI; }

function toDevanagari(num) {
    if (num <= 0) return "";
    var devanagariNumerals = ["०", "१", "२", "३", "४", "५", "६", "७", "८", "९"];
    return String(num).split('').map(function(digit) {
        return devanagariNumerals[parseInt(digit)];
    }).join('');
}

function toJulianDay(year, month, day) {
    month += 1;
    if (month <= 2) {
        year--;
        month += 12;
    }
    var a = Math.floor(year / 100);
    var b = 2 - a + Math.floor(a / 4);
    return Math.floor(365.25 * (year + 4716)) +
           Math.floor(30.6001 * (month + 1)) +
           day + b - 1524.5;
}

function fromJulianDay(jd) {
    jd += 0.5;
    var z = Math.floor(jd);
    var f = jd - z;
    var a, alpha;
    if (z < 2299161) {
        a = z;
    } else {
        alpha = Math.floor((z - 1867216.25) / 36524.25);
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

function getRashi(longitude) {
    return rashis[Math.floor(longitude / 30) % 12];
}

function getSolarMonth(longitude) {
    return solarMonths[Math.floor(longitude / 30) % 12];
}

function getSolarDay(ahar) {
    var intAhar = Math.floor(ahar);
    for (var i = 0; i <= 32; i++) {
        var t1 = trueLongitudeSun(intAhar - i);
        var t2 = trueLongitudeSun(intAhar - i + 1);
        if (Math.floor(t1 / 30) !== Math.floor(t2 / 30)) return i + 1;
    }
    return 1;
}

function getVikramYear(ahar) {
    var kaliYear = Math.floor(ahar * YugaRotation.sun / YugaCivilDays);
    return kaliYear - 3044;
}

function getBikramSambatInfo(ahar, sunLong) {
    var year = getVikramYear(ahar);
    var monthName = getSolarMonth(sunLong);
    var monthIndex = solarMonths.indexOf(monthName);
    var day = getSolarDay(ahar);
    return { year: year, monthIndex: monthIndex, day: day, monthName: monthName };
}

function findNewMoon(ahar) {
    var getElongation = function(a) { return zero360(trueLongitudeMoon(a) - trueLongitudeSun(a)); };
    var lo = ahar - 2, hi = ahar + 2;
    var el = getElongation(lo), eh = getElongation(hi);
    if (el < eh) el += 360;
    for (var i = 0; i < 30; i++) {
        var mid = (lo + hi) / 2;
        var em = getElongation(mid);
        if (el < em) em -= 360;
        if (em > 0) { lo = mid; el = em; }
        else { hi = mid; eh = em; }
    }
    return (lo + hi) / 2;
}

function calculateAdhikaMasa(ahar) {
    var tithi = getTithi(trueLongitudeSun(ahar), trueLongitudeMoon(ahar));
    var nextNew = findNewMoon(ahar + (30 - tithi));
    var prevNew = findNewMoon(nextNew - 29.5);
    var signPrev = Math.floor(trueLongitudeSun(prevNew) / 30);
    var signNext = Math.floor(trueLongitudeSun(nextNew) / 30);
    if (signPrev === signNext) return "अधिक " + solarMonths[(signNext + 1) % 12];
    if ((signNext - signPrev + 12) % 12 === 2) return "क्षय " + solarMonths[(signPrev + 1) % 12];
    return "उपलब्ध छैन";
}

function getSunriseSunset(date, lat, lon, tz) {
    lat = lat || 27.7;
    lon = lon || 85.3;
    tz = tz || 5.75;
    var oneDay = 86400000;
    var dayOfYear = Math.ceil((date - new Date(Date.UTC(date.getUTCFullYear(), 0, 0))) / oneDay);

    var declination = 23.45 * sinDeg(360 / 365 * (dayOfYear - 81));
    var B = (360 / 365) * (dayOfYear - 81);
    var eot = 9.87 * sinDeg(2 * B) - 7.53 * cosDeg(B) - 1.5 * sinDeg(B / 360 * 180 / Math.PI);
    var timeCorr = (4 * (lon - 15 * tz) + eot) / 60;
    var cosH = (sinDeg(-0.833) - sinDeg(lat) * sinDeg(declination)) /
               (cosDeg(lat) * cosDeg(declination));
    var H = cosH >= -1 && cosH <= 1 ? Math.acos(cosH) * 180 / Math.PI : 90;
    var noon = 12 - timeCorr;
    var rise = noon - H / 15;
    var set = noon + H / 15;

    var formatTime = function(h) {
        if (!isFinite(h)) return "N/A";
        var hr = Math.floor(h), min = Math.round((h - hr) * 60);
        if (min === 60) { hr++; min = 0; }
        var period = hr >= 12 ? "PM" : "AM";
        hr = hr % 12 || 12;
        return hr + ":" + (min < 10 ? '0' : '') + min + " " + period;
    };
    return { sunrise: formatTime(rise), sunset: formatTime(set) };
}

// Main calculation function
function calculate(date, lat, lon, tz) {
    lat = lat || 27.7;
    lon = lon || 85.3;
    tz = tz || 5.75;
    var jd = toJulianDay(date.getUTCFullYear(), date.getUTCMonth(), date.getUTCDate());

    // --- THIS IS THE CORRECTED LINE ---
    // Adjust ahar for local sunrise time based on timezone and longitude
    var ahar = jd - KaliEpoch + 0.25 + ((lon / 15 - tz) / 24);

    var sunLong = trueLongitudeSun(ahar);
    var moonLong = trueLongitudeMoon(ahar);
    var tithiVal = getTithi(sunLong, moonLong);
    var tithiNum = Math.floor(tithiVal) + 1;
    var paksha = tithiNum <= 15 ? "शुक्ल पक्ष" : "कृष्ण पक्ष";
    var tithiDay = tithiNum > 15 ? tithiNum - 15 : tithiNum;

    var tithiNames = [
        "प्रतिपदा", "द्वितीया", "तृतीया", "चतुर्थी", "पञ्चमी", "षष्ठी", "सप्तमी", "अष्टमी", "नवमी", "दशमी",
        "एकादशी", "द्वादशी", "त्रयोदशी", "चतुर्दशी", "पूर्णिमा", "अमावस्या"
    ];
    var nakshatras = [
        "अश्विनी", "भरणी", "कृत्तिका", "रोहिणी", "मृगशीर्ष", "आर्द्रा", "पुनर्वसु", "पुष्य", "अश्लेषा",
        "मघा", "पूर्व फाल्गुनी", "उत्तर फाल्गुनी", "हस्त", "चित्रा", "स्वाति", "विशाखा", "अनुराधा",
        "ज्येष्ठा", "मूल", "पूर्वाषाढ़ा", "उत्तराषाढ़ा", "श्रवण", "धनिष्ठा", "शतभिषा", "पूर्व भाद्रपद",
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

    var tithiName = tithiNames[tithiDay - 1];
    if (paksha === "कृष्ण पक्ष" && tithiDay === 15) tithiName = tithiNames[15];
    if (paksha === "शुक्ल पक्ष" && tithiDay === 15) tithiName = tithiNames[14];

    var karanaIdx = Math.floor(2 * tithiVal);
    var karanaName = karanas[0];
    if (karanaIdx > 0) {
        if (karanaIdx < 57) karanaName = karanas[karanaIdx % 7 || 7];
        else karanaName = karanas[karanaIdx - 57 + 8];
    }

    var bsInfo = getBikramSambatInfo(ahar, sunLong);
    var sunriseSunset = getSunriseSunset(date, lat, lon, tz);

    return {
        gregorianDate: Qt.formatDateTime(date, "dddd, MMMM d, yyyy"),
        bikramSambat: bsInfo.year + " " + bsInfo.monthName + " " + bsInfo.day,
        bsYear: bsInfo.year,
        bsMonthIndex: bsInfo.monthIndex,
        bsDay: bsInfo.day,
        weekday: formatWeekday(date),
        sunrise: sunriseSunset.sunrise,
        sunset: sunriseSunset.sunset,
        tithi: tithiName,
        paksha: paksha,
        nakshatra: nakshatras[Math.floor(moonLong * 27 / 360) % 27],
        yoga: yogas[Math.floor(zero360(sunLong + moonLong) * 27 / 360) % 27],
        karana: karanaName,
        sunRashi: getRashi(sunLong),
        moonRashi: getRashi(moonLong),
        adhikaMasa: calculateAdhikaMasa(ahar)
    };
}

function fromBikramSambat(bsYear, monthIndex, day) {
    var sakaYear = bsYear - 135;
    var kaliYear = sakaYear + 3179;
    var approxAhar = (kaliYear * YugaCivilDays / YugaRotation.sun);
    approxAhar += monthIndex * 30;
    for (var i = 0; i < 45; i++) {
        var sunLong = trueLongitudeSun(approxAhar);
        var targetLong = monthIndex * 30;
        var diff = zero360(targetLong - sunLong);
        if (diff > 180) diff -= 360;
        if (Math.abs(diff) < 0.1) break;
        approxAhar += diff / 0.9856;
    }
    var targetAhar = approxAhar + (day - 1);
    var jd = targetAhar + KaliEpoch;
    return fromJulianDay(jd);
}

function getBikramMonthInfo(bsYear, monthIndex) {
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

function getTodayBsInfo() {
    var now = new Date();
    var jd = toJulianDay(now.getUTCFullYear(), now.getUTCMonth(), now.getUTCDate());
    // Use the same correction as in the main calculate function
    var ahar = jd - KaliEpoch + 0.25 + ((85.3 / 15 - 5.75) / 24);
    var sunLong = trueLongitudeSun(ahar);
    return getBikramSambatInfo(ahar, sunLong);
}

function formatWeekday(date) {
    var weekdays = ["आइतबार", "सोमबार", "मङ्गलबार", "बुधबार", "बिहीबार", "शुक्रबार", "शनिबार"];
    return weekdays[date.getUTCDay()];
}
