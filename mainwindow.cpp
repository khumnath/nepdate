#include "mainwindow.h"
#include "qwidgetaction.h"
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
#include <QColorDialog>
#include <QMessageBox>
#include <QSlider>

#ifdef Q_OS_WIN
#include <windows.h>
#endif
#ifdef Q_OS_MAC
#include <Cocoa/Cocoa.h>
#endif

const QString MainWindow::kAppName = "nepdate-widget";
const QString MainWindow::kAutostartKey = "HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run";
const QString MainWindow::kAutostartDirLinux = QDir::homePath() + "/.config/autostart/";
const QString MainWindow::kDesktopFileLinux = kAutostartDirLinux + "nepdate-widget.desktop";

// Static regex patterns (precompiled once)
static const QRegularExpression numberRegex(R"(\b\d+\b)");
static const QRegularExpression commaRegex(",");

std::string MainWindow::getWeekdayName(int year, int month, int day) {
#if defined(__unix__) || (defined(__APPLE__) && defined(__MACH__))
    std::tm timeinfo = { 0, 0, 0, day, month - 1, year - 1900, 0, 0, 0, 0, nullptr };
#else
    std::tm timeinfo = { 0, 0, 0, day, month - 1, year - 1900, 0, 0, 0 };
#endif
    std::mktime(&timeinfo);
    const std::string nepaliWeekdays[] = { "आइतबार", "सोमबार", "मंगलबार", "बुधबार", "बिहिबार", "शुक्रबार", "शनिबार" };
    return nepaliWeekdays[timeinfo.tm_wday];
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    calendarWindow(nullptr),
    isDragging(false),
    dragStarted(false)
{
    ui->setupUi(this);

    disconnect(ui->dateButton, &QPushButton::clicked, this, &MainWindow::on_dateButton_clicked);
    ui->dateButton->setMouseTracking(true);
    this->installEventFilter(this);
    ui->dateButton->installEventFilter(this);
    ui->dateButton->setMouseTracking(true);

    // Initialize timer for distinguishing between click and drag
    dragDelayTimer = new QTimer(this);
    dragDelayTimer->setSingleShot(true);
    connect(dragDelayTimer, &QTimer::timeout, this, [this]() {
        if (isDragging) dragStarted = true;
    });

    setMouseTracking(true);
    QString tooltipstyle = R"(
    QToolTip {
      background-color: white; color: black; border: 1px solid gray; border-radius: 5px;
    }
    )";
    setStyleSheet(tooltipstyle);

    // Load settings
    QSettings settings("Nepdate", "NepdateWidget");
    // Load text color setting (default to green)
    textColor = settings.value("TextColor", QColor(Qt::green)).value<QColor>();
    // Load font size setting (default to 11)
    fontSize = settings.value("FontSize", 11).toInt();
    // Apply the loaded color and font size
    applyTextAndFont();

    updateTimer = new QTimer(this);
    connect(updateTimer, &QTimer::timeout, this, &MainWindow::updateDateButton);
    updateTimer->start(1000);

    setWindowFlags(Qt::Window | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint |
                   Qt::BypassWindowManagerHint | Qt::WindowDoesNotAcceptFocus);
    setMouseTracking(true);
    setAttribute(Qt::WA_TranslucentBackground);
    setWindowPosition();
    setupDefaultDate();
    this->setWindowFlag(Qt::Tool);

    // Set tooltip font once at startup
    QString tooltipFont = "Noto Sans Devanagari";
    QFontDatabase::addApplicationFont(":/resources/NotoSansDevanagari-VariableFont_wdth,wght.ttf");
    QToolTip::setFont(tooltipFont);

    // Platform-specific code
    #ifdef Q_OS_X11
    QWindow *window = this->windowHandle();
    if (window) {
        window->setFlag(Qt::X11BypassWindowManagerHint);
        window->setFlag(Qt::WindowStaysOnTopHint);
        window->setType(Qt::Notification);
    }
    #endif

    #ifdef Q_OS_WIN
    HWND hwnd = reinterpret_cast<HWND>(this->winId());
    SetWindowLong(hwnd, GWL_EXSTYLE, GetWindowLong(hwnd, GWL_EXSTYLE) | WS_EX_TOPMOST);
    SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0,
                 SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE | SWP_SHOWWINDOW);
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
        int x = screenGeometry.left() + (screenGeometry.width() - width()) / 1.3;
        int offsetY = 10;
        int y = screenGeometry.bottom() - height() + offsetY;
    #ifdef Q_OS_MAC
        int dockHeight = 0;
    #endif
        move(x, y);
    }
}

