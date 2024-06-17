#include <QGuiApplication>
#include <QScreen>
#include "mainwindow.h"
#include "qmenu.h"
#include "ui_mainwindow.h"
#include <QMouseEvent>
#include <QTimer>
#include <QToolTip>
#include "calendarwindow.h"
#include <QPushButton>
#include <QClipboard>


#include <QLocale>
std::string MainWindow::getWeekdayName(int year, int month, int day) {
    std::tm timeinfo = { 0, 0, 0, 0, day, month - 1, year - 1900, 0, 0, 0, 0};
    std::mktime(&timeinfo); // Update timeinfo to fill in the week day field

    const std::string nepaliWeekdays[] = { "आइतबार", "सोमबार", "मंगलबार", "बुधबार", "बिहिबार", "शुक्रबार", "शनिबार" };
    return nepaliWeekdays[timeinfo.tm_wday];
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::MainWindow), isDragging(false), calendarWindow(nullptr)
{
    ui->setupUi(this);
    // Initialize the timer
    updateTimer = new QTimer(this);


    // Connect the timer's timeout signal to a slot
    connect(updateTimer, &QTimer::timeout, this, &MainWindow::updateDateButton);

    // Start the timer to trigger its timeout signal every second (adjust as needed)
    updateTimer->start(1000); // Update every second

    // Set the window flags to make it borderless
    setWindowFlags(Qt::Window | Qt::FramelessWindowHint |  Qt::WindowStaysOnTopHint | Qt::Tool);

    // Get the geometry of the primary screen
    QScreen *primaryScreen = QGuiApplication::primaryScreen();
    QRect screenGeometry = primaryScreen->availableGeometry();

    // Calculate the position of the bottom-right corner
    int x = screenGeometry.right();
    int y = screenGeometry.bottom() + height(); // Adjusted to place the window at the bottom

    // Adjusting the position to avoid the dock panel (you need to determine the dock height)
    int dockHeight = 40; // Example height of the dock panel
    if (screenGeometry.bottom() - dockHeight < y) {
        y = screenGeometry.bottom() - dockHeight;
    }

    // Set the position of the window
    move(x, y);
    ;
    setAttribute(Qt::WA_TranslucentBackground);

    // Set today's date as default when setting up the UI
    setupDefaultDate();


}



MainWindow::~MainWindow()
{
    delete ui;
}
int MainWindow::is_leap_year(int year)
{
    int a =  year;
    if (a%100==0)
    {
        if(a%400==0)
        {
            return true;
        } else {
            return false;
        }

    } else {
        if (a%4==0)
        {
            return true;
        } else {
            return false;
        }
    }
}


int MainWindow::cnvToNepali(int mm, int dd, int yy) {
    // Perform the conversion using the bikram class
    bikram bsdate;
    bsdate.fromGregorian(yy, mm, dd); // Convert the provided Gregorian date to Bikram Sambat

    // Get the converted Nepali date components from the bikram class
    int nepaliYear = bsdate.getYear();
    int nepaliMonth = bsdate.getMonth();
    int nepaliDay = bsdate.getDay();

    // Calculate the weekday name for the given Gregorian date
    std::string weekdayName = getWeekdayName(yy, mm, dd);

    // Adjust the day of the week
    QString nepaliMonthName = get_nepali_month(nepaliMonth);
    QString nepaliDayOfWeekName = QString::fromStdString(weekdayName);

    // Construct the Nepali date format string
    QString nepaliFormat = QString::number(nepaliYear) + " " +
                           nepaliMonthName + " " +
                           QString::number(nepaliDay) + " गते " +
                           nepaliDayOfWeekName;

    QLocale nepaliLocale(QLocale::Nepali);

    nepaliFormat.replace(QString::number(nepaliYear), nepaliLocale.toString(nepaliYear));
    nepaliFormat.replace(QString::number(nepaliDay), nepaliLocale.toString(nepaliDay));
    nepaliFormat.replace(QRegularExpression(","), QString());

    // Set the Nepali formatted date to the button text and tooltip
    ui->dateButton->setText(nepaliFormat);
    ui->dateButton->setToolTipDuration(0);
    QFont tooltipFont("Noto Sans Devanagari", 12); // Replace "Noto Sans Devnagari" with the name of your desired font
    QToolTip::setFont(tooltipFont); // Set the tooltip font globally
    ui->dateButton->setToolTip(nepaliFormat);
    adjustTextColorBasedOnBackground();
    if (calendarWindow) {
        QPushButton *mitiButton = calendarWindow->findChild<QPushButton*>("mitiButton");
        if (mitiButton) {
            mitiButton->setText(nepaliFormat);
        }
    }

    return 0;
}
QColor MainWindow::getAverageColor(const QImage &image) {
    qint64 red = 0, green = 0, blue = 0;
    int pixelCount = image.width() * image.height();

    for (int y = 0; y < image.height(); ++y) {
        for (int x = 0; x < image.width(); ++x) {
            QColor color(image.pixel(x, y));
            red += color.red();
            green += color.green();
            blue += color.blue();
        }
    }

    return QColor(red / pixelCount, green / pixelCount, blue / pixelCount);
}

