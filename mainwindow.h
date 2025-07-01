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

struct NepaliDateStruct {
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
    void contextMenuEvent(QContextMenuEvent *event) override;
    void exitAll();
    bool eventFilter(QObject *watched, QEvent *event) override;

public slots:
    void openCalendarWindow();

private slots:
    void copyButtonText();
    void updateDateButton();
    void on_dateButton_clicked();
    bool isAutostartEnabled();

private:
    void setupDefaultDate();
    int cnvToNepali(int mm, int dd, int yy);
    std::string getWeekdayName(int year, int month, int day);
    QString getnepalimonth(int m);
    void setWindowPosition();
    void applyTextColor();  // New method for manual color application
    QColor getAverageColor(const QImage &image);
    int fontSize; // NEW: To store the current font size
        void applyTextAndFont();

    QDate lastUpdatedDate;
    QColor textColor;  // Added for manual color selection

    Ui::MainWindow *ui;
    QTimer *updateTimer;
    CalendarWindow *calendarWindow;
    bool isDragging;
    bool dragStarted;
    QPoint dragStartPosition;
    QTimer *dragDelayTimer;
    QPoint windowPosOnPress;

    // Autostart constants
    static const QString kAppName;
    static const QString kAutostartKey;
    static const QString kAutostartDirLinux;
    static const QString kDesktopFileLinux;
};

#endif // MAINWINDOW_H
