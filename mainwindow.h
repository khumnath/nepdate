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
#include <QImage>
#include <QTimer>
#include <QScreen>
#include <QClipboard>
#include <QLocale>
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
    void setAutostart(bool enabled);
    ~MainWindow();

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void contextMenuEvent(QContextMenuEvent *event) override;
    void exitAll();
    bool eventFilter(QObject *watched, QEvent *event) override;
public slots:
    void openCalendarWindow(const QString &link);

private slots:

    void copyButtonText();
    void updateDateButton();
    void on_dateButton_clicked();

private:
    void setupDefaultDate();
    int cnvToNepali(int mm, int dd, int yy);
    std::string getWeekdayName(int year, int month, int day);
    QString getnepalimonth(int m);
    void setWindowPosition();
    void adjustTextColorBasedOnBackground();
    QColor getAverageColor(const QImage &image);

    Ui::MainWindow *ui;
    QTimer *updateTimer;
    bool isDragging;
    QPoint dragStartPosition;
    CalendarWindow *calendarWindow;
};

#endif // MAINWINDOW_H
