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

#include "panchangacalculator.h"
#include "astronomy.h"
#include "bikram.h"
#include "eventsdata.h"
#include <QDateTime>
#include <cmath>

constexpr double KaliEpoch = 588465.5;
constexpr double rad = 180.0 / M_PI;
constexpr double DESHANTARA = (85.3 - 75.7682) / 360.0;
constexpr double SOLAR_YEAR_IN_DAYS = 365.2587564814815;

static double zero360(double x) { return x - std::floor(x / 360.0) * 360.0; }
static double deg2rad(double deg) { return deg * M_PI / 180.0; }
static double rad2deg(double r) { return r * 180.0 / M_PI; }

static const QStringList tithiNamesList = {
    "प्रतिपदा", "द्वितीया", "तृतीया",  "चतुर्थी",  "पञ्चमी",  "षष्ठी",
    "सप्तमी",   "अष्टमी",   "नवमी",   "दशमी",   "एकादशी", "द्वादशी",
    "त्रयोदशी", "चतुर्दशी",  "पूर्णिमा", "अमावस्या"};
static const QStringList nakshatras = {
    "अश्विनी",      "भरणी",        "कृत्तिका", "रोहिणी", "मृगशिरा",
    "आर्द्रा",       "पुनर्वसु",       "पुष्य",    "अश्लेषा",  "मघा",
    "पूर्व फाल्गुनी",  "उत्तर फाल्गुनी", "हस्त",    "चित्रा",  "स्वाती",
    "विशाखा",      "अनुराधा",      "ज्येष्ठा",  "मूल",     "पूर्वाषाढा",
    "उत्तराषाढा",   "श्रवण",        "धनिष्ठा", "शतभिषा", "पूर्व भाद्रपद",
    "उत्तर भाद्रपद", "रेवती"};
static const QStringList yogas = {
    "विष्कम्भ", "प्रीति", "आयुष्मान्",  "सौभाग्य", "शोभन", "अतिगण्ड", "सुकर्म",
    "धृति",    "शूल",    "गण्ड",     "वृद्धि",   "ध्रुव",  "व्याघात", "हर्षण",
    "वज्र",    "सिद्धि", "व्यतिपात", "वरीयान्", "परिघ", "शिव",    "सिद्ध",
    "साध्य",   "शुभ",    "शुक्ल",     "ब्रह्म",   "इन्द्र", "वैधृति"};
static const QStringList karanas = {"किंस्तुघ्न", "बव",     "बालव", "कौलव",
                                    "तैतिल",   "गर",     "वणिज", "विष्टि(भद्रा)",
                                    "शकुनि",   "चतुष्पाद", "नाग"};
static const QStringList rashis = {"मेष",  "वृषभ",   "मिथुन", "कर्क", "सिंह", "कन्या",
                                   "तुला", "वृश्चिक", "धनु",   "मकर", "कुम्भ", "मीन"};
static const QStringList solarMonthsList = {
    "वैशाख",   "ज्येष्ठ",     "आषाढ", "श्रावण", "भाद्रपद", "आश्विन",
    "कार्तिक", "मार्गशीर्ष", "पौष",  "माघ",   "फाल्गुन",  "चैत्र"};
static const QStringList weekdaysList = {
    "आइतबार", "सोमबार", "मङ्गलबार", "बुधबार", "बिहीबार", "शुक्रबार", "शनिबार"};

static const QStringList lunarMonthNamesList = {
    "वैशाख",   "ज्येष्ठ",     "आषाढ", "श्रावण", "भाद्र",  "आश्विन",
    "कार्तिक", "मार्गशीर्ष", "पौष",  "माघ",   "फाल्गुन", "चैत्र"};

PanchangaCalculator::PanchangaCalculator(QObject *parent) : QObject(parent) {}

QStringList PanchangaCalculator::getSolarMonths() const {
  return solarMonthsList;
}
QStringList PanchangaCalculator::getWeekdays() const { return weekdaysList; }

static const QStringList nepaliGregorianList = {
    "जनवरी", "फेब्रुअरी", "मार्च",   "अप्रिल",  "मे",      "जून",
    "जुलाई",  "अगस्ट",   "सेप्टेम्बर", "अक्टोबर", "नोभेम्बर", "डिसेम्बर"};

QStringList PanchangaCalculator::getNepaliGregorianMonths() const {
  return nepaliGregorianList;
}

QString PanchangaCalculator::toDevanagari(const QString &numStr) const {
  return BikramCalendar::toDevanagari(numStr);
}

QString PanchangaCalculator::fromDevanagari(const QString &devanagariStr) const {
  return BikramCalendar::fromDevanagari(devanagariStr);
}

double PanchangaCalculator::getJulianDay(int year, int month, int day) const {
  return BikramCalendar::toJulianDay(year, month, day);
}

QDate PanchangaCalculator::fromJulianDay(double jd) const {
  return BikramCalendar::fromJulianDay(jd);
}

double PanchangaCalculator::getAhar(const QDate &date, double lon,
                                    double tz) const {
  double jd = getJulianDay(date.year(), date.month() - 1, date.day());
  return jd - KaliEpoch + 0.25 + ((lon / 15.0 - tz) / 24.0);
}

astro_time_t aharToAstroTime(double ahar, double lon) {
  // ahar is the local civil ahargana (Deshantara adjusted).
  // To get JD in UTC: ahar + KaliEpoch - (lon / 360.0)
  double jd = ahar + KaliEpoch - (lon / 360.0);
  return Astronomy_TimeFromDays(jd - 2451545.0);
}

double PanchangaCalculator::getAyanamsa(double ahar) const {
  double jd = ahar + KaliEpoch - 0.25;
  double t = (jd - 2451545.0) / 36525.0;
  double years = (jd - 2451545.0) / 365.25;

  // Nutation in longitude (simplified IAU 1980)
  double omega = 125.04452 - 1934.136261 * t;
  double trueNutation = -0.00478 * std::sin(omega * M_PI / 180.0);
  trueNutation += 0.00005 * std::sin(2.0 * omega * M_PI / 180.0);

  // Modern Lahiri (Chitrapaksha Adjusted) Drik Standard from TS
  double meanLahiriAdj = 23.8570925 + (16.50 / 3600.0) +
                         (years * (50.290966 / 3600.0)) +
                         (1.1115 * t * t / 3600.0);

  return meanLahiriAdj + trueNutation;
}

double PanchangaCalculator::analyticalSunLong(double ahar) const {
  astro_time_t t = aharToAstroTime(ahar, 85.3240);
  astro_vector_t geoVec = Astronomy_GeoVector(BODY_SUN, t, ABERRATION);
  astro_ecliptic_t ecl = Astronomy_Ecliptic(geoVec);
  double sidereal = ecl.elon - getAyanamsa(ahar);
  return zero360(sidereal);
}

