#ifndef DAYTITHIWIDGET_H
#define DAYTITHIWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QIcon>
#include <QPixmap>
#include <QPainter>
#include <QResizeEvent>
#include <QFont>
#include <QFontDatabase>
#include <QtGlobal>
#include <QMouseEvent>
#include <QMessageBox>
#include "bikram.h"
#include <QDate>
#include "calendarlogic.h"
#include "panchanga.h"

class DayTithiWidget : public QWidget {
    Q_OBJECT

public:
    explicit DayTithiWidget(const QString &day, const QString &tithi, const QString &englishDay, QWidget *parent = nullptr)
        : QWidget(parent), dayLabelText(day), tithiLabelText(tithi), englishDayLabelText(englishDay) {

        // Day number (centered large text)
        dayLabel = new QLabel(day, this);
        dayLabel->setObjectName("dayLabel");
        dayLabel->setAlignment(Qt::AlignCenter);
        dayLabel->setStyleSheet("color: black; background-color: transparent;");

        // English day number (top-right, small, gray)
        englishDayLabel = new QLabel(englishDay, this);
        englishDayLabel->setObjectName("englishDayLabel");
        englishDayLabel->setAlignment(Qt::AlignRight | Qt::AlignTop);
        englishDayLabel->setStyleSheet("color:rgb(74, 32, 240); background-color: transparent;");
        englishDayLabel->adjustSize();

        // Tithi label (bottom left)
        tithiLabel = new QLabel(tithi, this);
        tithiLabel->setObjectName("tithiLabel");
        tithiLabel->setStyleSheet("color: #2563eb; background-color: transparent;");
        tithiLabel->setVisible(true);
        tithiLabel->adjustSize();

        // Icon label (bottom right)
        iconLabel = new QLabel(this);
        iconLabel->setAttribute(Qt::WA_TransparentForMouseEvents);
        iconLabel->setStyleSheet("background-color: transparent;");
        iconLabel->adjustSize();
    }

    void setTodayStyle() {
        QFont font = dayLabel->font();
        font.setBold(true);
        dayLabel->setFont(font);
        dayLabel->adjustSize();
        int contentWidth = dayLabel->width();
        int contentHeight = dayLabel->height();
        int maxDimension = qMax(contentWidth, contentHeight);
        int dynamicPadding = maxDimension / 3;
        int finalPadding = qBound(10, dynamicPadding, 30);
        int circleDiameter = maxDimension + (finalPadding * 2);
        if (circleDiameter < 40) circleDiameter = 40;
        dayLabel->setFixedSize(circleDiameter, circleDiameter);
        QString styleSheet = QString(
            "QLabel {"
            "   background-color:rgb(91, 240, 156);"
            "   color: white;"
            "   border-radius: %1px;" // Half of the diameter for a circle
            "   qproperty-alignment: AlignCenter;" // Center the text within the circular area
            "}"
        ).arg(circleDiameter / 2);

        dayLabel->setStyleSheet(styleSheet);
    }


