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
#include <QDebug>
#include <ctime>
#include <QRegularExpression>
#include <QDir>
#include <QSettings>

std::string MainWindow::getWeekdayName(int year, int month, int day) {
#if defined(__unix__) || (defined(__APPLE__) && defined(__MACH__))
    // On Unix-like systems, where tm_gmtoff and tm_zone exist
    std::tm timeinfo = { 0, 0, 0, day, month - 1, year - 1900, 0, 0, 0, 0, nullptr };
#else
    // On Windows or systems where tm_gmtoff and tm_zone don't exist
    std::tm timeinfo = { 0, 0, 0, day, month - 1, year - 1900, 0, 0, 0 };
#endif

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
    setWindowFlags(Qt::Tool | Qt::Window | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);

    // Set the window attributes to make it transparent
    setAttribute(Qt::WA_TranslucentBackground);

    // Set the window position
    setWindowPosition();

    // Set today's date as default when setting up the UI
    setupDefaultDate();
}

void MainWindow::setWindowPosition() {
    // Get the geometry of the primary screen
    QScreen *primaryScreen = QGuiApplication::primaryScreen();
    QRect screenGeometry = primaryScreen->availableGeometry();

    // Calculate the position of the bottom-right corner
    int x = screenGeometry.right() - width();
    int y = screenGeometry.bottom() - height();

    // Adjusting the position to avoid the dock panel (you need to determine the dock height)
    int dockHeight = 40; // Example height of the dock panel
    if (screenGeometry.bottom() - dockHeight < y) {
        y = screenGeometry.bottom() - dockHeight - height();
    }

    // Set the position of the window
    move(x, y);
}

MainWindow::~MainWindow()
{
    delete ui;
}

// This function is not used since leap year is calculated in system time.
int MainWindow::isleapyear(int year)
{
    int a =  year;
    if (a % 100 == 0)
    {
        if(a % 400 == 0)
        {
            return true;
        } else {
            return false;
        }
    } else {
        if (a % 4 == 0)
        {
            return true;
        } else {
            return false;
        }
    }
}
// considering bikram date as nepali date.
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
    QString nepaliMonthName = getnepalimonth(nepaliMonth);
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
    QString styleSheet = QString("QPushButton { color: %1; border: none; outline: none; }").arg(textColor.name());
    ui->dateButton->setStyleSheet(styleSheet);
}

void MainWindow::setupDefaultDate()
{
    // Convert the default Gregorian date to Nepali and update UI
    int mm = 1;
    int yy = 1;
    int dd = 1;
    cnvToNepali(mm, dd, yy);
}
void MainWindow::exitAll()
{
    QList<QWidget*> topLevelWidgets = QApplication::topLevelWidgets();
    for (QWidget* widget : topLevelWidgets) {
        MainWindow* mainWindow = qobject_cast<MainWindow*>(widget);
        if (mainWindow) {
            mainWindow->close();
        }
    }

    // After closing all windows, quit the application
    qApp->quit();
}

const QString kAppName = "nepdate-widget";
const QString kAutostartKey = "HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run";
const QString kAutostartDirLinux = QDir::homePath() + "/.config/autostart/";
const QString kDesktopFileLinux = kAutostartDirLinux + "nepdate-widget.desktop";

bool isAutostartEnabled() {
#ifdef Q_OS_WIN
    QSettings registrySettings(kAutostartKey, QSettings::NativeFormat);
    return registrySettings.contains(kAppName);
#else
    QFile desktopFile(kDesktopFileLinux);
    return desktopFile.exists();
#endif
}

void createDesktopFile(const QString& path, const QString& execPath) {
    QFile desktopFile(path);
    if (!desktopFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "Failed to open file for writing:" << path;
        return;
    }

    QTextStream out(&desktopFile);
    out << "[Desktop Entry]\n";
    out << "Type=Application\n";
    out << "Name=Nepali Calendar widget\n";
    out << "Exec=" << execPath << "\n"; // Correctly write the application path
    out << "Icon=calendar\n"; // Replace with your icon name if needed
    desktopFile.close();
}

void MainWindow::setAutostart(bool enabled) {
#ifdef Q_OS_WIN
    QSettings registrySettings(kAutostartKey, QSettings::NativeFormat);

    if (enabled) {
        registrySettings.setValue(kAppName, QDir::toNativeSeparators(QCoreApplication::applicationFilePath()));
    } else {
        registrySettings.remove(kAppName);
    }
#else // Assuming Q_OS_LINUX or any other Unix-like OS
    const QString execPath = QCoreApplication::applicationFilePath();

    if (enabled) {
        // Ensure the desktop file exists in the application directory
        if (!QFile::exists(kDesktopFileLinux)) {
            createDesktopFile(kDesktopFileLinux, execPath);
        }
        // Copy the desktop file to the autostart directory
        QFile::copy(kDesktopFileLinux, kAutostartDirLinux + "nepdate-widget.desktop");
    } else {
        QFile::remove(kAutostartDirLinux + "nepdate-widget.desktop");
    }
#endif
}

void MainWindow::contextMenuEvent(QContextMenuEvent *event) {
    QMenu menu(tr("Context menu"), this);
    QString styleSheet = "QMenu { background-color: #C9F8FA; color: black; border: 1px solid black; border-radius: 2px; }";

    QAction *copyAction = new QAction(tr("Copy Nepali Date"), this);
    connect(copyAction, &QAction::triggered, this, &MainWindow::copyButtonText);
    menu.addAction(copyAction);

    menu.addSeparator(); // Add a separator before the autostart action

    QAction *autostartAction = new QAction(
        isAutostartEnabled() ? tr("Disable Autostart") : tr("Enable Autostart"),
        this);
    connect(autostartAction, &QAction::triggered, this, [this, autostartAction]() {
        bool currentState = isAutostartEnabled();
        setAutostart(!currentState);
        autostartAction->setText(isAutostartEnabled() ? tr("Disable Autostart") : tr("Enable Autostart"));
    });
    menu.addAction(autostartAction);

    menu.addSeparator(); // Add a separator before the exit action

    QAction *exitAllAction = new QAction(tr("Exit"), this);
    connect(exitAllAction, &QAction::triggered, this, &MainWindow::exitAll);
    menu.addAction(exitAllAction);

    menu.setStyleSheet(styleSheet);
    menu.exec(event->globalPos());
}

void MainWindow::mousePressEvent(QMouseEvent *event) {
    // Update the drag start position to the current global position minus the top-left of the window frame
    dragStartPosition = event->globalPosition().toPoint() - frameGeometry().topLeft();
}

void MainWindow::mouseMoveEvent(QMouseEvent *event) {
    // Move the window based on the current global position minus the drag start position
    move(event->globalPosition().toPoint() - dragStartPosition);
}



void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        isDragging = false;
        event->accept();
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

void MainWindow::copyButtonText() {
    QClipboard *clipboard = QGuiApplication::clipboard();
    clipboard->setText(ui->dateButton->text());
}

void MainWindow::updateDateButton() {
    QDate today = QDate::currentDate();
    cnvToNepali(today.month(), today.day(), today.year());
}

QString MainWindow::getnepalimonth(int m) {
    const QString nepaliMonths[] = {
        "बैशाख", "जेष्ठ", "आषाढ", "श्रावण", "भाद्र", "आश्विन",
        "कार्तिक", "मंसिर", "पुष", "माघ", "फाल्गुण", "चैत्र"
    };

    if (m >= 1 && m <= 12) {
        return nepaliMonths[m - 1];
    } else {
        return QString();
    }
}



