#include <QGuiApplication>
#include <QScreen>
#include "mainwindow.h"
#include "qmenu.h"
#include "ui_mainwindow.h"
#include <QMouseEvent>
#include <QTimer>
#include <QToolTip>
#include <QGraphicsDropShadowEffect>
#include "calendarwindow.h"
#include "panchanga.h"
#include <QPushButton>
#include <QClipboard>
#include <QLocale>
#include <QDebug>
#include <ctime>
#include <QRegularExpression>
#include <QDir>
#include <QSettings>
#include <QString>
#include <QFontDatabase>
#ifdef Q_OS_WIN
#include <windows.h>
#endif

#ifdef Q_OS_MAC
#include <Cocoa/Cocoa.h>
#endif

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
    QMainWindow(parent), ui(new Ui::MainWindow), calendarWindow(nullptr), isDragging(false), dragStarted(false)
{
    ui->setupUi(this);
    this->installEventFilter(this);
    ui->dateButton->installEventFilter(this);

    // Initialize timer for distinguishing between click and drag
    dragDelayTimer = new QTimer(this);
    dragDelayTimer->setSingleShot(true);
    connect(dragDelayTimer, &QTimer::timeout, this, [this]() {
        if (isDragging) {
            dragStarted = true;
        }
    });
    setMouseTracking(true);

    QString tooltipstyle = R"(
    QToolTip {
     background-color: white; color: black; border: 1px solid gray; border-radius: 5px;
    }
)";
    setStyleSheet(tooltipstyle);

    int fontId = QFontDatabase::addApplicationFont(":/resources/Laila-Medium.ttf");
    QString fontFamily = QFontDatabase::applicationFontFamilies(fontId).at(0);
    QFont appFont(fontFamily);
    QFont dateButtonFont(fontFamily, 14);  // To Do: set text size from setting menu */
    ui->dateButton->setFont(dateButtonFont);
    updateTimer = new QTimer(this);
connect(updateTimer, &QTimer::timeout, this, [=]() {
    updateDateButton();
    adjustTextColorBasedOnBackground();

});
updateTimer->start(1000);


    setWindowFlags(Qt::Window | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::BypassWindowManagerHint | Qt::WindowDoesNotAcceptFocus);
    setMouseTracking(true);
    setAttribute(Qt::WA_TranslucentBackground);

    setWindowPosition();
    setupDefaultDate();
    this->setWindowFlag(Qt::Tool);


// Platform-specific code to ensure the window stays on top
#ifdef Q_OS_X11
    QWindow *window = this->windowHandle();
    if (window) {
        window->setFlag(Qt::X11BypassWindowManagerHint);
        window->setFlag(Qt::WindowStaysOnTopHint);
        window->setType(Qt::Notification);
    }
#endif

#ifdef Q_OS_WIN
    // Ensure the window stays on top
    HWND hwnd = reinterpret_cast<HWND>(this->winId());
    SetWindowLong(hwnd, GWL_EXSTYLE, GetWindowLong(hwnd, GWL_EXSTYLE) | WS_EX_TOPMOST);
    SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0,
                 SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE | SWP_SHOWWINDOW);

    // Fix me if there are any other methods to force on top.
    // Periodically enforce the "always on top" setting
    QTimer* topmostTimer = new QTimer(this);
    connect(topmostTimer, &QTimer::timeout, [hwnd]() {
        SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0,
                     SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE | SWP_SHOWWINDOW);
    });
    topmostTimer->start(500); // adjust the interval as needed
#endif

#ifdef Q_OS_MAC
    NSWindow *nsWindow = [this->windowHandle() nsWindow];
    [nsWindow setLevel:NSMainMenuWindowLevel + 1];
#endif
}

void MainWindow::setWindowPosition() {
    QSettings settings("Nepdate", "NepdateWidget");
    QPoint savedPos = settings.value("MainWindow/pos", QPoint(-1, -1)).toPoint();

    if (savedPos != QPoint(-1, -1)) {
        move(savedPos);
    } else {
        QScreen *primaryScreen = QGuiApplication::primaryScreen();
        QRect screenGeometry = primaryScreen->geometry();

        int x = screenGeometry.left() + (screenGeometry.width() - width()) / 1.3; // Before statusbar in most cases.
        int offsetY = 10;  // Number of pixels to move downward
        int y = screenGeometry.bottom() - height() + offsetY;

#ifdef Q_OS_MAC
    int dockHeight = 0; //No idea how mac os handles dock
#endif
        move(x, y);
      }
}


MainWindow::~MainWindow()
{
    QSettings settings("Nepdate", "NepdateWidget");
    settings.setValue("MainWindow/pos", this->pos());
    delete ui;
}