MainWindow::~MainWindow() {
    delete ui;
}

int MainWindow::cnvToNepali(int mm, int dd, int yy) {
    Bikram bsdate;
    bsdate.fromGregorian(yy, mm, dd);
    int nepaliYear = bsdate.getYear();
    int nepaliMonth = bsdate.getMonth();
    int nepaliDay = bsdate.getDay();

    std::string weekdayName = getWeekdayName(yy, mm, dd);
    QString nepaliMonthName = getnepalimonth(nepaliMonth);
    QString nepaliDayOfWeekName = QString::fromStdString(weekdayName);

    std::tm date = {};
    date.tm_year = yy - 1900;
    date.tm_mon = mm - 1;
    date.tm_mday = dd;
    TithiResult tithiResult = calculateTithi(date);
    QString tithipaksha = QString::fromStdString(tithiResult.paksha + " " + tithiResult.tithiName);

    QString nepaliFormat = QString::number(nepaliYear) + " " +
                           nepaliMonthName + " " +
                           QString::number(nepaliDay) + " गते " +
                           nepaliDayOfWeekName;
    QString nepalitooltip = nepaliFormat + " " + tithipaksha;

    QLocale nepaliLocale(QLocale::Nepali);
    nepaliFormat.replace(QString::number(nepaliYear), nepaliLocale.toString(nepaliYear));
    nepaliFormat.replace(QString::number(nepaliDay), nepaliLocale.toString(nepaliDay));
    nepaliFormat.replace(commaRegex, QString());
    nepalitooltip.replace(QString::number(nepaliYear), nepaliLocale.toString(nepaliYear));
    nepalitooltip.replace(QString::number(nepaliDay), nepaliLocale.toString(nepaliDay));
    nepalitooltip.replace(commaRegex, QString());

    // Only update if content changed
    if (ui->dateButton->text() != nepaliFormat ||
        ui->dateButton->toolTip() != nepalitooltip) {

        ui->dateButton->setText(nepaliFormat);
        ui->dateButton->setToolTip(nepalitooltip);
        ui->dateButton->setToolTipDuration(3000);
    }

    return 0;
}

// This function now applies text color, font size, and icon size
void MainWindow::applyTextAndFont() {
    // Set color and font size via stylesheet
    QString styleSheet = QString("QPushButton { color: %1; font-size: %2pt; background-color: rgba(0, 0, 0, 0); border: none; outline: none; text-align: left; padding-left: 5px; }")
                             .arg(textColor.name())
                             .arg(fontSize);
    ui->dateButton->setStyleSheet(styleSheet);

    int iconDimension = fontSize + 8;
    ui->dateButton->setIconSize(QSize(iconDimension, iconDimension));

    // Immediately save text color and font size
    QSettings settings("Nepdate", "NepdateWidget");
    settings.setValue("TextColor", textColor);
    settings.setValue("FontSize", fontSize);
}

bool MainWindow::eventFilter(QObject *watched, QEvent *event) {
    if (watched == this || watched == ui->dateButton) {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);

        if (event->type() == QEvent::MouseButtonPress) {
            if (mouseEvent->button() == Qt::LeftButton) {
                isDragging = true;
                dragStarted = false;
    #if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
                dragStartPosition = mouseEvent->globalPosition().toPoint() - this->pos();
    #else
                dragStartPosition = mouseEvent->globalPos() - this->pos();
    #endif
                event->accept();
                return true;
            }
        } else if (event->type() == QEvent::MouseMove) {
            if (isDragging) {
    #if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
                QPoint currentPos = mouseEvent->globalPosition().toPoint();
    #else
                QPoint currentPos = mouseEvent->globalPos();
    #endif
                int distance = (currentPos - (this->pos() + dragStartPosition)).manhattanLength();
                if (distance > 5) {
                    dragStarted = true;
                    move(currentPos - dragStartPosition);
                    event->accept();
                    return true;
                }
            } else if (watched == ui->dateButton) {
    #if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
                QToolTip::showText(mouseEvent->globalPosition().toPoint(), ui->dateButton->toolTip(), ui->dateButton);
    #else
                QToolTip::showText(mouseEvent->globalPos(), ui->dateButton->toolTip(), ui->dateButton);
    #endif
            }
        } else if (event->type() == QEvent::MouseButtonRelease) {
            if (mouseEvent->button() == Qt::LeftButton) {
                isDragging = false;
                if (!dragStarted && watched == ui->dateButton) {
                    openCalendarWindow();
                } else {
                    // Save position after move
                    QSettings settings("Nepdate", "NepdateWidget");
                    settings.setValue("MainWindow/pos", this->pos());
                }
                dragStarted = false;
                event->accept();
                return true;
            }

        } else if (event->type() == QEvent::Leave) {
            QToolTip::hideText();
        }
    }
    return QMainWindow::eventFilter(watched, event);
}

