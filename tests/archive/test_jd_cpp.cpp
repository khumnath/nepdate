#include <QCoreApplication>
#include <QDate>
#include <iostream>

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);
    std::cout << "JD 1700000: " << QDate::fromJulianDay(1700000).toString(Qt::ISODate).toStdString() << "\n";
    std::cout << "QDate(-56, 3, 13) to JD: " << QDate(-56, 3, 13).toJulianDay() << "\n";
    std::cout << "QDate(-57, 3, 16) to JD: " << QDate(-57, 3, 16).toJulianDay() << "\n";
    return 0;
}
