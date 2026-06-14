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
import QtQuick.Layouts 1.15
import "qrc:/qml/"

// Header.qml
Rectangle {
    id: headerRect
    Layout.fillWidth: true
    Layout.preferredHeight: 80
    border.width: 1
    radius: 0

    // Properties to receive objects from the parent (MainApp.qml)
    property var theme
    property string currentBsLabelStr
    property string currentAdLabelStr
    property bool isPrintMode: false

    // Signals to communicate events up to the parent
    signal printClicked()
    signal themeToggleClicked()

    // Use the passed-in theme object for styling
    color: theme ? theme.secondaryBg : "white"
    border.color: theme && theme.isDark ? "transparent" : (theme ? theme.borderColor : "grey")

    ColumnLayout {
        anchors.centerIn: parent
        spacing: 4

        Label {
            text: "नेपाली क्यालेण्डर"
            font.pixelSize: 24
            font.bold: true
            color: theme ? theme.primaryText : "black"
            horizontalAlignment: Text.AlignHCenter
            Layout.alignment: Qt.AlignHCenter
        }

        RowLayout {
            spacing: 20
            Layout.alignment: Qt.AlignHCenter

            Label {
                id: bsLabel
                text: "<b>बि.सं.</b> " + currentBsLabelStr
                renderType: Text.NativeRendering
                color: theme ? theme.secondaryText : "grey"
                font.pixelSize: 16
            }

            Rectangle {
                width: 1
                height: 20
                color: theme ? theme.borderColor : "lightgrey"
            }

            Label {
                id: adLabel
                text: currentAdLabelStr
                renderType: Text.NativeRendering
                color: theme ? theme.secondaryText : "grey"
                font.pixelSize: 16
            }
        }
    }

    Button {
        id: printBtn
        anchors.right: themeToggleBtn.left
        anchors.verticalCenter: parent.verticalCenter
        anchors.rightMargin: 5
        text: "🖨️"
        font.pixelSize: 18
        flat: true
        visible: !isPrintMode
        onClicked: headerRect.printClicked()

        background: Rectangle {
            color: parent.hovered && theme ? theme.tertiaryBg : "transparent"
            radius: 8
        }

        contentItem: Text {
            text: parent.text
            font: parent.font
            color: theme ? theme.primaryText : "black"
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            scale: parent.hovered ? 1.2 : 1.0
            z: parent.hovered ? 1 : 0
            Behavior on scale {
                NumberAnimation { duration: 150; easing.type: Easing.InOutQuad }
            }
        }
    }

    Button {
        id: themeToggleBtn
        visible: !isPrintMode
        anchors.right: parent.right
        anchors.verticalCenter: parent.verticalCenter
        anchors.rightMargin: 15
        text: theme && theme.isDark ? "☀️" : "🌙"
        font.pixelSize: 20
        flat: true
        onClicked: headerRect.themeToggleClicked()

        background: Rectangle {
            color: parent.hovered && theme ? theme.tertiaryBg : "transparent"
            radius: 8
        }

        contentItem: Text {
            text: parent.text
            font: parent.font
            color: theme ? theme.primaryText : "black"
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            scale: parent.hovered ? 1.2 : 1.0
            z: parent.hovered ? 1 : 0
            Behavior on scale {
                NumberAnimation { duration: 150; easing.type: Easing.InOutQuad }
            }
        }
    }
}
