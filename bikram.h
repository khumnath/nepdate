#ifndef BIKRAM_H
#define BIKRAM_H

#include "panchanga.h"

#include <cmath>

class Bikram {
private:
    int Year = 0;
    int Month = -1;
    int Day = 0;

    double YugaRotation_star = 1582237828;
    double YugaRotation_sun = 4320000;
    double YugaCivilDays = YugaRotation_star - YugaRotation_sun;
    double PlanetApogee_sun = 77 + 17.0 / 60;
    double PlanetCircumm_sun = 13 + 50.0 / 60;
    static constexpr double rad = 57.2957795; // 180 / pi

    // Precomputed Data (starting from 2000 BS)
    static const int BS_START_YEAR = 2000;
    static const int BS_END_YEAR = 2089; // Need adjustment depending how much data we have

    inline static const int NP_MONTHS_DATA[][13] = {
        {30,32,31,32,31,30,30,30,29,30,29,31,365}, // 2000
        {31,31,32,31,31,31,30,29,30,29,30,30,365}, // 2001
        {31,31,32,32,31,30,30,29,30,29,30,30,365}, // 2002
        {31,32,31,32,31,30,30,30,29,29,30,31,366}, // 2003
        {30,32,31,32,31,30,30,30,29,30,29,31,365}, // 2004
        {31,31,32,31,31,31,30,29,30,29,30,30,365}, // 2005
        {31,31,32,32,31,30,30,29,30,29,30,30,365}, // 2006
        {31,32,31,32,31,30,30,30,29,29,30,31,366}, // 2007
        {31,31,31,32,31,31,29,30,30,29,29,31,365}, // 2008
        {31,31,32,31,31,31,30,29,30,29,30,30,365}, // 2009
        {31,31,32,32,31,30,30,29,30,29,30,30,365}, // 2010
        {31,32,31,32,31,30,30,30,29,29,30,31,366}, // 2011
        {31,31,31,32,31,31,29,30,30,29,30,30,365}, // 2012
        {31,31,32,31,31,31,30,29,30,29,30,30,365}, // 2013
        {31,31,32,32,31,30,30,29,30,29,30,30,365}, // 2014
        {31,32,31,32,31,30,30,30,29,29,30,31,366}, // 2015
        {31,31,31,32,31,31,29,30,30,29,30,30,365}, // 2016
        {31,31,32,31,31,31,30,29,30,29,30,30,365}, // 2017
        {31,32,31,32,31,30,30,29,30,29,30,30,365}, // 2018
        {31,32,31,32,31,30,30,30,29,30,29,31,366}, // 2019
        {31,31,31,32,31,31,30,29,30,29,30,30,365}, // 2020
        {31,31,32,31,31,31,30,29,30,29,30,30,365}, // 2021
        {31,32,31,32,31,30,30,30,29,29,30,30,365}, // 2022
        {31,32,31,32,31,30,30,30,29,30,29,31,366}, // 2023
        {31,31,31,32,31,31,30,29,30,29,30,30,365}, // 2024
        {31,31,32,31,31,31,30,29,30,29,30,30,365}, // 2025
        {31,32,31,32,31,30,30,30,29,29,30,31,366}, // 2026
        {30,32,31,32,31,30,30,30,29,30,29,31,365}, // 2027
        {31,31,32,31,31,31,30,29,30,29,30,30,365}, // 2028
        {31,31,32,31,32,30,30,29,30,29,30,30,365}, // 2029
        {31,32,31,32,31,30,30,30,29,29,30,31,366}, // 2030
        {30,32,31,32,31,30,30,30,29,30,29,31,365}, // 2031
        {31,31,32,31,31,31,30,29,30,29,30,30,365}, // 2032
        {31,31,32,32,31,30,30,29,30,29,30,30,365}, // 2033
        {31,32,31,32,31,30,30,30,29,29,30,31,366}, // 2034
        {30,32,31,32,31,31,29,30,30,29,29,31,365}, // 2035
        {31,31,32,31,31,31,30,29,30,29,30,30,365}, // 2036
        {31,31,32,32,31,30,30,29,30,29,30,30,365}, // 2037
        {31,32,31,32,31,30,30,30,29,29,30,31,366}, // 2038
        {31,31,31,32,31,31,29,30,30,29,30,30,365}, // 2039
        {31,31,32,31,31,31,30,29,30,29,30,30,365}, // 2040
        {31,31,32,32,31,30,30,29,30,29,30,30,365}, // 2041
        {31,32,31,32,31,30,30,30,29,29,30,31,366}, // 2042
        {31,31,31,32,31,31,29,30,30,29,30,30,365}, // 2043
        {31,31,32,31,31,31,30,29,30,29,30,30,365}, // 2044
        {31,32,31,32,31,30,30,29,30,29,30,30,365}, // 2045
        {31,32,31,32,31,30,30,30,29,29,30,31,366}, // 2046
        {31,31,31,32,31,31,30,29,30,29,30,30,365}, // 2047
        {31,31,32,31,31,31,30,29,30,29,30,30,365}, // 2048
        {31,32,31,32,31,30,30,30,29,29,30,30,365}, // 2049
        {31,32,31,32,31,30,30,30,29,30,29,31,366}, // 2050
        {31,31,31,32,31,31,30,29,30,29,30,30,365}, // 2051
        {31,31,32,31,31,31,30,29,30,29,30,30,365}, // 2052
        {31,32,31,32,31,30,30,30,29,29,30,30,365}, // 2053
        {31,32,31,32,31,30,30,30,29,30,29,31,366}, // 2054
        {31,31,32,31,31,31,30,29,30,29,30,30,365}, // 2055
        {31,31,32,31,32,30,30,29,30,29,30,30,365}, // 2056
        {31,32,31,32,31,30,30,30,29,29,30,31,366}, // 2057
        {30,32,31,32,31,30,30,30,29,30,29,31,365}, // 2058
        {31,31,32,31,31,31,30,29,30,29,30,30,365}, // 2059
        {31,31,32,32,31,30,30,29,30,29,30,30,365}, // 2060
        {31,32,31,32,31,30,30,30,29,29,30,31,366}, // 2061
        {30,32,31,32,31,31,29,30,29,30,29,31,365}, // 2062
        {31,31,32,31,31,31,30,29,30,29,30,30,365}, // 2063
        {31,31,32,32,31,30,30,29,30,29,30,30,365}, // 2064
        {31,32,31,32,31,30,30,30,29,29,30,31,366}, // 2065
        {31,31,31,32,31,31,29,30,30,29,29,31,365}, // 2066
        {31,31,32,31,31,31,30,29,30,29,30,30,365}, // 2067
        {31,31,32,32,31,30,30,29,30,29,30,30,365}, // 2068
        {31,32,31,32,31,30,30,30,29,29,30,31,366}, // 2069
        {31,31,31,32,31,31,29,30,30,29,30,30,365}, // 2070
        {31,31,32,31,31,31,30,29,30,29,30,30,365}, // 2071
        {31,32,31,32,31,30,30,29,30,29,30,30,365}, // 2072
        {31,32,31,32,31,30,30,30,29,29,30,31,366}, // 2073
        {31,31,31,32,31,31,30,29,30,29,30,30,365}, // 2074
        {31,31,32,31,31,31,30,29,30,29,30,30,365}, // 2075
        {31,32,31,32,31,30,30,30,29,29,30,30,365}, // 2076
        {31,32,31,32,31,30,30,30,29,30,29,31,366}, // 2077
        {31,31,31,32,31,31,30,29,30,29,30,30,365}, // 2078
        {31,31,32,31,31,31,30,29,30,29,30,30,365}, // 2079
        {31,32,31,32,31,30,30,30,29,29,30,30,365}, // 2080
        {31,32,31,32,31,30,30,30,29,30,29,31,366}, // 2081
        {31,31,32,31,31,31,30,29,30,29,30,30,365}, // 2082
        {31,31,32,31,31,31,30,29,30,29,30,30,365}, // 2083
        {31,32,31,32,31,30,30,30,29,29,30,31,366}, // 2084
        {30,32,31,32,31,30,30,30,29,30,29,31,365}, // 2085
        {31,31,32,31,31,31,30,29,30,29,30,30,365}, // 2086
        {31,31,32,32,31,30,30,29,30,29,30,30,365}, // 2087
        {31,32,31,32,31,30,30,30,29,29,30,31,366}, // 2088
        {30,32,31,32,31,31,29,30,29,30,29,31,365}, // 2089
        {31,31,32,31,31,31,30,29,30,29,30,30,365}, // 2090
        {31,31,32,32,31,30,30,29,30,29,30,30,365}, // 2091
        {31,32,31,32,31,30,30,30,29,29,30,31,366}, // 2092
        {31,31,31,32,31,31,29,30,30,29,29,31,365}, // 2093
        {31,31,32,31,31,31,30,29,30,29,30,30,365}, // 2094
        {31,31,32,32,31,30,30,29,30,29,30,30,365}, // 2095
        {31,32,31,32,31,30,30,30,29,29,30,31,366}, // 2096
        {31,31,31,32,31,31,29,30,30,29,30,30,365}, // 2097
        {31,31,32,31,31,31,30,29,30,29,30,30,365}, // 2098
        {31,32,31,32,31,30,30,29,30,29,30,30,365}, // 2099
    };
    static const int NP_DATA_YEAR_COUNT = sizeof(NP_MONTHS_DATA) / sizeof(NP_MONTHS_DATA[0]);

