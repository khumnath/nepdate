
    // JavaScript class and functions for date conversion
    // Based on bikram.h from github.com/khumnath/nepdate
    // Author khumnath cg <mail@khumnath.com.np>
    // Licence gpl v3 or higher 
    class Bikram {
      constructor() {
        this.Year = 0;
        this.Month = 0;
        this.Day = 0;

        this.YugaRotation_star = 1582237828;
        this.YugaRotation_sun = 4320000;
        this.YugaCivilDays = this.YugaRotation_star - this.YugaRotation_sun;
        this.PlanetApogee_sun = 77 + 17 / 60;
        this.PlanetCircumm_sun = 13 + 50 / 60;
        this.rad = 57.2957795; // = 180/pi
      }

      getSauraMasaDay(ahar) {
        let tslong_tomorrow;
        let month;
        let day;
        if (this.todaySauraMasaFirstP(ahar)) {
          day = 1;
          tslong_tomorrow = this.getTslong(ahar + 1);
          month = Math.floor(tslong_tomorrow / 30) % 12;
          month = (month + 12) % 12;
        } else {
          [month, day] = this.getSauraMasaDay(ahar - 1);
          day = day + 1;
        }
        return [month, day];
      }

      todaySauraMasaFirstP(ahar) {
        let tslong_today = this.getTslong(ahar);
        let tslong_tomorrow = this.getTslong(ahar + 1);
        tslong_today = tslong_today - Math.floor(tslong_today / 30) * 30;
        tslong_tomorrow = tslong_tomorrow - Math.floor(tslong_tomorrow / 30) * 30;
        return (25 < tslong_today) && (tslong_tomorrow < 5);
      }

      getTslong(ahar) {
        let mslong;
        let t1 = (this.YugaRotation_sun * ahar / this.YugaCivilDays) % 1;
        mslong = 360 * t1;
        let x1 = mslong - this.PlanetApogee_sun;
        let y1 = this.PlanetCircumm_sun / 360;
        let y2 = Math.sin(x1 / this.rad);
        let y3 = y1 * y2;
        let x2 = Math.asin(y3) * this.rad;
        let x3 = mslong - x2;
        return x3;
      }

      getJulianDate(year, month, day) {
        if (month <= 2) {
          year -= 1;
          month += 12;
        }
        let a = Math.floor(year / 100.0);
        let b = 2 - a + Math.floor(a / 4.0);
        return Math.floor(365.25 * (year + 4716)) + Math.floor(30.6001 * (month + 1)) + day + b - 1524.5;
      }

      fromJulianDate(julian_date) {
        let a = Math.floor(julian_date + 0.5);
        let b = a + 1537;
        let c = Math.floor((b - 122.1) / 365.25);
        let d = Math.floor(365.25 * c);
        let e = Math.floor((b - d) / 30.6001);
        let f = b - d - Math.floor(30.6001 * e) + (julian_date + 0.5 - a);
        let day = Math.floor(f);
        let month;
        let year;
        if (e < 14) {
          month = e - 1;
        } else {
          month = e - 13;
        }
        if (month > 2) {
          year = c - 4716;
        } else {
          year = c - 4715;
        }
        return { year, month, day };
      }

      // Public method to convert Gregorian to Bikram Sambat
      fromGregorian(y, m, d) {
        let julian = this.getJulianDate(y, m, d);
        let ahar = julian - 588465.5;
        let [saura_masa_num, saura_masa_day] = this.getSauraMasaDay(ahar);
        let YearKali = Math.floor((ahar * this.YugaRotation_sun) / this.YugaCivilDays);
        let YearSaka = YearKali - 3179;
        let nepalimonth = saura_masa_num % 12;
        this.Year = YearSaka + 135 + Math.floor((saura_masa_num - nepalimonth) / 12);
        this.Month = (saura_masa_num + 1) % 12;
        this.Day = saura_masa_day;
      }

      // Public method to convert Bikram Sambat to Gregorian
      toGregorian(bsYear, bsMonth, bsDay) {
        let YearSaka = bsYear - 135;
        let YearKali = YearSaka + 3179;
        let ahar = Math.floor((YearKali * this.YugaCivilDays) / this.YugaRotation_sun);
        let [saura_masa_num, saura_masa_day] = this.getSauraMasaDay(ahar);
        bsMonth = (bsMonth + 11) % 12; // Adjust the month to the correct range
        while (saura_masa_num !== bsMonth || saura_masa_day !== bsDay) {
          if (saura_masa_num < bsMonth || (saura_masa_num === bsMonth && saura_masa_day < bsDay)) {
            ahar++;
          } else {
            ahar--;
          }
          [saura_masa_num, saura_masa_day] = this.getSauraMasaDay(ahar);
        }
        let julian_date = ahar + 588465.5;
        return this.fromJulianDate(julian_date);
      }

      fromNepali(bsYear, bsMonth, bsDay) {
        let { year, month, day } = this.toGregorian(bsYear, bsMonth, bsDay);
        this.Year = year;
        this.Month = month;
        this.Day = day;
      }

      getDayOfWeek() {
        let date = new Date(this.Year, this.Month - 1, this.Day);
        return date.getDay();
      }

      getYear() {
        return this.Year;
      }

      getMonth() {
  return (this.Month + 12) % 12 || 12;
}


      getDay() {
        return this.Day;
      }
getNepaliWeekdayName() {
    const weekdayNepali = ["आइतबार", "सोमबार", "मङ्गलबार", "बुधबार", "बिहिबार", "शुक्रबार", "शनिबार"];
    let weekdayIndex = this.getDayOfWeek(); // Using existing getDayOfWeek method
    return weekdayNepali[weekdayIndex];
  }

      getWeekdayName(year, month, day) {
        let date = new Date(year, month - 1, day);
        const weekday = ["Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"];
        return weekday[date.getDay()];
      }
getBikramMonthName() {
  const bsMonths = [
    { value: 1, name: 'बैशाख' }, { value: 2, name: 'जेठ' }, { value: 3, name: 'असार' },
    { value: 4, name: 'साउन' }, { value: 5, name: 'भदौ' }, { value: 6, name: 'असोज' },
    { value: 7, name: 'कार्तिक' }, { value: 8, name: 'मंसिर' }, { value: 9, name: 'पुष' },
    { value: 10, name: 'माघ' }, { value: 11, name: 'फाल्गुन' }, { value: 12, name: 'चैत' }
  ];
  return bsMonths[this.getMonth() - 1].name;
}

getGregorianMonthName() {
    const gregorianMonths = [
      { value: 1, name: 'January' }, { value: 2, name: 'February' }, { value: 3, name: 'March' },
      { value: 4, name: 'April' }, { value: 5, name: 'May' }, { value: 6, name: 'June' },
      { value: 7, name: 'July' }, { value: 8, name: 'August' }, { value: 9, name: 'September' },
      { value: 10, name: 'October' }, { value: 11, name: 'November' }, { value: 12, name: 'December' }
    ];
    return gregorianMonths[this.getMonth() - 1].name;
  }

  // Method to get Nepali day name for Gregorian date
  getGregorianDayName(year, month, day) {
    let date = new Date(year, month - 1, day);
    const weekdayNepali = ["sunday", "monday", "tuesday", "wednesday", "thursday", "friday", "saturday"];
    return weekdayNepali[date.getDay()];
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

      daysInGregorianMonth(gYear, gMonth) {
        const daysInMonth = [31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31];
        if (gMonth === 2 && ((gYear % 4 === 0 && gYear % 100 !== 0) || (gYear % 400 === 0))) {
          return 29;
        }
        return daysInMonth[gMonth - 1];
      }
    }

    // Initialize Bikram calendar object
    const bikram = new Bikram();

    // Function to update Gregorian date to Bikram Sambat
function updateBikramDate() {
  let gYear = parseInt(document.getElementById('gYear').value);
  let gMonth = parseInt(document.getElementById('gMonth').value);
  let gDay = parseInt(document.getElementById('gDay').value);

  bikram.fromGregorian(gYear, gMonth, gDay);

  document.getElementById('bikramYear').textContent = convertToNepaliDigits(bikram.getYear());
  document.getElementById('bikramMonth').textContent = bikram.getBikramMonthName();
  document.getElementById('bikramDay').textContent = convertToNepaliDigits(bikram.getDay());
  document.getElementById('bikramWeekday').textContent = bikram.getNepaliWeekdayName();
}

// Function to update Bikram Sambat date to Gregorian
function updateGregorianDate() {
  let bsYear = parseInt(document.getElementById('bsYear').value);
  let bsMonth = parseInt(document.getElementById('bsMonth').value);
  let bsDay = parseInt(document.getElementById('bsDay').value);

  bikram.fromNepali(bsYear, bsMonth, bsDay);

  document.getElementById('gYearResult').textContent = bikram.getYear();
  document.getElementById('gMonthResult').textContent = bikram.getGregorianMonthName();
  document.getElementById('gDayResult').textContent = bikram.getDay();
  document.getElementById('gregorianWeekday').textContent = bikram.getGregorianDayName(bikram.getYear(), bikram.getMonth(), bikram.getDay());
}


    // Populate dropdowns with years, months, and days
    document.addEventListener('DOMContentLoaded', function () {
      // For Gregorian to Bikram Sambat
      let gYearSelect = document.getElementById('gYear');
      let gMonthSelect = document.getElementById('gMonth');
      let gDaySelect = document.getElementById('gDay');

      // For Bikram Sambat to Gregorian
      let bsYearSelect = document.getElementById('bsYear');
      let bsMonthSelect = document.getElementById('bsMonth');
      let bsDaySelect = document.getElementById('bsDay');

      // Fill years (Gregorian)
      for (let year = 1800; year <= new Date().getFullYear() + 200; year++) {
        let gOption = document.createElement('option');
        gOption.value = year;
        gOption.textContent = year;
        gYearSelect.appendChild(gOption);

        let bsOption = document.createElement('option');
        bsOption.value = year + 56; // Adjusting BS Year
        bsOption.textContent = convertToNepaliDigits(year + 56);
        bsYearSelect.appendChild(bsOption);
      }

      // Fill months (Gregorian)
      const months = [
        { value: 1, name: 'January' }, { value: 2, name: 'February' }, { value: 3, name: 'March' },
        { value: 4, name: 'April' }, { value: 5, name: 'May' }, { value: 6, name: 'June' },
        { value: 7, name: 'July' }, { value: 8, name: 'August' }, { value: 9, name: 'September' },
        { value: 10, name: 'October' }, { value: 11, name: 'November' }, { value: 12, name: 'December' }
      ];

      for (let month of months) {
        let gOption = document.createElement('option');
        gOption.value = month.value;
        gOption.textContent = month.name;
        gMonthSelect.appendChild(gOption);
      }

      // Fill days (Gregorian and Bikram Sambat)
      for (let day = 1; day <= 31; day++) {
        let gOption = document.createElement('option');
        gOption.value = day;
        gOption.textContent = day;
        gDaySelect.appendChild(gOption);

        let bsOption = document.createElement('option');
        bsOption.value = day;
        bsOption.textContent = convertToNepaliDigits(day);
        bsDaySelect.appendChild(bsOption);
      }

      // Fill months (Bikram Sambat)
      const bsMonths = [
  { value: 1, name: 'बैशाख' }, { value: 2, name: 'जेठ' }, { value: 3, name: 'असार' },
  { value: 4, name: 'साउन' }, { value: 5, name: 'भदौ' }, { value: 6, name: 'असोज' },
  { value: 7, name: 'कार्तिक' }, { value: 8, name: 'मंसिर' }, { value: 9, name: 'पुष' },
  { value: 10, name: 'माघ' }, { value: 11, name: 'फाल्गुन' }, { value: 12, name: 'चैत' }
];


      for (let month of bsMonths) {
        let bsOption = document.createElement('option');
        bsOption.value = month.value;
        bsOption.textContent = month.name;
        bsMonthSelect.appendChild(bsOption);
      }


      // Set initial date to today
      const today = new Date();
      document.getElementById('gYear').value = today.getFullYear();
      document.getElementById('gMonth').value = today.getMonth() + 1;
      document.getElementById('gDay').value = today.getDate();
      updateBikramDate();

      // Set initial BS date to today
      bikram.fromGregorian(today.getFullYear(), today.getMonth() + 1, today.getDate());
      document.getElementById('bsYear').value = bikram.getYear();
      document.getElementById('bsMonth').value = bikram.getMonth();
      document.getElementById('bsDay').value = bikram.getDay();
      updateGregorianDate();
    });