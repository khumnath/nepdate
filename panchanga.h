#ifndef PANCHANGA_H
#define PANCHANGA_H

#include <cmath>
#include <string>

#define PI 3.14159265358979323846
#define r2d (180.0 / PI)
#define d2r (PI / 180.0)

// Tithi names in Nepali
static const char tithi[][30] = { "प्रतिपदा", "द्वितीया", "तृतीया", "चतुर्थी", "पंचमी", "षष्ठी", "सप्तमी", "अष्टमी",
                                 "नवमी", "दशमी", "एकादशी", "द्वादशी", "त्रयोदशी", "चतुर्दशी", "पूर्णिमा", "प्रतिपदा",
                                 "द्वितीया", "तृतीया", "चतुर्थी", "पंचमी", "षष्ठी", "सप्तमी", "अष्टमी", "नवमी", "दशमी",
                                 "एकादशी", "द्वादशी", "त्रयोदशी", "चतुर्दशी", "अमावस्या" };

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
    void calculateTithi() {
        double moon_longitude = getMoonLongitude();
        double sun_longitude = getSunLongitude();

        // Adjust for Nepal time (UTC+5:45)
        double nepalTimeOffset = 5.75; // in hours
        double localTimeAdjustment = nepalTimeOffset * 15; // Convert hours to degrees

        // Adjust the longitudes
        moon_longitude += localTimeAdjustment;
        sun_longitude += localTimeAdjustment;

        // Wrap around to keep within 0-360 degrees
        moon_longitude = fmod(moon_longitude + 360.0, 360.0);
        sun_longitude = fmod(sun_longitude + 360.0, 360.0);

        // Calculate the difference
        double difference = moon_longitude - sun_longitude;
        if (difference < 0) difference += 360.0;

        tithi_index = std::floor(difference / 12.0);
        paksha = (tithi_index < 15) ? "शुक्ल पक्ष" : "कृष्ण पक्ष";

        // Debug print
       // std::cout << "Tithi: " << tithi[(int)tithi_index] << " (" << paksha << ")" << std::endl;
    }

    double getMoonLongitude() {
        double L1 = 218.316 + 481267.8813 * t;
        double D = 297.8502 + 445267.1115 * t;
        // double M = 357.5291 + 35999.0503 * t;
        double M1 = 134.963 + 477198.8671 * t;
        // double F = 93.272 + 483202.0175 * t;

        double moonLongitude = L1
                               + (6.289 * sin(M1 * M_PI / 180.0))
                               - (1.274 * sin((2 * D - M1) * M_PI / 180.0))
                               - (0.658 * sin(2 * D * M_PI / 180.0))
                               - (0.214 * sin(2 * M1 * M_PI / 180.0))
                               + (0.11 * sin(D * M_PI / 180.0));

        return fmod(moonLongitude, 360.0);
    }

    double getSunLongitude() {
        double l0 = 280.4665 + 36000.7698 * t;
        double m = 357.5291 + 35999.0503 * t;
        double c = (1.9146 - 0.004817 * t - 0.000014 * t * t) * sin(m * M_PI / 180.0)
                   + (0.019993 - 0.000101 * t) * sin(2 * m * M_PI / 180.0)
                   + 0.000289 * sin(3 * m * M_PI / 180.0);

        double sunLongitude = l0 + c;
        return fmod(sunLongitude, 360.0);
    }
};

double gregorianToJulian(int year, int month, int day);

#endif // PANCHANGA_H
