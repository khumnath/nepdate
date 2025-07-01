#ifndef CONVERTOR_H
#define CONVERTOR_H
#include <QString>
#include "bikram.h"

struct BsDate {
    int year;
    int month;
    int day;
};
struct AdDate {
    int year;
    int month;
    int day;
};

// Convert AD to BS
template<typename OutT>
void adToBs(int adYear, int adMonth, int adDay, OutT& outBs) {
    Bikram bikram;
    bikram.fromGregorian(adYear, adMonth, adDay);
    outBs.year = bikram.getYear();
    outBs.month = bikram.getMonth();
    outBs.day = bikram.getDay();
}

// Convert BS to AD
template<typename OutT>
void bsToAd(int bsYear, int bsMonth, int bsDay, OutT& outAd) {
    Bikram bikram;
    int gYear, gMonth, gDay;
    bikram.toGregorian(bsYear, bsMonth, bsDay, gYear, gMonth, gDay);
    outAd.year = gYear;
    outAd.month = gMonth;
    outAd.day = gDay;
}

double toJulian(int year, int month, int day);
QString getTithiName(int year, int month, int day, QString& paksha);

#endif // CONVERTOR_H
