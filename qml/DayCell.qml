import QtQuick 2.15
import QtQuick.Controls 2.15
import "qrc:/PanchangaCalculator.js" as Panchanga
import "qrc:/qml/"

// DayCell.qml
Rectangle {
    id: dayCell
    radius: 5
    property int bsDay: 0
    property int adDay: 0
    property string tithi: ""
    property bool isToday: false
    property bool isSaturday: false
    property bool hasEvent: false
    property bool isHoliday: false
    property var theme
    property var cellDate

    signal clicked()

    color: {
        if (!theme) return "white";
        if (cellMouseArea.containsMouse) return theme.tertiaryBg;
        //holiday background disabled now
        //if (isHoliday) return theme.holidayBg;
        if (isSaturday) return theme.saturdayBg;
        if (isToday) return theme.todayBg;
        return theme.secondaryBg;
    }

    border.color: {
        if (!theme) return "grey";
        if (isToday) return theme.todayBorder;
        if (isSaturday) return theme.saturdayBorder;
        if (isHoliday) return theme.holidayBorder;
        return theme.borderColor;
    }
    border.width: isToday ? 2 : 1

    Component.onCompleted: {
        if (cellDate) {
            var dayInfo = Panchanga.calculate(cellDate);
            if (dayInfo.events && dayInfo.events.length > 0) {
                for (var i = 0; i < dayInfo.events.length; i++) {
                    if (dayInfo.events[i].holiday) {
                        isHoliday = true;
                        break;
                    }
                }
            }
        }
    }


    // Event Indicator
    Rectangle {
        id: eventIndicator
        width: 6
        height: 6
        radius: 4
        color: "orange"
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.margins: 5
        visible: hasEvent
        z: 1
    }

    Label {
        text: Panchanga.toDevanagari(bsDay || 0)
        font.bold: true
        font.pixelSize: cellMouseArea.containsMouse ? 28 : 25
        scale: cellMouseArea.containsMouse ? 1.2 : 1.0
        z: cellMouseArea.containsMouse ? 1 : 0
        Behavior on scale {
            NumberAnimation { duration: 150; easing.type: Easing.InOutQuad }
        }
        color: {
            if (!theme) return "black";
            if (isToday) return theme.accentText;
            if (isSaturday) return theme.saturdayText;
            return theme.primaryText;
        }
        anchors.centerIn: parent
    }

    Label {
        text: tithi || ""
        font.pixelSize: 11
        color: theme && (tithi === "पूर्णिमा" || tithi === "अमावस्या") ? theme.purnimaText : theme ? theme.secondaryText : theme.primaryText
        elide: Text.ElideRight
        font.bold: tithi === "पूर्णिमा" || tithi === "अमावस्या"
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        anchors.leftMargin: 8
        anchors.bottomMargin: 4
    }

    Rectangle {
        width: 12; height: 12; radius: 12; color: theme ? theme.tertiaryBg : "lightgrey"
        anchors.right: parent.right; anchors.top: parent.top; anchors.margins: 4
        Label { anchors.centerIn: parent; text: adDay || 0; font.pixelSize: 10; color: theme ? theme.adDayText : "blue" }
    }

    Item {
        width: 16
        height: 16
        visible: tithi === "पूर्णिमा" || tithi === "अमावस्या"
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.rightMargin: 4
        anchors.bottomMargin: 4


        Rectangle {
            anchors.fill: parent
            radius: width / 2
            color: tithi === "पूर्णिमा" ? "gold" : "black"
            border.width: 0.5
            border.color: {
                if (dayCell.theme && dayCell.theme.isDark) {
                    return "white";
                } else {
                    return "black";
                }
            }
        }
    }

    MouseArea {
        id: cellMouseArea
        anchors.fill: parent; cursorShape: Qt.PointingHandCursor; hoverEnabled: true
        onClicked: dayCell.clicked()
    }
}
