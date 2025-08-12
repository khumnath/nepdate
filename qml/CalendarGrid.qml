import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15
import "qrc:/qml/"

// CalendarGrid.qml
GridLayout {
    id: calendarGrid
    columns: 7
    rowSpacing: 0
    columnSpacing: 0
    Layout.fillWidth: true
    Layout.fillHeight: true
    Layout.margins: 20
    Layout.topMargin: 10

    property alias calendarModel: repeater.model
    property var theme

    signal dayClicked(var panchanga)

    Repeater {
        id: repeater
        delegate: Item {
            Layout.fillWidth: true
            Layout.fillHeight: modelData.type === "day" || modelData.type === "empty"
            Layout.preferredHeight: modelData.type === "header" ? 35 : -1

            Loader {
                id: delegateLoader
                anchors.fill: parent

                sourceComponent: {
                    if (modelData.type === "header") {
                        return headerComponent;
                    } else if (modelData.type === "day") {
                        return dayComponent;
                    } else { // for "empty" type
                        return emptyComponent;
                    }
                }

                onLoaded: {
                    if (modelData.type === "day") {
                        item.bsDay = modelData.bsDay;
                        item.adDay = modelData.adDay;
                        item.tithi = modelData.tithi;
                        item.isToday = modelData.isToday;
                        item.isSaturday = modelData.isSaturday;
                        item.theme = calendarGrid.theme;
                        item.clicked.connect(function() {
                            calendarGrid.dayClicked(modelData.panchanga)
                        });
                    } else if (modelData.type === "header") {
                        item.text = modelData.text;
                        item.theme = calendarGrid.theme;
                        item.cellIndex = index;
                    }
                }
            }
        }
    }


    Component {
        id: headerComponent
        Item {
            id: headerItemContainer
            property string text
            property var theme
            property int cellIndex: -1

            Rectangle {
                id: headerBackground
                anchors.fill: parent
                radius: (cellIndex === 0 || cellIndex === 6) ? 6 : 0
                color: theme ? theme.secondaryBg : "#FFFFFF"
                border.color: theme.borderColor
            }

            Rectangle {
                visible: cellIndex === 0
                width: headerBackground.radius
                height: headerBackground.radius
                color: headerBackground.color
                anchors.top: parent.top
                anchors.right: parent.right
            }
            Rectangle {
                visible: cellIndex === 0
                width: headerBackground.radius
                height: headerBackground.radius
                color: headerBackground.color
                anchors.bottom: parent.bottom
                anchors.left: parent.left
            }
            Rectangle {
                visible: cellIndex === 6
                width: headerBackground.radius
                height: headerBackground.radius
                color: headerBackground.color
                anchors.top: parent.top
                anchors.left: parent.left
            }
            Rectangle {
                visible: cellIndex === 6
                width: headerBackground.radius
                height: headerBackground.radius
                color: headerBackground.color
                anchors.bottom: parent.bottom
                anchors.left: parent.left
            }
            Rectangle {
                visible: cellIndex === 0 || cellIndex === 6
                width: headerBackground.radius
                height: headerBackground.radius
                color: headerBackground.color
                anchors.bottom: parent.bottom
                anchors.right: parent.right
            }

            Label {
                text: headerItemContainer.text
                color: {
                    if (cellIndex === 6) {
                        return "#E4080A"
                    } else {
                        return theme.accentText;
                    }
                }
                font.bold: true
                font.pixelSize: 14
                anchors.centerIn: parent
            }
        }
    }

    Component {
        id: dayComponent
        DayCell {}
    }

    Component {
        id: emptyComponent
        Rectangle {
            border.color: theme ? theme.borderColor : "grey"
            border.width: 1
            color: "transparent"
        }
    }
}
