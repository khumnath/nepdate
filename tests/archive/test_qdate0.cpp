#include <QCoreApplication>
#include <QDate>
#include <iostream>

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);
    QDate d0(0, 1, 1);
    QDate dm1(-1, 1, 1);
    QDate d1(1, 1, 1);
    std::cout << "Year 0 isValid: " << d0.isValid() << "\n";
    std::cout << "Year -1 isValid: " << dm1.isValid() << "\n";
    std::cout << "Year 1 isValid: " << d1.isValid() << "\n";
    
    std::cout << "JD for Year 1: " << d1.toJulianDay() << "\n";
    std::cout << "JD for Year 0: " << d0.toJulianDay() << "\n";
    std::cout << "JD for Year -1: " << dm1.toJulianDay() << "\n";
    return 0;
}
