#ifndef PANCHANGA_H
#define PANCHANGA_H

#include <cmath>
#include <string>
#include <ctime>
#include <vector>

// Constants
constexpr double PI = 3.14159265358979323846;
constexpr double D2R = PI / 180.0;
constexpr double R2D = 180.0 / PI;
constexpr double NEPAL_OFFSET = 5.75;
constexpr double KATHMANDU_LAT = 27.7172;
constexpr double KATHMANDU_LON = 85.324;

// Panchang arrays
extern const char* tithi[];
extern const char* paksha[];
extern const char* nakshatra[];
extern const char* rashi[];
extern const char* karan[];
extern const char* yoga[];

// Utility functions
inline double REV(double x) {
    double r = std::fmod(x, 360.0);
    return (r < 0) ? r + 360.0 : r;
}
std::string pad(int n);
std::string formatAMPM(int hours, int minutes);
double julianDay(int yy, int mm, int dd, double hour = 0.0, double zhr = NEPAL_OFFSET);
std::string calculateSunriseOrSunset(const std::tm& date, bool isSunrise,
                                     double latitude = KATHMANDU_LAT,
                                     double longitude = KATHMANDU_LON,
                                     double nepalOffset = NEPAL_OFFSET);

// Result structures
struct TithiResult {
    int tithiIndex;
    std::string tithiName;
    std::string paksha;
    int pakshaIndex;
    double elongation = 0;
};

struct NakshatraResult {
    int nakshatraIndex;
    std::string nakshatraName;
};

struct YogaResult {
    int yogaIndex;
    std::string yogaName;
};

struct KaranResult {
    int karanIndex;
    std::string karanName;
};

struct RashiResult {
    int rashiIndex;
    std::string rashiName;
};

// Astronomical calculation functions
double calc_ayanamsa(double d);
double sun_long(double d);
double moon_long(double d);

// Panchanga calculation functions
TithiResult calculateTithi(const std::tm& date);
NakshatraResult calculateNakshatra(const std::tm& date);
YogaResult calculateYoga(const std::tm& date);
KaranResult calculateKaran(const std::tm& date);
RashiResult calculateRashi(const std::tm& date);

// Utility conversion
double gregorianToJulian(int year, int month, int day);

#endif // PANCHANGA_H
