// ResultItem.qml

import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

RowLayout {
    property alias label: labelText.text
    property alias value: valueText.text
    property bool isHighlight: false
    Layout.alignment: Qt.AlignHCenter

    Layout.fillWidth: true
    spacing: 8

    Label {
        id: labelText
        color: "#a0d2ff"
        wrapMode: Text.WordWrap
        Layout.alignment: Qt.AlignLeft
    }

    Label {
        id: valueText
        text: "-"
        color: isHighlight ? "#ffd54f" : "white"
        font.bold: isHighlight
        wrapMode: Text.WordWrap
        Layout.alignment: Qt.AlignRight
        Layout.fillWidth: true
    }
}
