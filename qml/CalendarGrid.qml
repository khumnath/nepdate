import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15
import "qrc:/qml/"

// CalendarGrid.qml
ColumnLayout {
    id: calendarGridRoot
    Layout.fillWidth: true

    // Properties
    property alias calendarModel: repeater.model
    property alias eventListModel: eventLabel.eventModel
    property var theme

    // Signals
    signal dayClicked(var panchanga)

    // Components

    // Calendar grid layout
    GridLayout {
        id: calendarGrid
        columns: 7
        rowSpacing: 0
        columnSpacing: 0
        Layout.fillWidth: true
        Layout.preferredHeight: parent.width
        Layout.margins: 10
        Layout.topMargin: 10
        //Layout.bottomMargin: 10

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
                        } else {
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
                            item.hasEvent = modelData.hasEvent;
                            item.theme = calendarGridRoot.theme;
                            item.clicked.connect(function() {
                                calendarGridRoot.dayClicked(modelData.panchanga)
                            });
                        } else if (modelData.type === "header") {
                            item.text = modelData.text;
                            item.theme = calendarGridRoot.theme;
                            item.cellIndex = index;
                        }
                    }
                }
            }
        }
    }

    // Event List Footer
    Rectangle {
        id: eventFooter
        Layout.fillWidth: true
        //Layout.topMargin: 10
        implicitHeight: eventLabel.paintedHeight + 20
        color: theme.secondaryBg
        radius: 6
        visible: eventLabel.eventModel && eventLabel.eventModel.length > 0

        Label {
            id: eventLabel
            anchors.fill: parent
            anchors.margins: 10

            property var eventModel: [] // Model reference

            // Bind the text property to a function that builds the string
            text: {
                var str = "";
                if (eventModel) {
                    for (var i = 0; i < eventModel.length; i++) {
                        var item = eventModel[i];
                        str += item.bsDay + " :\u00A0" + item.eventName;
                        if (i < eventModel.length - 1) {
                            str += "  •  ";
                        }
                    }
                }
                return str;
            }

            font.pixelSize: 12
            color: theme ? theme.secondaryText : "black"
            wrapMode: Text.WordWrap
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }
    }


    // Component Definitions
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

            Rectangle { visible: cellIndex === 0; width: headerBackground.radius; height: headerBackground.radius; color: headerBackground.color; anchors.top: parent.top; anchors.right: parent.right }
            Rectangle { visible: cellIndex === 0; width: headerBackground.radius; height: headerBackground.radius; color: headerBackground.color; anchors.bottom: parent.bottom; anchors.left: parent.left }
            Rectangle { visible: cellIndex === 6; width: headerBackground.radius; height: headerBackground.radius; color: headerBackground.color; anchors.top: parent.top; anchors.left: parent.left }
            Rectangle { visible: cellIndex === 6; width: headerBackground.radius; height: headerBackground.radius; color: headerBackground.color; anchors.bottom: parent.bottom; anchors.left: parent.left }
            Rectangle { visible: cellIndex === 0 || cellIndex === 6; width: headerBackground.radius; height: headerBackground.radius; color: headerBackground.color; anchors.bottom: parent.bottom; anchors.right: parent.right }

            Label {
                text: headerItemContainer.text
                color: (cellIndex === 6) ? "#E4080A" : theme.accentText
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
