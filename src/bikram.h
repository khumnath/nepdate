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

private:
  static QDate fromBikramSambatAlgorithmic(const PanchangaCalculator *calc,
                                           int year, int monthIndex, int day);
  static bool todaySauraMasaFirstP(const PanchangaCalculator *calc,
                                   double ahar);
  static void getSauraMasaDay(const PanchangaCalculator *calc, double ahar,
                              int &month, int &day);
  static int getBsYearFromAhar(const PanchangaCalculator *calc, double ahar);
  static double findAharForBsDate(const PanchangaCalculator *calc, int bsYear,
                                  int monthIndex, int day);
};

#endif // BIKRAM_H