void MainWindow::adjustTextColorBasedOnBackground() {
    // Get the screen behind the window
    QScreen *screen = QGuiApplication::primaryScreen();
    QPixmap pixmap = screen->grabWindow(0, x(), y(), width(), height());
    QImage image = pixmap.toImage();

    // Calculate the average color
    QColor averageColor = getAverageColor(image);

    // Determine whether to use black or white text based on the brightness of the average color
    int brightness = (averageColor.red() + averageColor.green() + averageColor.blue()) / 3;
    QColor textColor = (brightness > 80) ? Qt::black : Qt::white;

    // Set the text color of the dateButton
    QString styleSheet = QString("QPushButton { color: %1; }").arg(textColor.name());
    ui->dateButton->setStyleSheet(styleSheet);
}



void MainWindow::setupDefaultDate()
{

    // Convert the default Gregorian date to Nepali and update UI
    int mm = 10;
    int yy = 1984;
    int dd = 4;
    cnvToNepali(mm, dd, yy);
}
QString MainWindow::get_nepali_month(int m) {
    QString n_month = "";

    switch (m) {
    case 1:
        n_month = "वैशाख"; // Vaisakh
        break;

    case 2:
        n_month = "जेष्ठ"; // Jestha
        break;

    case 3:
        n_month = "असार"; // Ashad
        break;

    case 4:
        n_month = "श्रावण"; // Shrawn
        break;

    case 5:
        n_month = "भदौ"; // Bhadra
        break;

    case 6:
        n_month = "आश्विन"; // Ashwin
        break;

    case 7:
        n_month = "कार्तिक"; // Kartik
        break;

    case 8:
        n_month = "मंसिर"; // Mangshir
        break;

    case 9:
        n_month = "पौष"; // Poush
        break;

    case 10:
        n_month = "माघ"; // Magh
        break;

    case 11:
        n_month = "फाल्गुण"; // Falgun
        break;

    case 12:
        n_month = "चैत्र"; // Chaitra
        break;
    }
    return n_month;
}


void MainWindow::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu menu(tr("Context menu"), this);
    QString styleSheet = "QMenu { background-color: #C9F8FA; color: black; border: 1px solid black; border-radius: 5px; }"
                         "QMenu::item { background-color: transparent; }"
                         "QMenu::item:selected { background-color: #1AEFF7; color: #184805;}";
    menu.setStyleSheet(styleSheet);
    menu.setWindowFlags(menu.windowFlags() | Qt::FramelessWindowHint);


    QAction *copyAction = menu.addAction("Copy Date");
    connect(copyAction, &QAction::triggered, this, &MainWindow::copyButtonText);
    QAction *exitAction = menu.addAction("Exit");
    connect(exitAction, &QAction::triggered, qApp, &QApplication::quit);
    menu.exec(event->globalPos());
}
void MainWindow::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        isDragging = true;
        dragStartPosition = event->globalPosition().toPoint() - frameGeometry().topLeft();
        event->accept();
    } else {
        QMainWindow::mousePressEvent(event);
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *event) {
    if (isDragging) {
        move(event->globalPosition().toPoint() - dragStartPosition);
        event->accept();
    } else {
        QMainWindow::mouseMoveEvent(event);
    }
}
void MainWindow::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        isDragging = false;
        event->accept();
    } else {
        QMainWindow::mouseReleaseEvent(event);
    }
}

void MainWindow::openCalendarWindow(const QString &link) {
    qDebug() << "Clicked on datebutton. Link: " << link;
    if (link == "#") {
        // Ignore the link since it doesn't represent a valid action
        return;
    }

    // Check if calendarWindow is nullptr or not visible
    if (!calendarWindow || !calendarWindow->isVisible()) {
        // If calendarWindow is nullptr or not visible, create a new instance
        calendarWindow = new CalendarWindow(this);

        // Connect the destroyed signal of calendarWindow to a lambda that sets calendarWindow to nullptr when the window is closed
        connect(calendarWindow, &CalendarWindow::destroyed, this, [this]() {
            calendarWindow = nullptr;
        });

        // Show the calendarWindow
        calendarWindow->show();
    } else {
        // If calendarWindow is already visible, raise it to the top
        calendarWindow->raise();
        calendarWindow->activateWindow();

    }
}

void MainWindow::on_dateButton_clicked()
{
    // Check if calendarWindow is nullptr or not visible
    if (!calendarWindow || !calendarWindow->isVisible()) {
        // If calendarWindow is nullptr or not visible, create a new instance
        calendarWindow = new CalendarWindow(this);

        // Connect the destroyed signal of calendarWindow to a lambda that sets calendarWindow to nullptr when the window is closed
        connect(calendarWindow, &CalendarWindow::destroyed, this, [this]() {
            calendarWindow = nullptr;
        });

        // Show the calendarWindow
        calendarWindow->show();
    } else {
        // If calendarWindow is already visible, raise it to the top
        calendarWindow->raise();
        calendarWindow->activateWindow();
    }
}
void MainWindow::copyButtonText()
{
    // Copy the text of the dateButton to the clipboard
    QApplication::clipboard()->setText(ui->dateButton->text());
}
void MainWindow::updateDateButton()
{
    // Get the current system date
    QDate currentDate = QDate::currentDate();

    // Convert the current system date to Nepali date format
    int mm = currentDate.month();
    int dd = currentDate.day();
    int yy = currentDate.year();
    cnvToNepali(mm, dd, yy);
}
