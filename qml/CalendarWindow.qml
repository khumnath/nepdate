// CalendarWindow.qml
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ApplicationWindow {
    id: window
    width: 400
    height: 450
    minimumWidth: 350
    minimumHeight: 400
    title: "Bikram Sambat Calendar"
    color: "#0c1a2d"
    visible: true

    // These properties will receive the date from main.qml
    property int initialYear
    property int initialMonth

    // This handler will close the window when it loses focus
    onActiveChanged: {
        if (!active) {
            close();
        }
    }

    BikramCalendar {
        id: calendarView
        anchors.fill: parent
        anchors.margins: 15
    }

    // When the window is ready, set the calendar's date
    Component.onCompleted: {
        if (initialYear) {
            calendarView.setDate(initialYear, initialMonth);
        }
    }
}
