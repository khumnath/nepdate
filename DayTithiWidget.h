#ifndef DAYTITHIWIDGET_H
#define DAYTITHIWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QIcon>
#include <QPixmap>
#include <QPainter>
#include <QResizeEvent>
#include <QFont>
#include <QFontDatabase>
#include <QMouseEvent>
#include <QStyleOption>
#include "panchanga.h"
#include "bikram.h"
#include <QDate>
#include "calendarlogic.h"
#include <QDialog>
#include <QPushButton>
#include <QTextEdit>
#include <QApplication>
#include <QClipboard>
#include <QTimer>

class DayTithiWidget : public QWidget {
    Q_OBJECT

public:
    explicit DayTithiWidget(const QString &day = "", const QString &tithi = "", const QString &englishDay = "", QWidget *parent = nullptr)
        : QWidget(parent), dayLabelText(day), tithiLabelText(tithi), englishDayLabelText(englishDay) {

        setAttribute(Qt::WA_Hover);
        QSizePolicy policy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        policy.setHorizontalStretch(1);
        policy.setVerticalStretch(1);
        setSizePolicy(policy);
        setMinimumSize(60, 60);

        QVBoxLayout *mainLayout = new QVBoxLayout(this);
        mainLayout->setContentsMargins(4, 4, 4, 4);
        mainLayout->setSpacing(2);

        QHBoxLayout *topLayout = new QHBoxLayout();
        topLayout->setContentsMargins(0, 0, 0, 0);
        topLayout->setSpacing(0);

        englishDayLabel = new QLabel(englishDay, this);
        englishDayLabel->setStyleSheet("color:rgb(74, 32, 240); background-color: transparent;");
        englishDayLabel->setAlignment(Qt::AlignRight);
        topLayout->addStretch();
        topLayout->addWidget(englishDayLabel);

        dayLabel = new QLabel(day, this);
        dayLabel->setAlignment(Qt::AlignCenter);
        dayLabel->setStyleSheet("color: black; background-color: transparent;");
        dayLabel->setMinimumSize(1, 1);

        tithiLabel = new QLabel(tithi, this);
        tithiLabel->setStyleSheet("color: #2563eb; background-color: transparent;");
        tithiLabel->setAlignment(Qt::AlignLeft);

        mainLayout->addLayout(topLayout);
        mainLayout->addStretch();
        mainLayout->addWidget(dayLabel);
        mainLayout->addStretch();
        mainLayout->addWidget(tithiLabel);

        iconLabel = new QLabel(this);
        iconLabel->setStyleSheet("background-color: transparent;");
        iconLabel->setAttribute(Qt::WA_TransparentForMouseEvents);

        applyResponsiveLayout();
    }

    void updateLabels(const QString &day, const QString &tithi, const QString &englishDay) {
        dayLabel->setText(day);
        tithiLabel->setText(tithi);
        englishDayLabel->setText(englishDay);
        applyResponsiveLayout();
    }

    void setTodayStyle() {
        QFont font = dayLabel->font();
        font.setBold(true);
        dayLabel->setFont(font);
        int size = 40;
        dayLabel->setFixedSize(size, size);
        QString styleSheet = QString(
                                 "QLabel {"
                                 "   background-color: rgb(91, 240, 156);"
                                 "   color: white;"
                                 "   border-radius: %1px;"
                                 "   qproperty-alignment: AlignCenter;"
                                 "}"
                                 ).arg(size / 2);

        dayLabel->setStyleSheet(styleSheet);
        applyResponsiveLayout();
    }

    void setSaturdayStyle() {
        dayLabel->setStyleSheet("color: red;");
        applyResponsiveLayout();
    }

    void setNormalStyle() {
        dayLabel->setStyleSheet("color: black; background-color: transparent;");
        QFont font = dayLabel->font();
        font.setBold(false);
        dayLabel->setFont(font);
        applyResponsiveLayout();
    }

    void setIcon(const QIcon &icon, qreal opacity = 1.0) {
        storedIcon = icon;
        storedIconOpacity = opacity;
    
        if (icon.isNull()) {
            iconLabel->clear();
            iconLabel->setVisible(false);
        } else {
            iconLabel->setVisible(true);
            adjustIconSize();
        }
    }
    

    void setSelected(bool selected) {
        isSelected = selected;
        update();
    }

signals:
    void dayCellClicked(DayTithiWidget *cell);  // use to manage global selection

protected:
    void resizeEvent(QResizeEvent *event) override {
        QWidget::resizeEvent(event);
        applyResponsiveLayout();
    }

