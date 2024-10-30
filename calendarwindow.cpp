#include "DayTithiWidget.h"
#include "panchanga.h"
#include "ui_calendarwindow.h"
#include "calendarwindow.h"
#include "bikram.h"
#include <QMessageBox>
#include <QDesktopServices>
#include <QUrl>
#include <QPalette>
#include <QScreen>
#include <QDate>
#include <QDebug>
#include <QIcon>
#include <QFile>
#include <QtWidgets>

CalendarWindow::CalendarWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CalendarWindow),
    blockSignals(false)
{
    ui->setupUi(this);
    this->setWindowTitle("नेपाली पात्रो तथा मिति परिवर्तक");
    connect(ui->aboutbutton, &QPushButton::clicked, this, &CalendarWindow::showMenu);
    // Center the window on the screen
    centerOnScreen();
    installEventFilter(this);

    QFile file(":/resources/style.qss");  // Stylesheet file
    if (file.open(QFile::ReadOnly | QFile::Text)) {
        QString styleSheet = file.readAll();
        qApp->setStyleSheet(styleSheet);
        file.close();
    }
    // Initialize current date to today's date
    QDate currentDate = QDate::currentDate();

    // Month names for Gregorian and Bikram Sambat calendars
    QStringList gregorianMonths = {"जनवरी", "फेब्रुअरी", "मार्च", "अप्रिल", "मे", "जुन",
                                   "जुलाई", "अगस्ट", "सेप्टेम्बर", "अक्टोबर", "नोभेम्बर", "डिसेम्बर"};

    QStringList bikramMonths = {"बैशाख", "जेठ", "असार", "श्रावण", "भाद्र",
                                "आश्विन", "कार्तिक", "मंसिर", "पौष", "माघ",
                                "फाल्गुन", "चैत"};


    // Populate AD combo boxes
    for (int year = 1900; year <= 2100; ++year) {
        ui->yearselectAD->addItem(QString::number(year));
        ui->yearselectAD->setEditable(true);
    }
    for (const QString &month : gregorianMonths) {
        ui->monthselectAD->addItem(month);
    }
    for (int day = 1; day <= 31; ++day) {
        ui->dayselectAD->addItem(QString::number(day));
    }

    // Populate BS combo boxes
    for (int year = 1970; year <= 2100; ++year) {
        ui->yearselectBS->addItem(QString::number(year));
        ui->yearselectBS->setEditable(true);
    }
    for (const QString &month : bikramMonths) {
        ui->monthselectBS->addItem(month);
    }

    // Set current date in AD combo boxes
    ui->yearselectAD->setCurrentText(QString::number(currentDate.year()));
    ui->monthselectAD->setCurrentIndex(currentDate.month() - 1);
    ui->dayselectAD->setCurrentText(QString::number(currentDate.day()));

    // Convert the current Gregorian date to Bikram Sambat
    converter.fromGregorian(currentDate.year(), currentDate.month(), currentDate.day());
    int bsYear = converter.getYear();
    int bsMonth = converter.getMonth();
    int bsDay = converter.getDay();
    converter.toGregorian(bsYear, bsMonth, bsDay, gYear, gMonth, gDay);
    double julianDate = gregorianToJulian(gYear, gMonth, gDay);
    Panchang panchang(julianDate);
    QString bsMonthName = getBikramMonthName(bsMonth);
    QString tithiName = QString::fromStdString(tithi[(int)panchang.tithi_index]);
    QString paksha = QString::fromStdString(panchang.paksha);
    QString tithipaksha = QString("%1 %2").arg(paksha).arg(tithiName);


    // Set current date in BS combo boxes
    ui->yearselectBS->setCurrentText(QString::number(bsYear));
    ui->monthselectBS->setCurrentIndex(bsMonth - 1);
    ui->dayselectBS->setCurrentText(QString::number(bsDay));

    // Populate BS day combo box based on current month and year
    populateBsDays(bsYear, bsMonth);

    // Update the output label
    int bsDaysInMonth = converter.daysInMonth(bsYear, bsMonth);
    ui->output->setText(QString("आज: बिक्रम सम्वत: %1 %2 %3 गते  %7\n %4 %5 मा जम्मा दिन सङ्ख्या: %6")
                            .arg(convertToNepaliNumerals(bsYear)).arg(bsMonthName).arg(convertToNepaliNumerals(bsDay)).arg(bsMonthName).arg(convertToNepaliNumerals(bsYear)).arg(convertToNepaliNumerals(bsDaysInMonth)).arg(tithipaksha));
    // Connect signals to slots
    connect(ui->yearselectAD, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &CalendarWindow::onAdYearChanged);
    connect(ui->monthselectAD, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &CalendarWindow::onAdMonthChanged);
    connect(ui->dayselectAD, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &CalendarWindow::onAdDayChanged);
    connect(ui->yearselectBS, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &CalendarWindow::onBsYearChanged);
    connect(ui->monthselectBS, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &CalendarWindow::onBsMonthChanged);
    connect(ui->dayselectBS, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &CalendarWindow::onBsDayChanged);
    connect(ui->todayButton, &QPushButton::clicked, this, &CalendarWindow::ontodayButtonclicked);

    // Initialize the calendar
    updateCalendar(bsYear, bsMonth);
    // Adjust cell sizes initially
    adjustCellSizes();
    adjustCalendarTableSize();
}
bool CalendarWindow::eventFilter(QObject *object, QEvent *event) {
    if (object == this && event->type() == QEvent::Show) {
        // Perform action when the window is shown
        ui->todayButton->click();
    }

    return QMainWindow::eventFilter(object, event);
}

