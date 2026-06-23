#include <QDateTime>
#include <QDebug>
int main() {
    QDateTime dt = QDateTime::fromMSecsSinceEpoch((2451545.0 - 2440587.5) * 86400000.0, Qt::UTC);
    qDebug() << dt;
    return 0;
}
