#ifndef CALENDARTABLE_H
#define CALENDARTABLE_H

#include <QObject>
#include <QTableWidget>
#include <QString>
#include <QDate>
#include "bikram.h"
#include <QComboBox>

class CalendarTableHelper {
public:
    static void updateCalendar(QTableWidget* table, Bikram& converter, int year, int month, int& gYear, int& gMonth, int& gDay);
    static void adjustCellSizes(QTableWidget* table);
    static void populateBsDays(QComboBox* dayCombo, Bikram& converter, int year, int month);
};

#endif // CALENDARTABLE_H
