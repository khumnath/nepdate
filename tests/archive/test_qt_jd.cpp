#include <QCoreApplication>
#include <QDate>
#include <iostream>
int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);
    QDate d(-57, 3, 16);
    std::cout << "JD: " << d.toJulianDay() << "\n";
    return 0;
}
