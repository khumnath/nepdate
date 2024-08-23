#ifndef DAYTITHIWIDGET_H
#define DAYTITHIWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QIcon>
#include <QPixmap>
#include <QPainter>

class DayTithiWidget : public QWidget {
    Q_OBJECT

public:
    explicit DayTithiWidget(const QString &day, const QString &tithi, QWidget *parent = nullptr)
        : QWidget(parent) {
        QVBoxLayout *mainLayout = new QVBoxLayout(this);
        mainLayout->setSpacing(0);
        mainLayout->setContentsMargins(0, 0, 0, 0);  // Remove margins

        dayLabel = new QLabel(day, this);
        dayLabel->setObjectName("dayLabel");  // Set object name for styling
        dayLabel->setStyleSheet("font-size: 19px; color: black; background-color: transparent; font-family: 'laila';");

        tithiLabel = new QLabel(tithi, this);
        tithiLabel->setObjectName("tithiLabel");  // Set object name for styling
        tithiLabel->setStyleSheet("font-size: 10px; color: blue; background-color: transparent; font-family: 'laila';");

        iconLabel = new QLabel(this);
        iconLabel->setObjectName("iconLabel");
        iconLabel->setAlignment(Qt::AlignJustify);
        iconLabel->setStyleSheet("background-color: transparent;");
        iconLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

        mainLayout->addWidget(dayLabel);
        mainLayout->addWidget(iconLabel);
        mainLayout->addWidget(tithiLabel);
        adjustIconSize();
    }

    void setTodayStyle() {
        dayLabel->setStyleSheet("background-color: transparent; font-size: 20px; font-weight: bold; text-decoration: underline; color: green; font-family: 'laila'; ");
    }

    void setSaturdayStyle() {
        dayLabel->setStyleSheet("background-color: transparent; font-size: 19px; color: red; font-family: 'laila';");
    }

    void setIcon(const QIcon &icon, qreal opacity = 1.0) {
        if (!icon.isNull()) {
            QPixmap pixmap = icon.pixmap(iconSize);
            QPixmap transparentPixmap(pixmap.size());
            transparentPixmap.fill(Qt::transparent);

            QPainter painter(&transparentPixmap);
            painter.setOpacity(opacity);
            painter.drawPixmap(0, 0, pixmap);
            painter.end();

            iconLabel->setPixmap(transparentPixmap);  // Set the transparent pixmap
        } else {
            iconLabel->clear();  // Clear the icon if no icon is provided
        }
    }

private:
    QLabel *dayLabel;
    QLabel *tithiLabel;
    QLabel *iconLabel;
    QSize iconSize = QSize(15, 15);
    void adjustIconSize() {
        // Calculate available space and adjust icon size if necessary
        int availableHeight = iconLabel->height()-3; // Adjust for borders
        int availableWidth = iconLabel->width()-3; // Adjust for borders

        int minSize = qMin(availableWidth, availableHeight);
        iconSize = QSize(minSize, minSize);
    }
};

#endif // DAYTITHIWIDGET_H