void MainWindow::contextMenuEvent(QContextMenuEvent *event) {
    QMenu menu(tr("Context menu"), this);
    QString styleSheet = "QMenu { background-color: white; color: black; border: 1px solid gray; border-radius: 5px; }"
                         "QMenu::item { background-color: transparent; }"
                         "QMenu::item:selected { background-color: #0892D0; color: white;}";
    menu.setStyleSheet(styleSheet);

    // Font size selection menu
    QMenu* fontSizeMenu = menu.addMenu(tr("Font Size"));
    auto addFontSizeAction = [&](int size) {
        QAction* action = new QAction(QString::number(size), this);
        action->setCheckable(true);
        action->setChecked(fontSize == size);
        connect(action, &QAction::triggered, this, [this, size]() {
            fontSize = size;
            applyTextAndFont();
        });
        fontSizeMenu->addAction(action);
    };

    addFontSizeAction(8);
    addFontSizeAction(9);
    addFontSizeAction(10);
    addFontSizeAction(11);
    addFontSizeAction(12);
    addFontSizeAction(14);
    addFontSizeAction(16);
    addFontSizeAction(17);
    addFontSizeAction(18);


    // Slider for font size
    QSlider* fontSizeSlider = new QSlider(Qt::Horizontal, this);
    fontSizeSlider->setRange(5, 32);
    fontSizeSlider->setValue(fontSize);
    fontSizeSlider->setTickPosition(QSlider::TicksBelow);
    fontSizeSlider->setTickInterval(1); // Show ticks every 1 points

    // QWidgetAction to embed the slider in the menu
    QWidgetAction* sliderAction = new QWidgetAction(this);
    sliderAction->setDefaultWidget(fontSizeSlider);
    fontSizeMenu->addAction(sliderAction);
    connect(fontSizeSlider, &QSlider::sliderPressed, this, [&]() {
        menu.setWindowOpacity(0.4);
    });

    connect(fontSizeSlider, &QSlider::sliderReleased, this, [&]() {
        menu.setWindowOpacity(1.0);
    });

    // Connect slider's valueChanged signal to update font size
    connect(fontSizeSlider, &QSlider::valueChanged, this, [this](int value) {
        fontSize = value;
        applyTextAndFont();

    });

    // Text color selection menu
    QMenu* colorMenu = menu.addMenu(tr("Text Color"));

    auto addColorAction = [&](const QString& label, const QColor& color) {
        QAction* action = new QAction(label, this);
        action->setCheckable(true);
        action->setChecked(textColor == color);
        connect(action, &QAction::triggered, this, [this, color]() {
            textColor = color;
            applyTextAndFont(); // Use the unified style function
        });
        colorMenu->addAction(action);
    };

    // Fixed color options
    addColorAction(tr("Gray"), Qt::darkGray);
    addColorAction(tr("White"), Qt::white);
    addColorAction(tr("Black"), Qt::black);
    addColorAction(tr("Green"), Qt::green);
    addColorAction(tr("Red"), Qt::red);
    addColorAction(tr("Yellow"), Qt::yellow);

    // QColorDialog for color selector
    QAction* selectCustomColorAction = new QAction(tr("Select Custom Color..."), this);
    connect(selectCustomColorAction, &QAction::triggered, this, &MainWindow::showColorDialog);
    colorMenu->addAction(selectCustomColorAction);

    QAction *copyAction = new QAction(tr("Copy Nepali Date"), this);
    connect(copyAction, &QAction::triggered, this, &MainWindow::copyButtonText);
    menu.addAction(copyAction);

    menu.addSeparator();

    QAction *autostartAction = new QAction(isAutostartEnabled() ? tr("Disable Autostart") : tr("Enable Autostart"), this);
    connect(autostartAction, &QAction::triggered, this, [this, autostartAction]() {
        bool currentState = isAutostartEnabled();
        setAutostart(!currentState);
        autostartAction->setText(isAutostartEnabled() ? tr("Disable Autostart") : tr("Enable Autostart"));
    });
    menu.addAction(autostartAction);

    menu.addSeparator();

    QAction *exitAllAction = new QAction(tr("Exit"), this);
    connect(exitAllAction, &QAction::triggered, this, &MainWindow::exitAll);
    menu.addAction(exitAllAction);

    menu.exec(event->globalPos());
}

