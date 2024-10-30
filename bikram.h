#ifndef BIKRAM_H
#define BIKRAM_H

#include <cmath>

class bikram {
private:
    int Year = 0;
    int Month = -1;
    int Day = 0;

    double YugaRotation_star = 1582237828;
    double YugaRotation_sun = 4320000;
    double YugaCivilDays;
    double PlanetApogee_sun = 77 + static_cast<float>(17) / 60;
    double PlanetCircumm_sun = 13 + static_cast<float>(50) / 60;
    static constexpr double rad = 57.2957795; // 180 / pi

    void get_saura_masa_day(long ahar, int* m, int* d) const;
    int today_saura_masa_first_p(long ahar) const;
    double get_tslong(long ahar) const;
    double get_julian_date(int year, int month, int day) const;
    void fromJulianDate(double julian_date, int& year, int& month, int& day) const;

public:
    void fromGregorian(int y, int m, int d);
    void toGregorian(int bsYear, int bsMonth, int bsDay, int& gYear, int& gMonth, int& gDay);
    int getYear() const;
    int getMonth() const;
    int getDay() const;
    int daysInMonth(int year, int month);
};

inline void bikram::get_saura_masa_day(long ahar, int* m, int* d) const {
    double tslong_tomorrow;
    int month;
    int day;
    if (today_saura_masa_first_p(ahar)) {
        day = 1;
        tslong_tomorrow = get_tslong(ahar + 1);
        month = (long)(tslong_tomorrow / 30) % 12;
        month = (month + 12) % 12;
    } else {
        get_saura_masa_day(ahar - 1, &month, &day);
        day += 1;
    }
    *m = month;
    *d = day;
}

inline int bikram::today_saura_masa_first_p(long ahar) const {
    double tslong_today = get_tslong(ahar);
    double tslong_tomorrow = get_tslong(ahar + 1);
    tslong_today = tslong_today - (int)(tslong_today / 30) * 30;
    tslong_tomorrow = tslong_tomorrow - (int)(tslong_tomorrow / 30) * 30;
    return (25 < tslong_today) && (tslong_tomorrow < 5) ? 1 : 0;
}

inline double bikram::get_tslong(long ahar) const {
    double mslong;
    double t1 = (YugaRotation_sun * ahar / YugaCivilDays);
    t1 -= static_cast<long>(t1);
    mslong = 360 * t1;
    double x1 = mslong - PlanetApogee_sun;
    double y1 = PlanetCircumm_sun / 360;
    double y2 = sin(x1 / rad);
    double y3 = y1 * y2;
    double x2 = asin(y3) * rad;
    double x3 = mslong - x2;
    return x3;
}

inline double bikram::get_julian_date(int year, int month, int day) const {
    if (month <= 2) {
        year -= 1;
        month += 12;
    }
    double a = floor(year / 100.0);
    double b = 2 - a + floor(a / 4.0);
    return floor(365.25 * (year + 4716)) + floor(30.6001 * (month + 1)) + day + b - 1524.5;
}

inline void bikram::fromJulianDate(double julian_date, int& year, int& month, int& day) const {
    int a = static_cast<int>(julian_date + 0.5);
    int b = a + 1537;
    int c = static_cast<int>((b - 122.1) / 365.25);
    int d = static_cast<int>(365.25 * c);
    int e = static_cast<int>((b - d) / 30.6001);
    double f = b - d - static_cast<int>(30.6001 * e) + (julian_date + 0.5 - a);
    day = static_cast<int>(f);
    month = (e < 14) ? (e - 1) : (e - 13);
    year = (month > 2) ? (c - 4716) : (c - 4715);
}

inline void bikram::fromGregorian(int y, int m, int d) {
    YugaCivilDays = YugaRotation_star - YugaRotation_sun;
    double julian = get_julian_date(y, m, d);
    long ahar = julian - 588465.5;
    int saura_masa_num;
    int saura_masa_day;
    get_saura_masa_day(ahar, &saura_masa_num, &saura_masa_day);
    long YearKali = static_cast<long>(ahar * YugaRotation_sun / YugaCivilDays);
    int YearSaka = YearKali - 3179;
    int nepalimonth = (saura_masa_num) % 12;
    Year = YearSaka + 135 + static_cast<int>((saura_masa_num - nepalimonth) / 12);
    Month = (saura_masa_num + 12) % 12;
    Day = saura_masa_day;
}

inline void bikram::toGregorian(int bsYear, int bsMonth, int bsDay, int& gYear, int& gMonth, int& gDay) {
    YugaCivilDays = YugaRotation_star - YugaRotation_sun;
    int YearSaka = bsYear - 135;
    long YearKali = YearSaka + 3179;
    long ahar = static_cast<long>((YearKali * YugaCivilDays) / YugaRotation_sun);
    int saura_masa_num, saura_masa_day;
    get_saura_masa_day(ahar, &saura_masa_num, &saura_masa_day);
    bsMonth = (bsMonth + 11) % 12;
    while (saura_masa_num != bsMonth || saura_masa_day != bsDay) {
        ahar += (saura_masa_num < bsMonth || (saura_masa_num == bsMonth && saura_masa_day < bsDay)) ? 1 : -1;
        get_saura_masa_day(ahar, &saura_masa_num, &saura_masa_day);
    }
    double julian_date = ahar + 588465.5;
    fromJulianDate(julian_date, gYear, gMonth, gDay);
}

inline int bikram::getYear() const {
    return Year;
}

inline int bikram::getMonth() const {
    return Month + 1; // Return month as 1-indexed
}

inline int bikram::getDay() const {
    return Day;
}

inline int bikram::daysInMonth(int bsYear, int bsMonth)  {
    int gYear, gMonth, gDay;
    int nextMonth = (bsMonth % 12) + 1;
    int nextYear = (bsMonth == 12) ? bsYear + 1 : bsYear;
    toGregorian(bsYear, bsMonth, 1, gYear, gMonth, gDay);
    double julian_date_start = get_julian_date(gYear, gMonth, gDay);
    toGregorian(nextYear, nextMonth, 1, gYear, gMonth, gDay);
    double julian_date_end = get_julian_date(gYear, gMonth, gDay);
    return static_cast<int>(julian_date_end - julian_date_start);
}

#endif // BIKRAM_H
