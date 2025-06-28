#include "panchanga.h"
#include <cmath>
#include <ctime>
#include <sstream>
#include <iomanip>

std::string formatAMPM(int hours, int minutes) {
    int h_12 = hours % 12;
    if (h_12 == 0) h_12 = 12;
    std::ostringstream oss;
    oss << h_12 << ":" << std::setw(2) << std::setfill('0') << minutes << (hours < 12 ? " AM" : " PM");
    return oss.str();
}

double julianDay(int yy, int mm, int dd, double hour, double zhr) {
    double hourUTC = hour - zhr;
    if (mm <= 2) {
        yy -= 1;
        mm += 12;
    }
    int A = yy / 100;
    int B = 2 - A + (A / 4);
    double jd = std::floor(365.25 * (yy + 4716)) + std::floor(30.6001 * (mm + 1)) + dd + B - 1524.5;
    return jd + hourUTC / 24.0;
}

std::string calculateSunriseOrSunset(const std::tm& date, bool isSunrise, double latitude, double longitude, double nepalOffset) {
    int year = date.tm_year + 1900;
    int month = date.tm_mon + 1;
    int day = date.tm_mday;
    double zenith = 90.8333;
    double lngHour = longitude / 15.0;
    int t_approx_factor = isSunrise ? 6 : 18;
    int N1 = std::floor(275 * month / 9.0);
    int N2 = std::floor((month + 9) / 12.0);
    int N3 = (1 + std::floor((year - 4 * std::floor(year / 4.0) + 2) / 3.0));
    int N = N1 - (N2 * N3) + day - 30;
    double t_approx = N + ((t_approx_factor - lngHour) / 24.0);
    double M_sun = (0.9856 * t_approx) - 3.289;
    double L_sun = REV(M_sun + (1.916 * sin(M_sun * D2R)) + (0.020 * sin(2 * M_sun * D2R)) + 282.634);
    double RA_sun = REV(R2D * atan(0.91764 * tan(L_sun * D2R)));
    double L_quadrant = std::floor(L_sun / 90.0) * 90.0;
    double RA_quadrant = std::floor(RA_sun / 90.0) * 90.0;
    RA_sun = RA_sun + (L_quadrant - RA_quadrant);
    RA_sun = RA_sun / 15.0;
    double sinDec_sun = 0.39782 * sin(L_sun * D2R);
    double cosDec_sun = cos(asin(sinDec_sun));
    double cosH = (cos(zenith * D2R) - (sinDec_sun * sin(latitude * D2R))) / (cosDec_sun * cos(latitude * D2R));
    if (cosH > 1) return "Sun never rises";
    if (cosH < -1) return "Sun never sets";
    double H_angle_hours;
    if (isSunrise) {
        H_angle_hours = (360.0 - R2D * acos(cosH)) / 15.0;
    } else {
        H_angle_hours = (R2D * acos(cosH)) / 15.0;
    }
    double T_ut = H_angle_hours + RA_sun - (0.06571 * t_approx) - 6.622;
    double UT_event = T_ut - lngHour;
    UT_event = REV(UT_event * 360.0 / 24.0) / 15.0;
    double localT_event = UT_event + nepalOffset;
    localT_event = REV(localT_event * 360.0 / 24.0) / 15.0;
    int hours_local = static_cast<int>(std::floor(localT_event));
    int minutes_local = static_cast<int>(std::floor((localT_event - hours_local) * 60.0));
    return pad(hours_local) + ":" + pad(minutes_local);
}

double gregorianToJulian(int year, int month, int day) {
    if (month <= 2) {
        year--;
        month += 12;
    }
    int A = year / 100;
    int B = 2 - A + (A / 4);
    double JD = std::floor(365.25 * (year + 4716)) + std::floor(30.6001 * (month + 1)) + day + B - 1524.5;
    return JD;
}

TithiResult calculateTithi(const std::tm& date) {
    int year = date.tm_year + 1900;
    int month = date.tm_mon + 1;
    int day = date.tm_mday;
    std::string sunriseStr = calculateSunriseOrSunset(date, true);
    if (sunriseStr.find("never") != std::string::npos) {
        return {-1, "", "", -1};
    }
    int sunriseHour = std::stoi(sunriseStr.substr(0, 2));
    int sunriseMin = std::stoi(sunriseStr.substr(3, 2));
    double jdAtSunrise = julianDay(year, month, day, sunriseHour + sunriseMin / 60.0, NEPAL_OFFSET);
    // double d_days_since_j2000 = jdAtSunrise - 2451543.5;
    // For simplicity, use the Panchang class for tithi index
    Panchang p(jdAtSunrise);
    int tithiIndex = static_cast<int>(p.tithi_index);
    int pakshaIndex = tithiIndex < 15 ? 0 : 1;
    return {tithiIndex, tithi[tithiIndex], paksha[pakshaIndex], pakshaIndex};
}

NakshatraResult calculateNakshatra(const std::tm& date) {
    int year = date.tm_year + 1900;
    int month = date.tm_mon + 1;
    int day = date.tm_mday;
    std::string sunriseStr = calculateSunriseOrSunset(date, true);
    if (sunriseStr.find("never") != std::string::npos) {
        return {-1, ""};
    }
    int sunriseHour = std::stoi(sunriseStr.substr(0, 2));
    int sunriseMin = std::stoi(sunriseStr.substr(3, 2));
    double jdAtSunrise = julianDay(year, month, day, sunriseHour + sunriseMin / 60.0, NEPAL_OFFSET);
    double d_days_since_j2000 = jdAtSunrise - 2451543.5;
    // Sidereal Moon longitude (simplified, for demo)
    double mlon = 218.316 + 13.176396 * d_days_since_j2000;
    int nakshatraIndex = static_cast<int>(REV(mlon) / (360.0 / 27));
    return {nakshatraIndex, nakshatra[nakshatraIndex]};
}

