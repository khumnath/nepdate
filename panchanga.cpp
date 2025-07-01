#include "panchanga.h"
#include <cmath>
#include <ctime>
#include <sstream>
#include <iomanip>
#include <string>

// Panchang arrays
const char* tithi[] = { "प्रतिपदा", "द्वितीया", "तृतीया", "चतुर्थी", "पञ्चमी", "षष्ठी", "सप्तमी", "अष्टमी", "नवमी", "दशमी", "एकादशी", "द्वादशी", "त्रयोदशी", "चतुर्दशी", "पूर्णिमा", "प्रतिपदा", "द्वितीया", "तृतीया", "चतुर्थी", "पञ्चमी", "षष्ठी", "सप्तमी", "अष्टमी", "नवमी", "दशमी", "एकादशी", "द्वादशी", "त्रयोदशी", "चतुर्दशी", "औंसी" };
const char* paksha[] = { "शुक्ल पक्ष", "कृष्ण पक्ष" };
const char* nakshatra[] = { "अश्विनी", "भरणी", "कृत्तिका", "रोहिणी", "मृगशिरा", "आर्द्रा", "पुनर्वसु", "पुष्य", "आश्रेषा", "मघा", "पूर्वा फाल्गुनी", "उत्तर फाल्गुनी", "हस्त", "चित्रा", "स्वाति", "विशाखा", "अनुराधा", "ज्येष्ठा", "मूला", "पूर्वाषाढा", "उत्तराषाढा", "श्रवण", "श्रविष्ठा", "शतभिषा", "पूर्वा भाद्रपदा", "उत्तर भाद्रपदा", "रेवती" };
const char* rashi[] = { "मेष", "वृष", "मिथुन", "कर्कट", "सिंह", "कन्या", "तुला", "वृश्चिक", "धनु", "मकर", "कुम्भ", "मीन" };
const char* karan[] = { "बव", "बालव", "कौलव", "तैतिल", "गर", "वणिज", "विष्टि", "शकुनि", "चतुष्पद", "नाग", "किंस्तुघ्न" };
const char* yoga[] = { "विष्कुम्भ", "प्रीति", "आयुष्मान्", "सौभाग्य", "शोभन", "अतिगण्ड", "सुकर्मा", "धृति", "शूल", "गण्ड", "वृद्धि", "ध्रुव", "व्याघात", "हर्षण", "वज्र", "सिद्धि", "व्यतीपात", "वरीयान्", "परिघ", "शिव", "सिद्ध", "साध्य", "शुभ", "शुक्ल", "ब्रह्म", "इन्द्र", "वैधृति" };

// Utility functions
std::string pad(int n) {
    return (n < 10 ? "0" : "") + std::to_string(n);
}


std::string formatAMPM(int hours, int minutes) {
    int h_12 = hours % 12;
    if (h_12 == 0) h_12 = 12;
    std::ostringstream oss;
    oss << h_12 << ":" << std::setw(2) << std::setfill('0')
        << minutes << (hours < 12 ? " AM" : " PM");
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
    double jd = std::floor(365.25 * (yy + 4716)) + std::floor(30.6001 * (mm + 1))
                + dd + B - 1524.5;
    return jd + hourUTC / 24.0;
}

double gregorianToJulian(int year, int month, int day) {
    if (month <= 2) {
        year--;
        month += 12;
    }
    int A = year / 100;
    int B = 2 - A + (A / 4);
    return std::floor(365.25 * (year + 4716))
        + std::floor(30.6001 * (month + 1))
        + day + B - 1524.5;
}

// Astronomical calculations
double calc_ayanamsa(double d) {
    double t = d / 36525.0;
    double p = (5028.796195 - 1.1054348 * t) * t / 3600.0;
    double lahiriCorrection = 23.856045 + 0.0001631 * t;
    return -1 * (p + lahiriCorrection);
}

double sun_long(double d) {
    double w = 282.9404 + 4.70935e-5 * d;
    double e = 0.016709 - 1.151e-9 * d;
    double M = REV(356.0470 + 0.9856002585 * d);
    double E = M + R2D * e * sin(M * D2R) * (1 + e * cos(M * D2R));
    double xv = cos(E * D2R) - e;
    double yv = sqrt(1 - e*e) * sin(E * D2R);
    double v = REV(R2D * atan2(yv, xv));
    return REV(v + w);
}

