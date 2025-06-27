#ifndef CALENDARWINDOW_H
#define CALENDARWINDOW_H

#include <QMainWindow>
#include "bikram.h"
#include "qdatetime.h"
#include <QMessageBox>
#include <QMenu>
#include <QDesktopServices>
#include <QUrl>

namespace Ui {
class CalendarWindow;
}

class CalendarWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit CalendarWindow(QWidget *parent = nullptr);
    ~CalendarWindow();

    // Static constant month names for Gregorian and Bikram Sambat calendars
    static const QStringList gregorianMonths;
    static const QStringList bikramMonths;

    // Methods to get month names
    QString getBikramMonthName(int month);
    QString getEnglishMonthName(int month);

    // Function to convert number to Nepali numeral string
    QString convertToNepaliNumerals(int number);

private slots:
    void onAdYearChanged(int index);
    void onAdMonthChanged(int index);
    void onAdDayChanged(int index);
    void onBsYearChanged(int index);
    void onBsMonthChanged(int index);
    void onBsDayChanged(int index);
    void ontodayButtonclicked();
    void onnextMonthButtonclicked();
    void onpreviousMonthButtonclicked();
    void showMenu();
    void showAbout();
    void openSourceCode();
    void setWindowPosition();

protected:
    void resizeEvent(QResizeEvent* event) override;
    bool eventFilter(QObject *object, QEvent *event) override;
    void closeEvent(QCloseEvent *event) override;

private:
    Ui::CalendarWindow *ui;
    Bikram converter;
    bool blockSignals;
    int gYear, gMonth, gDay;  // Moved to private section for encapsulation
    QDate currentBikramDate;

    // Private helper methods
    void centerOnScreen();
    void updateBsDateFromAd(int year, int month, int day);
    void updateAdDateFromBs(int year, int month, int day);
    void updateCalendar(int year, int month);
    void adjustCellSizes();
    void centerWidgets();
    void adjustCalendarTableSize();
    void populateBsDays(int year, int month);
};

#endif // CALENDARWINDOW_H