    void setSaturdayStyle() {
        dayLabel->setStyleSheet("color: red;");
    }

private:
    QLabel *dayLabel;
    QLabel *tithiLabel;
    QLabel *iconLabel;
    QLabel *englishDayLabel;
    QString dayLabelText;
    QString tithiLabelText;
    QString englishDayLabelText;
    QSize iconSize = QSize(24, 24);
    QIcon storedIcon;
    qreal storedIconOpacity = 1.0;
    void adjustIconSize() {
        int minSize = qMin(width(), height()) / 4;
        iconSize = QSize(minSize + 18, minSize + 18);
        iconLabel->setFixedSize(iconSize);
        iconLabel->setScaledContents(true);
        iconLabel->move(width() - iconSize.width() - 4, height() - iconSize.height() - 4);
        // Always re-render from original icon for sharpness
        if (!storedIcon.isNull()) {
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
            QPixmap pixmap = storedIcon.pixmap(iconSize);
#else
            QPixmap pixmap = storedIcon.pixmap(iconSize, QIcon::Normal, QIcon::On);
#endif
            QPixmap transparentPixmap(pixmap.size());
            transparentPixmap.fill(Qt::transparent);
            QPainter painter(&transparentPixmap);
            painter.setOpacity(storedIconOpacity);
            painter.drawPixmap(0, 0, pixmap);
            painter.end();
            iconLabel->setPixmap(transparentPixmap);
        }
    }
public:
    void setIcon(const QIcon &icon, qreal opacity = 1.0) {
        storedIcon = icon;
        storedIconOpacity = opacity;
        adjustIconSize();
    }

protected:
    void resizeEvent(QResizeEvent *event) override {
        QWidget::resizeEvent(event);
        int w = width();
        int h = height();
        int minDim = qMin(w, h);
        int dayFontSize = std::max(12, static_cast<int>(minDim * 0.14));
        int tithiFontSize = std::max(8, static_cast<int>(minDim * 0.05));
        int englishDayFontSize = std::max(8, static_cast<int>(minDim * 0.05));
        int fontId = QFontDatabase::addApplicationFont(":/resources/NotoSansDevanagari-VariableFont_wdth,wght.ttf");
        QString fontFamily = QFontDatabase::applicationFontFamilies(fontId).at(0);
        QFont dayFont = dayLabel->font();
        dayFont.setPointSize(dayFontSize);
        dayFont.setFamily(fontFamily);
        dayFont.setBold(false);
        dayLabel->setFont(dayFont);
        QFont tithiFont = tithiLabel->font();
        tithiFont.setPointSize(tithiFontSize);
        tithiFont.setItalic(true);
        tithiLabel->setFont(tithiFont);
        QFont englishDayFont = englishDayLabel->font();
        englishDayFont.setPointSize(englishDayFontSize);
        englishDayLabel->setFont(englishDayFont);
        // Position labels without adjustSize/repaint
        int dayX = 20;
        int dayY = (h / 3) - (dayLabel->height() / 2);
        dayLabel->move(dayX, dayY);
        englishDayLabel->move(width() - englishDayLabel->width() - 6, 6);
        tithiLabel->move(6, h - tithiLabel->height() - 6);
        adjustIconSize();
    }
    void mousePressEvent(QMouseEvent *event) override {
        if (event->button() == Qt::LeftButton) {
            int day = property("gDay").toInt(); // Use actual Gregorian day
            int month = property("month").toInt();
            int year = property("year").toInt();
            // Gregorian date
            QDate gdate(year, month, day);
            // Bikram date
            Bikram bs;
            bs.fromGregorian(year, month, day);
            int bsYear = bs.getYear();
            int bsMonth = bs.getMonth();
            int bsDay = bs.getDay();
            // Panchanga details
            std::tm date = {};
            date.tm_year = year - 1900;
            date.tm_mon = month - 1;
            date.tm_mday = day;
            TithiResult tithi = calculateTithi(date);
            YogaResult yoga = calculateYoga(date);
            KaranResult karan = calculateKaran(date);
            NakshatraResult nakshatra = calculateNakshatra(date);
            RashiResult rashi = calculateRashi(date);
            QString sunrise = QString::fromStdString(calculateSunriseOrSunset(date, true));
            QString sunset = QString::fromStdString(calculateSunriseOrSunset(date, false));
            // Show details dialog
            QString details = QString(
                "Gregorian: %1\nBikram: %2-%3-%4\nTithi: %5\nPaksha: %6\nYoga: %7\nKarana: %8\nNakshatra: %9\nRashi: %10\nSunrise: %11\nSunset: %12")
                .arg(QString::number(gdate.year()) + "-" + QString::number(gdate.month()).rightJustified(2, '0') + "-" + QString::number(gdate.day()).rightJustified(2, '0'))
                .arg(convertToNepaliNumerals(bsYear)).arg(convertToNepaliNumerals(bsMonth)).arg(convertToNepaliNumerals(bsDay))
                .arg(QString::fromStdString(tithi.tithiName))
                .arg(QString::fromStdString(tithi.paksha))
                .arg(QString::fromStdString(yoga.yogaName))
                .arg(QString::fromStdString(karan.karanName))
                .arg(QString::fromStdString(nakshatra.nakshatraName))
                .arg(QString::fromStdString(rashi.rashiName))
                .arg(sunrise)
                .arg(sunset);
            QWidget *topLevel = this->window();
            QMessageBox::information(topLevel, tr("Day Details"), details);
        }
        QWidget::mousePressEvent(event);
    }

};

#endif // DAYTITHIWIDGET_H