double moon_long(double d) {
    const double a = 60.2666; // Semi-major axis in Earth radii
    double N = REV(125.1228 - 0.0529538083 * d);
    double i = 5.1454;
    double w = REV(318.0634 + 0.1643573223 * d);
    double e = 0.054900;
    double M = REV(115.3654 + 13.0649929509 * d);
    double L0 = REV(218.316 + 13.176396 * d);

    // Solve Kepler's equation with initial approximation
    double E = M + R2D * e * sin(M * D2R) * (1 + e * cos(M * D2R)); // Better initial guess
    for (int iter = 0; iter < 10; iter++) {
        double E_prev = E;
        double delta = (E_prev - R2D * e * sin(E_prev * D2R) - M) / (1 - e * cos(E_prev * D2R));
        E = E_prev - delta;
        if (std::abs(delta) < 0.0001) break;
    }

    // Use semi-major axis in calculations
    double xv = a * (cos(E * D2R) - e);
    double yv = a * sqrt(1 - e*e) * sin(E * D2R);
    double v = REV(R2D * atan2(yv, xv));
    double r = sqrt(xv*xv + yv*yv);
    double xh = r * (cos(N*D2R)*cos((v+w)*D2R) - sin(N*D2R)*sin((v+w)*D2R)*cos(i*D2R));
    double yh = r * (sin(N*D2R)*cos((v+w)*D2R) + cos(N*D2R)*sin((v+w)*D2R)*cos(i*D2R));
    double moonLon = REV(R2D * atan2(yh, xh));

    // Periodic corrections
    double D = REV(L0 - sun_long(d));
    double Ms = REV(356.0470 + 0.9856002585 * d);
    double F = REV(L0 - N);

    moonLon += -1.274 * sin((M - 2*D) * D2R);
    moonLon += +0.658 * sin(2*D * D2R);
    moonLon += -0.186 * sin(Ms * D2R);
    moonLon += -0.059 * sin((2*M - 2*D) * D2R);
    moonLon += -0.057 * sin((M - 2*D + Ms) * D2R);
    moonLon += +0.053 * sin((M + 2*D) * D2R);
    moonLon += +0.046 * sin((2*D - Ms) * D2R);
    moonLon += +0.041 * sin((M - Ms) * D2R);
    moonLon += -0.035 * sin(D * D2R);
    moonLon += -0.031 * sin((M + Ms) * D2R);
    moonLon += -0.015 * sin((2*F - 2*D) * D2R);
    moonLon += +0.011 * sin((M - 4*D) * D2R);

    return REV(moonLon);
}

// Sunrise/Sunset calculation
std::string calculateSunriseOrSunset(const std::tm& date, bool isSunrise,
                                     double latitude,
                                     double longitude,
                                     double nepalOffset)
{
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
    double L_sun = M_sun + (1.916 * sin(M_sun * D2R)) + (0.020 * sin(2 * M_sun * D2R)) + 282.634;
    L_sun = REV(L_sun);

    double RA_sun = R2D * atan(0.91764 * tan(L_sun * D2R));
    RA_sun = REV(RA_sun);

    // Quadrant adjustment
    double L_quadrant = std::floor(L_sun / 90.0) * 90.0;
    double RA_quadrant = std::floor(RA_sun / 90.0) * 90.0;
    RA_sun += (L_quadrant - RA_quadrant);
    RA_sun /= 15.0;

    double sinDec_sun = 0.39782 * sin(L_sun * D2R);
    double cosDec_sun = cos(asin(sinDec_sun));
    double cosH = (cos(zenith * D2R) - (sinDec_sun * sin(latitude * D2R))) /
                  (cosDec_sun * cos(latitude * D2R));

    if (cosH > 1) return "Sun never rises";
    if (cosH < -1) return "Sun never sets";

    double H = R2D * acos(cosH);
    double H_angle_hours = (isSunrise ? (360.0 - H) : H) / 15.0;
    double T_ut = H_angle_hours + RA_sun - (0.06571 * t_approx) - 6.622;
    double UT_event = T_ut - lngHour;

    // Time normalization
    UT_event = REV(UT_event * 15.0) / 15.0;  // Convert to degrees and back

    double localT_event = UT_event + nepalOffset;
    localT_event = REV(localT_event * 15.0) / 15.0;  // Normalize to 0-24 range

    // Convert to local time
    int hours = static_cast<int>(localT_event);
    double fractional_hour = localT_event - hours;
    int minutes = static_cast<int>(fractional_hour * 60 + 0.5);  // Round to nearest minute

    // Handle 24-hour rollover
    if (hours >= 24) hours -= 24;
    if (hours < 0) hours += 24;

    return pad(hours) + ":" + pad(minutes);
}
// Calculation functions
TithiResult calculateTithi(const std::tm& date) {
    int year = date.tm_year + 1900;
    int month = date.tm_mon + 1;
    int day = date.tm_mday;
    std::string sunriseStr = calculateSunriseOrSunset(date, true);
    if (sunriseStr.find("never") != std::string::npos) {
        return {-1, "", "", -1, 0};
    }
    size_t colonPos = sunriseStr.find(':');
    int sunriseHour = std::stoi(sunriseStr.substr(0, colonPos));
    int sunriseMin = std::stoi(sunriseStr.substr(colonPos + 1));
    double jd = julianDay(year, month, day, sunriseHour + sunriseMin/60.0, NEPAL_OFFSET);
    //different epoch than real epoch to match jd noon at nepal
    double d = jd - 2451543.5;

    double ayanamsa = calc_ayanamsa(d);
    double tropicalSun = sun_long(d);
    double tropicalMoon = moon_long(d);
    double siderealSun = REV(tropicalSun + ayanamsa);
    double siderealMoon = REV(tropicalMoon + ayanamsa);

    double elongation = siderealMoon - siderealSun;
    if (elongation < 0) elongation += 360;

    int tithiIndex = static_cast<int>(std::floor(elongation / 12.0));
    int pakshaIndex = tithiIndex < 15 ? 0 : 1;

    return {tithiIndex,
                tithi[tithiIndex],
                paksha[pakshaIndex],
                pakshaIndex,
                elongation};
}

