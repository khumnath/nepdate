#include "calendartable.h"
#include "DayTithiWidget.h"
#include "calendarlogic.h"
#include "panchanga.h"
#include <QComboBox>
#include <QHeaderView>
#include <QIcon>
#include <QDate>
#include <QTableWidgetItem>
#include <QTableWidget>


void CalendarTableHelper::updateCalendar(QTableWidget* table, Bikram& converter, int year, int month, int& gYear, int& gMonth, int& gDay) {
    // Configure the table headers and style (only needs to be done once).
    table->setRowCount(6);
    table->setColumnCount(7);
    QStringList headers = {"आइत", "सोम", "मङ्गल", "बुध", "बिही", "शुक्र", "शनि"};
    table->setHorizontalHeaderLabels(headers);
    table->horizontalHeader()->setStyleSheet(
        "QHeaderView::section {background-color: #d3d3d3; color: blue; border: 1px solid gray;}"
    );
    table->setStyleSheet(
        "background-color: white;"
        "QTableWidget::item {border: 1px solid gray;}"
        "QTableWidget .dayLabel {font-size: 24px; color: black;}"
        "QTableWidget .tithiLabel {font-size: 8px; color: blue;}"
    );
    table->verticalHeader()->setVisible(false);

    // Calculate the start day and today's date for styling.
    converter.toGregorian(year, month, 1, gYear, gMonth, gDay);
    QDate firstDay(gYear, gMonth, gDay);
    int startDay = firstDay.dayOfWeek();
    int saturdayIndex = 6;
    int col = (startDay - saturdayIndex + 6) % 7;
    int row = 0;
    int daysInMonth = converter.daysInMonth(year, month);
    QDate today = QDate::currentDate();
    converter.fromGregorian(today.year(), today.month(), today.day());
    int todayBsYear = converter.getYear();
    int todayBsMonth = converter.getMonth();
    int todayBsDay = converter.getDay();

    // Static icons to avoid reloading them from resources every time.
    static QIcon purnimaIcon(":/resources/purnima.png");
    static QIcon amavasyaIcon(":/resources/amawasya.png");

    // Iterate through the days of the month and UPDATE or CREATE widgets.
    for (int day = 1; day <= daysInMonth; ++day) {
        int cellGYear, cellGMonth, cellGDay;
        converter.toGregorian(year, month, day, cellGYear, cellGMonth, cellGDay);

        // Create tm struct for panchanga calculations
        std::tm date_tm = {};
        date_tm.tm_year = cellGYear - 1900;
        date_tm.tm_mon = cellGMonth - 1;
        date_tm.tm_mday = cellGDay;

        // Calculate tithi for this date
        TithiResult tithiResult = calculateTithi(date_tm);

        // Try to get the existing widget from the cell.
        DayTithiWidget *customWidget = qobject_cast<DayTithiWidget*>(table->cellWidget(row, col));

        // If no widget exists in this cell, create a new one.
        if (!customWidget) {
            customWidget = new DayTithiWidget();
            // We need a QTableWidgetItem for the cell to exist.
            // setItem takes ownership of the new item.
            if (!table->item(row, col)) {
                 table->setItem(row, col, new QTableWidgetItem());
            }
            // Set the cell widget. The table becomes its parent.
            table->setCellWidget(row, col, customWidget);
        }

        // UPDATE the widget's content and properties.
        customWidget->updateLabels(convertToNepaliNumerals(day),
                                  QString::fromStdString(tithiResult.tithiName),
                                  QString::number(cellGDay));

        // Update the stored properties for click handler.
        customWidget->setProperty("year", cellGYear);
        customWidget->setProperty("month", cellGMonth);
        customWidget->setProperty("gDay", cellGDay);

        // Update the tooltip.
        QString tooltipText = QString("%1 (%2)").arg(QString::fromStdString(tithiResult.tithiName)).arg(QString::fromStdString(tithiResult.paksha));
        customWidget->setToolTip(tooltipText);

        // Update the style and icon based on the date.
        bool isToday = (year == todayBsYear && month == todayBsMonth && day == todayBsDay);
        bool isSaturday = (col == 6);
        if (isToday) {
            customWidget->setTodayStyle();
        } else if (isSaturday) {
            customWidget->setSaturdayStyle();
        } else {
            customWidget->setNormalStyle(); // Clear any special styling.
        }

        // Check for purnima (15) or amavasya (30)
        if (tithiResult.tithiIndex == 14) {  // Purnima is index 14
            customWidget->setIcon(purnimaIcon, 0.9);
        } else if (tithiResult.tithiIndex == 29) {  // Amavasya is index 29
            customWidget->setIcon(amavasyaIcon, 0.9);
        } else {
            customWidget->setIcon(QIcon(), 0.0);
        }

        // Move to the next cell.
        col++;
        if (col > 6) {
            col = 0;
            row++;
        }
    }

    // Clean up any remaining cells from the previous month.
    // This is the only deletion needed, for cells that are no longer part of the new month.
    int totalCells = table->rowCount() * table->columnCount();
    int lastFilledIndex = row * table->columnCount() + col;

    for (int i = lastFilledIndex; i < totalCells; ++i) {
        int r = i / table->columnCount();
        int c = i % table->columnCount();
        QWidget *widget = table->cellWidget(r, c);
        if (widget) {
            table->removeCellWidget(r, c);
            delete widget;
        }
        if (table->item(r, c)) {
             table->setItem(r, c, nullptr); // Let Qt delete the item.
        }
    }

    // 5. Final adjustments
    table->resizeRowsToContents();
    table->resizeColumnsToContents();
    CalendarTableHelper::adjustCellSizes(table);
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
