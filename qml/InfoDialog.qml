/*
 * Copyright (C) 2024 khumnath
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

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
    height: Math.min(implicitHeight, parent.height * 0.95)

    property var theme

    background: Rectangle {
        color: theme ? theme.tertiaryBg : "lightgrey"
        border.color: theme ? theme.borderColor : "grey"
        border.width: 1
        radius: 12
    }

    footer: null
    bottomPadding: 5
    topPadding: 5

    onAccepted: infoDialog.close()

    contentItem: ScrollView {
        id: scrollView
        contentWidth: availableWidth
        clip: true

        Column {
            id: contentColumn
            width: scrollView.availableWidth
            spacing: 25

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

            Text {
                text: appBuildInfo
                font.italic: true
                font.pixelSize: 11
                wrapMode: Text.WordWrap
                horizontalAlignment: Text.AlignHCenter
                anchors.horizontalCenter: parent.horizontalCenter
                color: theme ? theme.infoText : "transparent"
            }
        }

        Text {
            width: parent.width
            wrapMode: Text.WordWrap
            color: theme ? theme.primaryText : "black"
            text: "A lightweight Nepali calendar (Bikram Sambat) with daily Panchanga details written in QML.

" +
                  "All calculations are based on Kathmandu, Nepal and Nepal Standard Time (UTC+5:45).
" +
                  "For years between B.S. 2000–2089, the calendar uses pre-compiled official data.
" +
                  "All other dates are calculated using high-precision ephemeris via the Astronomy Engine (MIT License) combined with traditional Vedic rules."
        }

        Rectangle {
            width: parent.width
            color: theme ? theme.tertiaryBg : "lightgrey"
            implicitHeight: warningRow.implicitHeight + 12

            Item {
                id: warningRow
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.margins: 10
                implicitHeight: Math.max(warnIcon.implicitHeight, warnText.implicitHeight)

                Text {
                    id: warnIcon
                    text: "⚠️"
                    font.pixelSize: 22
                    color: "#DAA520"
                    style: Text.Outline
                    styleColor: "#000"
                    anchors.left: parent.left
                }

                Text {
                    id: warnText
                    text: "Warning: Dates, even those with available data, can have calculation errors. " +
                          "Always check with an official calendar approved by the Nepal Panchanga Nirnayak Samiti."
                    wrapMode: Text.AlignJustify
                    color: theme ? theme.saturdayText : "red"
                    anchors.left: warnIcon.right
                    anchors.leftMargin: 10
                    anchors.right: parent.right
                }
            }
        }

        Column {
            spacing: 10
            width: parent.width

            Text {
                text: "This is a free and open-source project. The core logic is powered by a high-performance C++ backend utilizing the Astronomy Engine library for modern Drik-Siddhanta calculations."
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
}