double PanchangaCalculator::analyticalMoonLong(double ahar) const {
  astro_time_t t = aharToAstroTime(ahar, 85.3240);
  astro_vector_t geoVec = Astronomy_GeoVector(BODY_MOON, t, ABERRATION);
  astro_ecliptic_t ecl = Astronomy_Ecliptic(geoVec);
  double sidereal = ecl.elon - getAyanamsa(ahar);
  return zero360(sidereal);
}

QDateTime PanchangaCalculator::getLocalDateTime(double tz) const {
  QDateTime currentUtc = QDateTime::currentDateTimeUtc();
  int totalSeconds = static_cast<int>(tz * 3600.0);
  return currentUtc.addSecs(totalSeconds);
}

QDate PanchangaCalculator::getLocalDate(double tz) const {
  return getLocalDateTime(tz).date();
}

double PanchangaCalculator::analyticalElongation(double ahar) const {
  return zero360(analyticalMoonLong(ahar) - analyticalSunLong(ahar));
}

double PanchangaCalculator::getTithi(double sunLong, double moonLong) const {
  return zero360(moonLong - sunLong) / 12.0;
}

double PanchangaCalculator::getNakshatra(double moonLong) const {
  return moonLong / (360.0 / 27.0);
}

double PanchangaCalculator::getYoga(double sunLong, double moonLong) const {
  return zero360(sunLong + moonLong) / (360.0 / 27.0);
}

void PanchangaCalculator::getTithiAtAhar(double ahar, QString &tithiName,
                                         QString &paksha, int &tithiNum) const {
  double sunLong = analyticalSunLong(ahar);
  double moonLong = analyticalMoonLong(ahar);
  double tithiVal = getTithi(sunLong, moonLong);
  tithiNum = std::floor(tithiVal) + 1;
  if (tithiNum > 30)
    tithiNum = 30;
  if (tithiNum < 1)
    tithiNum = 1;
  paksha = (tithiNum <= 15) ? "शुक्ल पक्ष" : "कृष्ण पक्ष";
  int tDay = (tithiNum > 15) ? tithiNum - 15 : tithiNum;
  int nameIndex = tDay - 1;
  if (tithiNum == 15)
    nameIndex = 14;
  else if (tithiNum == 30)
    nameIndex = 15;
  tithiName = tithiNamesList[nameIndex];
}

bool PanchangaCalculator::isTithiInWindow(const QString &targetTithi,
                                          const QString &targetPaksha,
                                          double startAhar,
                                          double endAhar) const {
  QString startTithi, startPaksha, endTithi, endPaksha;
  int startTithiNum, endTithiNum;
  getTithiAtAhar(startAhar, startTithi, startPaksha, startTithiNum);
  getTithiAtAhar(endAhar, endTithi, endPaksha, endTithiNum);

  if (startTithi == targetTithi && startPaksha == targetPaksha)
    return true;
  if (endTithi == targetTithi && endPaksha == targetPaksha)
    return true;

  int targetTithiNum = tithiNamesList.indexOf(targetTithi) + 1;
  if (targetPaksha == "कृष्ण पक्ष")
    targetTithiNum += 15;
  if (targetTithiNum == 16 && targetTithi == "पूर्णिमा")
    return false;

  double sVal =
      getTithi(analyticalSunLong(startAhar), analyticalMoonLong(startAhar));
  double eVal =
      getTithi(analyticalSunLong(endAhar), analyticalMoonLong(endAhar));

  int tStart = targetTithiNum - 1;
  int tEnd = targetTithiNum;

  if (eVal < sVal) {
    return (sVal < tEnd) || (eVal > tStart);
  }
  return (sVal < tEnd && eVal > tStart);
}

