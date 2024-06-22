#ifndef DAYTITHIWIDGET_H
#define DAYTITHIWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>

class DayTithiWidget : public QWidget {
    Q_OBJECT

public:
    explicit DayTithiWidget(const QString &day, const QString &tithi, QWidget *parent = nullptr)
        : QWidget(parent) {
        QVBoxLayout *layout = new QVBoxLayout(this);
        layout->setSpacing(0);
        layout->setContentsMargins(0, 0, 0, 0);  // Remove margins

        dayLabel = new QLabel(day, this);
        dayLabel->setObjectName("dayLabel");  // Set object name for styling
        dayLabel->setStyleSheet("font-size: 19px; color: black; background-color: transparent; font-family: 'laila';");

        tithiLabel = new QLabel(tithi, this);
        tithiLabel->setObjectName("tithiLabel");  // Set object name for styling
        tithiLabel->setStyleSheet("font-size: 10px; color: blue; background-color: transparent; font-family: 'laila';");

        layout->addWidget(dayLabel);
        layout->addWidget(tithiLabel);
    }
        void setTodayStyle() {
        setStyleSheet("background-color: transparent;");
        dayLabel->setStyleSheet("font-size: 20px; font-weight: bold; text-decoration: underline; color: green; font-family: 'laila';");
    }

    void setSaturdayStyle() {
        setStyleSheet("background-color: transparent;");
        dayLabel->setStyleSheet("font-size: 19px; color: red; font-family: 'laila';");
    }



private:
    QLabel *dayLabel;
    QLabel *tithiLabel;
};

#endif // DAYTITHIWIDGET_H
