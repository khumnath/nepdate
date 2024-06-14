#ifndef PANCHANGA_H
#define PANCHANGA_H

#include <cmath>
#include <string>

#define PI 3.14159265358979323846
#define r2d 180.0 / PI
#define d2r PI / 180.0

// Tithi names in Nepali
static const char tithi[][30] = { "प्रथमा", "द्वितीया", "तृतीया", "चतुर्थी", "पंचमी", "षष्ठी", "सप्तमी", "अष्टमी",
                                 "नवमी", "दशमी", "एकादशी", "द्वादशी", "त्रयोदशी", "चतुर्दशी", "पूर्णिमा", "प्रथमा",
                                 "द्वितीया", "तृतीया", "चतुर्थी", "पंचमी", "षष्ठी", "सप्तमी", "अष्टमी", "नवमी", "दशमी",
                                 "एकादशी", "द्वादशी", "त्रयोदशी", "चतुर्दशी", "अमावास्या" };
class Panchang {
public:
    double tdays;
    double t, tithi_index/*, nakshatra_index, yoga_index*/;
    std::string paksha;


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


        double difference = moon_longitude - sun_longitude;
        if (difference < 0) difference += 360.0;

        tithi_index = std::floor(difference / 12.0);


        paksha = (tithi_index < 15) ? "शुक्ल पक्ष" : "कृष्ण पक्ष";
       //debug print
        //std::cout << "Tithi: " << tithi[(int)tithi_index] << " (" << paksha << ")" << std::endl;
    }




    double getSunLongitude() {
        double l0 = 280.4665 + 36000.7698 * t;
        double m = 357.5291 + 35999.0503 * t;
        double c = (1.9146 - 0.004817 * t - 0.000014 * t * t) * sin(m * d2r)
                   + (0.019993 - 0.000101 * t) * sin(2 * m * d2r)
                   + 0.00029 * sin(3 * m * d2r);
        double theta = l0 + c;
        double lambda = theta - 0.00569 - 0.00478 * sin((125.04 - 1934.136 * t) * d2r);
        return fmod(lambda, 360.0);
    }

    double getMoonLongitude() {
        double L1 = 218.316 + 481267.8813 * t;
        double M1 = 134.963 + 477198.8676 * t;
        double F = 93.272 + 483202.0175 * t;

        L1 = fmod(L1, 360.0);
        M1 = fmod(M1, 360.0);
        F = fmod(F, 360.0);

        double longitude = L1 + 6.289 * sin(M1 * d2r);
        longitude = fmod(longitude, 360.0);
        return longitude;
    }

};

double gregorianToJulian(int year, int month, int day) {
    if (month <= 2) {
        year--;
        month += 12;
    }
    int A = year / 100;
    int B = 2 - A + A / 4;
    double JD = std::floor(365.25 * (year + 4716)) + std::floor(30.6001 * (month + 1)) + day + B - 1524.5;
    return JD;
}


#endif // PANCHANGA_H
