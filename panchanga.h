#ifndef PANCHANGA_H
#define PANCHANGA_H

#include <cmath>
#include <string>
#include <ctime>

#define PI 3.14159265358979323846

// --- Constants ---
constexpr double D2R = PI / 180.0;
constexpr double R2D = 180.0 / PI;
constexpr double NEPAL_OFFSET = 5.75;
constexpr double KATHMANDU_LAT = 27.7172;
constexpr double KATHMANDU_LON = 85.324;

extern const char* tithi[];
extern const char* paksha[];
extern const char* nakshatra[];
extern const char* rashi[];
extern const char* karan[];
extern const char* yoga[];
// --- Utility functions ---
inline double REV(double x) { return x - std::floor(x / 360.0) * 360.0; }
inline std::string pad(int n) { return (n < 10 ? "0" : "") + std::to_string(n); }
std::string formatAMPM(int hours, int minutes);

double julianDay(int yy, int mm, int dd, double hour = 0.0, double zhr = NEPAL_OFFSET);
std::string calculateSunriseOrSunset(const std::tm& date, bool isSunrise, double latitude = KATHMANDU_LAT, double longitude = KATHMANDU_LON, double nepalOffset = NEPAL_OFFSET);

struct TithiResult {
    int tithiIndex;
    std::string tithiName;
    std::string paksha;
    int pakshaIndex;
};
TithiResult calculateTithi(const std::tm& date);

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


NakshatraResult calculateNakshatra(const std::tm& date);
YogaResult calculateYoga(const std::tm& date);
KaranResult calculateKaran(const std::tm& date);
RashiResult calculateRashi(const std::tm& date);

class Panchang {
public:
    double tdays;
    double t, tithi_index;
    std::string paksha;

    // Constructor accepting a Julian date
    Panchang(double julianDate) {
        tdays = julianDate - 2451545.0;  // Days since J2000.0
        t = tdays / 36525.0;
        calculate();
    }

    void calculate() {
        calculateTithi();
    }

private:
    void calculateTithi();
    double getMoonLongitude();
    double getSunLongitude();
};

double gregorianToJulian(int year, int month, int day);

#endif // PANCHANGA_H