QVariantList PanchangaCalculator::getEventsForDate(
    const QDate &date, double lat, double lon, double tz, int bsYear,
    int bsMonthIndex, int bsDay, const QString &sunriseStr,
    const QString &sunsetStr, const QString &udayaTithi,
    const QString &udayaPaksha, int masaNum, bool isAdhika,
    const QString &adhikaStatus) const {
  QVariantList events;
  double ahar = getAhar(date, lon, tz);

  auto timeToFrac = [](const QString &t) {
    QStringList parts = t.split(":");
    if (parts.size() < 2)
      return 0.25;
    return (parts[0].toDouble() * 3600 + parts[1].toDouble() * 60) / 86400.0;
  };

  double sunriseFrac = timeToFrac(sunriseStr);
  double sunsetFrac = timeToFrac(sunsetStr);

  double aharSunrise = ahar + sunriseFrac;
  double aharSunset = ahar + sunsetFrac;
  double aharNoon = ahar + (sunriseFrac + sunsetFrac) / 2.0;

  double pradoshStart = aharSunset;
  double pradoshEnd = aharSunset + 0.05;
  double madhyannaStart = aharNoon - 0.05;
  double madhyannaEnd = aharNoon + 0.05;
  double midnightFrac = (sunsetFrac + (1.0 + sunriseFrac)) / 2.0;
  double nishithaStart = ahar + midnightFrac - (1.0 / 60.0);
  double nishithaEnd = ahar + midnightFrac + (1.0 / 60.0);

  QString secondaryMonthName;
  if (adhikaStatus.startsWith("क्षय")) {
    QStringList parts = adhikaStatus.split(" ");
    if (parts.size() > 1) {
      QString solarName = parts[1];
      QMap<QString, QString> solarToLunar = {
          {"बैशाख", "वैशाख"},     {"जेठ", "ज्येष्ठ"},       {"असार", "आषाढ"},
          {"साउन", "श्रावण"},    {"भदौ", "भाद्र"},      {"असोज", "आश्विन"},
          {"कार्तिक", "कार्तिक"}, {"मंसिर", "मार्गशीर्ष"}, {"पुस", "पौष"},
          {"माघ", "माघ"},       {"फागुन", "फाल्गुन"},    {"चैत", "चैत्र"}};
      secondaryMonthName = solarToLunar.value(solarName, "");
    }
  }

  for (int i = 0; i < numLunarEvents; ++i) {
    const LunarEventRule &rule = lunarEvents[i];

    if (isAdhika)
      continue;

    int dynamicMasaNum = masaNum;
    if (rule.paksha == "कृष्ण पक्ष" && udayaPaksha == "शुक्ल पक्ष") {
      dynamicMasaNum = (masaNum + 1) % 12;
    }
    QString customMonthName = lunarMonthNamesList[dynamicMasaNum];

    bool isMonthMatch =
        (rule.lunarMonth.isEmpty() || rule.lunarMonth == customMonthName);
    if (!secondaryMonthName.isEmpty() &&
        rule.lunarMonth == secondaryMonthName) {
      isMonthMatch = true;
    }

    if (!isMonthMatch)
      continue;

    bool match = false;
    if (rule.rule == EventRule::Udaya) {
      match = (udayaTithi == rule.tithi && udayaPaksha == rule.paksha);
    } else if (rule.rule == EventRule::Madhyanna) {
      match = isTithiInWindow(rule.tithi, rule.paksha, madhyannaStart,
                              madhyannaEnd);
    } else if (rule.rule == EventRule::Nishitha) {
      match =
          isTithiInWindow(rule.tithi, rule.paksha, nishithaStart, nishithaEnd);
    } else if (rule.rule == EventRule::Pradosh) {
      match =
          isTithiInWindow(rule.tithi, rule.paksha, pradoshStart, pradoshEnd);
    }

    if (match) {
      QVariantMap eventMap;
      eventMap["name"] = rule.nameNe;
      eventMap["detail"] = rule.detailNe;
      eventMap["holiday"] = rule.holiday;
      events.append(eventMap);
    }
  }

  for (int i = 0; i < numGregorianEvents; ++i) {
    const FixedEventRule &rule = gregorianEvents[i];
    if (rule.month == date.month() && rule.day == date.day()) {
      QVariantMap eventMap;
      eventMap["name"] = rule.nameNe;
      eventMap["detail"] = rule.detailNe;
      eventMap["holiday"] = rule.holiday;
      events.append(eventMap);
    }
  }

  for (int i = 0; i < numBikramEvents; ++i) {
    const FixedEventRule &rule = bikramEvents[i];
    if (rule.month == bsMonthIndex + 1 && rule.day == bsDay) {
      if (rule.year == 0 || rule.year == bsYear) {
        QVariantMap eventMap;
        eventMap["name"] = rule.nameNe;
        eventMap["detail"] = rule.detailNe;
        eventMap["holiday"] = rule.holiday;
        events.append(eventMap);
      }
    }
  }

  if (udayaTithi == "एकादशी") {
    QVariantMap ev;
    ev["name"] = "एकादशी व्रत";
    ev["detail"] = "एकादशीको नियमित व्रत";
    ev["holiday"] = false;
    events.append(ev);
  }

  int weekday = BikramCalendar::getHistoricalWeekday(date); // 0=Sunday..6=Saturday

  if (weekday == 2 && udayaTithi == "चतुर्थी" && udayaPaksha == "कृष्ण पक्ष") {
    QVariantMap ev;
    ev["name"] = "मंगल चतुर्थी";
    ev["detail"] =
        "कृष्ण पक्षको चतुर्थी मंगलबार पर्नु मंगल चतुर्थी हो। यस दिन भगवान् गणेशको पूजा-अर्चना "
        "गरिन्छ। व्रत बस्दा विघ्न-बाधा हट्ने, रोग-शोक कम हुने र कार्यसिद्धि हुने विश्वास छ।";
    ev["holiday"] = false;
    events.append(ev);
  }
  if (weekday == 1 && udayaTithi == "अमावस्या") {
    QVariantMap ev;
    ev["name"] = "सोमवती औंसी";
    ev["detail"] = "सोमबारको अमावस्या सोमवती औंसी हो। यस दिन व्रत बस्नेले दीर्घायु, "
                   "स्वास्थ्य र सौभाग्य प्राप्त गर्ने विश्वास छ। विशेष गरी महिलाले पतिको "
                   "आयु र परिवारको कल्याणका लागि व्रत गर्छन्।";
    ev["holiday"] = false;
    events.append(ev);
  }
  if (weekday == 0 && udayaTithi == "सप्तमी" && udayaPaksha == "शुक्ल पक्ष") {
    QVariantMap ev;
    ev["name"] = "रवि सप्तमी";
    ev["detail"] = "आइतबारको दिन शुक्ल पक्षको सप्तमी पर्नुलाई रवि सप्तमी भनिन्छ। यस "
                   "दिन सूर्य देवताको विशेष पूजा र व्रत गरिन्छ।";
    ev["holiday"] = false;
    events.append(ev);
  }
  if (weekday == 3 && udayaTithi == "अष्टमी" && udayaPaksha == "शुक्ल पक्ष") {
    QVariantMap ev;
    ev["name"] = "बुधाष्टमी";
    ev["detail"] = "बुधबारको दिन शुक्ल पक्षको अष्टमी पर्नु बुधाष्टमी हो। यस दिन विशेष "
                   "गरी भगवान् शिव र माता पार्वतीको पूजा गरिन्छ।";
    ev["holiday"] = false;
    events.append(ev);
  }
  if (weekday == 6 && udayaTithi == "अमावस्या") {
    QVariantMap ev;
    ev["name"] = "शनिश्चरे औंसी";
    ev["detail"] = "शनिबारका दिन औंसी पर्नुलाई शनिश्चरे औंसी भनिन्छ। यस दिन शनिको पूजा "
                   "र दान गर्दा शनि दोष कम हुने धार्मिक विश्वास छ।";
    ev["holiday"] = false;
    events.append(ev);
  }

  if (isTithiInWindow("त्रयोदशी", "शुक्ल पक्ष", pradoshStart, pradoshEnd) ||
      isTithiInWindow("त्रयोदशी", "कृष्ण पक्ष", pradoshStart, pradoshEnd)) {
    if (weekday == 1) {
      QVariantMap ev;
      ev["name"] = "सोम प्रदोष व्रत";
      ev["detail"] =
          "त्रयोदशी सोमबार पर्नु सोम प्रदोष हो। यस दिन भगवान् शिवको पूजा-अर्चना "
          "गरिन्छ। व्रत बस्दा स्वास्थ्य, दीर्घायु र सुख-समृद्धि प्राप्त हुने विश्वास छ।";
      ev["holiday"] = false;
      events.append(ev);
    } else if (weekday == 2) {
      QVariantMap ev;
      ev["name"] = "भौम प्रदोष व्रत";
      ev["detail"] = "मंगलबार पर्ने प्रदोष व्रतलाई भौम प्रदोष भनिन्छ। यस दिन शिव पूजा "
                     "गर्नाले ऋणबाट मुक्ति र संकट नाश हुने जनविश्वास छ।";
      ev["holiday"] = false;
      events.append(ev);
    } else if (weekday == 6) {
      QVariantMap ev;
      ev["name"] = "शनि प्रदोष व्रत";
      ev["detail"] =
          "शनिबार पर्ने प्रदोष व्रतलाई शनि प्रदोष भनिन्छ। यस दिन सन्तान प्राप्तिको "
          "कामना गर्दै र शनि दोष निवारणका लागि भगवान् शिवको पूजा गरिन्छ।";
      ev["holiday"] = false;
      events.append(ev);
    }
  }

  return events;
}
double PanchangaCalculator::getKarana(double sunLong, double moonLong) const {
  return zero360(moonLong - sunLong) / 6.0;
}

