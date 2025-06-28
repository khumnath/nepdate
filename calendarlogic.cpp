#include "calendarlogic.h"

const QStringList bikramMonths = {
    "वैशाख", "जेठ", "असार", "श्रावण", "भाद्र",
    "आश्विन", "कार्तिक", "मंसिर", "पौष", "माघ",
    "फाल्गुन", "चैत्र"
};
const QStringList gregorianMonths = {
    "जनवरी", "फेब्रुअरी", "मार्च", "अप्रिल", "मे", "जुन",
    "जुलाई", "अगस्ट", "सेप्टेम्बर", "अक्टोबर", "नोभेम्बर", "डिसेम्बर"
};

QString convertToNepaliNumerals(int number) {
    QString nepaliNumerals = QString::number(number);
    nepaliNumerals.replace("0", "०");
    nepaliNumerals.replace("1", "१");
    nepaliNumerals.replace("2", "२");
    nepaliNumerals.replace("3", "३");
    nepaliNumerals.replace("4", "४");
    nepaliNumerals.replace("5", "५");
    nepaliNumerals.replace("6", "६");
    nepaliNumerals.replace("7", "७");
    nepaliNumerals.replace("8", "८");
    nepaliNumerals.replace("9", "९");
    return nepaliNumerals;
}

QString getBikramMonthName(int month) {
    if (month < 1 || month > 12) return "";
    return bikramMonths.at(month - 1);
}

QString getEnglishMonthName(int month) {
    if (month < 1 || month > 12) return "";
    return gregorianMonths.at(month - 1);
}
