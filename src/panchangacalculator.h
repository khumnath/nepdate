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

#ifndef PANCHANGACALCULATOR_H
#define PANCHANGACALCULATOR_H

#include <QObject>
#include <QDate>
#include <QVariantMap>
#include <QVariantList>
#include <QStringList>
#include <functional>

class PanchangaCalculator : public QObject {
    Q_OBJECT
    Q_PROPERTY(QStringList solarMonths READ getSolarMonths CONSTANT)
    Q_PROPERTY(QStringList weekdays READ getWeekdays CONSTANT)
    Q_PROPERTY(QStringList nepaliGregorianMonths READ getNepaliGregorianMonths CONSTANT)

public:
    explicit PanchangaCalculator(QObject *parent = nullptr);

    Q_INVOKABLE QVariantMap calculate(const QDate &date, double lat = 27.7172, double lon = 85.3240, double tz = 5.75, bool isBikramSambat = true);
    Q_INVOKABLE QDate fromBikramSambat(int year, int monthIndex, int day);
    Q_INVOKABLE QVariantMap getBikramMonthInfo(int year, int monthIndex);
    Q_INVOKABLE QString toDevanagari(const QString &numStr) const;
    Q_INVOKABLE QString fromDevanagari(const QString &devanagariStr) const;
    
    Q_INVOKABLE QVariantMap generateDebugInfo(const QDate &date, double lat = 27.7172, double lon = 85.3240, double tz = 5.75);
    Q_INVOKABLE QDate getLocalDate(double tz = 5.75) const;
    Q_INVOKABLE QDateTime getLocalDateTime(double tz = 5.75) const;
    Q_INVOKABLE void clearCache();

    QStringList getSolarMonths() const;
    QStringList getWeekdays() const;
    QStringList getNepaliGregorianMonths() const;

private:
    double getJulianDay(int year, int month, int day) const;
    QDate fromJulianDay(double jd) const;
    
public:
    double getAhar(const QDate &date, double lon, double tz) const;
    
    // Astronomical Logic
    double analyticalSunLong(double ahar) const;
    double analyticalMoonLong(double ahar) const;
    double analyticalElongation(double ahar) const;
    double getAyanamsa(double ahar) const;

    // NOAA Solar Math
    double calcGeomMeanLongSun(double t) const;
    double calcGeomMeanAnomalySun(double t) const;
    double calcEccentricityEarthOrbit(double t) const;
    double calcSunEqOfCenter(double t) const;
    double calcSunTrueLong(double t) const;
    double calcSunApparentLong(double t) const;
    double calcMeanObliquityOfEcliptic(double t) const;
    double calcObliquityCorrection(double t) const;
    double calcSunDeclination(double t) const;
    double calcEquationOfTime(double t) const;
    double calcHourAngleSunrise(double lat, double solarDec) const;
    
    // Month Logic
    double findTransit(double startAhar, int mode, double target, double span) const;
    QVariantList findElementsForDay(double ahar, int mode, const QStringList& names, double periodSpan, double anglePerElement) const;
    
    // Tithi etc
    double getTithi(double sunLong, double moonLong) const;
    double getNakshatra(double moonLong) const;
    double getYoga(double sunLong, double moonLong) const;
    double getKarana(double sunLong, double moonLong) const;
    
    QString resolveTithiName(int tithiDay, const QString& paksha) const;
    
    // Event integration
    QVariantList getEventsForDate(const QDate &date, double lat, double lon, double tz, 
                                  int bsYear, int bsMonthIndex, int bsDay,
                                  const QString &sunriseStr, const QString &sunsetStr,
                                  const QString &udayaTithi, const QString &udayaPaksha, 
                                  int masaNum, bool isAdhika, const QString &adhikaStatus) const;
    void getTithiAtAhar(double ahar, QString &tithiName, QString &paksha, int &tithiNum) const;
    bool isTithiInWindow(const QString &targetTithi, const QString &targetPaksha, 
                         double startAhar, double endAhar) const;

    // Month Logic
    QVariantMap toBikramSambat(const QDate &date) const;
    
    // Transits and Timings
    double findTransit(double searchStartAhar, 
                       std::function<double(double)> getValueFunc, 
                       double targetValue, double maxDays, double wrapValue = 360.0) const;
    QVariantList findElementsForDay(double startAhar, double endAhar,
      std::function<double(double)> getValueFunc, double divisor,
      const QStringList &nameArray, const QDate &baseDate, double lon,
      double tz, bool isBikramSambat, std::function<QString(int)> getSpecialName,
      double sunriseAhar = -1.0, double nextSunriseAhar = -1.0, double explicitWrapValue = -1.0) const;
    QVariantMap getDailyElementsAndTimings(double ahar, const QDate &baseDate, double lat, double lon, double tz, double sunriseMinutes, bool isBikramSambat) const;
    QString formatAharTime(double ahar, const QDate &baseDate, double lon, double tz, bool isBikramSambat, double nextSunriseAhar = -1.0) const;
    
    double findNewMoon(double ahar) const;
    double findPurnima(double ahar) const;
    int calculateAdhikaMasa(double purnimaAhar) const;
    
    // Sunrise / Sunset
    QVariantMap getSunriseSunset(const QDate &date, double lat, double lon) const;
    
    QVariantMap calculationCache;
};

#endif // PANCHANGACALCULATOR_H