// considering bikram date as nepali date.
int MainWindow::cnvToNepali(int mm, int dd, int yy) {
    // Perform the conversion using the bikram class
    Bikram bsdate;
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
    double julianDate = gregorianToJulian(yy, mm, dd);
    Panchang panchang(julianDate);
    QString tithiName = QString::fromStdString(tithi[(int)panchang.tithi_index]);
    QString paksha = QString::fromStdString(panchang.paksha);
    QString tithipaksha = QString("%1 %2").arg(paksha, tithiName);

    // Construct the Nepali date format string
    QString nepaliFormat = QString::number(nepaliYear) + " " +
                           nepaliMonthName + " " +
                           QString::number(nepaliDay) + " गते " +
                           nepaliDayOfWeekName;
    QString nepalitooltip = nepaliFormat + " " +
                           tithipaksha;

    QLocale nepaliLocale(QLocale::Nepali);

    nepaliFormat.replace(QString::number(nepaliYear), nepaliLocale.toString(nepaliYear));
    nepaliFormat.replace(QString::number(nepaliDay), nepaliLocale.toString(nepaliDay));
    nepaliFormat.replace(QRegularExpression(","), QString());
    nepalitooltip.replace(QString::number(nepaliYear), nepaliLocale.toString(nepaliYear));
    nepalitooltip.replace(QString::number(nepaliDay), nepaliLocale.toString(nepaliDay));
    nepalitooltip.replace(QRegularExpression(","), QString());

    // Set the Nepali formatted date to the button text and tooltip
    ui->dateButton->setText(nepaliFormat);
    ui->dateButton->setToolTipDuration(3000);
    QFont tooltipFont("Laila", 12); // Replace "Noto Sans Devnagari" with the name of your desired font
    QToolTip::setFont(tooltipFont);
    ui->dateButton->setToolTip(nepalitooltip);
    adjustTextColorBasedOnBackground();
    return 0;
}

// The event filter to handle tooltip display
bool MainWindow::eventFilter(QObject *watched, QEvent *event) {
    // Handle dragging for the entire window
    if (event->type() == QEvent::MouseButtonPress) {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
        if (mouseEvent->button() == Qt::LeftButton) {
            isDragging = true;
            dragStartPosition = mouseEvent->globalPosition().toPoint() - pos();
            dragDelayTimer->start(200);
            event->accept();
        }
    } else if (event->type() == QEvent::MouseMove) {
        if (isDragging && dragDelayTimer->isActive()) {
            QPoint movePosition = static_cast<QMouseEvent *>(event)->globalPosition().toPoint();
            if ((movePosition - (pos() + dragStartPosition)).manhattanLength() > 5) {
                dragDelayTimer->stop();
            }
        }

        if (isDragging) {
            move(static_cast<QMouseEvent *>(event)->globalPosition().toPoint() - dragStartPosition);
            event->accept();
        } else {
            // Show tooltip only when not dragging
            QToolTip::showText(static_cast<QMouseEvent *>(event)->globalPosition().toPoint(), ui->dateButton->toolTip(), ui->dateButton);
        }
    } else if (event->type() == QEvent::MouseButtonRelease) {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
        if (mouseEvent->button() == Qt::LeftButton) {
            dragDelayTimer->stop();
            isDragging = false;
            event->accept();
        }
    }

    // Handle specific events for the dateButton
    if (watched == ui->dateButton) {
        if (event->type() == QEvent::MouseButtonPress) {
            QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
            if (mouseEvent->button() == Qt::LeftButton) {
                // Check if it is a click, not a drag
                if (!isDragging) {
                    openCalendarWindow();
                }
            }
        } else if (event->type() == QEvent::Leave) {
            QToolTip::hideText();
        }
    }
    return QMainWindow::eventFilter(watched, event);  // Pass unhandled events to the base class
}

// Helper function to calculate the luminance of a color
double luminance(QColor color) {
    double r = color.redF();
    double g = color.greenF();
    double b = color.blueF();
    r = (r <= 0.03928) ? r / 12.92 : std::pow((r + 0.055) / 1.055, 2.4);
    g = (g <= 0.03928) ? g / 12.92 : std::pow((g + 0.055) / 1.055, 2.4);
    b = (b <= 0.03928) ? b / 12.92 : std::pow((b + 0.055) / 1.055, 2.4);

    // Calculates the relative luminance of the color using the formula specified in the sRGB color space.
    return 0.2126 * r + 0.7152 * g + 0.0722 * b;
}

