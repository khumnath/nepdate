#include "calendarwindow.h"
#include "ui_calendarwindow.h"
#include <QScreen>
#include <QUrl>
#include <QDesktopServices>

CalendarWindow::CalendarWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::CalendarWindow)
{
    ui->setupUi(this);


    // Center the window on the screen
    centerOnScreen();
}

CalendarWindow::~CalendarWindow()
{
    delete ui;
}

void CalendarWindow::centerOnScreen()
{
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

void CalendarWindow::on_pushButton_clicked()
{
    QDesktopServices::openUrl(QUrl("http://github.com/khumnath/nepdate", QUrl::TolerantMode));
}