void CalendarWindow::ontodayButtonclicked() {
    // Get today's date
    QDate today = QDate::currentDate();

    // Update AD combo boxes
    ui->yearselectAD->setCurrentText(QString::number(today.year()));
    ui->monthselectAD->setCurrentIndex(today.month() - 1);
    ui->dayselectAD->setCurrentText(QString::number(today.day()));

    // Convert the current Gregorian date to Bikram Sambat
    converter.fromGregorian(today.year(), today.month(), today.day());

    int bsYear = converter.getYear();
    int bsMonth = converter.getMonth();
    int bsDay = converter.getDay();
    QString bsMonthName = getBikramMonthName(bsMonth);
    converter.toGregorian(bsYear, bsMonth, bsDay, gYear, gMonth, gDay);
    double julianDate = gregorianToJulian(gYear, gMonth, gDay);
    Panchang panchang(julianDate);
    QString tithiName = QString::fromStdString(tithi[(int)panchang.tithi_index]);
    QString paksha = QString::fromStdString(panchang.paksha);
    QString tithipaksha = QString("%1 %2").arg(paksha).arg(tithiName);

    // Update BS combo boxes
    ui->yearselectBS->setCurrentText(QString::number(bsYear));
    ui->monthselectBS->setCurrentIndex(bsMonth - 1);
    ui->dayselectBS->setCurrentText(QString::number(bsDay));

    // Update the output label
    int bsDaysInMonth = converter.daysInMonth(bsYear, bsMonth);
    ui->output->setText(QString("आज: बिक्रम सम्वत: %1 %2 %3 गते %7\n %4 %5 मा जम्मा दिन सङ्ख्या: %6")
                            .arg(convertToNepaliNumerals(bsYear)).arg(bsMonthName).arg(convertToNepaliNumerals(bsDay)).arg(bsMonthName).arg(convertToNepaliNumerals(bsYear)).arg(convertToNepaliNumerals(bsDaysInMonth)).arg(tithipaksha));
    // Update the calendar
    updateCalendar(bsYear, bsMonth);
}


void CalendarWindow::resizeEvent(QResizeEvent* event) {
    QMainWindow::resizeEvent(event);
    adjustCellSizes();
    // Re-center the widgets horizontally
    centerWidgets();

    // Resize the calendarTable
    adjustCalendarTableSize();
}

void CalendarWindow::centerWidgets() {
    // Center widgets horizontally
    int screenWidth = width();
    int windowHeight = height();

    int x = (screenWidth - ui->centralwidget->width()) / 2;
    int y = (windowHeight - ui->centralwidget->height()) / 2;

    ui->centralwidget->move(x, y);
}

void CalendarWindow::adjustCalendarTableSize() {
    // Adjust calendarTable size
    int tableWidth = ui->calendarTable->viewport()->width();
    int tableHeight = ui->calendarTable->viewport()->height();

    int numColumns = ui->calendarTable->columnCount();
    int numRows = ui->calendarTable->rowCount();

    if (numColumns > 0 && numRows > 0) {
        int columnWidth = tableWidth / numColumns;
        int rowHeight = tableHeight / numRows;

        for (int i = 0; i < numColumns; ++i) {
            ui->calendarTable->setColumnWidth(i, columnWidth);
        }
        for (int i = 0; i < numRows; ++i) {
            ui->calendarTable->setRowHeight(i, rowHeight);
        }
    }
}

