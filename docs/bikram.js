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
                const year = month > 2 ? c - 4716 : c - 4715;
                return { year, month, day };
            }

            getSauraMasaDay(ahar) {
                let month , day;
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
        let { year: gYear, month: gMonth, day: gDay } = this.toGregorian(bsYear, bsMonth, 1);
        let julian_date_start = this.getJulianDate(gYear, gMonth, gDay);
        let nextMonth = (bsMonth % 12) + 1;
        let nextYear = (bsMonth === 12) ? bsYear + 1 : bsYear;
        let { year: gYearNext, month: gMonthNext, day: gDayNext } = this.toGregorian(nextYear, nextMonth, 1);
        let julian_date_end = this.getJulianDate(gYearNext, gMonthNext, gDayNext);
        return Math.floor(julian_date_end - julian_date_start);
      }

            fromGregorian(y, m, d) {
                const julian = this.getJulianDate(y, m, d);
                const ahar = julian - 588465.5;
                const { month: saura_masa_num, day: saura_masa_day } = this.getSauraMasaDay(ahar);
                const YearKali = Math.floor(ahar * this.YugaRotation_sun / this.YugaCivilDays);
                const YearSaka = YearKali - 3179;
                const nepalimonth = saura_masa_num % 12;
                this.Year = YearSaka + 135 + Math.floor((saura_masa_num - nepalimonth) / 12);
                this.Month = (saura_masa_num + 12) % 12;
                this.Day = saura_masa_day;
            }

            toGregorian(bsYear, bsMonth, bsDay) {
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
