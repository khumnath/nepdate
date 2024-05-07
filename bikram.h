// file name: bikram.h
/* bikram library  to calculate bikram sambat calendar date from a given */
/* gregorian date.                                                             */
/*                                                                             */
/* This  is a port of the Perl script to c++ to calculate bikram sambat date*/
/* implementation of the Pancanga (Hindu Almanac) by M. Fushimi and M. Yano is wide area like panchanga nakshatra moon and sun faces with longitude based calculations*/
/* which is docuemnted at following locations                                  */
/* http://www.cc.kyoto-su.ac.jp/~yanom/pancanga/message314.html                */
/* http://www.cc.kyoto-su.ac.jp/~yanom/sanskrit/pancanga/pancanga3.14          */
/* This is ported  and implemented  by Shajeeb T Muhammad                      */
/* And modified for bikram sambat(vikram samvat) by khumnath<mail.@khumnath.com.np*/
/* visit https://github.com/khumnath or https://khumnath.com.np for contact
License: GNU LGPL v3.0
TERMS OF USE:
    Permission is granted to use this code, with or
    without modification, in any website or application
    provided that credit is given to the original work
    with a link back to PrayTimes.org.
This program is distributed in the hope that it will
be useful, but WITHOUT ANY WARRANTY.
PLEASE DO NOT REMOVE THIS COPYRIGHT BLOCK.
------------------------------------------------------------------------------
*/

#include <ctime>
#include <cmath> // Include this header for math functions
#include <string>

class bikram
{


private:
    int Year=0;
    int Month=-1;
    int Day=0;


    double YugaRotation_star = 1582237828;
    double YugaRotation_sun  = 4320000;
    double YugaCivilDays;
    double PlanetApogee_sun  = 77 + (float)17 / 60;
    double PlanetCircumm_sun = 13 + (float)50 / 60;
    //enable if need on calculation. //double rad = 57.2957795; // = 180/pi
    double rad=57.2957795; // = 180/pi

    void get_saura_masa_day (long ahar, int *m, int *d) {
        double tslong_tomorrow;
        int month;
        int day;
        if (today_saura_masa_first_p(ahar)) {

            day = 1;
            tslong_tomorrow = get_tslong(ahar + 1);
            month = (long)(tslong_tomorrow / 30) % 12;
            month = (month + 12) % 12;

        } else {
            get_saura_masa_day(ahar - 1, &month, &day);
            day = day + 1;

        }
        *m = month;
        *d = day;

    }

    int today_saura_masa_first_p(long ahar) {
        double tslong_today;
        double tslong_tomorrow;

        tslong_today = get_tslong(ahar);
        tslong_tomorrow = get_tslong(ahar + 1);

        tslong_today = tslong_today - (int)(tslong_today / 30) * 30;
        tslong_tomorrow = tslong_tomorrow - (int)(tslong_tomorrow / 30) * 30;

        if ((25 < tslong_today) && (tslong_tomorrow < 5)) {
            return 1;
        } else {
            return 0;
        }

    }

    double get_tslong (long ahar) {
        double mslong;
        //long k1;
        double t1  = (YugaRotation_sun * ahar / YugaCivilDays);

        t1 = t1 - (long)t1;
        mslong = 360 * t1;

        double x1 = mslong - PlanetApogee_sun;
        double y1 = PlanetCircumm_sun / 360;

        double y2 = sin(x1 / rad); // Use sin from cmath
        double y3 = y1 * y2;
        double x2 = asin(y3) * rad; // Use asin from cmath
        double x3 = mslong - x2;

        return x3;

    }

    double get_julian_date(int year, int month, int day)
    {
        if (month <= 2)
        {
            year -= 1;
            month += 12;
        }

        double a = floor(year / 100.0); // Use floor from cmath
        double b = 2 - a + floor(a / 4.0); // Use floor from cmath

        return floor(365.25 * (year + 4716)) + floor(30.6001 * (month + 1)) + day + b - 1524.5;
    }



public:

    void fromGregorian (int y, int m, int d) {

        YugaCivilDays = YugaRotation_star - YugaRotation_sun;
        double julian=get_julian_date(y,m,d);
        long ahar = julian  - 588465.5;
        int saura_masa_num;
        int saura_masa_day;
        get_saura_masa_day(ahar, &saura_masa_num, &saura_masa_day);
        long YearKali = (long)(ahar * YugaRotation_sun / YugaCivilDays);
        int YearSaka  = YearKali - 3179 ;
        int nepalimonth = (saura_masa_num + 12) % 12;
        Year = YearSaka + 134  + (int)((saura_masa_num - nepalimonth + 12) / 12);
        Month = (saura_masa_num + 12) % 12;
        Day = saura_masa_day;

    }
    int getDayOfWeek() {
        std::tm timeinfo = { 0, 0, 0, Day, Month - 1, Year - 1900, 0, 0, 0, 0, "" };
         // Note: Month is 0-based in std::tm, and years should be years since 1900
        std::mktime(&timeinfo); // Update timeinfo to fill in the week day field

        return timeinfo.tm_wday;
    }

    int getYear () { return Year; }
    int getMonth () { return Month+1; }
    int getDay () { return Day; }
};
