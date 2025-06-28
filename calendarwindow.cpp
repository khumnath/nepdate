#include "DayTithiWidget.h"
#include "panchanga.h"
#include "ui_calendarwindow.h"
#include "calendarwindow.h"
#include "bikram.h"
#include "calendarlogic.h"
#include "calendartable.h"
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
#include <QSettings>
#include <QCloseEvent>
#include <QTimer>
#include "convertor.h"

CalendarWindow::CalendarWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CalendarWindow),
    blockSignals(false)
{
    ui->setupUi(this);    
        setWindowPosition();



    // Apply the custom font
    int fontId = QFontDatabase::addApplicationFont(":/resources/NotoSansDevanagari-VariableFont_wdth,wght.ttf");
    QString fontFamily = "Noto Sans Devnagari";
    if (fontId != -1) {
        fontFamily = QFontDatabase::applicationFontFamilies(fontId).at(0);
    }
        qApp->setFont(fontFamily);


    this->setWindowTitle("नेपाली पात्रो तथा मिति परिवर्तक");
    connect(ui->aboutbutton, &QPushButton::clicked, this, &CalendarWindow::showMenu);
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
    // Populate AD combo boxes
    for (int year = 1900; year <= 2100; ++year) {
        ui->yearselectAD->addItem(QString::number(year));
        ui->yearselectAD->setEditable(true);
    }
    for (int month = 1; month <= 12; ++month) {
        ui->monthselectAD->addItem(getEnglishMonthName(month));
    }
    for (int day = 1; day <= 31; ++day) {
        ui->dayselectAD->addItem(QString::number(day));
    }

    // Populate BS combo boxes
    for (int year = 1970; year <= 2100; ++year) {
        ui->yearselectBS->addItem(QString::number(year));
        ui->yearselectBS->setEditable(true);
    }
    for (int month = 1; month <= 12; ++month) {
        ui->monthselectBS->addItem(getBikramMonthName(month));
    }
    int year = ui->yearselectBS->currentText().toInt();
    int month = ui->monthselectBS->currentIndex() + 1;
    populateBsDays(year, month);


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
    connect(ui->previousMonthButton_2, &QPushButton::clicked, this, &CalendarWindow::onpreviousMonthButtonclicked);
    connect(ui->nextMonthButton, &QPushButton::clicked, this, &CalendarWindow::onnextMonthButtonclicked);




    connect(ui->todayButton, &QPushButton::clicked, this, &CalendarWindow::ontodayButtonclicked);

    // Initialize the calendar
    updateCalendar(bsYear, bsMonth);
    // Adjust cell sizes initially
    adjustCellSizes();
    adjustCalendarTableSize();
}


void CalendarWindow::setWindowPosition() {
    QSettings settings("Nepdate", "Nepdatecalendar");

    // Load saved size
    QSize savedSize = settings.value("MainWindow/size", QSize(-1, -1)).toSize();
    if (savedSize != QSize(-1, -1)) {
        resize(savedSize);
    } else {
        QRect screenGeometry = QApplication::primaryScreen()->availableGeometry();
        int width = screenGeometry.width() * 0.7;
        int height = screenGeometry.height() * 0.6;
        resize(width, height);
    }

    // Defer restoring position until after window has been shown
    QTimer::singleShot(0, this, [this]() { // <-- No captured QSettings
        QSettings settings("Nepdate", "Nepdatecalendar"); // Create new instance inside lambda

        QPoint savedPos = settings.value("MainWindow/pos", QPoint(-1, -1)).toPoint();

        if (savedPos != QPoint(-1, -1)) {
            // Make sure position is on a valid screen
            bool foundValidScreen = false;
            for (QScreen *screen : QGuiApplication::screens()) {
                if (screen->geometry().contains(savedPos)) {
                    move(savedPos);
                    foundValidScreen = true;
                    break;
                }
            }

            // If not found on any screen, center it
            if (!foundValidScreen) {
                QRect screenGeometry = QApplication::primaryScreen()->availableGeometry();
                int x = (screenGeometry.width() - width()) / 2;
                int y = (screenGeometry.height() - height()) / 2;
                move(x, y);
            }
        }
    });
}