    void mousePressEvent(QMouseEvent *event) override {
        if (event->button() == Qt::LeftButton) {
            isSelected = true;
            update();
            emit dayCellClicked(this);

            int day = property("gDay").toInt();
            int month = property("month").toInt();
            int year = property("year").toInt();

            QDate gdate(year, month, day);
            Bikram bs;
            bs.fromGregorian(year, month, day);

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

            QString details = QString(
                "Gregorian: %1\n"
                "Bikram: %2-%3-%4\n"
                "Tithi: %5\n"
                "Paksha: %6\n"
                "Yoga: %7\n"
                "Karana: %8\n"
                "Nakshatra: %9\n"
                "Rashi: %10\n"
                "Sunrise: %11\n"
                "Sunset: %12")
                .arg(gdate.toString("yyyy-MM-dd"))
                .arg(convertToNepaliNumerals(bs.getYear()))
                .arg(convertToNepaliNumerals(bs.getMonth()))
                .arg(convertToNepaliNumerals(bs.getDay()))
                .arg(QString::fromStdString(tithi.tithiName))
                .arg(QString::fromStdString(tithi.paksha))
                .arg(QString::fromStdString(yoga.yogaName))
                .arg(QString::fromStdString(karan.karanName))
                .arg(QString::fromStdString(nakshatra.nakshatraName))
                .arg(QString::fromStdString(rashi.rashiName))
                .arg(sunrise)
                .arg(sunset);

            QDialog *detailsDialog = new QDialog(this);
            detailsDialog->setWindowTitle(tr("Day Details"));
            detailsDialog->setMinimumSize(350, 300);

            QVBoxLayout *dialogLayout = new QVBoxLayout(detailsDialog);

            QTextEdit *detailsTextEdit = new QTextEdit(detailsDialog);
            detailsTextEdit->setPlainText(details);
            detailsTextEdit->setReadOnly(true);
            detailsTextEdit->setTextInteractionFlags(Qt::TextSelectableByMouse | Qt::TextSelectableByKeyboard);
            detailsTextEdit->setStyleSheet("color: black; background-color: white;");
            detailsTextEdit->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
            detailsTextEdit->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

            QFontMetrics fm(detailsTextEdit->font());
            int lineHeight = fm.lineSpacing();
            int numLines = details.count('\n') + 1;
            detailsTextEdit->setFixedHeight(lineHeight * numLines + detailsTextEdit->contentsMargins().top() + detailsTextEdit->contentsMargins().bottom() + 10);

            dialogLayout->addWidget(detailsTextEdit);

            QLabel *copiedMessageLabel = new QLabel(detailsDialog);
            copiedMessageLabel->setAlignment(Qt::AlignCenter);
            copiedMessageLabel->setStyleSheet("color: green; font-weight: bold;");
            copiedMessageLabel->hide();
            dialogLayout->addWidget(copiedMessageLabel);

            QHBoxLayout *buttonLayout = new QHBoxLayout();
            buttonLayout->addStretch(); // Add stretch before buttons

            QPushButton *copyButton = new QPushButton(tr("Copy"), detailsDialog);
            copyButton->setStyleSheet("color: white; background: #3EACBA; border: 1px solid #379AA4; background-image: linear-gradient(to top, #48C6D4, #3EACBA); border-radius: 5px; padding: 5px 15px;");
            connect(copyButton, &QPushButton::clicked, [detailsTextEdit, copiedMessageLabel]() {
                QApplication::clipboard()->setText(detailsTextEdit->toPlainText());
                copiedMessageLabel->setText(QObject::tr("Copied to clipboard!"));
                copiedMessageLabel->show();
                QTimer::singleShot(2000, copiedMessageLabel, &QLabel::hide);
            });
            buttonLayout->addWidget(copyButton);

            QPushButton *okButton = new QPushButton(tr("OK"), detailsDialog);
            okButton->setStyleSheet( "color: white; background: #3EACBA; border: 1px solid #379AA4; background-image: linear-gradient(to top, #48C6D4, #3EACBA); border-radius: 5px; padding: 5px 15px;");
            connect(okButton, &QPushButton::clicked, detailsDialog, &QDialog::accept);
            buttonLayout->addWidget(okButton);

            buttonLayout->addStretch(); // Add stretch after buttons

            dialogLayout->addLayout(buttonLayout);

            detailsDialog->exec();
            delete detailsDialog;
        }
        QWidget::mousePressEvent(event);
    }

    bool event(QEvent *event) override {
        if (event->type() == QEvent::Enter) {
            isHovered = true;
            update();
        } else if (event->type() == QEvent::Leave) {
            isHovered = false;
            update();
        }
        return QWidget::event(event);
    }

    void paintEvent(QPaintEvent *event) override {
        Q_UNUSED(event);
        QPainter painter(this);
        QColor bgColor = Qt::white;

        if (isSelected) {
            bgColor = QColor("#bfdbfe");  // light blue
        } else if (isHovered) {
            bgColor = QColor("#d1fae5");  // light green
        }

        painter.fillRect(rect(), bgColor);

        QStyleOption opt;
        opt.initFrom(this);
        style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);
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

    bool isHovered = false;
    bool isSelected = false;

    void adjustIconSize() {
        int minSize = qMin(width(), height()) / 6;
        iconSize = QSize(minSize + 18, minSize + 18);
        iconLabel->setFixedSize(iconSize);
        iconLabel->setScaledContents(true);
        iconLabel->move(width() - iconSize.width() - 4, height() - iconSize.height() - 4);
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

    void applyResponsiveLayout() {
        int w = width();
        int h = height();
        int minDim = qMin(w, h);

        int dayFontSize = std::max(14, static_cast<int>(minDim * 0.27));

        int tithiFontSize = std::max(8, static_cast<int>(minDim * 0.05));
        int englishDayFontSize = std::max(8, static_cast<int>(minDim * 0.05));

        static QString fontFamily;
        if (fontFamily.isEmpty()) {
            int fontId = QFontDatabase::addApplicationFont(":/resources/NotoSansDevanagari-VariableFont_wdth,wght.ttf");
            fontFamily = QFontDatabase::applicationFontFamilies(fontId).value(0, "Noto Sans");
        }

        QFont dayFont;
        dayFont.setFamily(fontFamily);
        dayFont.setPointSize(dayFontSize);
        dayFont.setBold(dayLabel->font().bold());
        dayLabel->setFont(dayFont);

        QFont tithiFont;
        tithiFont.setPointSize(tithiFontSize);
        tithiFont.setItalic(true);
        tithiLabel->setFont(tithiFont);

        QFont englishFont;
        englishFont.setPointSize(englishDayFontSize);
        englishDayLabel->setFont(englishFont);

        dayLabel->adjustSize();
        tithiLabel->adjustSize();
        englishDayLabel->adjustSize();

        adjustIconSize();
    }
};

#endif // DAYTITHIWIDGET_H
