#include <QCoreApplication>
#include <QDate>
#include <iostream>
#include "../src/panchangacalculator.h"
#include "../src/bikram.h"

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);
    PanchangaCalculator calc;
    
    QDate d = calc.fromBikramSambat(1, 0, 1);
    QVariantMap res = calc.calculate(d, 27.7172, 85.3240, 5.75);
    std::cout << "C++ Date: Y=" << d.year() << " M=" << d.month() << " D=" << d.day() << "\n";
    std::cout << "C++ JD: " << std::fixed << BikramCalendar::toJulianDay(d) << "\n";
    std::cout << "C++ BS Date: " << res["bsYear"].toInt() << "-" << res["bsMonthIndex"].toInt() + 1 << "-" << res["bsDay"].toInt() << "\n";
    std::cout << "C++ Weekday: " << res["weekday"].toString().toStdString() << "\n";
    std::cout << "C++ Today Ahargana: " << std::fixed << calc.getAhar(d, 85.3240, 5.75) << "\n";

    return 0;
}