QVariantMap PanchangaCalculator::getSunriseSunset(const QDate &date, double lat,
                                                  double lon) const {
  QVariantMap res;

  double jd = BikramCalendar::toJulianDay(date);
  int jdInt = static_cast<int>(std::floor(jd + 0.5));

  // Julian Century
  double t = (jd - 2451545.0) / 36525.0;

  double eqTime = calcEquationOfTime(t);
  double solarDec = calcSunDeclination(t);
  double haSunrise = calcHourAngleSunrise(lat, solarDec);

  double tzOffset = 5.75; // Hardcoded to Nepal

  double solarNoonMinutes = 720.0 - (4.0 * lon) - eqTime + (tzOffset * 60.0);
  double sunriseMinutes = solarNoonMinutes - (haSunrise * 4.0);
  double sunsetMinutes = solarNoonMinutes + (haSunrise * 4.0);

  int srHr = static_cast<int>(std::floor(sunriseMinutes / 60.0)) % 24;
  if (srHr < 0)
    srHr += 24;
  int srMin = static_cast<int>(std::floor(sunriseMinutes)) % 60;

  int ssHr = static_cast<int>(std::floor(sunsetMinutes / 60.0)) % 24;
  if (ssHr < 0)
    ssHr += 24;
  int ssMin = static_cast<int>(std::floor(sunsetMinutes)) % 60;

  QString srStr = QString("%1:%2")
                      .arg(srHr, 2, 10, QChar('0'))
                      .arg(srMin, 2, 10, QChar('0'));
  QString ssStr = QString("%1:%2")
                      .arg(ssHr, 2, 10, QChar('0'))
                      .arg(ssMin, 2, 10, QChar('0'));

  res["sunrise"] = srStr;
  res["sunset"] = ssStr;
  res["sunriseMinutes"] = sunriseMinutes;
  res["sunsetMinutes"] = sunsetMinutes;

  return res;
}

double PanchangaCalculator::calcGeomMeanLongSun(double t) const {
  double L0 = 280.46646 + t * (36000.76983 + t * 0.0003032);
  while (L0 > 360.0)
    L0 -= 360.0;
  while (L0 < 0.0)
    L0 += 360.0;
  return L0;
}

double PanchangaCalculator::calcGeomMeanAnomalySun(double t) const {
  return 357.52911 + t * (35999.05029 - 0.0001537 * t);
}

double PanchangaCalculator::calcEccentricityEarthOrbit(double t) const {
  return 0.016708634 - t * (0.000042037 + 0.0000001267 * t);
}

double PanchangaCalculator::calcSunEqOfCenter(double t) const {
  double m = calcGeomMeanAnomalySun(t);
  double mrad = deg2rad(m);
  double sinm = std::sin(mrad);
  double sin2m = std::sin(mrad * 2.0);
  double sin3m = std::sin(mrad * 3.0);
  return sinm * (1.914602 - t * (0.004817 + 0.000014 * t)) +
         sin2m * (0.019993 - 0.000101 * t) + sin3m * 0.000289;
}

double PanchangaCalculator::calcSunTrueLong(double t) const {
  return calcGeomMeanLongSun(t) + calcSunEqOfCenter(t);
}

double PanchangaCalculator::calcSunApparentLong(double t) const {
  double o = calcSunTrueLong(t);
  double omega = 125.04 - 1934.136 * t;
  return o - 0.00569 - 0.00478 * std::sin(deg2rad(omega));
}

double PanchangaCalculator::calcMeanObliquityOfEcliptic(double t) const {
  double seconds = 21.448 - t * (46.8150 + t * (0.00059 - t * 0.001813));
  return 23.0 + (26.0 + (seconds / 60.0)) / 60.0;
}

double PanchangaCalculator::calcObliquityCorrection(double t) const {
  double e0 = calcMeanObliquityOfEcliptic(t);
  double omega = 125.04 - 1934.136 * t;
  return e0 + 0.00256 * std::cos(deg2rad(omega));
}

double PanchangaCalculator::calcSunDeclination(double t) const {
  double e = calcObliquityCorrection(t);
  double lambda = calcSunApparentLong(t);
  double sint = std::sin(deg2rad(e)) * std::sin(deg2rad(lambda));
  return rad2deg(std::asin(sint));
}

double PanchangaCalculator::calcEquationOfTime(double t) const {
  double epsilon = calcObliquityCorrection(t);
  double l0 = calcGeomMeanLongSun(t);
  double e = calcEccentricityEarthOrbit(t);
  double m = calcGeomMeanAnomalySun(t);

  double y = std::tan(deg2rad(epsilon) / 2.0);
  y *= y;

  double sin2l0 = std::sin(2.0 * deg2rad(l0));
  double sinm = std::sin(deg2rad(m));
  double cos2l0 = std::cos(2.0 * deg2rad(l0));
  double sin4l0 = std::sin(4.0 * deg2rad(l0));
  double sin2m = std::sin(2.0 * deg2rad(m));

  double eTime = y * sin2l0 - 2.0 * e * sinm + 4.0 * e * y * sinm * cos2l0 -
                 0.5 * y * y * sin4l0 - 1.25 * e * e * sin2m;
  return rad2deg(eTime) * 4.0;
}

double PanchangaCalculator::calcHourAngleSunrise(double lat,
                                                 double solarDec) const {
  double latRad = deg2rad(lat);
  double sdRad = deg2rad(solarDec);
  double haArg =
      (std::cos(deg2rad(90.833)) / (std::cos(latRad) * std::cos(sdRad))) -
      std::tan(latRad) * std::tan(sdRad);
  if (haArg < -1.0)
    haArg = -1.0;
  if (haArg > 1.0)
    haArg = 1.0;
  return rad2deg(std::acos(haArg));
}

QVariantMap PanchangaCalculator::getBikramMonthInfo(int year, int monthIndex) {
  return BikramCalendar::getBikramMonthInfo(this, year, monthIndex);
}

QVariantMap PanchangaCalculator::toBikramSambat(const QDate &date) const {
  return BikramCalendar::toBikramSambat(this, date);
}

QDate PanchangaCalculator::fromBikramSambat(int year, int monthIndex, int day) {
  return BikramCalendar::fromBikramSambat(this, year, monthIndex, day);
}

double PanchangaCalculator::findNewMoon(double ahar) const {
  auto getElongation = [this](double a) {
    return zero360(analyticalMoonLong(a) - analyticalSunLong(a));
  };

  double guess = ahar;
  for (int i = 0; i < 10; i++) {
    double elong = getElongation(guess);
    if (elong < 5.0 || elong > 355.0)
      break;
    guess += (elong < 180.0 ? -elong : 360.0 - elong) / 12.19;
  }

  double lo = guess - 2.0;
  double hi = guess + 2.0;
  for (int j = 0; j < 30; j++) {
    double mid = (lo + hi) / 2.0;
    if (getElongation(mid) < 180.0) {
      hi = mid;
    } else {
      lo = mid;
    }
  }
  return (lo + hi) / 2.0;
}

