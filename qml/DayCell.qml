// DayCell.qml
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "PancangaCalculator.js" as Logic

Frame {
    id: dayCell
    property int dayNumber: 0
    property bool isToday: false
    property bool isSaturday: false
    property bool isCurrentMonth: false

    property var calculator: new Logic.PancangaCalculator()

    Layout.preferredWidth: 40
    Layout.preferredHeight: 40
    padding: 0

    background: Rectangle {
        color: isCurrentMonth ? "#142d50" : "#0c1a2d"
        border.color: isCurrentMonth ? "#2c4a7a" : "transparent"
        radius: 4
    }

    Label {
        id: dayLabel
        text: isCurrentMonth ? calculator.toDevanagari(dayNumber) : ""
        anchors.centerIn: parent

        color: {
            if (isToday) return "black";
            if (isSaturday) return "#ff6b6b";
            return "white";
        }

        background: Rectangle {
            color: isToday ? "#2ecc71" : "transparent"
            radius: width / 2
            width: 26
            height: 26
            anchors.centerIn: parent
        }
    }
}
