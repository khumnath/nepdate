import QtQuick 2.15
import QtQuick.Controls 2.15
import "qrc:/qml/"

// InfoDialog.qml
Dialog {
    id: infoDialog
    modal: true
    title: ""
    width: Math.min(parent.width * 0.9, 700)
    x: parent ? (parent.width - width) / 2 : 0
    y: parent ? (parent.height - height) / 2 : 0
    height: Math.min(implicitHeight, parent.height * 0.9)

    property var theme

    background: Rectangle {
        color: theme ? theme.tertiaryBg : "lightgrey"
        border.color: theme ? theme.borderColor : "grey"
        border.width: 1
        radius: 12
    }

    footer: null

    onAccepted: infoDialog.close()

    contentItem: Column {
        id: contentColumn
        width: parent.width
        spacing: 25
        anchors.horizontalCenter: parent.horizontalCenter

        Column {
            width: parent.width
            spacing: 5
            anchors.horizontalCenter: parent.horizontalCenter

            Text {
                text: "Nepali Calendar"
                font.pixelSize: 24
                font.bold: true
                horizontalAlignment: Text.AlignHCenter
                anchors.horizontalCenter: parent.horizontalCenter
                color: theme ? theme.primaryText : "black"
            }

            Text {
                text: "Version: " + appVersion
                font.italic: true
                font.pixelSize: 16
                horizontalAlignment: Text.AlignHCenter
                anchors.horizontalCenter: parent.horizontalCenter
                color: theme ? theme.primaryText : "black"
            }
        }

        Text {
            width: parent.width
            wrapMode: Text.WordWrap
            color: theme ? theme.primaryText : "black"
            text: "A lightweight Nepali calendar (Bikram Sambat) with daily Panchanga details written in QML.\n\n" +
                  "All calculations are based on Kathmandu, Nepal and Nepal Standard Time (UTC+5:45).\n" +
                  "For years between B.S. 2000–2089, the calendar uses pre-compiled data.\n" +
                  "All other dates are calculated using traditional astronomical algorithms based on the Surya Siddhanta."
        }

        Rectangle {
            width: parent.width
            color: theme ? theme.tertiaryBg : "lightgrey"
            implicitHeight: warningRow.implicitHeight + 12

            Row {
                id: warningRow
                spacing: 10
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.margins: 10

                Text {
                    text: "⚠️"
                    font.pixelSize: 22
                    color: "#DAA520"
                    style: Text.Outline
                    styleColor: "#000"
                }

                Text {
                    text: "Warning: Dates, even those with available data, can have calculation errors." +
                          "Always check with an official calendar approved by the Nepal Panchanga Nirnayak Samiti."
                    wrapMode: Text.AlignJustify
                    color: theme ? theme.saturdayText : "red"
                    width: parent.width
                }
            }
        }

        Column {
            spacing: 10
            width: parent.width

            Text {
                text: "This is a free and open-source project. The project started as a Qt C++ application, " +
                      "then a JavaScript version was created. You are running this application using JavaScript with QML now."
                color: theme ? theme.primaryText : "black"
                width: parent.width
                wrapMode: Text.WordWrap
                horizontalAlignment: Text.AlignHCenter
            }

            Text {
                text: "License: GPL-3.0 or later"
                color: theme ? theme.accentText : "blue"
                font.underline: true
                anchors.horizontalCenter: parent.horizontalCenter
                MouseArea {
                    anchors.fill: parent
                    cursorShape: Qt.PointingHandCursor
                    onClicked: { Qt.openUrlExternally("https://opensource.org/licenses/GPL-3.0"); infoDialog.accept() }
                }
            }

            Text {
                text: "Source Code: Available on GitHub"
                color: theme ? theme.accentText : "blue"
                font.underline: true
                anchors.horizontalCenter: parent.horizontalCenter
                MouseArea {
                    anchors.fill: parent
                    cursorShape: Qt.PointingHandCursor
                    onClicked: { Qt.openUrlExternally("https://github.com/khumnath/nepdate"); infoDialog.accept() }
                }
            }

            Text {
                text: "Contributors"
                color: theme ? theme.accentText : "blue"
                font.underline: true
                anchors.horizontalCenter: parent.horizontalCenter
                MouseArea {
                    anchors.fill: parent
                    cursorShape: Qt.PointingHandCursor
                    onClicked: { Qt.openUrlExternally("https://github.com/khumnath/nepdate/graphs/contributors"); infoDialog.accept() }
                }
            }
        }

        Button {
            text: "Ok"
            font.bold: true
            anchors.horizontalCenter: parent.horizontalCenter
            onClicked: infoDialog.accept()

            background: Rectangle {
                color: theme.tertiaryBg
                border.color: parent.hovered ? theme.accent : "gray"
                border.width: parent.hovered ? 2 : 1
                radius: 12
            }

            contentItem: Text {
                text: parent.text
                font: parent.font
                color: theme.primaryText
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                padding: 5
            }
        }
    }
}
