#include <QCoreApplication>
#include <QDebug>
#include <iostream>
#include "panchangacalculator.h"

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);
    PanchangaCalculator calc;
    QDate d = calc.fromBikramSambat(1, 0, 1);
    std::cout << "C++ Date: " << d.toString(Qt::ISODate).toStdString() << "\n";
    std::cout << "C++ JD: " << d.toJulianDay() << "\n";
    return 0;
}
