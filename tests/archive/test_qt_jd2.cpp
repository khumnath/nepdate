#include <QCoreApplication>
#include <QDate>
#include <iostream>
int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);
    std::cout << QDate::fromJulianDay(1700678).toString(Qt::ISODate).toStdString() << "\n";
    return 0;
}
