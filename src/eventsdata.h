#ifndef EVENTSDATA_H
#define EVENTSDATA_H

#include <QString>

enum class EventRule { Udaya, Madhyanna, Nishitha, Pradosh };

struct LunarEventRule {
    QString nameNe;
    QString detailNe;
    QString lunarMonth;
    QString paksha;
    QString tithi;
    EventRule rule;
    bool holiday;
};

struct FixedEventRule {
    QString nameNe;
    QString detailNe;
    int month;
    int day;
    int year; // 0 for recurring
    bool holiday;
};

extern const LunarEventRule lunarEvents[];
extern const int numLunarEvents;

extern const FixedEventRule gregorianEvents[];
extern const int numGregorianEvents;

extern const FixedEventRule bikramEvents[];
extern const int numBikramEvents;

#endif // EVENTSDATA_H
