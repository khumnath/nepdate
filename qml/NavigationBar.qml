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
        font.pixelSize: 13
        topPadding: 12
        bottomPadding: 12
        leftPadding: 10
        rightPadding: 10

        background: Rectangle {
            radius: 12
            color: parent.hovered && theme ? theme.tertiaryBg : (theme ? theme.secondaryBg : "white")
            border.color: theme ? theme.borderColor : "grey"
            border.width: 1
        }
        contentItem: Text {
            text: parent.text
            font: parent.font
            color: theme ? theme.primaryText : "black"
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }
    }

    component CustomTabButton: TabButton {
        font.pixelSize: 13
        topPadding: 12
        bottomPadding: 12
        leftPadding: 10
        rightPadding: 10

        contentItem: Text {
            text: parent.text
            font: parent.font
            color: parent.checked && theme ? theme.accentText : (parent.hovered && theme ? theme.primaryText : (theme ? theme.secondaryText : "grey"))
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }
        background: Rectangle {
            radius: 8
            color: (parent.checked || parent.hovered) && theme ? theme.tertiaryBg : "transparent"
            border.color: parent.checked && theme ? theme.accent : (theme ? theme.borderColor : "grey")
            border.width: parent.checked ? 2 : 1
        }
    }

    NavButton {
        property string prevBsMonth: (calendarLogic && calendarLogic.prevMonthName) || ""
        property string prevAdMonth: (calendarLogic && calendarLogic.prevAdMonthName) || ""
        text: "< " + (tabBar.currentIndex === 0 ? prevBsMonth : prevAdMonth)

        onClicked: {
            if (tabBar.currentIndex === 0) {
                calendarLogic.navigateBsMonth(-1)
            } else {
                calendarLogic.navigateAdMonth(-1)
            }
        }
        visible: text.trim() !== "< "
    }

    Item { Layout.fillWidth: true }

    RowLayout {
        Layout.alignment: Qt.AlignHCenter
        spacing: 0

        TabBar {
            id: tabBar
            currentIndex: 0
            background: Rectangle { color: "transparent" }
            onCurrentIndexChanged: navigationBar.tabChanged(currentIndex)

            CustomTabButton { text: "बिक्रम संवत" }
            CustomTabButton { text: "ग्रेगोरियन" }
        }

        NavButton {
            text: "आज"
            onClicked: navigationBar.todayClicked()
            Layout.leftMargin: 10
        }
    }

    Item { Layout.fillWidth: true }

    NavButton {
        property string nextBsMonth: (calendarLogic && calendarLogic.nextMonthName) || ""
        property string nextAdMonth: (calendarLogic && calendarLogic.nextAdMonthName) || ""
        text: (tabBar.currentIndex === 0 ? nextBsMonth : nextAdMonth) + " >"

        onClicked: {
            if (tabBar.currentIndex === 0) {
                calendarLogic.navigateBsMonth(1)
            } else {
                calendarLogic.navigateAdMonth(1)
            }
        }
        visible: text.trim() !== " >"
    }
}