NakshatraResult calculateNakshatra(const std::tm& date) {
    int year = date.tm_year + 1900;
    int month = date.tm_mon + 1;
    int day = date.tm_mday;
    std::string sunriseStr = calculateSunriseOrSunset(date, true);
    if (sunriseStr.find("never") != std::string::npos) {
        return {-1, ""};
    }
    size_t colonPos = sunriseStr.find(':');
    int sunriseHour = std::stoi(sunriseStr.substr(0, colonPos));
    int sunriseMin = std::stoi(sunriseStr.substr(colonPos + 1));
    double jd = julianDay(year, month, day, sunriseHour + sunriseMin/60.0, NEPAL_OFFSET);
    //different epoch than real epoch to match jd noon at nepal
    double d = jd - 2451543.5;

    double ayanamsa = calc_ayanamsa(d);
    double tropicalMoon = moon_long(d);
    double siderealMoon = REV(tropicalMoon + ayanamsa);

        // Ensure angle is in 0-360 range
        siderealMoon = REV(siderealMoon);

        // Calculate nakshatra index with proper bounds
        double nakshatraSize = 360.0 / 27.0;
        int index = static_cast<int>(siderealMoon / nakshatraSize);

        // Normalize index to 0-26 range
        if (index >= 27) index %= 27;
        if (index < 0) index = (index % 27 + 27) % 27;  // Handle negative

        return {index, nakshatra[index]};
    }

YogaResult calculateYoga(const std::tm& date) {
    int year = date.tm_year + 1900;
    int month = date.tm_mon + 1;
    int day = date.tm_mday;
    std::string sunriseStr = calculateSunriseOrSunset(date, true);
    if (sunriseStr.find("never") != std::string::npos) {
        return {-1, ""};
    }
    size_t colonPos = sunriseStr.find(':');
    int sunriseHour = std::stoi(sunriseStr.substr(0, colonPos));
    int sunriseMin = std::stoi(sunriseStr.substr(colonPos + 1));
    double jd = julianDay(year, month, day, sunriseHour + sunriseMin/60.0, NEPAL_OFFSET);
    //different epoch than real epoch to match jd noon at nepal
    double d = jd - 2451543.5;

    double ayanamsa = calc_ayanamsa(d);
    double tropicalSun = sun_long(d);
    double tropicalMoon = moon_long(d);
    double siderealSun = REV(tropicalSun + ayanamsa);
    double siderealMoon = REV(tropicalMoon + ayanamsa);

    double yogaAngle = REV(siderealSun + siderealMoon);
    int index = static_cast<int>(yogaAngle / (360.0 / 27));
    index = index % 27;  // Ensure within bounds
    return {index, yoga[index]};
}

KaranResult calculateKaran(const std::tm& date) {
    TithiResult res = calculateTithi(date);
    if (res.tithiIndex == -1) return {-1, ""};

    // Calculate karanaSubIndex directly from elongation
    int karanaSubIndex = static_cast<int>(res.elongation / 6.0);

    int index;
    if (karanaSubIndex == 0) {
        index = 10;  // Bava
    } else if (karanaSubIndex == 57) {
        index = 7;   // Chatushpada
    } else if (karanaSubIndex == 58) {
        index = 8;   // Naga
    } else if (karanaSubIndex == 59) {
        index = 9;   // Kimstughna
    } else {
        index = (karanaSubIndex - 1) % 7;
    }

    return {index, karan[index]};
}
RashiResult calculateRashi(const std::tm& date) {
    int year = date.tm_year + 1900;
    int month = date.tm_mon + 1;
    int day = date.tm_mday;
    std::string sunriseStr = calculateSunriseOrSunset(date, true);
    if (sunriseStr.find("never") != std::string::npos) {
        return {-1, ""};
    }
    size_t colonPos = sunriseStr.find(':');
    int sunriseHour = std::stoi(sunriseStr.substr(0, colonPos));
    int sunriseMin = std::stoi(sunriseStr.substr(colonPos + 1));
    double jd = julianDay(year, month, day, sunriseHour + sunriseMin/60.0, NEPAL_OFFSET);
    //different epoch than real epoch to match jd noon at nepal
    double d = jd - 2451543.5;

    double ayanamsa = calc_ayanamsa(d);
    double tropicalMoon = moon_long(d);
    double siderealMoon = REV(tropicalMoon + ayanamsa);

    int index = static_cast<int>(siderealMoon / 30.0) % 12;
    return {index, rashi[index]};
}
