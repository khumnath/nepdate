// PancangaCalculator.js
.pragma library

class PancangaCalculator {
    constructor() {
        // SuryaSiddhanta constants
        this.YugaRotation = {
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

        this.YugaCivilDays = this.YugaRotation.star - this.YugaRotation.sun;
        this.KaliEpoch = 588465.5; // Julian Day of the Kali Yuga epoch

        // Planetary constants
        this.PlanetApogee = {
            'sun': 77 + 17 / 60
        };
        this.PlanetCircumm = {
            'sun': 13 + 50 / 60,
            'moon': 31 + 50 / 60
        };

        // Zodiac signs
        this.rashis = [
            "मेष", "वृषभ", "मिथुन", "कर्क", "सिंह", "कन्या",
            "तुला", "वृश्चिक", "धनु", "मकर", "कुम्भ", "मीन"
        ];

        // Solar months (adjusted for Bikram Sambat)
        this.solarMonths = [
            "वैशाख", "ज्येष्ठ", "आषाढ़", "श्रावण", "भाद्रपद", "आश्विन",
            "कार्तिक", "मार्गशीर्ष", "पौष", "माघ", "फाल्गुन", "चैत्र"
        ];
    }

    // --- Helper Math Functions ---
    zero360(x) {
        x = x - Math.floor(x / 360) * 360;
        return x < 0 ? x + 360 : x;
    }

    sinDeg(deg) { return Math.sin(deg * Math.PI / 180); }
    cosDeg(deg) { return Math.cos(deg * Math.PI / 180); }
    arcsinDeg(x) { return Math.asin(x) * 180 / Math.PI; }

    // --- Number Conversion ---
    toDevanagari(num) {
        if (num <= 0) return ""; // Don't display zero or negative numbers
        const devanagariNumerals = ["०", "१", "२", "३", "४", "५", "६", "७", "८", "९"];
        return String(num).split('').map(digit => devanagariNumerals[parseInt(digit)]).join('');
    }

    // --- Date Conversion Functions ---
    toJulianDay(year, month, day) {
        // JS month is 0-11, so we add 1 for this algorithm
        month += 1;
        if (month <= 2) {
            year--;
            month += 12;
        }
        const a = Math.floor(year / 100);
        const b = 2 - a + Math.floor(a / 4);
        return Math.floor(365.25 * (year + 4716)) +
               Math.floor(30.6001 * (month + 1)) +
               day + b - 1524.5;
    }

    fromJulianDay(jd) {
        jd += 0.5;
        const z = Math.floor(jd);
        const f = jd - z;
        let a, alpha;
        if (z < 2299161) {
            a = z;
        } else {
            alpha = Math.floor((z - 1867216.25) / 36524.25);
            a = z + 1 + alpha - Math.floor(alpha / 4);
        }
        const b = a + 1524;
        const c = Math.floor((b - 122.1) / 365.25);
        const d = Math.floor(365.25 * c);
        const e = Math.floor((b - d) / 30.6001);
        const day = Math.floor(b - d - Math.floor(30.6001 * e) + f);
        const month = (e < 14) ? e - 1 : e - 13;
        const year = (month > 2) ? c - 4716 : c - 4715;
        const date = new Date(0);
        date.setUTCFullYear(year, month - 1, day);
        date.setUTCHours(0, 0, 0, 0);
        return date;
    }

    // --- Core Astronomical Calculations ---
    meanLongitude(ahar, rotation) {
        return this.zero360(rotation * ahar * 360 / this.YugaCivilDays);
    }

    mandaEquation(meanLong, apogee, circ) {
        const argument = meanLong - apogee;
        return this.arcsinDeg(circ / 360 * this.sinDeg(argument));
    }

    trueLongitudeSun(ahar) {
        const meanLong = this.meanLongitude(ahar, this.YugaRotation.sun);
        const manda = this.mandaEquation(meanLong, this.PlanetApogee.sun, this.PlanetCircumm.sun);
        return this.zero360(meanLong - manda);
    }

    trueLongitudeMoon(ahar) {
        const meanLong = this.meanLongitude(ahar, this.YugaRotation.moon);
        const apogee = this.meanLongitude(ahar, this.YugaRotation.Candrocca) + 90;
        const manda = this.mandaEquation(meanLong, apogee, this.PlanetCircumm.moon);
        return this.zero360(meanLong - manda);
    }

    // --- Pancanga Element Calculations ---
    getTithi(sunLong, moonLong) { return this.zero360(moonLong - sunLong) / 12; }
    getRashi(longitude) { return this.rashis[Math.floor(longitude / 30) % 12]; }
    getSolarMonth(longitude) { return this.solarMonths[Math.floor(longitude / 30) % 12]; }

    getSolarDay(ahar) {
        const integerAhar = Math.floor(ahar);
        for (let i = 0; i <= 32; i++) {
            const checkAhar = integerAhar - i;
            const sign_t1 = Math.floor(this.trueLongitudeSun(checkAhar) / 30);
            const sign_t2 = Math.floor(this.trueLongitudeSun(checkAhar + 1) / 30);
            if (sign_t1 !== sign_t2) {
                return i + 1;
            }
        }
        return 1; // Fallback
    }

    getVikramYear(ahar) {
        const kaliYear = Math.floor(ahar * this.YugaRotation.sun / this.YugaCivilDays);
        return kaliYear - 3044;
    }

    getBikramSambatInfo(ahar, sunLong) {
        const year = this.getVikramYear(ahar);
        const monthName = this.getSolarMonth(sunLong);
        const monthIndex = this.solarMonths.indexOf(monthName);
        const day = this.getSolarDay(ahar);
        return {
            year: year,
            monthIndex: monthIndex,
            day: day,
            monthName: monthName,
            formattedString: `${year} - ${monthName} - ${day}`
        };
    }

    findNewMoon(ahar) {
        const getElongation = (currentAhar) => this.zero360(this.trueLongitudeMoon(currentAhar) - this.trueLongitudeSun(currentAhar));
        let lowAhar = ahar - 2, highAhar = ahar + 2;
        let lowElong = getElongation(lowAhar), highElong = getElongation(highAhar);
        if (lowElong < highElong) lowElong += 360;
        for (let i = 0; i < 30; i++) {
            let midAhar = (lowAhar + highAhar) / 2;
            let midElong = getElongation(midAhar);
            if (lowElong < midElong) midElong -= 360;
            if (midElong > 0) { lowAhar = midAhar; lowElong = midElong; }
            else { highAhar = midAhar; highElong = midElong; }
        }
        return (lowAhar + highAhar) / 2;
    }

    calculateAdhikaMasa(ahar) {
        const tithi = this.getTithi(this.trueLongitudeSun(ahar), this.trueLongitudeMoon(ahar));
        const nextNewMoonAhar = this.findNewMoon(ahar + (30 - tithi));
        const prevNewMoonAhar = this.findNewMoon(nextNewMoonAhar - 29.5);
        const solarSignAtPrev = Math.floor(this.trueLongitudeSun(prevNewMoonAhar) / 30);
        const solarSignAtNext = Math.floor(this.trueLongitudeSun(nextNewMoonAhar) / 30);
        if (solarSignAtPrev === solarSignAtNext) {
            return `Adhika ${this.solarMonths[(solarSignAtNext + 1) % 12]}`;
        }
        if ((solarSignAtNext - solarSignAtPrev + 12) % 12 === 2) {
            return `Kshaya ${this.solarMonths[(solarSignAtPrev + 1) % 12]}`;
        }
        return "No";
    }

    // --- Sunrise/Sunset Calculation ---
    getSunriseSunset(date, latitude, longitude, timezone) {
        const toRad = (deg) => deg * Math.PI / 180;
        const toDeg = (rad) => rad * 180 / Math.PI;
        const dayOfYear = (date - new Date(date.getFullYear(), 0, 0)) / 86400000;
        const declinationAngle = 23.45 * this.sinDeg(360 / 365 * (dayOfYear - 81));
        const solarDeclination = toRad(declinationAngle);
        const B = toRad((360 / 365) * (dayOfYear - 81));
        const eot = 9.87 * this.sinDeg(2 * B) - 7.53 * this.cosDeg(B) - 1.5 * this.sinDeg(B);
        const timeCorrection = (4 * (longitude - (15 * timezone)) + eot) / 60;
        const hourAngleArg = (this.sinDeg(-0.833) - this.sinDeg(latitude) * Math.sin(solarDeclination)) / (this.cosDeg(latitude) * Math.cos(solarDeclination));
        const hourAngle = toDeg(Math.acos(hourAngleArg));
        const solarNoon = 12 - timeCorrection;
        const sunrise = solarNoon - hourAngle / 15;
        const sunset = solarNoon + hourAngle / 15;
        const formatTime = (hours) => {
            if (isNaN(hours)) return "N/A";
            let h = Math.floor(hours);
            let m = Math.round((hours - h) * 60);
            if (m === 60) { h++; m = 0; }
            const period = h >= 12 && h < 24 ? "PM" : "AM";
            let hour12 = h % 12;
            if (hour12 === 0) hour12 = 12;
            return `${hour12}:${m < 10 ? '0' : ''}${m} ${period}`;
        };
        return { sunrise: formatTime(sunrise), sunset: formatTime(sunset) };
    }

    // --- Main Calculation Function ---
    calculate(date, latitude, longitude, timezone) {
        const jd = this.toJulianDay(date.getFullYear(), date.getMonth(), date.getDate());
        let ahar = jd - this.KaliEpoch;
        ahar += 0.25 + ((longitude / 15 - timezone) / 24);

        const sunLong = this.trueLongitudeSun(ahar);
        const moonLong = this.trueLongitudeMoon(ahar);
        const tithi = this.getTithi(sunLong, moonLong);
        const tithiNum = Math.floor(tithi) + 1;
        const paksha = tithiNum <= 15 ? "शुक्ल पक्ष" : "कृष्ण पक्ष";
        const tithiDay = tithiNum > 15 ? tithiNum - 15 : tithiNum;

        const tithiNames = ["प्रतिपदा", "द्वितीया", "तृतीया", "चतुर्थी", "पञ्चमी", "षष्ठी", "सप्तमी", "अष्टमी", "नवमी", "दशमी", "एकादशी", "द्वादशी", "त्रयोदशी", "चतुर्दशी", "पूर्णिमा", "अमावस्या"];
        const nakshatras = ["अश्विनी", "भरणी", "कृत्तिका", "रोहिणी", "मृगशीर्ष", "आर्द्रा", "पुनर्वसु", "पुष्य", "अश्लेषा", "मघा", "पूर्व फाल्गुनी", "उत्तर फाल्गुनी", "हस्त", "चित्रा", "स्वाति", "विशाखा", "अनुराधा", "ज्येष्ठा", "मूल", "पूर्वाषाढ़ा", "उत्तराषाढ़ा", "श्रवण", "धनिष्ठा", "शतभिषा", "पूर्व भाद्रपद", "उत्तर भाद्रपद", "रेवती"];
        const yogas = ["विष्कम्भ", "प्रीति", "आयुष्मान्", "सौभाग्य", "शोभन", "अतिगण्ड", "सुकर्म", "धृति", "शूल", "गण्ड", "वृद्धि", "ध्रुव", "व्याघात", "हर्षण", "वज्र", "सिद्धि", "व्यतिपात", "वरीयान्", "परिघ", "शिव", "सिद्ध", "साध्य", "शुभ", "शुक्ल", "ब्रह्म", "इन्द्र", "वैधृति"];
        const karanas = ["किंस्तुघ्न", "बव", "बालव", "कौलव", "तैतिल", "गर", "वणिज", "विष्टि", "शकुनि", "चतुष्पाद", "नाग"];

        let tithiName = tithiNames[tithiDay - 1];
        if (paksha === "कृष्ण पक्ष" && tithiDay === 15) tithiName = tithiNames[15];
        if (paksha === "शुक्ल पक्ष" && tithiDay === 15) tithiName = tithiNames[14];

        const karanaIndex = Math.floor(2 * tithi);
        let karanaName = karanas[0];
        if (karanaIndex > 0) {
            if (karanaIndex < 57) karanaName = karanas[(karanaIndex % 7) || 7];
            else karanaName = karanas[karanaIndex - 57 + 8];
        }

        const { sunrise, sunset } = this.getSunriseSunset(date, latitude, longitude, timezone);
        const optionsDate = { weekday: 'long', year: 'numeric', month: 'long', day: 'numeric' };
        const optionsWeekday = { weekday: 'long' };

        const bsInfo = this.getBikramSambatInfo(ahar, sunLong);

        return {
            gregorianDate: date.toLocaleDateString('en-US', optionsDate),
            bikramSambat: bsInfo.formattedString,
            bsYear: bsInfo.year,
            bsMonthIndex: bsInfo.monthIndex,
            bsDay: bsInfo.day,
            weekday: date.toLocaleDateString('en-US', optionsWeekday),
            sunrise: sunrise,
            sunset: sunset,
            tithi: tithiName,
            paksha: paksha,
            nakshatra: nakshatras[Math.floor(moonLong * 27 / 360) % 27],
            yoga: yogas[Math.floor(this.zero360(sunLong + moonLong) * 27 / 360) % 27],
            karana: karanaName,
            sunRashi: this.rashis[Math.floor(sunLong / 30) % 12],
            moonRashi: this.rashis[Math.floor(moonLong / 30) % 12],
            adhikaMasa: this.calculateAdhikaMasa(ahar)
        };
    }

    fromBikramSambat(bsYear, solarMonthIndex, solarDay) {
        const sakaYear = bsYear - 135;
        const kaliYear = sakaYear + 3179;
        const approxAharAtYearStart = (kaliYear * this.YugaCivilDays / this.YugaRotation.sun);
        let searchAhar = approxAharAtYearStart + (solarMonthIndex * 30);

        for (let i = 0; i < 30; i++) {
            let sunLong = this.trueLongitudeSun(searchAhar);
            let currentSign = Math.floor(sunLong / 30);
            let requiredLong = solarMonthIndex * 30;
            if (currentSign < solarMonthIndex || (currentSign === 11 && solarMonthIndex === 0)) {
                 searchAhar += (requiredLong - sunLong + (sunLong < requiredLong ? 0 : 360)) / 0.985;
            } else {
                 searchAhar -= (sunLong - requiredLong) / 0.985;
            }
        }
        const sankrantiAhar = searchAhar;
        const targetAhar = sankrantiAhar + (solarDay - 1);
        const julianDay = targetAhar + this.KaliEpoch;
        return this.fromJulianDay(julianDay);
    }

    getBikramMonthInfo(bsYear, monthIndex) {
        const firstDayGregorian = this.fromBikramSambat(bsYear, monthIndex, 1);
        let nextMonth = monthIndex + 1;
        let nextYear = bsYear;
        if (nextMonth > 11) {
            nextMonth = 0;
            nextYear++;
        }
        const nextMonthFirstDayGregorian = this.fromBikramSambat(nextYear, nextMonth, 1);
        const jd1 = this.toJulianDay(firstDayGregorian.getUTCFullYear(), firstDayGregorian.getUTCMonth(), firstDayGregorian.getUTCDate());
        const jd2 = this.toJulianDay(nextMonthFirstDayGregorian.getUTCFullYear(), nextMonthFirstDayGregorian.getUTCMonth(), nextMonthFirstDayGregorian.getUTCDate());
        const totalDays = Math.round(jd2 - jd1);
        const startDayOfWeek = firstDayGregorian.getUTCDay();
        return {
            totalDays: totalDays,
            startDayOfWeek: startDayOfWeek,
            monthName: this.solarMonths[monthIndex],
            year: bsYear
        };
    }

    getTodayBsInfo() {
        const today = new Date();
        const jd = this.toJulianDay(today.getFullYear(), today.getMonth(), today.getDate());
        const ahar = jd - this.KaliEpoch;
        const sunLong = this.trueLongitudeSun(ahar);
        return this.getBikramSambatInfo(ahar, sunLong);
    }
}
