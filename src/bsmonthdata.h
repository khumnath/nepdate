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

// -*- C++ -*-
#ifndef BSMONTHDATA_H
#define BSMONTHDATA_H

#include <QString>

struct BsMonthData {
    static constexpr int BS_START_YEAR = 2000;
    static constexpr int BS_END_YEAR = 2089;
    static constexpr int NUM_YEARS = BS_END_YEAR - BS_START_YEAR + 1;
    static const int NP_MONTHS_DATA[NUM_YEARS][13];
};

#endif // BSMONTHDATA_H
