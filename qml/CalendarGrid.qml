import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15
import "qrc:/qml/"

// CalendarGrid.qml
GridLayout {
    id: calendarGrid
    columns: 7
    rowSpacing: 8
    columnSpacing: 8
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

            // Use a Loader to conditionally instantiate the correct component.
            // This is the key to fixing the "Unable to assign [undefined]" errors.
            Loader {
                id: delegateLoader
                anchors.fill: parent

                // Set the source component based on the model data type
                sourceComponent: {
                    if (modelData.type === "header") {
                        return headerComponent;
                    } else if (modelData.type === "day") {
                        return dayComponent;
                    } else { // for "empty" type
                        return emptyComponent;
                    }
                }

                // After the correct component is loaded, set its specific properties.
                onLoaded: {
                    if (modelData.type === "day") {
                        item.bsDay = modelData.bsDay;
                        item.adDay = modelData.adDay;
                        item.tithi = modelData.tithi;
                        item.isToday = modelData.isToday;
                        item.isSaturday = modelData.isSaturday;
                        item.theme = calendarGrid.theme;
                        // Connect the signal from the loaded DayCell to the CalendarGrid's signal
                        item.clicked.connect(function() {
                            calendarGrid.dayClicked(modelData.panchanga)
                        });
                    } else if (modelData.type === "header") {
                        item.text = modelData.text;
                        item.theme = calendarGrid.theme;
                    }
                }
            }
        }
    }

    // --- Component Definitions for the Loader ---

    Component {
        id: headerComponent
        Rectangle {
            // These properties are set by the Loader
            property string text
            property var theme

            radius: 8
            color: theme && theme.isDark ? theme.secondaryBg : (theme ? theme.tertiaryBg : "lightgrey")
            border.width: 0

            Label {
                text: parent.text
                color: theme && theme.isDark ? theme.secondaryText : (theme ? theme.accentText : "blue")
                font.bold: true
                font.pixelSize: 12
                anchors.fill: parent
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }
        }
    }

    Component {
        id: dayComponent
        // The DayCell itself. Its properties will be set by the Loader's onLoaded handler.
        DayCell {}
    }

    Component {
        id: emptyComponent
        // An empty item for the blank days at the start of the month.
        Item {}
    }
}