QVariantMap PanchangaCalculator::calculate(const QDate &date, double lat,
                                           double lon, double tz, bool isBikramSambat) {
  QVariantMap result;

  // Find Bikram Sambat date safely
  QVariantMap bsInfo = toBikramSambat(date);
  int tempYear = bsInfo["year"].toInt();
  int tempMonth = bsInfo["monthIndex"].toInt();
  int bsDay = bsInfo["day"].toInt();
  result["isComputed"] = bsInfo["isComputed"];
  result["isUnverified"] = bsInfo["isUnverified"];

  if (bsInfo["isComputed"].toBool()) {
    result["statusMessage"] = "This date is out of available data. Generated "
                              "using computational fallback.";
  } else if (bsInfo["isUnverified"].toBool()) {
    result["statusMessage"] =
        "Future date not yet verified by official Panchanga.";
  } else {
    result["statusMessage"] = "";
  }

  QVariantMap sunriseSunset = getSunriseSunset(date, lat, lon);
  QString sunriseStr = sunriseSunset["sunrise"].toString();
  QString sunsetStr = sunriseSunset["sunset"].toString();

  // Calculate Ahar at Sunrise
  double ahar = getAhar(date, lon, tz); // base
  double sunriseMinutes = sunriseSunset["sunriseMinutes"].toDouble();
  if (sunriseMinutes > 0) {
    // getAhar normally gives Ahar at Midnight local time + Deshantara.
    // We want exact Ahar at Sunrise local time.
    double localFraction = sunriseMinutes / 1440.0; // Minutes to days
    double jd0 =
        getJulianDay(date.year(), date.month() - 1, date.day()); // Midnight UTC
    double aharAtUtcMidnight = jd0 - KaliEpoch;
    // Local time = UTC + tz -> UTC = Local time - tz
    double utcFraction = localFraction - (tz / 24.0);
    ahar = aharAtUtcMidnight + utcFraction + (lon / 360.0);
  } else if (sunriseStr != "N/A" && sunriseStr.contains(":")) {
    QStringList parts = sunriseStr.split(":");
    int h = parts[0].toInt();
    int m = parts[1].toInt();
    double utcFraction = ((h - tz) * 3600.0 + m * 60.0) / 86400.0;
    double jd0 =
        getJulianDay(date.year(), date.month() - 1, date.day()); // Midnight UTC
    double aharAtUtcMidnight = jd0 - KaliEpoch;
    ahar = aharAtUtcMidnight + utcFraction + (lon / 360.0);
  }

  double sunLong = analyticalSunLong(ahar);
  double moonLong = analyticalMoonLong(ahar);

  double tithiVal = getTithi(sunLong, moonLong);
  int tithiNum = std::floor(tithiVal) + 1;
  if (tithiNum > 30)
    tithiNum = 30; // Clamp
  if (tithiNum < 1)
    tithiNum = 1;
  QString paksha = (tithiNum <= 15) ? "शुक्ल पक्ष" : "कृष्ण पक्ष";
  int tDay = (tithiNum > 15) ? tithiNum - 15 : tithiNum;
  int nameIndex = tDay - 1;
  if (tithiNum == 15)
    nameIndex = 14; // Purnima
  else if (tithiNum == 30)
    nameIndex = 15; // Amavasya
  QString tithiName = tithiNamesList[nameIndex];

  result["bsYear"] = tempYear;
  result["bsMonthIndex"] = tempMonth;
  result["bsDay"] = bsDay;
  result["monthName"] = solarMonthsList[tempMonth];
  result["tithi"] = tithiName;
  result["paksha"] = paksha;

  int nakshatraIdx = static_cast<int>(std::floor(getNakshatra(moonLong))) % 27;
  int yogaIdx = static_cast<int>(std::floor(getYoga(sunLong, moonLong))) % 27;

  result["nakshatra"] = nakshatras[nakshatraIdx];
  result["yoga"] = yogas[yogaIdx];
  int karanaIdx = static_cast<int>(std::floor(2 * tithiVal));
  QString karanaName = karanaIdx > 0
                           ? (karanaIdx < 57 ? karanas[(karanaIdx - 1) % 7 + 1]
                                             : karanas[karanaIdx - 57 + 8])
                           : karanas[0];
  result["karana"] = karanaName;

  result["weekday"] = weekdaysList[BikramCalendar::getHistoricalWeekday(date)];

  // Custom Gregorian formatting: add Julian suffix for pre-1582 dates
  int adYear = date.year();
  QString yearStr;
  if (adYear <= 0) {
    yearStr = QString::number(std::abs(adYear - 1)) + " BCE (Julian)";
  } else if (adYear < 1582) {
    yearStr = QString::number(adYear) + " (Julian)";
  } else {
    yearStr = QString::number(adYear);
  }
  result["gregorianDate"] = date.toString("dddd, MMMM d, ") + yearStr;

  // Lunar Month and Adhika Masa
  double lunarMonthStart = findNewMoon(ahar);
  if (lunarMonthStart > ahar) {
    lunarMonthStart = findNewMoon(lunarMonthStart - 29.53);
  }
  double lunarMonthEnd = findNewMoon(lunarMonthStart + 29.53);

  int startSign =
      static_cast<int>(std::floor(analyticalSunLong(lunarMonthStart) / 30.0));
  int endSign =
      static_cast<int>(std::floor(analyticalSunLong(lunarMonthEnd) / 30.0));

  if (endSign < startSign)
    endSign += 12;

  bool isAdhika = false;
  if (endSign == startSign) {
    isAdhika = true;
  }

  int mIdx = static_cast<int>(std::floor(sunLong / 30.0)) % 12;
  // Purnimanta adjustment
  double clong = analyticalSunLong(lunarMonthStart);
  int masaNum = mIdx;
  if ((static_cast<int>(std::floor(clong / 30.0)) % 12) == masaNum) {
    masaNum = (masaNum + 1) % 12;
  }
  masaNum = (masaNum - 1 + 12) % 12;

  if (paksha == "कृष्ण पक्ष" && !isAdhika) {
    masaNum = (masaNum + 1) % 12;
  }

  QString lunarMonthStr = lunarMonthNamesList[masaNum];
  if (isAdhika) {
    lunarMonthStr = "अधिक " + lunarMonthStr;
  }

  result["lunarMonth"] = lunarMonthStr;
  result["adhikaMasa"] = isAdhika ? "अधिक मास" : "छैन";

  result["sunrise"] = sunriseStr;
  result["sunset"] = sunsetStr;

  // According to Bikram Sambat rules, the Solar month dictates the Sun Rashi
  // for the entire civil day. Baisakh (0) = Mesha (0), Jestha (1) = Vrishabha
  // (1), Ashadh (2) = Mithun (2), etc.
  int sunRashiIdx = tempMonth;
  result["sunRashi"] = rashis[sunRashiIdx];

  int moonRashiIdx = static_cast<int>(std::floor(moonLong / 30.0)) % 12;
  result["moonRashi"] = rashis[moonRashiIdx];

  QVariantList eventsList = getEventsForDate(
      date, lat, lon, tz, tempYear, tempMonth, bsDay,
      sunriseSunset["sunrise"].toString(), sunriseSunset["sunset"].toString(),
      tithiName, paksha, masaNum, isAdhika, result["adhikaMasa"].toString());
  result["events"] = eventsList;

  // Append Daily Transits
  QVariantMap transits =
      getDailyElementsAndTimings(ahar, date, lat, lon, tz, sunriseMinutes, isBikramSambat);
  result["dailyTransits"] = transits;

  // Calculate Bhadra
  QVariantList karanaElements = transits["karanas"].toList();
  QVariantMap bhadraInfo;
  bool bhadraFound = false;

  for (const QVariant &kVar : karanaElements) {
    QVariantMap kMap = kVar.toMap();
    QString name = kMap["name"].toString();
    if (name.contains("विष्टि") || name.contains("Vishti")) {
      int rIdx = static_cast<int>(std::floor(moonLong / 30.0)) % 12;
      QString residence = "";
      QString status = "";
      bool isHarmful = false;

      if (rIdx == 3 || rIdx == 4 || rIdx == 10 || rIdx == 11) {
        residence = "पृथ्वी लोक (मृत्युलोक)";
        status = "अशुभ — भद्रा पृथ्वी लोकमा हुँदा अत्यन्त अशुभ मानिन्छ। विवाह, नयाँ कार्य "
                 "र यात्रा जस्ता शुभ कार्य पूर्णतः वर्जित छन्।";
        isHarmful = true;
      } else if (rIdx == 0 || rIdx == 1 || rIdx == 2 || rIdx == 7) {
        residence = "स्वर्ग लोक (स्वर्ग)";
        status = "तटस्थ — भद्रा स्वर्ग लोकमा हुँदा मध्यम मानिन्छ। कुनै ठूलो हानि हुँदैन, तर "
                 "यो सर्वोत्तम समय भने होइन।";
        isHarmful = false;
      } else {
        residence = "पाताल लोक (पाताल)";
        status = "शुभ — भद्रा पाताल लोकमा हुँदा शुभ मानिन्छ। नयाँ कार्य, पूजा र सबै शुभ "
                 "कार्यका लागि उत्तम समय हो।";
        isHarmful = false;
      }

      bhadraInfo["isActive"] = true;
      bhadraInfo["isActiveCivil"] = true; // Found in today's civil transits
      bhadraInfo["residence"] = residence;
      bhadraInfo["status"] = status;
      bhadraInfo["isHarmful"] = isHarmful;
      bhadraFound = true;
      break;
    }
  }

  if (bhadraFound) {
    result["bhadra"] = bhadraInfo;
  } else {
    result["bhadra"] = QVariant();
  }

  return result;
}

