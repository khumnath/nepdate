import QtQuick 2.15
import QtQuick.Controls 2.15
import "qrc:/PanchangaCalculator.js" as Panchanga
import "qrc:/qml/"

// DayCell.qml
Rectangle {
    id: dayCell
    radius: 12
    scale: cellMouseArea.containsMouse ? 1.2 : 1.0
    z: cellMouseArea.containsMouse ? 1 : 0
    Behavior on scale {
        NumberAnimation { duration: 150; easing.type: Easing.InOutQuad }
    }

    // --- Added default values to prevent "Unable to assign [undefined]" errors ---
    property int bsDay: 0
    property int adDay: 0
    property string tithi: ""
    property bool isToday: false
    property bool isSaturday: false
    property var theme

    signal clicked()

    color: {
        if (!theme) return "white";
        if (cellMouseArea.containsMouse) return theme.tertiaryBg;
        if (isSaturday) return theme.saturdayBg;
        if (isToday) return theme.todayBg;
        return theme.secondaryBg;
    }

    border.color: {
        if (!theme) return "grey";
        if (isToday) return theme.todayBorder;
        if (isSaturday) return theme.saturdayBorder;
        return theme.borderColor;
    }
    border.width: isToday ? 2 : 1

    Label {
        text: Panchanga.toDevanagari(bsDay || 0)
        font.bold: true
        font.pixelSize: cellMouseArea.containsMouse ? 28 : 25
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
        color: theme && (tithi === "पूर्णिमा" || tithi === "अमावस्या") ? theme.purnimaText : (theme ? theme.secondaryText : "grey")
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

    Text {
        visible: tithi === "पूर्णिमा" || tithi === "अमावस्या"
        text: tithi === "पूर्णिमा" ? "🌕" : "🌑"
        font.pixelSize: 14
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.rightMargin: 4
        anchors.bottomMargin: 4
    }

    MouseArea {
        id: cellMouseArea
        anchors.fill: parent; cursorShape: Qt.PointingHandCursor; hoverEnabled: true
        onClicked: dayCell.clicked()
    }
}
