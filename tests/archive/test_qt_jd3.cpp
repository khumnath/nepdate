#include <QCoreApplication>
#include <QDate>
#include <iostream>
int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);
    QDate d = QDate::fromJulianDay(1700678);
    std::cout << "Year: " << d.year() << " Month: " << d.month() << " Day: " << d.day() << "\n";
    return 0;
}
