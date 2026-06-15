#include <QCoreApplication>
#include <QDate>
#include <QDebug>

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);
    qDebug() << "QDate(1, 1, 1):" << QDate(1, 1, 1);
    qDebug() << "QDate(0, 1, 1):" << QDate(0, 1, 1);
    qDebug() << "QDate(-1, 1, 1):" << QDate(-1, 1, 1);
    return 0;
}