void CalendarWindow::closeEvent(QCloseEvent *event) {
    // Save window geometry
    QSettings settings("Nepdate", "Nepdatecalendar");
    settings.setValue("MainWindow/pos", pos());
    settings.setValue("MainWindow/size", size());
    settings.setValue("MainWindow/maximized", isMaximized());

    QMainWindow::closeEvent(event);
}

bool CalendarWindow::eventFilter(QObject *object, QEvent *event) {
    if (object == this && event->type() == QEvent::Show) {
        // Perform action when the window is shown
        ui->todayButton->click();
    }

    return QMainWindow::eventFilter(object, event);
}

const QStringList CalendarWindow::bikramMonths = {
    "वैशाख", "जेठ", "असार", "श्रावण", "भाद्र",
    "आश्विन", "कार्तिक", "मंसिर", "पौष", "माघ",
    "फाल्गुन", "चैत्र"
};
const QStringList CalendarWindow::gregorianMonths = {"जनवरी", "फेब्रुअरी", "मार्च", "अप्रिल", "मे", "जुन",
                                                     "जुलाई", "अगस्ट", "सेप्टेम्बर", "अक्टोबर", "नोभेम्बर", "डिसेम्बर"};

QString CalendarWindow::getBikramMonthName(int month) {
    if (month < 1 || month > 12) {
        return ""; // Return an empty string for invalid month
    }
    return bikramMonths.at(month - 1); // Assuming 1-based month input
}
QString CalendarWindow::getEnglishMonthName(int month) {
    if (month < 1 || month > 12) {
        return ""; // Return an empty string for invalid month
    }
    return gregorianMonths.at(month - 1); // Assuming 1-based month input
}

QString CalendarWindow::convertToNepaliNumerals(int number) {
    QString nepaliNumerals = QString::number(number);
    nepaliNumerals.replace("0", "०");
    nepaliNumerals.replace("1", "१");
    nepaliNumerals.replace("2", "२");
    nepaliNumerals.replace("3", "३");
    nepaliNumerals.replace("4", "४");
    nepaliNumerals.replace("5", "५");
    nepaliNumerals.replace("6", "६");
    nepaliNumerals.replace("7", "७");
    nepaliNumerals.replace("8", "८");
    nepaliNumerals.replace("9", "९");
    return nepaliNumerals;
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
// Slot for Previous Month button
void CalendarWindow::onpreviousMonthButtonclicked() {
    int currentIndex = ui->monthselectBS->currentIndex();

    if (currentIndex > 0) {
        ui->monthselectBS->setCurrentIndex(currentIndex - 1);
    } else {
        // Wrap around to December
        ui->monthselectBS->setCurrentIndex(11); // December (0-based index)
    }

    // The change will automatically trigger the connected slot for month selection
}

// Slot for Next Month button
void CalendarWindow::onnextMonthButtonclicked() {
    int currentIndex = ui->monthselectBS->currentIndex();

    if (currentIndex < 11) { // 11 is December
        ui->monthselectBS->setCurrentIndex(currentIndex + 1);
    } else {
        // Wrap around to January
        ui->monthselectBS->setCurrentIndex(0); // January (0-based index)
    }

    // The change will automatically trigger the connected slot for month selection
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
    <center style="background-color: white; padding: 20px;">
        <h2 style="font-size: 24px; font-weight: bold; color: black;">Nepali Calendar</h2>
        <p style="color: black;"><b>Author:</b> <span style="font-weight: normal;">khumnath</span></p>
        <p style="color: black;"><b>Version:</b> <span style="color: green; font-weight: bold;">%1</span></p>
        <p style="color: black;">This application is written in C++ and the Qt framework. For more information, visit my
           <a href="https://github.com/khumnath/nepdate" style="color: blue; text-decoration: underline;">GitHub page</a>.
        </p>
    </center>)").arg(version);

    QMessageBox msgBox(QMessageBox::Information, "About", aboutText, QMessageBox::Ok, this);
    msgBox.setTextFormat(Qt::RichText);
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
    BsDate bs;
    adToBs(year, month, day, bs);
    ui->yearselectBS->setCurrentText(QString::number(bs.year));
    ui->monthselectBS->setCurrentIndex(bs.month - 1);
    populateBsDays(bs.year, bs.month);
    ui->dayselectBS->setCurrentText(QString::number(bs.day));
    updateBsDateFromAd(year, month, day);
    blockSignals = false;
}