CalendarWindow::~CalendarWindow() {
    delete ui;
}
void CalendarWindow::showMenu() {
    QMenu menu(this);

    QAction *aboutAction = menu.addAction("About");
    QAction *sourceCodeAction = menu.addAction("Source Code");

    connect(aboutAction, &QAction::triggered, this, &CalendarWindow::showAbout);
    connect(sourceCodeAction, &QAction::triggered, this, &CalendarWindow::openSourceCode);

    menu.exec(QCursor::pos());
}

void CalendarWindow::showAbout() {
    QString version;
    QFile versionFile(":/resources/VERSION.txt"); // Use the resource path

    if (versionFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&versionFile);
        version = in.readLine(); // Read the first line
        versionFile.close();
    } else {
        qDebug() << "Could not open version file from resources.";
        version = "unknown"; // Fallback in case the file cannot be read
    }

    QString aboutText = QString(R"(
    <center>
        <h2 class='about.h2'>About</h2>
        <p class='about.p'>Nepali Calendar</p>
        <p><b>Author:</b> <span class='about'>khumnath</span></p>
        <p><b>Version:</b> %1</p>
        <p>This application is written in C++ and Qt framework. For more information, visit my
           <a href="https://github.com/khumnath/nepdate" class="about">GitHub page</a>.
        </p>
    </center>)").arg(version);

    QMessageBox msgBox(QMessageBox::Information, "About", aboutText, QMessageBox::Ok, this);
    msgBox.exec();
}


void CalendarWindow::openSourceCode() {
    // Open the URL using QDesktopServices
    QDesktopServices::openUrl(QUrl("https://github.com/khumnath/nepdate"));
    activateWindow();
}

void CalendarWindow::centerOnScreen() {
    // Get the screen geometry
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->availableGeometry();

    // Calculate the center point of the screen
    int screenWidth = screenGeometry.width();
    int screenHeight = screenGeometry.height();
    int windowWidth = this->width();
    int windowHeight = this->height();

    int x = (screenWidth - windowWidth) / 2;
    int y = (screenHeight - windowHeight) / 2;

    // Set the window position
    this->move(x, y);
}

void CalendarWindow::onAdYearChanged(int /*index*/) {
    if (blockSignals) return;
    blockSignals = true;

    int year = ui->yearselectAD->currentText().toInt();
    int month = ui->monthselectAD->currentIndex() + 1;
    int day = ui->dayselectAD->currentText().toInt();

    updateBsDateFromAd(year, month, day);

    blockSignals = false;
}

void CalendarWindow::onAdMonthChanged(int /*index*/) {
    if (blockSignals) return;
    blockSignals = true;

    int year = ui->yearselectAD->currentText().toInt();
    int month = ui->monthselectAD->currentIndex() + 1;
    int day = ui->dayselectAD->currentText().toInt();

    updateBsDateFromAd(year, month, day);

    blockSignals = false;
}

void CalendarWindow::onAdDayChanged(int /*index*/) {
    if (blockSignals) return;
    blockSignals = true;

    int year = ui->yearselectAD->currentText().toInt();
    int month = ui->monthselectAD->currentIndex() + 1;
    int day = ui->dayselectAD->currentText().toInt();
    int bsYear = converter.getYear();
    int bsMonth = converter.getMonth();
    populateBsDays(bsYear, bsMonth);
    updateBsDateFromAd(year, month, day);

    blockSignals = false;
}

void CalendarWindow::onBsYearChanged(int /*index*/) {
    if (blockSignals) return;
    blockSignals = true;

    int year = ui->yearselectBS->currentText().toInt();
    int month = ui->monthselectBS->currentIndex() + 1;
    int day = ui->dayselectBS->currentText().toInt();

    // Update BS day combo box based on current month and year
    populateBsDays(year, month);

    updateAdDateFromBs(year, month, day);

    // Update the calendar
    updateCalendar(year, month);

    blockSignals = false;
}

void CalendarWindow::onBsMonthChanged(int /*index*/) {
    if (blockSignals) return;
    blockSignals = true;

    int year = ui->yearselectBS->currentText().toInt();
    int month = ui->monthselectBS->currentIndex() + 1;
    int day = ui->dayselectBS->currentText().toInt();
    populateBsDays(year, month);
    // Update BS day combo box based on current month and year
    updateAdDateFromBs(year, month, day);

    // Update the calendar
    updateCalendar(year, month);

    blockSignals = false;
}

