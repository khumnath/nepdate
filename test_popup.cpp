#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "src/panchangacalculator.h"

int main(int argc, char *argv[]) {
    QGuiApplication app(argc, argv);
    PanchangaCalculator calc;
    QVariantMap res = calc.generateDebugInfo(QDate(2024, 5, 1));
    qDebug() << "C++ output:" << res.keys();
    return 0;
}
