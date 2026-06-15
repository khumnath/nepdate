#include <QCoreApplication>
#include <QDate>
#include <iostream>
#include "../src/bikram.h"
#include "../src/panchangacalculator.h"

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);
    PanchangaCalculator calc;
    QDate d = BikramCalendar::fromBikramSambat(&calc, 1, 0, 1);
    std::cout << "C++ date: " << d.year() << "-" << d.month() << "-" << d.day() << std::endl;
    return 0;
}