QVariantMap PanchangaCalculator::generateDebugInfo(const QDate &date,
                                                   double lat, double lon,
                                                   double tz) {
  QVariantMap res;

  QVariantMap sunriseSunset = getSunriseSunset(date, lat, lon);
  QString sunriseStr = sunriseSunset["sunrise"].toString();
  double sunriseMinutes = sunriseSunset["sunriseMinutes"].toDouble();

  double ahar = getAhar(date, lon, tz); // base
  if (sunriseMinutes > 0) {
    double localFraction = sunriseMinutes / 1440.0;
    double jd0 =
        getJulianDay(date.year(), date.month() - 1, date.day()); // Midnight UTC
    double aharAtUtcMidnight = jd0 - KaliEpoch;
    double utcFraction = localFraction - (tz / 24.0);
    ahar = aharAtUtcMidnight + utcFraction + (lon / 360.0);
  } else if (sunriseStr != "N/A" && sunriseStr.contains(":")) {
    QStringList parts = sunriseStr.split(":");
    int h = parts[0].toInt();
    int m = parts[1].toInt();
    double utcFraction = ((h - tz) * 3600.0 + m * 60.0) / 86400.0;
    double jd0 =
        getJulianDay(date.year(), date.month() - 1, date.day()); // Midnight UTC
    double aharAtUtcMidnight = jd0 - KaliEpoch;
    ahar = aharAtUtcMidnight + utcFraction + (lon / 360.0);
  }

  double sunLong = analyticalSunLong(ahar);
  double moonLong = analyticalMoonLong(ahar);
  double ayanamsa = getAyanamsa(ahar);

  // Re-calculate tropicals for debug
  astro_time_t t = aharToAstroTime(ahar, lon);
  astro_vector_t sunVec = Astronomy_GeoVector(BODY_SUN, t, ABERRATION);
  astro_ecliptic_t sunEcl = Astronomy_Ecliptic(sunVec);
  astro_vector_t moonVec = Astronomy_GeoVector(BODY_MOON, t, ABERRATION);
  astro_ecliptic_t moonEcl = Astronomy_Ecliptic(moonVec);

  double tropicalSun = sunEcl.elon;
  double tropicalMoon = moonEcl.elon;

  double tithiVal = getTithi(sunLong, moonLong);
  int tithiNum = std::floor(tithiVal) + 1;
  if (tithiNum > 30)
    tithiNum = 30;
  if (tithiNum < 1)
    tithiNum = 1;
  QString paksha = (tithiNum <= 15) ? "शुक्ल पक्ष" : "कृष्ण पक्ष";
  int tDay = (tithiNum > 15) ? tithiNum - 15 : tithiNum;

  int nameIndex = tDay - 1;
  if (tithiNum == 15)
    nameIndex = 14;
  else if (tithiNum == 30)
    nameIndex = 15;
  QString tithiName = tithiNamesList[nameIndex];

  int nakshatraIdx = static_cast<int>(std::floor(getNakshatra(moonLong))) % 27;
  int yogaIdx = static_cast<int>(std::floor(getYoga(sunLong, moonLong))) % 27;
  int karanaIdx = static_cast<int>(std::floor(2 * tithiVal));
  QString karanaName = karanaIdx > 0
                           ? (karanaIdx < 57 ? karanas[(karanaIdx - 1) % 7 + 1]
                                             : karanas[karanaIdx - 57 + 8])
                           : karanas[0];

  // Sun Rashi for debug should also match civil month or we can show both.
  // We'll show the actual one for debug
  int debugSunRashiIdx = static_cast<int>(std::floor(sunLong / 30.0)) % 12;
  int debugMoonRashiIdx = static_cast<int>(std::floor(moonLong / 30.0)) % 12;

  QString debugHtml =
      "<pre style=\"font-family: monospace; font-size: 8pt; color: white; "
      "white-space: pre; line-height: 1.2;\">"
      "<span style=\"font-size: 10pt; color: #00CED1;\">Debug Information "
      "(Native Engine):</span>\n"
      "Gregorian Date: " +
      date.toString("yyyy-MM-dd") +
      "\n"
      "<span style=\"color: #B118E7;\">--- Solar Outputs ---</span>\n"
      "sunRashi: " +
      rashis[debugSunRashiIdx] +
      " | index: " + QString::number(debugSunRashiIdx + 1) +
      "\n"
      "<span style=\"color: #B118E7;\">--- Lunar Outputs ---</span>\n"
      "tithi: " +
      tithiName + " | index: " + QString::number(tDay) +
      "\n"
      "tithiAngle: " +
      QString::number(tithiVal * 12.0, 'f', 4) +
      "°\n"
      "paksha: " +
      paksha +
      "\n"
      "nakshatra: " +
      nakshatras[nakshatraIdx] +
      " | index: " + QString::number(nakshatraIdx + 1) +
      "\n"
      "yoga: " +
      yogas[yogaIdx] + " | index: " + QString::number(yogaIdx + 1) +
      "\n"
      "yogaAngle: " +
      QString::number(zero360(sunLong + moonLong), 'f', 4) +
      "°\n"
      "karana: " +
      karanaName + " | index: " + QString::number(karanaIdx) +
      "\n"
      "karanaAngle: " +
      QString::number(2 * tithiVal, 'f', 4) +
      "\n"
      "moonRashi: " +
      rashis[debugMoonRashiIdx] +
      " | index: " + QString::number(debugMoonRashiIdx + 1) +
      "\n"
      "<span style=\"color: #B118E7;\">--- Astronomical Positions ---</span>\n"
      "Sidereal Sun: " +
      QString::number(sunLong, 'f', 4) +
      "°\n"
      "Tropical Sun: " +
      QString::number(tropicalSun, 'f', 4) +
      "°\n"
      "Sidereal Moon: " +
      QString::number(moonLong, 'f', 4) +
      "°\n"
      "Tropical Moon: " +
      QString::number(tropicalMoon, 'f', 4) +
      "°\n"
      "Ayanamsa (Lahiri): " +
      QString::number(ayanamsa, 'f', 4) +
      "°\n"
      "Elongation (Moon - Sun): " +
      QString::number(zero360(moonLong - sunLong), 'f', 4) +
      "°\n"
      "<span style=\"color: #B118E7;\">--- Metadata ---</span>\n"
      "Target Location: Lat " +
      QString::number(lat, 'f', 4) + ", Lon " + QString::number(lon, 'f', 4) +
      "\n"
      "Sunrise Time: " +
      sunriseStr +
      "\n"
      "Ahar (at Sunrise): " +
      QString::number(ahar, 'f', 6) +
      "\n"
      "Julian Day (UTC): " +
      QString::number(ahar + KaliEpoch - (lon / 360.0), 'f', 6) +
      "\n"
      "</pre>";

  res["debug"] = debugHtml;
  return res;
}

