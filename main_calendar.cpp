#include <QApplication>
#include "calendarwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    CalendarWindow window;
    window.show();
    return app.exec();
}
