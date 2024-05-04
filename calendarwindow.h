#ifndef CALENDARWINDOW_H
#define CALENDARWINDOW_H

#include <QMainWindow>
#include <QScreen>


QT_BEGIN_NAMESPACE
namespace Ui { class CalendarWindow; }
QT_END_NAMESPACE

class CalendarWindow : public QMainWindow
{
    Q_OBJECT

public:
    CalendarWindow(QWidget *parent = nullptr);
    ~CalendarWindow();
    void updateNepaliDate(const QString& nepaliDate);

private slots:
    void on_pushButton_clicked();

private:
    Ui::CalendarWindow *ui;

    // Helper method to center the window on the screen
    void centerOnScreen();
};

#endif // CALENDARWINDOW_H
