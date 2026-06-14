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
