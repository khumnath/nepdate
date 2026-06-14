import QtQuick 2.15

Item {
    Component.onCompleted: {
        var d1 = new Date(Date.UTC(-57, 2, 16));
        if (-57 >= 0 && -57 < 100) d1.setUTCFullYear(-57);
        console.log("d1 year:", d1.getUTCFullYear());
        
        // Let's test the CalendarLogic.qml exact logic
        var bsMonthStartDate = new Date(Date.UTC(-57, 2, 16));
        if (-57 >= 0 && -57 < 100) bsMonthStartDate.setUTCFullYear(-57);
        var startAdYear = bsMonthStartDate.getUTCFullYear();
        var startAdMonth = bsMonthStartDate.getUTCMonth();
        
        var nextAdYear = startAdMonth === 11 ? startAdYear + 1 : startAdYear;
        var nextAdMonth = (startAdMonth + 1) % 12;
        var firstOfNextAdMonth = new Date(Date.UTC(nextAdYear, nextAdMonth, 1));
        if (nextAdYear >= 0 && nextAdYear < 100) firstOfNextAdMonth.setUTCFullYear(nextAdYear);
        
        console.log("firstOfNextAdMonth year:", firstOfNextAdMonth.getUTCFullYear());
        Qt.quit()
    }
}
