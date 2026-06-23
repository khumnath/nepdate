/*
 * Copyright (C) 2024 khumnath
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

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
