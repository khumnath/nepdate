#include <QCoreApplication>
#include <iostream>
#include "panchangacalculator.h"

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);
    PanchangaCalculator calc;
    double ahar = BikramCalendar::findAharForBsDate(&calc, 1, 0, 1);
    std::cout << "C++ ahar: " << ahar << "\n";
    return 0;
}