// Function to create and show the color dialog
void MainWindow::showColorDialog() {
    QColor initialColor = textColor;
    QColor selectedColor = QColorDialog::getColor(initialColor, this, tr("Select Color"));

    if (selectedColor.isValid()) {
        textColor = selectedColor;
        applyTextAndFont();
    } else {
        QMessageBox::information(this, tr("Color Selection"), tr("Color selection cancelled."));
    }
}


void MainWindow::openCalendarWindow() {
    if (!calendarWindow || !calendarWindow->isVisible()) {
        calendarWindow = new CalendarWindow(this);
        calendarWindow->setWindowModality(Qt::NonModal);
        connect(calendarWindow, &CalendarWindow::destroyed, this, [this]() {
            calendarWindow = nullptr;
        });
    }
    calendarWindow->show();
    calendarWindow->raise();
    calendarWindow->activateWindow();
}

void MainWindow::on_dateButton_clicked() {
    openCalendarWindow();
}

void MainWindow::copyButtonText() {
    QClipboard *clipboard = QGuiApplication::clipboard();
    clipboard->setText(ui->dateButton->text());
}

void MainWindow::setupDefaultDate() {
    QDate today = QDate::currentDate();
    if (today != lastUpdatedDate) {
        lastUpdatedDate = today;
        cnvToNepali(today.month(), today.day(), today.year());
    }
}

void MainWindow::updateDateButton() {
    QDate today = QDate::currentDate();
    if (today == lastUpdatedDate) return;
    lastUpdatedDate = today;
    cnvToNepali(today.month(), today.day(), today.year());
}

QString MainWindow::getnepalimonth(int m) {
    static const QString nepaliMonths[] = {
        "बैशाख", "जेष्ठ", "आषाढ", "श्रावण", "भाद्र", "आश्विन",
        "कार्तिक", "मंसिर", "पुष", "माघ", "फाल्गुण", "चैत्र"
    };
    if (m >= 1 && m <= 12) {
        return nepaliMonths[m - 1];
    } else {
        return QString();
    }
}

void MainWindow::exitAll() {
    QList<QWidget*> topLevelWidgets = QApplication::topLevelWidgets();
    for (QWidget* widget : topLevelWidgets) {
        MainWindow* mainWindow = qobject_cast<MainWindow*>(widget);
        if (mainWindow) {
            mainWindow->close();
        }
    }
    qApp->quit();
}

bool MainWindow::isAutostartEnabled() {
#ifdef Q_OS_WIN
    QSettings registrySettings(kAutostartKey, QSettings::NativeFormat);
    return registrySettings.contains(kAppName);
#else
    QFile desktopFile(kDesktopFileLinux);
    return desktopFile.exists();
#endif
}

void MainWindow::setAutostart(bool enabled) {
#ifdef Q_OS_WIN
    QSettings registrySettings(kAutostartKey, QSettings::NativeFormat);
    if (enabled) {
        registrySettings.setValue(kAppName, QDir::toNativeSeparators(QCoreApplication::applicationFilePath()));
    } else {
        registrySettings.remove(kAppName);
    }
#else
    if (enabled) {
        QDir().mkpath(kAutostartDirLinux);
        QFile desktopFile(kDesktopFileLinux);
        if (desktopFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&desktopFile);
            out << "[Desktop Entry]\n";
            out << "Type=Application\n";
            out << "Name=Nepali Calendar widget\n";
            out << "Exec=" << QCoreApplication::applicationFilePath() << "\n";
            out << "Icon=calendar\n";
            desktopFile.close();
        }
    } else {
        QFile::remove(kDesktopFileLinux);
    }
#endif
}
