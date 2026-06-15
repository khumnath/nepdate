#include <QCoreApplication>
#include <QDate>
#include <iostream>
#include "src/bikram.h"
#include "src/panchangacalculator.h"

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);
    PanchangaCalculator calc;
    QDate target(1, 1, 1);
    QVariantMap res = BikramCalendar::toBikramSambat(&calc, target);
    std::cout << "C++ BS Year: " << res["year"].toInt() << "\n";
    std::cout << "C++ BS Month: " << res["monthIndex"].toInt() + 1 << "\n";
    std::cout << "C++ BS Day: " << res["day"].toInt() << "\n";
    std::cout << "C++ Day of Week: " << target.dayOfWeek() << " (1=Mon, 7=Sun)\n";
    return 0;
}
