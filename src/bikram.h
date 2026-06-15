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

#ifndef BIKRAM_H
#define BIKRAM_H

#include <QDate>
#include <QString>
#include <QVariantMap>

class PanchangaCalculator;

class BikramCalendar {
public:
  static QVariantMap toBikramSambat(const PanchangaCalculator *calc,
                                    const QDate &date);
  static QDate fromBikramSambat(const PanchangaCalculator *calc, int year,
                                int monthIndex, int day);
  static QVariantMap getBikramMonthInfo(const PanchangaCalculator *calc,
                                        int year, int monthIndex);
  static QString toDevanagari(const QString &numStr);
  static QString fromDevanagari(const QString &devanagariStr);

public:
  static QDate fromBikramSambatAlgorithmic(const PanchangaCalculator *calc,
                                           int bsYearInput, int year,
                                           int monthIndex, int day);
  static bool todaySauraMasaFirstP(const PanchangaCalculator *calc,
                                   double ahar);
  static void getSauraMasaDay(const PanchangaCalculator *calc, double ahar,
                              int &month, int &day);
  static int getBsYearFromAhar(const PanchangaCalculator *calc, double ahar);
  static int getBsYearFromAharAndMonth(double ahar, int m);
  static double findAharForBsDate(const PanchangaCalculator *calc, int bsYear,
                                  int monthIndex, int day);

  // Custom Julian Day conversions.
  // toJulianDay: hybrid Julian/Gregorian (Julian before Oct 15 1582, Gregorian after).
  // fromJulianDay: proleptic Gregorian for all dates (matches JS Date behavior).
  static double toJulianDay(int year, int month0, int day);
  static double toJulianDay(const QDate &date);
  static QDate fromJulianDay(double jd);

  // Historical weekday from our hybrid JDN. Returns 0=Sunday..6=Saturday.
  static int getHistoricalWeekday(const QDate &date);
};

#endif // BIKRAM_H
