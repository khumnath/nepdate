#ifndef CALENDARWINDOW_H
#define CALENDARWINDOW_H
#include <QMainWindow>

#include "bikram.h"
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

    int gYear, gMonth, gDay;
    explicit CalendarWindow(QWidget *parent = nullptr);
    ~CalendarWindow(); 
    QString getBikramMonthName(int month) {
        QStringList bikramMonths = {"बैशाख", "जेठ", "असार", "श्रावण", "भाद्र",
                                     "आश्विन", "कार्तिक", "मंसिर", "पौष", "माघ",
                                     "फाल्गुन", "चैत"};
        if (month >= 1 && month <= 12) {
            return bikramMonths[month - 1];
        } else {
            return QString("Invalid Month");
        }
    }
    QString getEnglishMonthName(int month) {
        QStringList englishMonths = {"जनवरी", "फेब्रुअरी", "मार्च", "अप्रिल", "मे", "जुन",
                                     "जुलाई", "अगस्ट", "सेप्टेम्बर", "अक्टोबर", "नोभेम्बर", "डिसेम्बर"};

        // Adjust month to use as an index (0-based in QStringList)
        int index = month - 1;

        if (index >= 0 && index < englishMonths.size()) {
            return englishMonths.at(index);
        }

        return "";  // Return empty string if index is out of range
    }

    // Function to convert number to Nepali numeral string
    QString convertToNepaliNumerals(int number) {
        QString nepaliNumbers[] = {"०", "१", "२", "३", "४", "५", "६", "७", "८", "९"};
        QString result;
        QString numStr = QString::number(number);

        for (QChar ch : numStr) {
            if (ch.isDigit()) {
                int digit = ch.digitValue();
                result += nepaliNumbers[digit];
            } else {
                result += ch;
            }
        }

        return result;
    }




private slots:
    void onAdYearChanged(int index);
    void onAdMonthChanged(int index);
    void onAdDayChanged(int index);
    void onBsYearChanged(int index);
    void onBsMonthChanged(int index);
    void onBsDayChanged(int index);
    void ontodayButtonclicked();
    void showMenu();
    void showAbout();
    void openSourceCode();

protected:
    void resizeEvent(QResizeEvent* event) override;
    bool eventFilter(QObject *object, QEvent *event) override;

private:
    Ui::CalendarWindow *ui;
    bikram converter;
    bool blockSignals;
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