void CalendarWindow::onBsYearChanged(int /*index*/) {
    if (blockSignals) return;
    blockSignals = true;

    int year = ui->yearselectBS->currentText().toInt();
    int month = ui->monthselectBS->currentIndex() + 1;

    // Update BS day combo box based on current month and year
    populateBsDays(year, month);

    // Now, use the current day selection to update AD date
    int day = ui->dayselectBS->currentText().toInt();
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

    // Ensure the selected day remains consistent
    if (ui->dayselectBS->currentText().toInt() != day) {
        ui->dayselectBS->setCurrentText(QString::number(day));
    }

    blockSignals = false;
}
void CalendarWindow::updateBsDateFromAd(int year, int month, int day) {
    BsDate bs;
    adToBs(year, month, day, bs);
    QString bsMonthName = getBikramMonthName(bs.month);
    AdDate ad;
    bsToAd(bs.year, bs.month, bs.day, ad);
    double julianDate = toJulian(ad.year, ad.month, ad.day);
    QString paksha;
    QString tithiName = getTithiName(ad.year, ad.month, ad.day, paksha);
    QString tithipaksha = QString("%1 %2").arg(paksha).arg(tithiName);
    ui->yearselectBS->setCurrentText(QString::number(bs.year));
    ui->monthselectBS->setCurrentIndex(bs.month - 1);
    populateBsDays(bs.year, bs.month);
    ui->dayselectBS->setCurrentText(QString::number(bs.day));
    int bsDaysInMonth = Bikram().daysInMonth(bs.year, bs.month);
    QDate systemDate = QDate::currentDate();
    if (QDate(year, month, day) == systemDate) {
        ui->output->setText(QString("आज: बिक्रम सम्वत: %1 %2 %3 गते %5\n %2 %1 मा जम्मा दिन सङ्ख्या: %4")
            .arg(convertToNepaliNumerals(bs.year))
            .arg(bsMonthName)
            .arg(convertToNepaliNumerals(bs.day))
            .arg(convertToNepaliNumerals(bsDaysInMonth))
            .arg(tithipaksha));
    } else {
        ui->output->setText(QString("विक्रम सम्वत मा परिवर्तन गरियो: %1 %2 %3 गते %5\n %2 %1 मा जम्मा दिन सङ्ख्या: %4")
            .arg(convertToNepaliNumerals(bs.year))
            .arg(bsMonthName)
            .arg(convertToNepaliNumerals(bs.day))
            .arg(convertToNepaliNumerals(bsDaysInMonth))
            .arg(tithipaksha));
    }
    updateCalendar(bs.year, bs.month);
}

void CalendarWindow::updateAdDateFromBs(int year, int month, int day) {
    AdDate ad;
    bsToAd(year, month, day, ad);
    ui->yearselectAD->setCurrentText(QString::number(ad.year));
    ui->monthselectAD->setCurrentIndex(ad.month - 1);
    ui->dayselectAD->setCurrentText(QString::number(ad.day));
    int bsDaysInMonth = Bikram().daysInMonth(year, month);
    QString gmonthname = getEnglishMonthName(ad.month);
    double julianDate = toJulian(ad.year, ad.month, ad.day);
    QString paksha;
    QString tithiName = getTithiName(ad.year, ad.month, ad.day, paksha);
    QString tithipaksha = QString("%1 %2").arg(paksha).arg(tithiName);
    ui->output->setText(QString("ईसवी सन मा परिवर्तन गरियो: %1 %2 %3 गते %5 \n%2 %1 मा जम्मा दिन सङ्ख्या: %4")
        .arg(convertToNepaliNumerals(ad.year)).arg(gmonthname).arg(convertToNepaliNumerals(ad.day)).arg(convertToNepaliNumerals(bsDaysInMonth)).arg(tithipaksha));
    updateCalendar(year, month);
    populateBsDays(year, month);
}


void CalendarWindow::updateCalendar(int year, int month) {
    CalendarTableHelper::updateCalendar(ui->calendarTable, converter, year, month, gYear, gMonth, gDay);
}

void CalendarWindow::adjustCellSizes() {
    CalendarTableHelper::adjustCellSizes(ui->calendarTable);
}

void CalendarWindow::populateBsDays(int year, int month) {
    CalendarTableHelper::populateBsDays(ui->dayselectBS, converter, year, month);
}

