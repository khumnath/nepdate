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

// --- Nepali names ---
static const char tithi[][30] = { "प्रतिपदा", "द्वितीया", "तृतीया", "चतुर्थी", "पंचमी", "षष्ठी", "सप्तमी", "अष्टमी",
                                 "नवमी", "दशमी", "एकादशी", "द्वादशी", "त्रयोदशी", "चतुर्दशी", "पूर्णिमा", "प्रतिपदा",
                                 "द्वितीया", "तृतीया", "चतुर्थी", "पंचमी", "षष्ठी", "सप्तमी", "अष्टमी", "नवमी", "दशमी",
                                 "एकादशी", "द्वादशी", "त्रयोदशी", "चतुर्दशी", "अमावस्या" };
static const char* paksha[] = { "शुक्ल पक्ष", "कृष्ण पक्ष" };
static const char* nakshatra[] = { "अश्विनी", "भरणी", "कृत्तिका", "रोहिणी", "मृगशिरा", "आर्द्रा", "पुनर्वसु", "पुष्य", "आश्रेषा", "मघा", "पूर्वा फाल्गुनी", "उत्तर फाल्गुनी", "हस्त", "चित्रा", "स्वाति", "विशाखा", "अनुराधा", "ज्येष्ठा", "मूला", "पूर्वाषाढा", "उत्तराषाढा", "श्रवण", "श्रविष्ठा", "शतभिषा", "पूर्वा भाद्रपदा", "उत्तर भाद्रपदा", "रेवती" };
static const char* rashi[] = { "मेष", "वृष", "मिथुन", "कर्कट", "सिंह", "कन्या", "तुला", "वृश्चिक", "धनु", "मकर", "कुम्भ", "मीन" };
static const char* karan[] = { "बव", "बालव", "कौलव", "तैतिल", "गर", "वणिज", "विष्टि", "शकुनि", "चतुष्पद", "नाग", "किंस्तुघ्न" };
static const char* yoga[] = { "विष्कुम्भ", "प्रीति", "आयुष्मान्", "सौभाग्य", "शोभन", "अतिगण्ड", "सुकर्मा", "धृति", "शूल", "गण्ड", "वृद्धि", "ध्रुव", "व्याघात", "हर्षण", "वज्र", "सिद्धि", "व्यतीपात", "वरीयान्", "परिघ", "शिव", "सिद्ध", "साध्य", "शुभ", "शुक्ल", "ब्रह्म", "इन्द्र", "वैधृति" };

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
