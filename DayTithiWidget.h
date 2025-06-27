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
        int minSize = qMin(width(), height()) / 2;
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

        // Font sizes
        int dayFontSize = std::max(12, static_cast<int>(minDim * 0.14));
        int tithiFontSize = std::max(8, static_cast<int>(minDim * 0.05));
        int englishDayFontSize = std::max(8, static_cast<int>(minDim * 0.05));

        // --- Update Fonts ---
        int fontId = QFontDatabase::addApplicationFont(":/resources/Martel-Bold.ttf");
        QString fontFamily = QFontDatabase::applicationFontFamilies(fontId).at(0);
        QFont dayFont = dayLabel->font();
        dayFont.setPointSize(dayFontSize);
        dayFont.setFamily(fontFamily);
        dayFont.setBold(true);
        dayLabel->setFont(dayFont);
        dayLabel->repaint();

        QFont tithiFont = tithiLabel->font();
        tithiFont.setPointSize(tithiFontSize);
        tithiFont.setItalic(true);
        tithiLabel->setFont(tithiFont);


        dayLabel->adjustSize();
        int dayX = 20;
        int dayY = (h / 3) - (dayLabel->height() / 2); // Move up
        dayLabel->move(dayX, dayY);

        // --- Reposition English Day Label (top-right) ---
        englishDayLabel->adjustSize();
        englishDayLabel->move(width() - englishDayLabel->width() - 6, 6);


        // --- Reposition Tithi Label (bottom-left) ---
        tithiLabel->adjustSize();
        tithiLabel->move(6, h - tithiLabel->height() - 6);


        QFont englishDayFont = englishDayLabel->font();
        englishDayFont.setPointSize(englishDayFontSize);
        englishDayLabel->setFont(englishDayFont);
        englishDayLabel->adjustSize();
        adjustIconSize(); // Should reposition icon manually too
    }

};

#endif // DAYTITHIWIDGET_H