// Helper function to calculate the contrast ratio between two luminance values
double contrastRatio(double lum1, double lum2) {
    double brighter = std::max(lum1, lum2);
    double darker = std::min(lum1, lum2);
    return (brighter + 0.05) / (darker + 0.05);
}

// Main function to adjust the text color based on the background color
void MainWindow::adjustTextColorBasedOnBackground() {
    QPoint globalPos = ui->dateButton->mapToGlobal(QPoint(0, 0));
    QScreen *screen = QGuiApplication::primaryScreen();
    QPixmap pixmap = screen->grabWindow(0, globalPos.x(), globalPos.y(),
                                        ui->dateButton->width(), ui->dateButton->height());
    QImage image = pixmap.toImage();
    QImage scaledImage = image.scaled(10, 10, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    QColor averageColor = getAverageColor(scaledImage);
    double bgLuminance = luminance(averageColor);

    // Get the luminance of black and white colors.
    double whiteLuminance = luminance(Qt::white);
    double blackLuminance = luminance(Qt::black);

    // Calculate the contrast ratio between the background color and black/white text.
    double whiteContrast = contrastRatio(bgLuminance, whiteLuminance);
    double blackContrast = contrastRatio(bgLuminance, blackLuminance);

    // Choose the text color with the highest contrast ratio.
    QColor bestTextColor = (blackContrast > whiteContrast) ? Qt::black : Qt::white;

    // Store the previous color for smooth transitions.
    static QColor previousColor = bestTextColor;

    // Implement a smooth transition mechanism to avoid rapid color changes.
    const double contrastThreshold = (bgLuminance < 0.5) ? 1.5 : 2.0;  // Adjust this threshold to fine-tune the transition
    if (contrastRatio(luminance(previousColor), bgLuminance) < contrastThreshold) {
        previousColor = bestTextColor;
    }

    // Set the text color of the dateButton while preserving its transparency.
    QString styleSheet = QString("QPushButton { color: %1; background-color: rgba(0, 0, 0, 0); border: none; outline: none; }")
                             .arg(previousColor.name());
    ui->dateButton->setStyleSheet(styleSheet);
}

// Function to calculate the average color of an image
QColor MainWindow::getAverageColor(const QImage &image) {
    // Initializes variables to store the accumulated red, green, and blue values.
    qint64 red = 0, green = 0, blue = 0;

    // Calculate the total number of pixels in the image.
    int pixelCount = image.width() * image.height();

    // Iterate through each pixel in the image.
    for (int y = 0; y < image.height(); ++y) {
        for (int x = 0; x < image.width(); ++x) {
            // Get the color of the current pixel.
            QColor color(image.pixel(x, y));
            red += color.red();
            green += color.green();
            blue += color.blue();
        }
    }

    // Calculates the average red, green, and blue values and returns the resulting average color.
    return QColor(red / pixelCount, green / pixelCount, blue / pixelCount);
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
    out << "Exec=" << execPath << "\n";
    out << "Icon=calendar\n";
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
    QString styleSheet = "QMenu { background-color: white; color: black; border: 1px solid gray; border-radius: 5px; }"
                         "QMenu::item { background-color: transparent; }"
                         "QMenu::item:selected { background-color: #0892D0; color: white;}";
    menu.setStyleSheet(styleSheet);

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

void MainWindow::openCalendarWindow() {
    if (!calendarWindow || !calendarWindow->isVisible()) {
        calendarWindow = new CalendarWindow(this);
        calendarWindow->setWindowModality(Qt::NonModal);

        connect(calendarWindow, &CalendarWindow::destroyed, this, [this]() {
            calendarWindow = nullptr;
        });
        calendarWindow->show();
    } else {
        calendarWindow->raise();
        calendarWindow->activateWindow();
    }
}

void MainWindow::on_dateButton_clicked()
{
    // Call openCalendarWindow when the dateButton is clicked
    openCalendarWindow();
}


void MainWindow::copyButtonText() {
    QClipboard *clipboard = QGuiApplication::clipboard();
    clipboard->setText(ui->dateButton->text());
}

void MainWindow::updateDateButton() {
    QDate today = QDate::currentDate();
    if (today != lastUpdatedDate) {
        lastUpdatedDate = today;
        cnvToNepali(today.month(), today.day(), today.year());
        adjustTextColorBasedOnBackground();
        this->show();
        this->raise();
    }
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

//for tithi calculation
double gregorianToJulian(int year, int month, int day) {
    if (month <= 2) {
        year--;
        month += 12;
    }
    int A = year / 100;
    int B = 2 - A + A / 4;
    double JD = std::floor(365.25 * (year + 4716)) + std::floor(30.6001 * (month + 1)) + day + B - 1524.5;
    return JD;
}


