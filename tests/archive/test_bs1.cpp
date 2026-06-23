#include "src/bikram.h"
#include <iostream>
#include <QDate>
int main() {
    double ahar = Bikram::findAharForBsDate(1, 0, 1);
    std::cout << "C++ ahar: " << ahar << std::endl;
    QDate d = Bikram::fromBikramSambat(1, 0, 1);
    std::cout << "C++ date: " << d.year() << "-" << d.month() << "-" << d.day() << std::endl;
    return 0;
}