void CalendarWindow::onBsDayChanged(int /*index*/) {
    if (blockSignals) return;
    blockSignals = true;

    int year = ui->yearselectBS->currentText().toInt();
    int month = ui->monthselectBS->currentIndex() + 1;
    int day = ui->dayselectBS->currentText().toInt();

    updateAdDateFromBs(year, month, day);
    populateBsDays(year, month);

    blockSignals = false;
}

void CalendarWindow::updateBsDateFromAd(int year, int month, int day) {
    converter.fromGregorian(year, month, day);

    int bsYear = converter.getYear();
    int bsMonth = converter.getMonth();
    int bsDay = converter.getDay();

    QString bsMonthName = getBikramMonthName(bsMonth);
    converter.toGregorian(bsYear, bsMonth, bsDay, gYear, gMonth, gDay);
    double julianDate = gregorianToJulian(gYear, gMonth, gDay);
    Panchang panchang(julianDate);
    QString tithiName = QString::fromStdString(tithi[(int)panchang.tithi_index]);
    QString paksha = QString::fromStdString(panchang.paksha);
    QString tithipaksha = QString("%1 %2").arg(paksha).arg(tithiName);


    ui->yearselectBS->setCurrentText(QString::number(bsYear));
    ui->monthselectBS->setCurrentIndex(bsMonth - 1);  // Adjust index to start from 0
    // Populate BS day combo box based on current month and year
    populateBsDays(bsYear, bsMonth);
    ui->dayselectBS->setCurrentText(QString::number(bsDay));

    // Populate BS day combo box based on current month and year
    populateBsDays(bsYear, bsMonth);

    int bsDaysInMonth = converter.daysInMonth(bsYear, bsMonth);
    ui->output->setText(QString("विक्रम सम्वत मा परिवर्तन गरियो: %1 %2 %3 गते %5 \n%2 %1 मा जम्मा दिन सङ्ख्या: %4")
                            .arg(convertToNepaliNumerals(bsYear)).arg(bsMonthName).arg(convertToNepaliNumerals(bsDay)).arg(convertToNepaliNumerals(bsDaysInMonth)).arg(tithipaksha));



    // Update the calendar
    updateCalendar(bsYear, bsMonth);
}

void CalendarWindow::updateAdDateFromBs(int year, int month, int day) {
    converter.toGregorian(year, month, day, gYear, gMonth, gDay);

    ui->yearselectAD->setCurrentText(QString::number(gYear));
    ui->monthselectAD->setCurrentIndex(gMonth - 1);
    ui->dayselectAD->setCurrentText(QString::number(gDay));

    int bsDaysInMonth = converter.daysInMonth(year, month);
    QString bsMonthName = getBikramMonthName(month);
    QString adMonthName = getEnglishMonthName(gMonth);
    ui->output->setText(QString("अङ्ग्रेजी मिति मा परिवर्तन गरियो: %1 %2 %3 \n %4 %5 मा जम्मा दिन सङ्ख्या: %6")
                            .arg(convertToNepaliNumerals(gYear)).arg(adMonthName).arg(convertToNepaliNumerals(gDay)).arg(bsMonthName).arg(convertToNepaliNumerals(year)).arg(convertToNepaliNumerals(bsDaysInMonth)));

    populateBsDays(year, month);
}

