#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QPushButton>
#include <QMainWindow>
#include <QStyledItemDelegate>
#include <QCalendarWidget>
#include <QPainter>
#include <QDate>
#include <QMouseEvent>
#include <QPushButton>
#include <QVBoxLayout>
#include "calendarwindow.h"

struct NepaliDateStruct { // Renamed the structure to avoid conflict
    int yy;
    int mm;
    int dd;
    int day;
    char wmonth[20];
    char wday[20];
};

struct DateStruct2 {
    int yy;
    int mm;
    int dd;
    int day;
};

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    int is_leap_year(int);
    int cnvToNepali(int, int, int);
    NepaliDateStruct cnvToEnglish(int, int, int); // Updated to use the renamed structure
    int what();
    int err(int errno, char *desc);
    static int calculateDayOfWeek(int year, int month, int day);
    ~MainWindow();

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void contextMenuEvent(QContextMenuEvent *event) override;

public slots:
    void openCalendarWindow(const QString &link);

private slots:
    QBitmap createRoundRectMask(const QSize &size, int radius);
    QString get_nepali_month(int);
    void copyButtonText();
    void on_dateButton_clicked();
    void updateDateButton();

private:
    void setupDefaultDate();
    // Declaration of getWeekdayName function
    std::string getWeekdayName(int year, int month, int day);
    QTimer *updateTimer;

    Ui::MainWindow *ui;
    bool isDragging;
    QPoint dragStartPosition;
    CalendarWindow *calendarWindow = nullptr;
    QTimer *timer;
};

#endif // MAINWINDOW_H
