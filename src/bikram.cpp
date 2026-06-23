/*
 * Copyright (C) khumnath
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

#include "bikram.h"
#include "panchangacalculator.h"
#include "bsmonthdata.h"
#include <cmath>

#include "constants.h"

QString BikramCalendar::toDevanagari(const QString &numStr) {
  QString res;
  QString devNum = "०१२३४५६७८९";
  for (QChar c : numStr) {
    if (c.isDigit()) {
      res += devNum[c.digitValue()];
    } else {
      res += c;
    }
  }
  return res;
}

QString BikramCalendar::fromDevanagari(const QString &devanagariStr) {
  QString res;
  QString devNum = "०१२३४५६७८९";
  for (QChar c : devanagariStr) {
    int idx = devNum.indexOf(c);
    if (idx != -1) {
      res += QString::number(idx);
    } else {
      res += c;
    }
  }
  return res;
}

bool BikramCalendar::todaySauraMasaFirstP(const PanchangaCalculator* calc, double ahar) {
  double ahar_local_today = ahar - DESHANTARA;
  double ahar_local_tomorrow = ahar_local_today + 1.0;
  double tslong_today = calc->analyticalSunLong(ahar_local_today);
  double tslong_tomorrow = calc->analyticalSunLong(ahar_local_tomorrow);
  tslong_today -= std::floor(tslong_today / 30.0) * 30.0;
  tslong_tomorrow -= std::floor(tslong_tomorrow / 30.0) * 30.0;
  return (25.0 < tslong_today && tslong_tomorrow < 5.0);
}

void BikramCalendar::getSauraMasaDay(const PanchangaCalculator* calc, double ahar, int &month, int &day) {
  double currentAhar = std::floor(ahar);
  int daysElapsed = 0;
  while (daysElapsed < 40) {
    if (todaySauraMasaFirstP(calc, currentAhar)) {
      double tslong_tomorrow = calc->analyticalSunLong(currentAhar - DESHANTARA + 1.0);
      int m = static_cast<int>(std::floor(tslong_tomorrow / 30.0)) % 12;
      month = (m + 12) % 12;
      day = daysElapsed + 1;
      return;
    }
    currentAhar--;
    daysElapsed++;
  }
  month = 0;
  day = 1; // Fallback
}

int BikramCalendar::getBsYearFromAhar(const PanchangaCalculator* calc, double ahar) {
  int m, d;
  getSauraMasaDay(calc, ahar, m, d);
  double aharForYearCalc = ahar + (4 - m) * 30.0;
  int yearKali = static_cast<int>(std::floor(aharForYearCalc / SOLAR_YEAR_IN_DAYS));
  int yearSaka = yearKali - 3179;
  return yearSaka + 135;
}

int BikramCalendar::getBsYearFromAharAndMonth(double ahar, int m) {
  double aharForYearCalc = ahar + (4 - m) * 30.0;
  int yearKali = static_cast<int>(std::floor(aharForYearCalc / SOLAR_YEAR_IN_DAYS));
  int yearSaka = yearKali - 3179;
  return yearSaka + 135;
}

double BikramCalendar::findAharForBsDate(const PanchangaCalculator* calc, int bsYear, int monthIndex, int day) {
  int yearSaka = bsYear - 135;
  int yearKali = yearSaka + 3179;
  double ahar = std::floor(yearKali * SOLAR_YEAR_IN_DAYS);
  
  for (int i = 0; i < 5; i++) {
    int foundBsYear = getBsYearFromAhar(calc, ahar);
    int yearDifference = bsYear - foundBsYear;
    if (yearDifference == 0)
      break;
    ahar += std::round(yearDifference * SOLAR_YEAR_IN_DAYS);
  }
  
  int m, d;
  getSauraMasaDay(calc, ahar, m, d);
  double dayDifference = std::round((monthIndex * 30.5 + day) - (m * 30.5 + d));
  ahar += dayDifference;
  getSauraMasaDay(calc, ahar, m, d);
  
  int safetyCounter = 0;
  while (safetyCounter < 400) {
    int currentYear = getBsYearFromAharAndMonth(ahar, m);
    if (currentYear == bsYear && m == monthIndex && d == day) {
        break;
    }
    
    long currentTotal = currentYear * 12 + m;
    long targetTotal = bsYear * 12 + monthIndex;
    
    if (currentTotal < targetTotal || (currentTotal == targetTotal && d < day)) {
        ahar += 1.0;
    } else {
        ahar -= 1.0;
    }
    
    getSauraMasaDay(calc, ahar, m, d);
    safetyCounter++;
  }
  return ahar;
}

QDate BikramCalendar::fromBikramSambatAlgorithmic(const PanchangaCalculator* calc, int bsYearInput, int year, int monthIndex, int day) {
  double ahar = findAharForBsDate(calc, year, monthIndex, day);
  double jd = ahar + KaliEpoch;
  QDate adDate = fromJulianDay(jd);

  // Self-correction loop: attempt ±0..±4 day adjustments to find exact match
  int offsets[] = {0, 1, -1, 2, -2, 3, -3, 4, -4};
  for (int offset : offsets) {
    QDate testDate = adDate.addDays(offset);
    QVariantMap check = toBikramSambat(calc, testDate);
    int checkYear = check["year"].toInt();
    // Map the check year back to UI representation for comparison
    if (checkYear == bsYearInput && check["monthIndex"].toInt() == monthIndex && check["day"].toInt() == day) {
      return testDate;
    }
  }
  return adDate;
}

QVariantMap BikramCalendar::toBikramSambat(const PanchangaCalculator* calc, const QDate &date) {
  QVariantMap res;

  int daysFromEpoch = QDate(1943, 4, 14).daysTo(date);
  if (daysFromEpoch >= 0) {
    int tempYear = BsMonthData::BS_START_YEAR;
    int tempDays = daysFromEpoch;

    while (tempYear < BsMonthData::BS_START_YEAR + BsMonthData::NUM_YEARS) {
      int yearDays = BsMonthData::NP_MONTHS_DATA[tempYear - BsMonthData::BS_START_YEAR][12];
      if (tempDays < yearDays) {
        int tempMonth = 0;
        while (tempMonth < 12) {
          int monthDays = BsMonthData::NP_MONTHS_DATA[tempYear - BsMonthData::BS_START_YEAR][tempMonth];
          if (tempDays < monthDays) {
            res["year"] = tempYear;
            res["monthIndex"] = tempMonth;
            res["day"] = tempDays + 1;
            res["isComputed"] = false;
            res["isUnverified"] = (tempYear > 2083 && tempYear <= 2089);
            return res;
          }
          tempDays -= monthDays;
          tempMonth++;
        }
      }
      tempDays -= yearDays;
      tempYear++;
    }
  }

  // Fallback logic: custom toJulianDay for consistency
  double jd = toJulianDay(date);
  double ahar = jd - KaliEpoch; // jd is already at midnight (xxx.5)

  int m, d;
  getSauraMasaDay(calc, ahar, m, d);
  double aharForYearCalc = ahar + (4 - m) * 30.0;
  int yearKali = static_cast<int>(std::floor(aharForYearCalc / SOLAR_YEAR_IN_DAYS));
  int yearSaka = yearKali - 3179;
  int finalBsYear = yearSaka + 135;
  if (finalBsYear <= 0) finalBsYear -= 1; // Eliminate Year 0 for historical continuity

  res["year"] = finalBsYear;
  res["monthIndex"] = m;
  res["day"] = d;
  res["isComputed"] = true;
  res["isUnverified"] = false;
  return res;
}

QDate BikramCalendar::fromBikramSambat(const PanchangaCalculator* calc, int bsYearInput, int monthIndex, int day) {
  int year = bsYearInput < 0 ? bsYearInput + 1 : bsYearInput; // Map UI Year -1 back to internal Year 0
  int dataYear = year - BsMonthData::BS_START_YEAR;
  if (dataYear < 0 || dataYear >= BsMonthData::NUM_YEARS) {
    return fromBikramSambatAlgorithmic(calc, bsYearInput, year, monthIndex, day);
  }
  int totalDays = 0;
  for (int y = 0; y < dataYear; y++) {
    totalDays += BsMonthData::NP_MONTHS_DATA[y][12];
  }
  for (int m = 0; m < monthIndex; m++) {
    totalDays += BsMonthData::NP_MONTHS_DATA[dataYear][m];
  }
  totalDays += (day - 1);
  return QDate(1943, 4, 14).addDays(totalDays);
}

QVariantMap BikramCalendar::getBikramMonthInfo(const PanchangaCalculator* calc, int year, int monthIndex) {
  QVariantMap info;
  int dataYear = year - BsMonthData::BS_START_YEAR;

  if (dataYear >= 0 && dataYear < BsMonthData::NUM_YEARS) {
    int days = BsMonthData::NP_MONTHS_DATA[dataYear][monthIndex];
    int totalDays = 0;
    for (int y = 0; y < dataYear; y++) {
      totalDays += BsMonthData::NP_MONTHS_DATA[y][12];
    }
    for (int m = 0; m < monthIndex; m++) {
      totalDays += BsMonthData::NP_MONTHS_DATA[dataYear][m];
    }
    QDate startDate = QDate(1943, 4, 14).addDays(totalDays);

    info["year"] = year;
    info["monthIndex"] = monthIndex;
    info["monthName"] = solarMonthsList[monthIndex];
    info["totalDays"] = days;
    info["startAdDate"] = startDate;
    info["startDayOfWeek"] = getHistoricalWeekday(startDate);
    info["isComputed"] = false;
    return info;
  }

  // Astronomical fallback
  QDate firstDayAd = fromBikramSambat(calc, year, monthIndex, 1);
  int nextMonthIndex = (monthIndex + 1) % 12;
  int nextYear = (monthIndex == 11) ? year + 1 : year;
  QDate firstDayOfNextMonthAd = fromBikramSambat(calc, nextYear, nextMonthIndex, 1);

  int totalDays = firstDayAd.daysTo(firstDayOfNextMonthAd);

  info["year"] = year;
  info["monthIndex"] = monthIndex;
  info["monthName"] = solarMonthsList[monthIndex];
  info["totalDays"] = totalDays;
  info["startAdDate"] = firstDayAd;
  info["startDayOfWeek"] = getHistoricalWeekday(firstDayAd);
  info["isComputed"] = true;
  return info;
}

// Custom Julian Day conversions.

double BikramCalendar::toJulianDay(int year, int month0, int day) {
  // month0 is 0-indexed (0=January, 11=December), JS convention.
  int month1 = month0 + 1;
  int a = (14 - month1) / 12;
  // Use long long for intermediate calculations to prevent overflow for negative years
  long long y = (long long)year + 4800 - a;
  int m = month1 + 12 * a - 3;
  long long jdn = day + (153 * m + 2) / 5 + 365 * y + y / 4;

  // Hybrid: Gregorian after Oct 15, 1582; Julian before
  if (year > 1582 || (year == 1582 && month0 > 9) || (year == 1582 && month0 == 9 && day >= 15)) {
    jdn = jdn - y / 100 + y / 400 - 32045;
  } else {
    jdn = jdn - 32083;
  }
  return (double)jdn - 0.5; // midnight
}

double BikramCalendar::toJulianDay(const QDate &date) {
  return toJulianDay(date.year(), date.month() - 1, date.day());
}

QDate BikramCalendar::fromJulianDay(double jd) {
  // Proleptic Gregorian calendar for all dates
  jd += 0.5;
  double z = std::floor(jd);
  double alpha = std::floor((z - 1867216.25) / 36524.25);
  double a = z + 1 + alpha - std::floor(alpha / 4);
  double b = a + 1524;
  double c = std::floor((b - 122.1) / 365.25);
  double d = std::floor(365.25 * c);
  double e = std::floor((b - d) / 30.6001);
  int day = static_cast<int>(b - d - std::floor(30.6001 * e));
  int month = (e < 14) ? static_cast<int>(e) - 1 : static_cast<int>(e) - 13;
  int year = (month > 2) ? static_cast<int>(c) - 4716 : static_cast<int>(c) - 4715;
  return QDate(year, month, day);
}

int BikramCalendar::getHistoricalWeekday(const QDate &date) {
  double jd = toJulianDay(date);
  int z = static_cast<int>(std::floor(jd + 0.5));
  int wd = (z + 1) % 7;
  return wd >= 0 ? wd : wd + 7; // 0=Sunday..6=Saturday
}
