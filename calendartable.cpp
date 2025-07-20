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


void CalendarTableHelper::updateCalendar(QTableWidget *table, Bikram &converter, int year, int month, int &gYear, int &gMonth, int &gDay) {
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
    // Calculate the column where the 1st day of the month should start.
    int firstDayCol = (startDay - saturdayIndex + 6) % 7;
    int daysInMonth = converter.daysInMonth(year, month);
    QDate today = QDate::currentDate();
    converter.fromGregorian(today.year(), today.month(), today.day());
    int todayBsYear = converter.getYear();
    int todayBsMonth = converter.getMonth();
    int todayBsDay = converter.getDay();

    // Static icons to avoid reloading them from resources every time.
    static QIcon purnimaIcon(":/resources/purnima.png");
    static QIcon amavasyaIcon(":/resources/amawasya.png");

    // Loop through all 6 rows and 7 columns of the table.
    int currentDayOfMonth = 1; // Counter for the day of the current month (1 to daysInMonth)

    for (int r = 0; r < table->rowCount(); ++r) {
        for (int c = 0; c < table->columnCount(); ++c) {
            // Calculate the linear index of the current cell
            int cellIndex = r * table->columnCount() + c;

            // Determine if the current cell (r, c) should contain a day of the current month.
            bool isCurrentMonthDayCell = (cellIndex >= firstDayCol) && (currentDayOfMonth <= daysInMonth);

            if (isCurrentMonthDayCell) {

                // Try to get the existing custom widget from the cell.
                DayTithiWidget *customWidget = qobject_cast<DayTithiWidget*>(table->cellWidget(r, c));
                // If no widget exists, create a new one.
                if (!customWidget) {
                    customWidget = new DayTithiWidget();
                    table->setCellWidget(r, c, customWidget); // The table takes ownership.
                }

                QTableWidgetItem *tableItem = table->item(r, c);
                if (!tableItem) {
                    tableItem = new QTableWidgetItem();
                    table->setItem(r, c, tableItem); // The table takes ownership.
                }
                // Set flags to allow selection and ensure it's enabled.
                tableItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);


                // Calculate Gregorian date for the current Bikram day being displayed.
                int cellGYear, cellGMonth, cellGDay;
                converter.toGregorian(year, month, currentDayOfMonth, cellGYear, cellGMonth, cellGDay);

                // Create tm struct for panchanga calculations.
                std::tm date_tm = {};
                date_tm.tm_year = cellGYear - 1900;
                date_tm.tm_mon = cellGMonth - 1;
                date_tm.tm_mday = cellGDay;

                // Calculate tithi for this date.
                TithiResult tithiResult = calculateTithi(date_tm);

                // Update the DayTithiWidget's content and properties.
                customWidget->updateLabels(convertToNepaliNumerals(currentDayOfMonth),
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
                bool isToday = (year == todayBsYear && month == todayBsMonth && currentDayOfMonth == todayBsDay);
                bool isSaturday = (c == 6); // Saturday is the last column (index 6).
                if (isToday) {
                    customWidget->setTodayStyle();
                } else if (isSaturday) {
                    customWidget->setSaturdayStyle();
                } else {
                    customWidget->setNormalStyle(); // Clear any previous special styling.
                }

                // Check for purnima (15) or amavasya (30)
                if (tithiResult.tithiIndex == 14) {  // Purnima is tithi index 14
                    customWidget->setIcon(purnimaIcon, 0.9);
                } else if (tithiResult.tithiIndex == 29) {  // Amavasya is tithi index 29
                    customWidget->setIcon(amavasyaIcon, 0.9);
                } else {
                    customWidget->setIcon(QIcon(), 0.0); // Clear icon if not Purnima/Amavasya.
                }

                currentDayOfMonth++; // Increment to the next day of the current month.
            } else {
                // This cell is "blank" (either before the 1st day or after the last day of the month).

                // Remove and delete any DayTithiWidget that might be lingering from a previous month.
                QWidget *widget = table->cellWidget(r, c);
                if (widget) {
                    table->removeCellWidget(r, c);
                    delete widget; // Explicitly delete the widget.
                }

                // Ensure a QTableWidgetItem exists for this blank cell.
                QTableWidgetItem *tableItem = table->item(r, c);
                if (!tableItem) {
                    tableItem = new QTableWidgetItem();
                    table->setItem(r, c, tableItem); // The table takes ownership.
                }
                // Set flags to make it enabled but not selectable, prevent text, and set background.
                tableItem->setFlags(Qt::ItemIsEnabled);
                tableItem->setText(""); // Ensure no text is displayed.
                tableItem->setBackground(QBrush(QColor("#F0F0F0"))); // Light grey background for blank cells.
            }
        }
    }

    // Final adjustments for cell sizing.
    table->resizeRowsToContents();
    table->resizeColumnsToContents();
    CalendarTableHelper::adjustCellSizes(table);
}
void CalendarTableHelper::adjustCellSizes(QTableWidget *table) {
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

void CalendarTableHelper::populateBsDays(QComboBox *dayCombo, Bikram &converter, int year, int month) {
    int daysInMonth = converter.daysInMonth(year, month);
    int currentDay = converter.getDay();
    dayCombo->clear();
    for (int day = 1; day <= daysInMonth; ++day) dayCombo->addItem(QString::number(day));
    dayCombo->setCurrentText(QString::number(currentDay));
}
