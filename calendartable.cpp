#include "calendartable.h"
#include "DayTithiWidget.h"
#include "calendarlogic.h"
#include <QComboBox>
#include <QHeaderView>
#include <QIcon>
#include <QDate>
#include <QTableWidgetItem>
#include <QTableWidget>
#include "panchanga.h"

void CalendarTableHelper::updateCalendar(QTableWidget* table, Bikram& converter, int year, int month, int& gYear, int& gMonth, int& gDay) {
    int daysInMonth = converter.daysInMonth(year, month);
    table->clear();
    table->setRowCount(6);
    table->setColumnCount(7);
    QStringList headers = {"आइत", "सोम", "मङ्गल", "बुध", "बिही", "शुक्र", "शनि"};
    table->setHorizontalHeaderLabels(headers);
    table->horizontalHeader()->setStyleSheet(
        "QHeaderView::section {background-color: #d3d3d3;color: blue;border: 1px solid gray;}"
    );
    table->setStyleSheet(
        "background-color: white;QTableWidget::item {border: 1px solid gray;}QTableWidget .dayLabel {font-size: 24px;color: black;}QTableWidget .tithiLabel {font-size: 8px;color: blue;}"
    );
    converter.toGregorian(year, month, 1, gYear, gMonth, gDay);
    QDate firstDay(gYear, gMonth, gDay);
    int startDay = firstDay.dayOfWeek();
    QDate today = QDate::currentDate();
    converter.fromGregorian(today.year(), today.month(), today.day());
    int todayBsYear = converter.getYear();
    int todayBsMonth = converter.getMonth();
    int todayBsDay = converter.getDay();
    int saturdayIndex = 6;
    QIcon purnimaIcon(":/resources/purnima.png");
    QIcon amavasyaIcon(":/resources/amawasya.png");
    int row = 0;
    int col = (startDay - saturdayIndex + 6) % 7;
    for (int day = 1; day <= daysInMonth; ++day) {
        int cellGYear, cellGMonth, cellGDay;
        converter.toGregorian(year, month, day, cellGYear, cellGMonth, cellGDay);
        double julianDate = gregorianToJulian(cellGYear, cellGMonth, cellGDay);
        Panchang panchang(julianDate);
        QString tithiName = QString::fromStdString(tithi[(int)panchang.tithi_index]);
        QString englishDayStr = QString::number(cellGDay);
        DayTithiWidget *customWidget = new DayTithiWidget(convertToNepaliNumerals(day), tithiName, englishDayStr);
        customWidget->setProperty("year", cellGYear);
        customWidget->setProperty("month", cellGMonth);
        customWidget->setProperty("gDay", cellGDay); // Store actual Gregorian day
        std::tm date_to_pass = {}; // This zero-initializes all members of the struct
        date_to_pass.tm_year = cellGYear - 1900;
        date_to_pass.tm_mon = cellGMonth - 1;
        date_to_pass.tm_mday = cellGDay;
        TithiResult tithiResult = calculateTithi(date_to_pass);
        QString tooltipText = QString("%1 (%2)")
            .arg(QString::fromStdString(tithiResult.tithiName))
            .arg(QString::fromStdString(tithiResult.paksha));
        customWidget->setToolTip(tooltipText);
        QTableWidgetItem *item = new QTableWidgetItem();
        table->setItem(row, col, item);
        bool isToday = (year == todayBsYear && month == todayBsMonth && day == todayBsDay);
        bool isSaturday = (col == saturdayIndex);
        if (isToday) customWidget->setTodayStyle();
        else if (isSaturday) customWidget->setSaturdayStyle();
        if (panchang.tithi_index == 14) customWidget->setIcon(purnimaIcon, 0.9);
        else if (panchang.tithi_index == 29) customWidget->setIcon(amavasyaIcon, 0.9);
        else customWidget->setIcon(QIcon(), 0.0);
        table->setCellWidget(row, col, customWidget);
        col++;
        if (col > 6) { col = 0; row++; }
    }
    table->resizeRowsToContents();
    table->resizeColumnsToContents();
    CalendarTableHelper::adjustCellSizes(table);
    table->verticalHeader()->setVisible(false);
}

void CalendarTableHelper::adjustCellSizes(QTableWidget* table) {
    int tableWidth = table->viewport()->width();
    int tableHeight = table->viewport()->height();
    int numColumns = table->columnCount();
    int numRows = table->rowCount();
    if (numColumns > 0 && numRows > 0) {
        int columnWidth = tableWidth / numColumns;
        int rowHeight = tableHeight / numRows;
        for (int i = 0; i < numColumns; ++i) table->setColumnWidth(i, columnWidth);
        for (int i = 0; i < numRows; ++i) table->setRowHeight(i, rowHeight);
    }
}

void CalendarTableHelper::populateBsDays(QComboBox* dayCombo, Bikram& converter, int year, int month) {
    int daysInMonth = converter.daysInMonth(year, month);
    int currentDay = converter.getDay();
    dayCombo->clear();
    for (int day = 1; day <= daysInMonth; ++day) dayCombo->addItem(QString::number(day));
    dayCombo->setCurrentText(QString::number(currentDay));
}
