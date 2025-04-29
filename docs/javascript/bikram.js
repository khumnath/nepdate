class Bikram {
    constructor() {
        this.Year = 0;
        this.Month = -1;
        this.Day = 0;
        this.YugaRotation_star = 1582237828;
        this.YugaRotation_sun = 4320000;
        this.YugaCivilDays = this.YugaRotation_star - this.YugaRotation_sun;
        this.PlanetApogee_sun = 77 + (17 / 60);
        this.PlanetCircumm_sun = 13 + (50 / 60);
        this.rad = 57.2957795; // = 180/pi
        this.BS_START_YEAR = 2000;
        this.BS_END_YEAR = 2089;
        this.NP_MONTHS_DATA = [
            [30, 32, 31, 32, 31, 30, 30, 30, 29, 30, 29, 31, 365], // 2000
            [31, 31, 32, 31, 31, 31, 30, 29, 30, 29, 30, 30, 365], // 2001
            [31, 31, 32, 32, 31, 30, 30, 29, 30, 29, 30, 30, 365], // 2002
            [31, 32, 31, 32, 31, 30, 30, 30, 29, 29, 30, 31, 366], // 2003
            [30, 32, 31, 32, 31, 30, 30, 30, 29, 30, 29, 31, 365], // 2004
            [31, 31, 32, 31, 31, 31, 30, 29, 30, 29, 30, 30, 365], // 2005
            [31, 31, 32, 32, 31, 30, 30, 29, 30, 29, 30, 30, 365], // 2006
            [31, 32, 31, 32, 31, 30, 30, 30, 29, 29, 30, 31, 366], // 2007
            [31, 31, 31, 32, 31, 31, 29, 30, 30, 29, 29, 31, 365], // 2008
            [31, 31, 32, 31, 31, 31, 30, 29, 30, 29, 30, 30, 365], // 2009
            [31, 31, 32, 32, 31, 30, 30, 29, 30, 29, 30, 30, 365], // 2010
            [31, 32, 31, 32, 31, 30, 30, 30, 29, 29, 30, 31, 366], // 2011
            [31, 31, 31, 32, 31, 31, 29, 30, 30, 29, 30, 30, 365], // 2012
            [31, 31, 32, 31, 31, 31, 30, 29, 30, 29, 30, 30, 365], // 2013
            [31, 31, 32, 32, 31, 30, 30, 29, 30, 29, 30, 30, 365], // 2014
            [31, 32, 31, 32, 31, 30, 30, 30, 29, 29, 30, 31, 366], // 2015
            [31, 31, 31, 32, 31, 31, 29, 30, 30, 29, 30, 30, 365], // 2016
            [31, 31, 32, 31, 31, 31, 30, 29, 30, 29, 30, 30, 365], // 2017
            [31, 32, 31, 32, 31, 30, 30, 29, 30, 29, 30, 30, 365], // 2018
            [31, 32, 31, 32, 31, 30, 30, 30, 29, 30, 29, 31, 366], // 2019
            [31, 31, 31, 32, 31, 31, 30, 29, 30, 29, 30, 30, 365], // 2020
            [31, 31, 32, 31, 31, 31, 30, 29, 30, 29, 30, 30, 365], // 2021
            [31, 32, 31, 32, 31, 30, 30, 30, 29, 29, 30, 30, 365], // 2022
            [31, 32, 31, 32, 31, 30, 30, 30, 29, 30, 29, 31, 366], // 2023
            [31, 31, 31, 32, 31, 31, 30, 29, 30, 29, 30, 30, 365], // 2024
            [31, 31, 32, 31, 31, 31, 30, 29, 30, 29, 30, 30, 365], // 2025
            [31, 32, 31, 32, 31, 30, 30, 30, 29, 29, 30, 31, 366], // 2026
            [30, 32, 31, 32, 31, 30, 30, 30, 29, 30, 29, 31, 365], // 2027
            [31, 31, 32, 31, 31, 31, 30, 29, 30, 29, 30, 30, 365], // 2028
            [31, 31, 32, 31, 32, 30, 30, 29, 30, 29, 30, 30, 365], // 2029
            [31, 32, 31, 32, 31, 30, 30, 30, 29, 29, 30, 31, 366], // 2030
            [30, 32, 31, 32, 31, 30, 30, 30, 29, 30, 29, 31, 365], // 2031
            [31, 31, 32, 31, 31, 31, 30, 29, 30, 29, 30, 30, 365], // 2032
            [31, 31, 32, 32, 31, 30, 30, 29, 30, 29, 30, 30, 365], // 2033
            [31, 32, 31, 32, 31, 30, 30, 30, 29, 29, 30, 31, 366], // 2034
            [30, 32, 31, 32, 31, 31, 29, 30, 30, 29, 29, 31, 365], // 2035
            [31, 31, 32, 31, 31, 31, 30, 29, 30, 29, 30, 30, 365], // 2036
            [31, 31, 32, 32, 31, 30, 30, 29, 30, 29, 30, 30, 365], // 2037
            [31, 32, 31, 32, 31, 30, 30, 30, 29, 29, 30, 31, 366], // 2038
            [31, 31, 31, 32, 31, 31, 29, 30, 30, 29, 30, 30, 365], // 2039
            [31, 31, 32, 31, 31, 31, 30, 29, 30, 29, 30, 30, 365], // 2040
            [31, 31, 32, 32, 31, 30, 30, 29, 30, 29, 30, 30, 365], // 2041
            [31, 32, 31, 32, 31, 30, 30, 30, 29, 29, 30, 31, 366], // 2042
            [31, 31, 31, 32, 31, 31, 29, 30, 30, 29, 30, 30, 365], // 2043
            [31, 31, 32, 31, 31, 31, 30, 29, 30, 29, 30, 30, 365], // 2044
            [31, 32, 31, 32, 31, 30, 30, 29, 30, 29, 30, 30, 365], // 2045
            [31, 32, 31, 32, 31, 30, 30, 30, 29, 29, 30, 31, 366], // 2046
            [31, 31, 31, 32, 31, 31, 30, 29, 30, 29, 30, 30, 365], // 2047
            [31, 31, 32, 31, 31, 31, 30, 29, 30, 29, 30, 30, 365], // 2048
            [31, 32, 31, 32, 31, 30, 30, 30, 29, 29, 30, 30, 365], // 2049
            [31, 32, 31, 32, 31, 30, 30, 30, 29, 30, 29, 31, 366], // 2050
            [31, 31, 31, 32, 31, 31, 30, 29, 30, 29, 30, 30, 365], // 2051
            [31, 31, 32, 31, 31, 31, 30, 29, 30, 29, 30, 30, 365], // 2052
            [31, 32, 31, 32, 31, 30, 30, 30, 29, 29, 30, 30, 365], // 2053
            [31, 32, 31, 32, 31, 30, 30, 30, 29, 30, 29, 31, 366], // 2054
            [31, 31, 32, 31, 31, 31, 30, 29, 30, 29, 30, 30, 365], // 2055
            [31, 31, 32, 31, 32, 30, 30, 29, 30, 29, 30, 30, 365], // 2056
            [31, 32, 31, 32, 31, 30, 30, 30, 29, 29, 30, 31, 366], // 2057
            [30, 32, 31, 32, 31, 30, 30, 30, 29, 30, 29, 31, 365], // 2058
            [31, 31, 32, 31, 31, 31, 30, 29, 30, 29, 30, 30, 365], // 2059
            [31, 32, 31, 32, 31, 30, 30, 30, 29, 29, 30, 31, 366], // 2060
            [30, 32, 31, 32, 31, 31, 29, 30, 29, 30, 29, 31, 365], // 2061
            [31, 31, 32, 31, 31, 31, 30, 29, 30, 29, 30, 30, 365], // 2062
            [31, 31, 32, 32, 31, 30, 30, 29, 30, 29, 30, 30, 365], // 2063
            [31, 32, 31, 32, 31, 30, 30, 30, 29, 29, 30, 31, 366], // 2064
            [31, 31, 31, 32, 31, 31, 29, 30, 30, 29, 29, 31, 365], // 2065
            [31, 31, 32, 31, 31, 31, 30, 29, 30, 29, 30, 30, 365], // 2066
            [31, 31, 32, 32, 31, 30, 30, 29, 30, 29, 30, 30, 365], // 2067
            [31, 32, 31, 32, 31, 30, 30, 30, 29, 29, 30, 31, 366], // 2068
            [31, 31, 31, 32, 31, 31, 29, 30, 30, 29, 30, 30, 365], // 2069
            [31, 31, 32, 31, 31, 31, 30, 29, 30, 29, 30, 30, 365], // 2070
            [31, 32, 31, 32, 31, 30, 30, 29, 30, 29, 30, 30, 365], // 2071
            [31, 32, 31, 32, 31, 30, 30, 30, 29, 29, 30, 31, 366], // 2072
            [31, 31, 31, 32, 31, 31, 30, 29, 30, 29, 30, 30, 365], // 2073
            [31, 31, 32, 31, 31, 31, 30, 29, 30, 29, 30, 30, 365], // 2074
            [31, 32, 31, 32, 31, 30, 30, 29, 30, 29, 30, 30, 365], // 2075
            [31, 32, 31, 32, 31, 30, 30, 30, 29, 30, 29, 31, 366], // 2076
            [31, 31, 31, 32, 31, 31, 30, 29, 30, 29, 30, 30, 365], // 2077
            [31, 31, 32, 31, 31, 31, 30, 29, 30, 29, 30, 30, 365], // 2078
            [31, 32, 31, 32, 31, 30, 30, 30, 29, 29, 30, 30, 365], // 2079
            [31, 32, 31, 32, 31, 30, 30, 30, 29, 30, 29, 31, 366], // 2080
            [31, 31, 32, 31, 31, 31, 30, 29, 30, 29, 30, 30, 365], // 2081
            [31, 31, 32, 31, 31, 31, 30, 29, 30, 29, 30, 30, 365], // 2082
            [31, 32, 31, 32, 31, 30, 30, 30, 29, 29, 30, 31, 366], // 2083
            [30, 32, 31, 32, 31, 30, 30, 30, 29, 30, 29, 31, 365], // 2084
            [31, 31, 32, 31, 31, 31, 30, 29, 30, 29, 30, 30, 365], // 2085
            [31, 31, 32, 32, 31, 30, 30, 29, 30, 29, 30, 30, 365], // 2086
            [31, 32, 31, 32, 31, 30, 30, 30, 29, 29, 30, 31, 366], // 2087
            [30, 32, 31, 32, 31, 31, 29, 30, 29, 30, 29, 31, 365], // 2088
            [31, 31, 32, 31, 31, 31, 30, 29, 30, 29, 30, 30, 365], // 2089
        ];
        this.NP_DATA_YEAR_COUNT = this.NP_MONTHS_DATA.length;
    }

    getJulianDate(year, month, day) {
        if (month <= 2) {
            year -= 1;
            month += 12;
        }
        const a = Math.floor(year / 100);
        const b = 2 - a + Math.floor(a / 4);
        return Math.floor(365.25 * (year + 4716)) + Math.floor(30.6001 * (month + 1)) + day + b - 1524.5;
    }

    fromJulianDate(julian_date) {
        const a = Math.floor(julian_date + 0.5);
        const b = a + 1537;
        const c = Math.floor((b - 122.1) / 365.25);
        const d = Math.floor(365.25 * c);
        const e = Math.floor((b - d) / 30.6001);
        const f = b - d - Math.floor(30.6001 * e) + (julian_date + 0.5 - a);
        const day = Math.floor(f);
        const month = e < 14 ? e - 1 : e - 13;
        const year = month > 2 ? c - 4716 : c - 4715;return { year, month, day };
    }

    getSauraMasaDay(ahar) {
        let month, day;
        if (this.todaySauraMasaFirstP(ahar)) {
            day = 1;
            const tslong_tomorrow = this.getTslong(ahar + 1);
            month = (Math.floor(tslong_tomorrow / 30) % 12 + 12) % 12;
        } else {
            const previous = this.getSauraMasaDay(ahar - 1);
            day = previous.day + 1;
            month = previous.month;
        }
        return { month, day };
    }

    todaySauraMasaFirstP(ahar) {
        const tslong_today = this.getTslong(ahar);
        const tslong_tomorrow = this.getTslong(ahar + 1);
        const today_mod = tslong_today - Math.floor(tslong_today / 30) * 30;
        const tomorrow_mod = tslong_tomorrow - Math.floor(tslong_tomorrow / 30) * 30;
        return (25 < today_mod && tomorrow_mod < 5) ? 1 : 0;
    }

    getTslong(ahar) {
        const t1 = (this.YugaRotation_sun * ahar / this.YugaCivilDays) % 1;
        const mslong = 360 * t1;
        const x1 = mslong - this.PlanetApogee_sun;
        const y1 = this.PlanetCircumm_sun / 360;
        const y2 = Math.sin(x1 / this.rad);
        const y3 = y1 * y2;
        const x2 = Math.asin(y3) * this.rad;
        return mslong - x2;
    }

    daysInMonth(bsYear, bsMonth) {
        if (bsYear >= this.BS_START_YEAR && bsYear < this.BS_START_YEAR + this.NP_DATA_YEAR_COUNT) {
            return this.NP_MONTHS_DATA[bsYear - this.BS_START_YEAR][bsMonth - 1];
        }
        // Fallback
        const { year: gYear, month: gMonth, day: gDay } = this.toGregorian(bsYear, bsMonth, 1);
        const julian_date_start = this.getJulianDate(gYear, gMonth, gDay);
        const nextMonth = (bsMonth % 12) + 1;
        const nextYear = (bsMonth === 12) ? bsYear + 1 : bsYear;
        const { year: gYearNext, month: gMonthNext, day: gDayNext } = this.toGregorian(nextYear, nextMonth, 1);
        const julian_date_end = this.getJulianDate(gYearNext, gMonthNext, gDayNext);
        return Math.floor(julian_date_end - julian_date_start);
    }

    fromGregorian(y, m, d) {
        const julian = this.getJulianDate(y, m, d);
        const ref_julian = this.getJulianDate(1943, 4, 14); // 1 Baisakh 2000 BS = 14 April 1943
        let diff = Math.floor(julian - ref_julian);
        let bs_year = this.BS_START_YEAR;

        if (diff >= 0) {
            while (bs_year <= this.BS_START_YEAR + this.NP_DATA_YEAR_COUNT - 1) {
                for (let i = 0; i < 12; ++i) {
                    if (diff < this.NP_MONTHS_DATA[bs_year - this.BS_START_YEAR][i]) {
                        this.Year = bs_year;
                        this.Month = i + 1;
                        this.Day = diff + 1;
                        return;
                    } else {
                        diff -= this.NP_MONTHS_DATA[bs_year - this.BS_START_YEAR][i];
                    }
                }
                bs_year++;
            }
        }
        // Fallback
        const ahar = julian - 588465.5;
        const { month: saura_masa_num, day: saura_masa_day } = this.getSauraMasaDay(ahar);
        const YearKali = Math.floor(ahar * this.YugaRotation_sun / this.YugaCivilDays);
        const YearSaka = YearKali - 3179;
        const nepalimonth = saura_masa_num % 12;
        this.Year = YearSaka + 135 + Math.floor((saura_masa_num - nepalimonth) / 12);
        this.Month = (saura_masa_num + 12) % 12 + 1;
        this.Day = saura_masa_day;
    }

    toGregorian(bsYear, bsMonth, bsDay) {
        if (bsYear >= this.BS_START_YEAR && bsYear < this.BS_START_YEAR + this.NP_DATA_YEAR_COUNT) {
            const ref_julian = this.getJulianDate(1943, 4, 14); // 1 Baisakh 2000 BS
            let total_days = 0;
            for (let year = this.BS_START_YEAR; year < bsYear; ++year) {
                total_days += this.NP_MONTHS_DATA[year - this.BS_START_YEAR][12];
            }
            for (let month = 0; month < bsMonth - 1; ++month) {
                total_days += this.NP_MONTHS_DATA[bsYear - this.BS_START_YEAR][month];
            }
            total_days += (bsDay - 1);
            const target_julian = ref_julian + total_days;
            const { year, month, day } = this.fromJulianDate(target_julian);
            return { year, month, day };
        }
        // Fallback
        const YearSaka = bsYear - 135;
        const YearKali = YearSaka + 3179;
        let ahar = Math.floor((YearKali * this.YugaCivilDays) / this.YugaRotation_sun);
        let { month: saura_masa_num, day: saura_masa_day } = this.getSauraMasaDay(ahar);
        bsMonth = (bsMonth + 11) % 12;

        while (saura_masa_num !== bsMonth || saura_masa_day !== bsDay) {
            ahar += (saura_masa_num < bsMonth || (saura_masa_num === bsMonth && saura_masa_day < bsDay)) ? 1 : -1;
            ({ month: saura_masa_num, day: saura_masa_day } = this.getSauraMasaDay(ahar));
        }

        const julian_date = ahar + 588465.5;
        return this.fromJulianDate(julian_date);
    }
    fromNepali(bsYear, bsMonth, bsDay) {
        const { year, month, day } = this.toGregorian(bsYear, bsMonth, bsDay);
        this.Year = year;
        this.Month = month - 1; // Adjust for 0-based month
        this.Day = day;
    }

    getYear() {
        return this.Year;
    }

    getMonth() {
        return this.Month + 1; // Return 1-based month
    }

    getDay() {
        return this.Day;
    }

    getWeekdayName(year, month, day) {
        const days = ["Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"];
        const nepaliDays = ["आइतबार", "सोमबार", "मंगलबार", "बुधबार", "बिहीबार", "शुक्रबार", "शनिबार"];
        const timeinfo = new Date(year, month - 1, day);
        const weekdayIndex = timeinfo.getDay();
        return document.getElementById("language").value === "nepali" ? nepaliDays[weekdayIndex] : days[weekdayIndex];
    }

    getMonthName(month) {
        const englishMonths = ["January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"];
        const gregorianMonthsInNepali = ["जनवरी", "फेब्रुअरी", "मार्च", "अप्रिल", "मे", "जुन", "जुलाई", "अगस्ट", "सेप्टेम्बर", "अक्टोबर", "नोभेम्बर", "डिसेम्बर"];
        const romanizedNepaliMonths = ["Baishakh", "Jestha", "Ashadh", "Shravan", "Bhadra", "Ashwin", "Kartik", "Mangsir", "Poush", "Magh", "Falgun", "Chaitra"];
        const nepaliMonths = ["बैसाख", "जेष्ठ", "आषाढ", "श्रावण", "भाद्र", "आश्विन", "कार्तिक", "मंसिर", "पौष", "माघ", "फागुन", "चैत"];

        const language = document.getElementById("language").value;

        if (language === "nepali") {
            // Use the Nepali month index for Bikram Sambat
            return nepaliMonths[month - 1]; // month is 1-based
        } else {
            // Use the Romanized month names for English
            return romanizedNepaliMonths[month - 1]; // month is 1-based
        }
    }
}
