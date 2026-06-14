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

// EventDisplay.qml  (used in PanchangaDetailDialog)
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
            if (events && events.length > 0) {
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
        visible: events && events.length > 0
    }
}