YogaResult calculateYoga(const std::tm& date) {
    int year = date.tm_year + 1900;
    int month = date.tm_mon + 1;
    int day = date.tm_mday;
    std::string sunriseStr = calculateSunriseOrSunset(date, true);
    if (sunriseStr.find("never") != std::string::npos) {
        return {-1, ""};
    }
    int sunriseHour = std::stoi(sunriseStr.substr(0, 2));
    int sunriseMin = std::stoi(sunriseStr.substr(3, 2));
    double jdAtSunrise = julianDay(year, month, day, sunriseHour + sunriseMin / 60.0, NEPAL_OFFSET);
    double d_days_since_j2000 = jdAtSunrise - 2451543.5;
    // Sidereal Sun longitude (simplified, for demo)
    double slon = 280.4665 + 0.9856474 * d_days_since_j2000;
    double mlon = 218.316 + 13.176396 * d_days_since_j2000;
    double yogaAngle = REV(slon + mlon);
    int yogaIndex = static_cast<int>(yogaAngle / (360.0 / 27));
    return {yogaIndex, yoga[yogaIndex]};
}

KaranResult calculateKaran(const std::tm& date) {
    TithiResult tithiRes = calculateTithi(date);
    int karanIndex = tithiRes.tithiIndex % 7;
    if (tithiRes.tithiIndex == 0) karanIndex = 10;
    else if (tithiRes.tithiIndex == 29) karanIndex = 9;
    else if (tithiRes.tithiIndex == 28) karanIndex = 8;
    return {karanIndex, karan[karanIndex]};
}

RashiResult calculateRashi(const std::tm& date) {
    int year = date.tm_year + 1900;
    int month = date.tm_mon + 1;
    int day = date.tm_mday;
    std::string sunriseStr = calculateSunriseOrSunset(date, true);
    if (sunriseStr.find("never") != std::string::npos) {
        return {-1, ""};
    }
    int sunriseHour = std::stoi(sunriseStr.substr(0, 2));
    int sunriseMin = std::stoi(sunriseStr.substr(3, 2));
    double jdAtSunrise = julianDay(year, month, day, sunriseHour + sunriseMin / 60.0, NEPAL_OFFSET);
    double d_days_since_j2000 = jdAtSunrise - 2451543.5;
    double mlon = 218.316 + 13.176396 * d_days_since_j2000;
    int rashiIndex = static_cast<int>(REV(mlon) / 30.0);
    return {rashiIndex, rashi[rashiIndex]};
}

double getSunLongitude(double d) {
    // Based on the TypeScript logic
    double w = 282.9404 + 4.70935e-5 * d;
    double e = 0.016709 - 1.151e-9 * d;
    double M = REV(356.0470 + 0.9856002585 * d);
    double E = M + R2D * e * sin(M * D2R) * (1 + e * cos(M * D2R));
    double xv = cos(E * D2R) - e;
    double yv = sqrt(1 - e * e) * sin(E * D2R);
    double v = REV(R2D * atan2(yv, xv));
    return REV(v + w);
}

double getMoonLongitude(double d) {
    double N = REV(125.1228 - 0.0529538083 * d);
    double i = 5.1454;
    double w = REV(318.0634 + 0.1643573223 * d);
    double a = 60.2666;
    double e = 0.054900;
    double M = REV(115.3654 + 13.0649929509 * d);
    double L0 = REV(218.316 + 13.176396 * d);
    double E = M + R2D * e * sin(M * D2R) * (1 + e * cos(M * D2R));
    double xv = a * (cos(E * D2R) - e);
    double yv = a * sqrt(1 - e * e) * sin(E * D2R);
    double v_true_anomaly = REV(R2D * atan2(yv, xv));
    double r_dist = sqrt(xv * xv + yv * yv);
    double xh = r_dist * (cos(N * D2R) * cos((v_true_anomaly + w) * D2R) - sin(N * D2R) * sin((v_true_anomaly + w) * D2R) * cos(i * D2R));
    double yh = r_dist * (sin(N * D2R) * cos((v_true_anomaly + w) * D2R) + cos(N * D2R) * sin((v_true_anomaly + w) * D2R) * cos(i * D2R));
    double moonLon = REV(R2D * atan2(yh, xh));
    // Periodic corrections (simplified, as in TS)
    double D_elong = REV(L0 - getSunLongitude(d));
    double Ms_global = REV(356.0470 + 0.9856002585 * d);
    moonLon += -1.274 * sin((M - 2 * D_elong) * D2R);
    moonLon += +0.658 * sin((2 * D_elong) * D2R);
    moonLon += -0.186 * sin(Ms_global * D2R);
    moonLon += -0.059 * sin((2 * M - 2 * D_elong) * D2R);
    moonLon += -0.057 * sin((M - 2 * D_elong + Ms_global) * D2R);
    moonLon += +0.053 * sin((M + 2 * D_elong) * D2R);
    moonLon += +0.046 * sin((2 * D_elong - Ms_global) * D2R);
    moonLon += +0.041 * sin((M - Ms_global) * D2R);
    moonLon += -0.035 * sin(D_elong * D2R);
    moonLon += -0.031 * sin((M + Ms_global) * D2R);
    moonLon += -0.015 * sin((2 * (L0 - N) - 2 * D_elong) * D2R);
    moonLon += +0.011 * sin((M - 4 * D_elong) * D2R);
    return REV(moonLon);
}

void Panchang::calculateTithi() {
    TithiResult calculateTithi(const std::tm& date);
}