void PanchangaCalculator::clearCache() { calculationCache.clear(); }

// -----------------------------------------------------------------------------
// Transits and Timings
// -----------------------------------------------------------------------------

QString PanchangaCalculator::formatAharTime(double ahar, const QDate &baseDate,
                                            double lon, double tz, bool isBikramSambat,
                                            double nextSunriseAhar) const {
  if (ahar == -1.0)
    return "N/A";

  double jdUTC = ahar + KaliEpoch - (lon / 360.0);
  double jdLocal = jdUTC + (tz / 24.0);

  double jdnDouble = std::floor(jdLocal + 0.5);
  double fraction = (jdLocal + 0.5) - jdnDouble;
  
  double totalSeconds = fraction * 86400.0;
  int displaySeconds = static_cast<int>(std::floor(totalSeconds));
  displaySeconds = (displaySeconds % 86400 + 86400) % 86400;

  QTime targetTime = QTime(0, 0).addSecs(displaySeconds);
  
  QString timeStr;
  if (isBikramSambat) {
      int h = targetTime.hour();
      int m = targetTime.minute();
      int s = targetTime.second();
      
      QString period;
      if (h < 12) period = "बिहान";
      else if (h < 17) period = "दिउँसो";
      else if (h < 20) period = "साँझ";
      else period = "राति";
      
      int displayH = h % 12;
      if (displayH == 0) displayH = 12;
      
      timeStr = toDevanagari(QString::number(displayH)) + ":" +
                toDevanagari(QString("%1").arg(m, 2, 10, QChar('0'))) + ":" +
                toDevanagari(QString("%1").arg(s, 2, 10, QChar('0'))) + " " + period;
  } else {
      timeStr = targetTime.toString("hh:mm:ss AP");
  }
  
  QDate targetDate = fromJulianDay(jdLocal);

  if (targetDate > baseDate) {
      bool isPostMidnightVedic = false;
      if (nextSunriseAhar != -1.0) {
          isPostMidnightVedic = (ahar < nextSunriseAhar);
      } else {
          isPostMidnightVedic = targetTime.hour() < 6;
      }

      if (isPostMidnightVedic) {
          int daysDiff = baseDate.daysTo(targetDate);
          int extendedHour = targetTime.hour() + daysDiff * 24;
          QString extendedStr = QString("(%1:%2:%3)")
                                    .arg(extendedHour)
                                    .arg(targetTime.minute(), 2, 10, QChar('0'))
                                    .arg(targetTime.second(), 2, 10, QChar('0'));
          if (isBikramSambat) {
              extendedStr = toDevanagari(extendedStr);
          }
          timeStr += " " + extendedStr;
      }
  }

  if (targetDate != baseDate) {
      if (isBikramSambat) {
          QVariantMap bsTarget = toBikramSambat(targetDate);
          QVariantMap bsBase = toBikramSambat(baseDate);
          int targetMonthIdx = bsTarget["monthIndex"].toInt();
          int targetDay = bsTarget["day"].toInt();
          int baseMonthIdx = bsBase["monthIndex"].toInt();
          
          QString dayStr = toDevanagari(QString::number(targetDay));
          if (targetMonthIdx != baseMonthIdx) {
              QStringList nepaliMonths = getSolarMonths();
              QString monthName = (targetMonthIdx >= 0 && targetMonthIdx < nepaliMonths.size())
                                      ? nepaliMonths[targetMonthIdx]
                                      : "";
              return monthName + " " + dayStr + " गते " + timeStr;
          } else {
              return dayStr + " गते " + timeStr;
          }
      } else {
          int targetMonth = targetDate.month();
          int baseMonth = baseDate.month();
          int targetDay = targetDate.day();
          if (targetMonth != baseMonth) {
              QString monthName = QLocale::system().monthName(targetMonth, QLocale::ShortFormat);
              return monthName + " " + QString::number(targetDay) + " " + timeStr;
          } else {
              return QString::number(targetDay) + " " + timeStr;
          }
      }
  }
  return timeStr;
}

double
PanchangaCalculator::findTransit(double searchStartAhar,
                                 std::function<double(double)> getValueFunc,
                                 double targetValue, double maxDays, double wrapValue) const {
  double v0 = getValueFunc(searchStartAhar);
  double halfWrap = wrapValue / 2.0;
  auto getUnwrapped = [&](double ah) {
    double v = getValueFunc(ah);
    while (v < v0 - halfWrap)
      v += wrapValue;
    while (v > v0 + halfWrap)
      v -= wrapValue;
    return v;
  };

  double target = targetValue;
  while (target < v0 - halfWrap)
    target += wrapValue;
  while (target > v0 + halfWrap)
    target -= wrapValue;

  double low = searchStartAhar;
  double high = searchStartAhar + maxDays;
  double startVal = getUnwrapped(low);
  double endVal = getUnwrapped(high);

  if (startVal <= target && target <= endVal) {
    // Current window ok
  } else if (target < startVal) {
    low = searchStartAhar - maxDays;
    high = searchStartAhar;
    startVal = getUnwrapped(low);
    endVal = getUnwrapped(high);
    if (!(startVal <= target && target <= endVal)) {
      return -1.0;
    }
  } else {
    return -1.0;
  }

  for (int i = 0; i < 20; i++) {
    double mid = (low + high) / 2.0;
    if (getUnwrapped(mid) < target) {
      low = mid;
    } else {
      high = mid;
    }
  }
  return high;
}

