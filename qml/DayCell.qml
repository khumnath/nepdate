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

import QtQuick 2.15
import QtQuick.Controls 2.15
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
        if (isToday && (!window || !window.isPrintMode)) return theme.todayBg;
        return theme.secondaryBg;
    }

    border.color: {
        if (!theme) return "grey";
        if (isToday && (!window || !window.isPrintMode)) return theme.todayBorder;
        if (isSaturday) return theme.saturdayBorder;
        if (isHoliday) return theme.holidayBorder;
        return theme.borderColor;
    }
    border.width: (isToday && (!window || !window.isPrintMode)) ? 2 : 1

    Component.onCompleted: {
        if (cellDate) {
            var dayInfo = PanchangaNative.calculate(cellDate);
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


    // Event Indicator (Bottom Right)
    Rectangle {
        id: eventIndicator
        width: 8
        height: 8
        radius: 4
        color: "#22c55e" // Green color in web style
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.margins: 6
        visible: hasEvent
    }

    // Purnima / Amavasya Indicator (Top Left)
    Rectangle {
        width: 14
        height: 14
        radius: 7
        color: tithi === "पूर्णिमा" ? "#fbbf24" : (theme && theme.isDark ? "#334155" : "#475569")
        visible: tithi === "पूर्णिमा" || tithi === "अमावस्या"
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.margins: 6
    }

    // BS Day (Center)
    Label {
        text: PanchangaNative.toDevanagari(bsDay || 0)
        font.bold: true
        font.pixelSize: cellMouseArea.containsMouse ? 28 : 24
        scale: cellMouseArea.containsMouse ? 1.1 : 1.0
        Behavior on scale { NumberAnimation { duration: 150; easing.type: Easing.InOutQuad } }
        color: {
            if (!theme) return "black";
            if (isSaturday) return theme.saturdayText;
            if (tithi === "पूर्णिमा" || tithi === "अमावस्या") return theme.primaryText;
            return theme.primaryText;
        }
        anchors.centerIn: parent
    }

    // Tithi (Bottom Left)
    Label {
        text: tithi || ""
        font.pixelSize: 10
        color: theme ? ((tithi === "पूर्णिमा" || tithi === "अमावस्या") ? theme.purnimaText : theme.secondaryText) : "black"
        elide: Text.ElideRight
        font.bold: tithi === "पूर्णिमा" || tithi === "अमावस्या"
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        anchors.leftMargin: 6
        anchors.bottomMargin: 6
        width: parent.width - eventIndicator.width - 14
    }

    // AD Day (Top Right)
    Label {
        text: adDay || 0
        font.pixelSize: 11
        color: theme ? theme.adDayText : "grey"
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.margins: 6
    }

    MouseArea {
        id: cellMouseArea
        anchors.fill: parent; cursorShape: Qt.PointingHandCursor; hoverEnabled: true
        onClicked: dayCell.clicked()
    }
}
