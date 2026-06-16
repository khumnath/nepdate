#include <QCoreApplication>
#include <QDate>
#include <iostream>
#include <cassert>

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);
    
    // Test QDate across century boundaries
    struct TestCase { int y; int m; int d; const char* label; };
    TestCase tests[] = {
        {1, 1, 1, "Year 1 AD"},
        {50, 6, 15, "Year 50 AD"},
        {99, 12, 31, "Year 99 AD"},
        {100, 1, 1, "Year 100 AD"},
        {500, 6, 15, "Year 500 AD"},
        {999, 12, 31, "Year 999 AD"},
        {1000, 1, 1, "Year 1000 AD"},
        {1800, 1, 1, "Year 1800 AD"},
        {1900, 1, 1, "Year 1900 AD"},
        {2000, 1, 1, "Year 2000 AD"},
        {-57, 3, 16, "Year 57 BC"},
    };
    
    const char* dayNames[] = {"", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun"};
    
    for (const auto& t : tests) {
        QDate d(t.y, t.m, t.d);
        std::cout << t.label << ": QDate(" << t.y << "," << t.m << "," << t.d << ") -> "
                  << "valid=" << d.isValid() 
                  << " year=" << d.year() 
                  << " month=" << d.month() 
                  << " day=" << d.day()
                  << " dow=" << dayNames[d.dayOfWeek()]
                  << " jd=" << d.toJulianDay()
                  << std::endl;
        assert(d.isValid());
    }
    return 0;
}