void CalendarWindow::updateCalendar(int year, int month) {
    int daysInMonth = converter.daysInMonth(year, month);

    // Clear previous items
    ui->calendarTable->clear();
    ui->calendarTable->setRowCount(6);
    ui->calendarTable->setColumnCount(7);

    // Set weekday headers
    QStringList headers = {"आइत", "सोम", "मङ्गल", "बुध", "बिही", "शुक्र", "शनि"};
    ui->calendarTable->setHorizontalHeaderLabels(headers);

    // Apply styles to the header
    ui->calendarTable->horizontalHeader()->setStyleSheet(
        "QHeaderView::section {"
        "background-color: #d3d3d3;"
        "color: blue;"
        "border: 1px solid gray;"
        "}"
        );

    // Apply styles to the table
    ui->calendarTable->setStyleSheet(
        "background-color: white;"
        "QTableWidget::item {"
        "border: 1px solid gray;" // Border for cells
        "}"
        "QTableWidget .dayLabel {"
        "font-size: 24px;"
        "color: black;"

        "}"
        "QTableWidget .tithiLabel {"
        "font-size: 8px;"
        "color: blue;"

        "}"
        );

    // Find the first day of the month
    int gYear, gMonth, gDay;
    converter.toGregorian(year, month, 1, gYear, gMonth, gDay);
    QDate firstDay(gYear, gMonth, gDay);
    int startDay = firstDay.dayOfWeek(); // 0 (Monday) to 6 (Sunday)

    // Get today's Gregorian date
    QDate today = QDate::currentDate();

    // Convert today's Gregorian date to Bikram Sambat
    converter.fromGregorian(today.year(), today.month(), today.day());
    int todayBsYear = converter.getYear();
    int todayBsMonth = converter.getMonth();
    int todayBsDay = converter.getDay();

    // Identify Saturday index (assuming "शनि" is at index 6)
    int saturdayIndex = 6; // Modify this if "शनि" is at a different index

    // Load moon icons
    QIcon purnimaIcon(":/resources/purnima.png");
    QIcon amavasyaIcon(":/resources/amawasya.png");

    // Fill the calendar
    int row = 0;
    int col = (startDay - saturdayIndex + 6) % 7;  // Calculate offset based on Saturday index

    for (int day = 1; day <= daysInMonth; ++day) {
        // Convert BS date to Gregorian date
        converter.toGregorian(year, month, day, gYear, gMonth, gDay);

        // Calculate Julian date
        double julianDate = gregorianToJulian(gYear, gMonth, gDay);
        Panchang panchang(julianDate);
        QString tithiName = QString::fromStdString(tithi[(int)panchang.tithi_index]);

        // Create custom widget for day and tithi
        DayTithiWidget *customWidget = new DayTithiWidget(convertToNepaliNumerals(day), tithiName);

        // Set tooltip
        QString paksha = QString::fromStdString(panchang.paksha);
        QString tooltipText = QString("%1 (%2)").arg(tithiName).arg(paksha);
        customWidget->setToolTip(tooltipText);

        QTableWidgetItem *item = new QTableWidgetItem();
        ui->calendarTable->setItem(row, col, item);

        // Check if the current cell represents today's Bikram Sambat date
        bool isToday = (year == todayBsYear && month == todayBsMonth && day == todayBsDay);
        bool isSaturday = (col == saturdayIndex);

        if (isToday && isSaturday) {
            // If today is both Saturday and the current date, apply the "today" style
            item->setBackground(QColor(153, 255, 204)); // light green
            customWidget->setTodayStyle(); // defined in DayTithiWidget.h
        } else if (isToday) {
            // If it's just today, apply the "today" style
            item->setBackground(QColor(153, 255, 204)); // light green
            customWidget->setTodayStyle(); // defined in DayTithiWidget.h
        } else if (isSaturday) {
            // If it's just Saturday, apply the "Saturday" style
            customWidget->setSaturdayStyle();
        }

        if (panchang.tithi_index == 14) {
            customWidget->setIcon(purnimaIcon, 0.8);  // Example opacity set to 0.8
        } else if (panchang.tithi_index == 29) {
            customWidget->setIcon(amavasyaIcon, 0.8);  // Example opacity set to 0.8
        } else {
            customWidget->setIcon(QIcon(), 0.0);  // Clear icon and set transparency
        }

        ui->calendarTable->setCellWidget(row, col, customWidget);
        col++;
        if (col > 6) {
            col = 0;
            row++;
        }
    }

    ui->calendarTable->resizeRowsToContents();
    ui->calendarTable->resizeColumnsToContents();

    // Adjust cell sizes
    adjustCellSizes();

    // Hide the numbers in the first column
    ui->calendarTable->verticalHeader()->setVisible(false);
}
void CalendarWindow::adjustCellSizes() {
    int tableWidth = ui->calendarTable->viewport()->width();
    int tableHeight = ui->calendarTable->viewport()->height();
    int numColumns = ui->calendarTable->columnCount();
    int numRows = ui->calendarTable->rowCount();
    if (numColumns > 0 && numRows > 0) {
        int columnWidth = tableWidth / numColumns;
        int rowHeight = tableHeight / numRows;

        for (int i = 0; i < numColumns; ++i) {
            ui->calendarTable->setColumnWidth(i, columnWidth);
        }
        for (int i = 0; i < numRows; ++i) {
            ui->calendarTable->setRowHeight(i, rowHeight);
        }
    }
}

void CalendarWindow::populateBsDays(int year, int month) {
    int daysInMonth = converter.daysInMonth(year, month);
    int currentDay = converter.getDay();

    // Clear previous items
    ui->dayselectBS->clear();

    for (int day = 1; day <= daysInMonth; ++day) {
        ui->dayselectBS->addItem(QString::number(day));
    }

    // Set the current day
    ui->dayselectBS->setCurrentText(QString::number(currentDay));
}