QVariantList PanchangaCalculator::findElementsForDay(
    double startAhar, double endAhar,
    std::function<double(double)> getValueFunc, double divisor,
    const QStringList &nameArray, const QDate &baseDate, double lon, double tz, bool isBikramSambat,
    std::function<QString(int)> getSpecialName,
    double sunriseAhar, double nextSunriseAhar, double explicitWrapValue) const {

  QVariantList elements;

  double valAtStart = getValueFunc(startAhar);
  int currentIndex = static_cast<int>(std::floor(valAtStart / divisor));
  double targetEndValue = (currentIndex + 1) * divisor;

  double wrapValue = (explicitWrapValue != -1.0) ? explicitWrapValue : (nameArray.size() * divisor);
  
  double startTimeAhar =
      findTransit(startAhar, getValueFunc, currentIndex * divisor, 2.0, wrapValue);
  double endTimeAhar =
      findTransit(startTimeAhar != -1.0 ? startTimeAhar : startAhar,
                  getValueFunc, targetEndValue, 2.0, wrapValue);

  QString name = getSpecialName ? getSpecialName(currentIndex)
                                : nameArray[currentIndex % nameArray.size()];

  if (sunriseAhar != -1.0 && nextSunriseAhar != -1.0) {
      if (startTimeAhar > sunriseAhar && endTimeAhar < nextSunriseAhar) {
          name += " <span style=\"color:#ef4444\">(क्षय)</span>";
      } else if (startTimeAhar < sunriseAhar && endTimeAhar > nextSunriseAhar) {
          name += " (वृद्धि)";
      }
  }

  QVariantMap el;
  el["index"] = currentIndex;
  el["name"] = name;
  el["startTime"] = formatAharTime(startTimeAhar, baseDate, lon, tz, isBikramSambat, nextSunriseAhar);
  el["endTime"] = formatAharTime(endTimeAhar, baseDate, lon, tz, isBikramSambat, nextSunriseAhar);
  elements.append(el);

  while (endTimeAhar != -1.0 && endTimeAhar < endAhar) {
    double currentStartTimeAhar = endTimeAhar;
    currentIndex++;
    targetEndValue = (currentIndex + 1) * divisor;

    endTimeAhar =
        findTransit(currentStartTimeAhar, getValueFunc, targetEndValue, 2.0, wrapValue);
    name = getSpecialName ? getSpecialName(currentIndex)
                          : nameArray[currentIndex % nameArray.size()];

    if (sunriseAhar != -1.0 && nextSunriseAhar != -1.0) {
        if (currentStartTimeAhar > sunriseAhar && endTimeAhar < nextSunriseAhar) {
            name += " <span style=\"color:#ef4444\">(क्षय)</span>";
        } else if (currentStartTimeAhar < sunriseAhar && endTimeAhar > nextSunriseAhar) {
            name += " (वृद्धि)";
        }
    }

    if (endTimeAhar != -1.0 && endTimeAhar > currentStartTimeAhar) {
      QVariantMap nextEl;
      nextEl["index"] = currentIndex;
      nextEl["name"] = name;
      nextEl["startTime"] =
          formatAharTime(currentStartTimeAhar, baseDate, lon, tz, isBikramSambat, nextSunriseAhar);
      nextEl["endTime"] = formatAharTime(endTimeAhar, baseDate, lon, tz, isBikramSambat, nextSunriseAhar);
      elements.append(nextEl);
    } else {
      if (!elements.isEmpty() &&
          elements.last().toMap()["name"].toString() != name) {
        QVariantMap lastEl;
        lastEl["index"] = currentIndex;
        lastEl["name"] = name;
        lastEl["startTime"] =
            formatAharTime(currentStartTimeAhar, baseDate, lon, tz, isBikramSambat, nextSunriseAhar);
        lastEl["endTime"] =
            formatAharTime(endTimeAhar, baseDate, lon, tz, isBikramSambat, nextSunriseAhar);
        elements.append(lastEl);
      }
      break;
    }
  }
  return elements;
}

QVariantMap PanchangaCalculator::getDailyElementsAndTimings(
    double ahar, const QDate &baseDate, double lat, double lon, double tz, double sunriseMinutes, bool isBikramSambat) const {
  double midnightAhar = ahar - (sunriseMinutes / 1440.0);
  double startAhar = midnightAhar;
  double endAhar = ahar + 1.15;

  QVariantMap res;

  double nextSunriseMinutes = getSunriseSunset(baseDate.addDays(1), lat, lon)["sunriseMinutes"].toDouble();
  double nextSunriseAhar = midnightAhar + 1.0 + (nextSunriseMinutes / 1440.0);

  res["tithis"] = findElementsForDay(
      startAhar, endAhar,
      [&](double a) {
        return getTithi(analyticalSunLong(a), analyticalMoonLong(a));
      },
      1.0, tithiNamesList, baseDate, lon, tz, isBikramSambat, [&](int idx) {
        int tNum = std::abs(idx) % 30 + 1;
        int nIdx = (tNum > 15) ? tNum - 16 : tNum - 1;
        if (tNum == 15)
          nIdx = 14;
        else if (tNum == 30)
          nIdx = 15;
        return tithiNamesList[nIdx];
      }, ahar, nextSunriseAhar, 30.0);

  res["nakshatras"] = findElementsForDay(
      startAhar, endAhar, [&](double a) { return getNakshatra(analyticalMoonLong(a)); },
      1.0, nakshatras, baseDate, lon, tz, isBikramSambat, nullptr, -1.0, nextSunriseAhar, 27.0);
      
  res["yogas"] = findElementsForDay(
      startAhar, endAhar,
      [&](double a) {
        return getYoga(analyticalSunLong(a), analyticalMoonLong(a));
      },
      1.0, yogas, baseDate, lon, tz, isBikramSambat, nullptr, -1.0, nextSunriseAhar, 27.0);
      
  res["karanas"] = findElementsForDay(
      startAhar, endAhar,
      [&](double a) {
        return 2.0 * getTithi(analyticalSunLong(a), analyticalMoonLong(a));
      },
      1.0, karanas, baseDate, lon, tz, isBikramSambat, [&](int idx) {
        int nIdx = std::abs(idx) % 60;
        if (nIdx == 0) return karanas[0];
        if (nIdx < 57) return karanas[(nIdx - 1) % 7 + 1];
        return karanas[nIdx - 57 + 8];
      }, -1.0, nextSunriseAhar, 60.0);

  return res;
}