    void getSauraMasaDay(long ahar, int* m, int* d) const;
    int todaySauraMasaFirstP(long ahar) const;
    double getTslong(long ahar) const;
    double getJulianDate(int year, int month, int day) const;
    void fromJulianDate(double julian_date, int& year, int& month, int& day) const;

    void fromGregorianAstronomical(int y, int m, int d);  // fallback
    void toGregorianAstronomical(int bsYear, int bsMonth, int bsDay, int& gYear, int& gMonth, int& gDay); // fallback

public:
    void fromGregorian(int y, int m, int d);
    void toGregorian(int bsYear, int bsMonth, int bsDay, int& gYear, int& gMonth, int& gDay);
    int getYear() const;
    int getMonth() const;
    int getDay() const;
    int daysInMonth(int bsYear, int bsMonth);
};

inline void Bikram::getSauraMasaDay(long ahar, int* m, int* d) const {
    double tslong_tomorrow;
    int month;
    int day;
    if (todaySauraMasaFirstP(ahar)) {
        day = 1;
        tslong_tomorrow = getTslong(ahar + 1);
        month = static_cast<int>(tslong_tomorrow / 30) % 12;
        month = (month + 12) % 12;
    } else {
        getSauraMasaDay(ahar - 1, &month, &day);
        day += 1;
    }
    *m = month;
    *d = day;
}

