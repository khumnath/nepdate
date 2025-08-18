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
        font.pixelSize: 12
        color: theme ? theme.primaryText : "black"
        wrapMode: Text.WordWrap
        horizontalAlignment: Text.AlignHCenter
        Layout.fillWidth: true
        visible: events.length > 0
    }
}
