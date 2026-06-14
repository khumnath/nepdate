import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

// NavigationBar.qml
RowLayout {
    id: navigationBar
    Layout.fillWidth: true
    Layout.leftMargin: 20
    Layout.rightMargin: 20
    Layout.topMargin: 5
    Layout.bottomMargin: 2
    spacing: 10

    property var theme
    property var calendarLogic
    property bool isPrintMode: false

    signal tabChanged(int newIndex)
    signal todayClicked()

    visible: !isPrintMode

    component NavButton: Button {
        id: control
        property string iconName: ""
        property bool iconOnRight: false

        font.pixelSize: 14
        font.bold: true
        topPadding: 10
        bottomPadding: 10
        leftPadding: 15
        rightPadding: 15

        property string dynamicSvgSource: {
            if (iconName === "") return "";
            var colorStr = (theme ? theme.primaryText.toString() : "#000000").replace("#", "%23");
            var pathData = "";
            if (iconName === "chevron-left") pathData = "m15 18-6-6 6-6";
            else if (iconName === "chevron-right") pathData = "m9 18 6-6-6-6";
            else if (iconName === "chevrons-left") pathData = "m11 17-5-5 5-5\"/><path d=\"m18 17-5-5 5-5";
            else if (iconName === "chevrons-right") pathData = "m13 17 5-5-5-5\"/><path d=\"m6 17 5-5-5-5";
            
            return "data:image/svg+xml;utf8,<svg xmlns=\"http://www.w3.org/2000/svg\" width=\"24\" height=\"24\" viewBox=\"0 0 24 24\" fill=\"none\" stroke=\"" + colorStr + "\" stroke-width=\"2\" stroke-linecap=\"round\" stroke-linejoin=\"round\"><path d=\"" + pathData + "\"/></svg>";
        }

        contentItem: Row {
            spacing: 5

            Image {
                source: control.dynamicSvgSource
                sourceSize.width: 16
                sourceSize.height: 16
                visible: control.iconName !== "" && !control.iconOnRight
                anchors.verticalCenter: parent.verticalCenter
            }

            Text {
                text: control.text || ""
                font: control.font
                color: theme ? theme.primaryText : "black"
                visible: text.trim() !== ""
                anchors.verticalCenter: parent.verticalCenter
            }

            Image {
                source: control.dynamicSvgSource
                sourceSize.width: 16
                sourceSize.height: 16
                visible: control.iconName !== "" && control.iconOnRight
                anchors.verticalCenter: parent.verticalCenter
            }
        }

        background: Rectangle {
            radius: 10
            color: parent.hovered && theme ? theme.tertiaryBg : (theme ? theme.secondaryBg : "white")
            border.color: theme ? theme.borderColor : "lightgrey"
            border.width: 1
        }
    }

    component CustomTabButton: TabButton {
        font.pixelSize: 13
        topPadding: 10
        bottomPadding: 10
        leftPadding: 15
        rightPadding: 15

        contentItem: Text {
            text: parent.text
            font: parent.font
            color: parent.checked && theme ? theme.accentText : (parent.hovered && theme ? theme.primaryText : (theme ? theme.secondaryText : "grey"))
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }
        background: Rectangle {
            radius: 10
            color: (parent.checked || parent.hovered) && theme ? theme.tertiaryBg : "transparent"
            border.color: parent.checked && theme ? theme.accent : (theme ? theme.borderColor : "lightgrey")
            border.width: parent.checked ? 2 : 1
        }
    }

    NavButton {
        text: ""
        iconName: "chevrons-left"
        onClicked: {
            if (tabBar.currentIndex === 0) {
                calendarLogic.navigateBsYear(-1)
            } else {
                calendarLogic.navigateAdYear(-1)
            }
        }
    }

    NavButton {
        property string prevBsMonth: (calendarLogic && calendarLogic.prevMonthName) || ""
        property string prevAdMonth: (calendarLogic && calendarLogic.prevAdMonthName) || ""
        text: (tabBar.currentIndex === 0 ? prevBsMonth : prevAdMonth)
        iconName: "chevron-left"

        onClicked: {
            if (tabBar.currentIndex === 0) {
                calendarLogic.navigateBsMonth(-1)
            } else {
                calendarLogic.navigateAdMonth(-1)
            }
        }
        visible: (text || "").trim() !== ""
    }

    Item { Layout.fillWidth: true }

    RowLayout {
        Layout.alignment: Qt.AlignHCenter
        spacing: 5

        TabBar {
            id: tabBar
            currentIndex: 0
            background: Rectangle { color: "transparent" }
            onCurrentIndexChanged: navigationBar.tabChanged(currentIndex)

            CustomTabButton { text: "बिक्रम संवत" }
            CustomTabButton { text: "ग्रेगोरियन" }
        }

        NavButton {
            text: tabBar.currentIndex === 0 ? "आज" : "Today"
            onClicked: navigationBar.todayClicked()
            Layout.leftMargin: 10
            font.bold: false
            font.pixelSize: 13
        }
    }

    Item { Layout.fillWidth: true }

    NavButton {
        property string nextBsMonth: (calendarLogic && calendarLogic.nextMonthName) || ""
        property string nextAdMonth: (calendarLogic && calendarLogic.nextAdMonthName) || ""
        text: (tabBar.currentIndex === 0 ? nextBsMonth : nextAdMonth)
        iconName: "chevron-right"
        iconOnRight: true

        onClicked: {
            if (tabBar.currentIndex === 0) {
                calendarLogic.navigateBsMonth(1)
            } else {
                calendarLogic.navigateAdMonth(1)
            }
        }
        visible: (text || "").trim() !== ""
    }

    NavButton {
        text: ""
        iconName: "chevrons-right"
        onClicked: {
            if (tabBar.currentIndex === 0) {
                calendarLogic.navigateBsYear(1)
            } else {
                calendarLogic.navigateAdYear(1)
            }
        }
    }
}
