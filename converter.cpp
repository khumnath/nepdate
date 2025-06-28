#include "converter.h"
#include "panchanga.h"
double toJulian(int year, int month, int day) {
    return gregorianToJulian(year, month, day);
}

QString getTithiName(int year, int month, int day, QString& paksha) {
    double julian = toJulian(year, month, day);
    Panchang panchang(julian);
    paksha = QString::fromStdString(panchang.paksha);
    return QString::fromStdString(tithi[(int)panchang.tithi_index]);
}