inline int Bikram::todaySauraMasaFirstP(long ahar) const {
    double tslong_today = getTslong(ahar);
    double tslong_tomorrow = getTslong(ahar + 1);
    tslong_today -= static_cast<int>(tslong_today / 30) * 30;
    tslong_tomorrow -= static_cast<int>(tslong_tomorrow / 30) * 30;
    return (25 < tslong_today && tslong_tomorrow < 5) ? 1 : 0;
}

inline double Bikram::getTslong(long ahar) const {
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

inline double Bikram::getJulianDate(int year, int month, int day) const {
    if (month <= 2) {
        year -= 1;
        month += 12;
    }
    double a = floor(year / 100.0);
    double b = 2 - a + floor(a / 4.0);
    return floor(365.25 * (year + 4716)) + floor(30.6001 * (month + 1)) + day + b - 1524.5;
}

inline void Bikram::fromJulianDate(double julian_date, int& year, int& month, int& day) const {
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

// ------------- Modified fromGregorian to use precomputed first --------------
inline void Bikram::fromGregorian(int y, int m, int d) {
    // First calculate Julian days from fixed known date
    double julian = getJulianDate(y, m, d);
    double ref_julian = getJulianDate(1943, 4, 14); // 1 Baisakh 2000 BS = 14 April 1943

    long diff = static_cast<long>(julian - ref_julian);
    int bs_year = BS_START_YEAR;

    if (diff >= 0) {
        while (bs_year <= BS_START_YEAR + NP_DATA_YEAR_COUNT - 1) {
            for (int i = 0; i < 12; ++i) {
                if (diff < NP_MONTHS_DATA[bs_year - BS_START_YEAR][i]) {
                    Year = bs_year;
                    Month = i + 1;
                    Day = static_cast<int>(diff) + 1;
                    return;
                } else {
                    diff -= NP_MONTHS_DATA[bs_year - BS_START_YEAR][i];
                }
            }
            bs_year++;
        }
    }
    // fallback to astronomical if outside precomputed
    fromGregorianAstronomical(y, m, d);
}

// -------------  Use precomputed first --------------
inline void Bikram::toGregorian(int bsYear, int bsMonth, int bsDay, int& gYear, int& gMonth, int& gDay) {
    if (bsYear >= BS_START_YEAR && bsYear < BS_START_YEAR + NP_DATA_YEAR_COUNT) {
        double ref_julian = getJulianDate(1943, 4, 14); // 1 Baisakh 2000 BS
        long total_days = 0;
        for (int year = BS_START_YEAR; year < bsYear; ++year) {
            total_days += NP_MONTHS_DATA[year - BS_START_YEAR][12];
        }
        for (int month = 0; month < bsMonth - 1; ++month) {
            total_days += NP_MONTHS_DATA[bsYear - BS_START_YEAR][month];
        }
        total_days += (bsDay - 1);

        double target_julian = ref_julian + total_days;
        fromJulianDate(target_julian, gYear, gMonth, gDay);
        return;
    }
    // fallback to astronomical
    toGregorianAstronomical(bsYear, bsMonth, bsDay, gYear, gMonth, gDay);
}

// fallback methods

inline void Bikram::fromGregorianAstronomical(int y, int m, int d) {
    double julian = getJulianDate(y, m, d);
    long ahar = julian - 588465.5;
    int saura_masa_num;
    int saura_masa_day;
    getSauraMasaDay(ahar, &saura_masa_num, &saura_masa_day);
    long YearKali = static_cast<long>(ahar * YugaRotation_sun / YugaCivilDays);
    int YearSaka = YearKali - 3179;
    int nepalimonth = (saura_masa_num) % 12;
    Year = YearSaka + 135 + static_cast<int>((saura_masa_num - nepalimonth) / 12);
    Month = (saura_masa_num + 12) % 12 + 1;
    Day = saura_masa_day;
}

inline void Bikram::toGregorianAstronomical(int bsYear, int bsMonth, int bsDay, int& gYear, int& gMonth, int& gDay) {
    int YearSaka = bsYear - 135;
    long YearKali = YearSaka + 3179;
    long ahar = static_cast<long>((YearKali * YugaCivilDays) / YugaRotation_sun);
    int saura_masa_num, saura_masa_day;
    getSauraMasaDay(ahar, &saura_masa_num, &saura_masa_day);
    bsMonth = (bsMonth + 11) % 12;
    while (saura_masa_num != bsMonth || saura_masa_day != bsDay) {
        ahar += (saura_masa_num < bsMonth || (saura_masa_num == bsMonth && saura_masa_day < bsDay)) ? 1 : -1;
        getSauraMasaDay(ahar, &saura_masa_num, &saura_masa_day);
    }
    double julian_date = ahar + 588465.5;
    fromJulianDate(julian_date, gYear, gMonth, gDay);
}

inline int Bikram::getYear() const {
    return Year;
}

inline int Bikram::getMonth() const {
    return Month;
}

inline int Bikram::getDay() const {
    return Day;
}

inline int Bikram::daysInMonth(int bsYear, int bsMonth) {
    if (bsYear >= BS_START_YEAR && bsYear < BS_START_YEAR + NP_DATA_YEAR_COUNT) {
        return NP_MONTHS_DATA[bsYear - BS_START_YEAR][bsMonth - 1];
    }
    // fallback method
    int gYear, gMonth, gDay;
    int nextMonth = (bsMonth % 12) + 1;
    int nextYear = (bsMonth == 12) ? bsYear + 1 : bsYear;
    toGregorian(bsYear, bsMonth, 1, gYear, gMonth, gDay);
    double julian_date_start = getJulianDate(gYear, gMonth, gDay);
    toGregorian(nextYear, nextMonth, 1, gYear, gMonth, gDay);
    double julian_date_end = getJulianDate(gYear, gMonth, gDay);
    return static_cast<int>(julian_date_end - julian_date_start);
}

#endif // BIKRAM_H
