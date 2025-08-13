import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

// EventDisplay.qml  (used in PanchangaDetailDialog
ColumnLayout {
    id: eventRoot
    Layout.fillWidth: true
    spacing: 8

    // Properties
    property var theme
    property var events: []

    //  Components

    // Title for the events section. Only visible if there are events.
    Label {
        text: "कार्यक्रम हरु"
        font.pixelSize: 18
        font.bold: true
        color: theme ? theme.accentText : "darkblue"
        Layout.alignment: Qt.AlignHCenter
        Layout.bottomMargin: 5
        visible: events.length > 0
    }

    //Label to display all events, joined by commas.
    Label {
        text: {
            if (events.length > 0) {
                return events.map(function(event) {
                    return event.name + (event.detail ? ": " + event.detail : "");
                }).join(", ");
            }
            return "";
        }
        font.pixelSize: 16
        color: theme ? theme.primaryText : "black"
        wrapMode: Text.WordWrap
        horizontalAlignment: Text.AlignHCenter
        Layout.fillWidth: true
        visible: events.length > 0
    }

    // Message to display if no events are found for the selected day.
    Label {
        text: "कुनै कार्यक्रम भेटिएन"
        font.pixelSize: 16
        font.italic: true
        color: theme ? theme.secondaryText : "grey"
        horizontalAlignment: Text.AlignHCenter
        Layout.fillWidth: true
        visible: events.length === 0
    }

    // Bottom margin to the event section.
    Rectangle {
        height: 20
        color: "transparent"
        Layout.fillWidth: true
    }
}
