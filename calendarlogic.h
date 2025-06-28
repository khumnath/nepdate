#ifndef CALENDARLOGIC_H
#define CALENDARLOGIC_H
#include <QString>
#include <QStringList>

QString convertToNepaliNumerals(int number);
QString getBikramMonthName(int month);
QString getEnglishMonthName(int month);
extern const QStringList bikramMonths;
extern const QStringList gregorianMonths;

#endif // CALENDARLOGIC_H
