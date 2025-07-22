// ResultCard.qml

import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

Rectangle {
    property alias title: titleLabel.text
    default property alias content: contentColumn.data

    color: "#142d50"
    radius: 8
    border.color: "#ff9800"
    border.width: 1

    Layout.fillWidth: true
    width: parent ? parent.width : 300 // fallback

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 15
        spacing: 10
        Layout.alignment: Qt.AlignHCenter

        Label {
            id: titleLabel
            font.bold: true
            color: "#ffc107"
            font.pointSize: 14
            wrapMode: Text.Wrap
        }

        ColumnLayout {
            id: contentColumn
            Layout.alignment: Qt.AlignHCenter
            spacing: 10
            Layout.fillWidth: true
        }
    }
}
