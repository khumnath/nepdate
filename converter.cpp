#include "converter.h"
#include "panchanga.h"
#include <ctime>

double toJulian(int year, int month, int day) {
    return gregorianToJulian(year, month, day);
}

QString getTithiName(int year, int month, int day, QString& paksha) {
    // Create tm struct for the date
    std::tm date = {};
    date.tm_year = year - 1900;
    date.tm_mon = month - 1;
    date.tm_mday = day;

    // Calculate tithi using the new function
    TithiResult result = calculateTithi(date);

    paksha = QString::fromStdString(result.paksha);
    return QString::fromStdString(result.tithiName);
}

