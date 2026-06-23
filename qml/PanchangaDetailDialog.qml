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

// PanchangaDetailDialog.qml
Dialog {
    id: panchangaDetailDialogRoot
    width: Math.min(parent.width * 0.9, 600)
    height: Math.min(contentItem.implicitHeight, parent.height)
    modal: true
    closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside
    x: (parent.width - width) / 2
    y: (parent.height - height) / 2
    padding: 0
    background: null

    // Properties
    property var theme
    property var panchangaData: null
    property bool debugVisible: false
    property string currentDebugInfo: ""

    // Components
    contentItem: Rectangle {
        id: contentRect
        width: panchangaDetailDialogRoot.width
        implicitHeight: contentColumn.implicitHeight
        color: theme ? theme.secondaryBg : "white"
        radius: 12
        border.color: theme ? theme.borderColor : "grey"
        border.width: 1
        clip: true

        ScrollView {
            anchors.fill: parent
            clip: true
            contentWidth: availableWidth
            ScrollBar.horizontal.policy: ScrollBar.AlwaysOff

            ColumnLayout {
                id: contentColumn
                width: contentRect.width

                // Header section of the dialog.
                Rectangle {
                    id: modalHeader
                    Layout.fillWidth: true
                    height: 60
                    color: "transparent"

                    Label {
                        id: modalTitle
                        text: "दिनको विवरण"
                        font.pixelSize: 20
                        font.bold: true
                        color: theme ? theme.modalHeaderText : "black"
                        anchors.centerIn: parent
                        z: 1
                    }

                    ToolButton {
                        text: "✕"
                        anchors.right: parent.right
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.rightMargin: 15
                        font.pixelSize: 20
                        onClicked: panchangaDetailDialogRoot.close()
                        contentItem: Text {
                            text: parent.text
                            font: parent.font
                            color: theme ? theme.primaryText : "black"
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                        }
                        background: null
                    }
                }

            // The main content area.
            ColumnLayout {
                id: contentArea
                Layout.fillWidth: true
                spacing: 15

                Button {
                    id: showDebugButton
                    visible: !panchangaDetailDialogRoot.debugVisible
                    text: "डिबग जानकारी देखाउनुहोस्"
                    Layout.fillWidth: true
                    height: 40
                    onClicked: panchangaDetailDialogRoot.debugVisible = true
                    Layout.leftMargin: 30
                    Layout.rightMargin: 20

                    background: Rectangle {
                        color: theme.tertiaryBg
                        border.color: parent.hovered ? theme.accent : "gray"
                        border.width: parent.hovered ? 2 : 1
                        radius: 12
                    }

                    contentItem: Text {
                        text: parent.text
                        font: parent.font
                        color: theme.isDark ? theme.primaryText : theme.primaryText
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        padding: 5
                    }
                }

                // Layout for the Panchanga details.
                ColumnLayout {
                    id: panchangaDetails
                    visible: !panchangaDetailDialogRoot.debugVisible
                    spacing: 8
                    Layout.fillWidth: true
                    Layout.leftMargin: 30
                    Layout.rightMargin: 20
                }

                Rectangle {
                    id: debugInfoArea
                    visible: panchangaDetailDialogRoot.debugVisible
                    z: 99
                    Layout.fillWidth: true
                    implicitHeight: debugInfoText.paintedHeight + 20
                    color: "black"
                    border.color: theme ? theme.accent : "blue"
                    border.width: 1
                    radius: 8
                    Layout.leftMargin: 30
                    Layout.rightMargin: 20

                    Flickable {
                        anchors.fill: parent
                        contentWidth: Math.min(parent.width * 0.96, 600)
                        contentHeight: parent.height
                        clip: true

                        ScrollView {
                            anchors.fill: parent
                            clip: true

                            TextEdit {
                                id: debugInfoText
                                text: currentDebugInfo
                                color: "white"
                                font.family: "monospace"
                                font.pointSize: 8
                                readOnly: true
                                wrapMode: TextEdit.NoWrap
                                textFormat: TextEdit.RichText
                                selectByMouse: true
                                padding: 10
                            }
                        }
                    }
                }

                Button {
                    id: hideDebugButton
                    visible: panchangaDetailDialogRoot.debugVisible
                    text: "डिबग लुकाउनुहोस्"
                    Layout.fillWidth: true
                    height: 40
                    onClicked: panchangaDetailDialogRoot.debugVisible = false
                    Layout.leftMargin: 30
                    Layout.rightMargin: 20

                    background: Rectangle {
                        radius: 8
                        color: theme.tertiaryBg
                        border.color: theme.accent
                        border.width: 1
                    }
                    contentItem: Text {
                        text: parent.text
                        font: parent.font
                        color: theme.modalButtonText
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }
                }
            }

            }
        }
    }


    //  Handlers and Functions
    onOpened: {
        if (panchangaData) {
            clearPanchangaDetails();
            modalTitle.text = panchangaData.gregorianDate;

            if (panchangaData.statusMessage && panchangaData.statusMessage !== "") {
                createDetailRow("जानकारी", panchangaData.statusMessage).parent = panchangaDetails;
            }

            var eventComponent = Qt.createComponent("EventDisplay.qml");
            if (eventComponent.status === Component.Ready) {
                var eventObj = eventComponent.createObject(panchangaDetails, {
                                                               "theme": theme,
                                                               "events": panchangaData.events
                                                           });
            } else {
                console.log("Error creating event component:", eventComponent.errorString());
            }

            var formatElements = function(elements, primaryName) {
                var out = "";
                if (primaryName) {
                    out += "<table width='100%' cellpadding='0' cellspacing='0'><tr><td align='right'><b><font size='4'>" + primaryName + "</font></b></td></tr></table>";
                }
                
                if (!elements || elements.length === 0) {
                    if (!primaryName) return "-";
                    return out;
                }
                
                var parts = [];
                for (var i = 0; i < elements.length; i++) {
                    var item = elements[i];
                    var name = item.name;
                    var startTimeStr = item.startTime !== "N/A" ? item.startTime : "---";
                    var endTimeStr = item.endTime !== "N/A" ? item.endTime : "---";
                    
                    var line = "<b><font size='4'>" + name + "</font></b>";
                    line += "<table width='100%' cellpadding='0' cellspacing='0'><tr>";
                    line += "<td align='left'>प्रारम्भ: " + startTimeStr + "</td>";
                    line += "<td align='right'>अन्त्य: " + endTimeStr + "</td>";
                    line += "</tr></table>";
                    parts.push(line);
                }
                
                return out + (primaryName && parts.length > 0 ? "<br>" : "") + parts.join("<br>");
            };

            var transits = panchangaData.dailyTransits || {};
            var tithiDisplay = formatElements(transits.tithis, panchangaData.tithi + " (" + panchangaData.paksha + ")");
            var nakshatraDisplay = formatElements(transits.nakshatras, panchangaData.nakshatra);
            var yogaDisplay = formatElements(transits.yogas, panchangaData.yoga);
            var karanaDisplay = formatElements(transits.karanas, panchangaData.karana);

            var details = [
                        createDetailRow("बिक्रम संवत", toDevanagari(panchangaData.bsYear || "") + " " + (panchangaData.monthName || "") + " " + toDevanagari(panchangaData.bsDay || "")),
                        createDetailRow("वार", panchangaData.weekday || ""),
                        createDetailRow("तिथि", tithiDisplay),
                        createDetailRow("नक्षत्र", nakshatraDisplay),
                        createDetailRow("योग", yogaDisplay),
                        createDetailRow("करण", karanaDisplay),
                        createDetailRow("सूर्य राशि", panchangaData.sunRashi || ""),
                        createDetailRow("चन्द्र राशि", panchangaData.moonRashi || ""),
                        createDetailRow("उदयास्त", "सूर्योदय " + (panchangaData.sunrise || "--:--") + " | सूर्यास्त " + (panchangaData.sunset || "--:--")),
                        createDetailRow("अधिक/क्षय मास", panchangaData.adhikaMasa || "छैन"),
                        createDetailRow("चन्द्रमास",  panchangaData.lunarMonth || "")
                    ];

            if (panchangaData.bhadra && panchangaData.bhadra.isActive) {
                var bhadraText = "बास: " + panchangaData.bhadra.residence + "
" + panchangaData.bhadra.status;
                details.push(createDetailRow("भद्रा", bhadraText));
            }

            for (var i = 0; i < details.length; ++i) {
                details[i].parent = panchangaDetails;
            }
            generateDebugInfo();
        }
    }

    onClosed: {
        clearPanchangaDetails();
        PanchangaNative.clearCache();
        debugVisible = false;
    }

    function clearPanchangaDetails() {
        for (var i = panchangaDetails.children.length - 1; i >= 0; i--) {
            panchangaDetails.children[i].destroy();
        }
    }

    function createDetailRow(label, value) {
        var rowItem = Qt.createQmlObject('import QtQuick.Layouts 1.15; import QtQuick.Controls 2.15; import QtQuick 2.15; RowLayout { spacing: 10; Layout.fillWidth: true }', panchangaDetails);
        var label1 = Qt.createQmlObject('import QtQuick.Controls 2.15; Label {}', rowItem);
        label1.text = "<b>" + label + ":</b>";
        label1.color = theme ? theme.accentText : "blue";
        label1.font.bold = true;
        label1.width = 150;
        label1.font.pixelSize = 14;
        label1.Layout.alignment = Qt.AlignTop;

        var label2 = Qt.createQmlObject('import QtQuick.Controls 2.15; import QtQuick 2.15; import QtQuick.Layouts 1.15; Label {}', rowItem);
        label2.text = value;
        label2.textFormat = Text.RichText;
        label2.color = theme ? theme.primaryText : "black";
        label2.wrapMode = Text.WordWrap;
        label2.font.pixelSize = 14;
        label2.Layout.fillWidth = true;
        label2.Layout.alignment = Qt.AlignTop;
        return rowItem;
    }

    function generateDebugInfo() {
        try {
            var year = panchangaData.adYear;
            var monthIndex = panchangaData.adMonth - 1; // 0-based
            var day = panchangaData.adDay;

            if (year !== undefined && monthIndex >= 0 && day !== undefined) {
                var debugDate = new Date(0);
                debugDate.setUTCFullYear(year, monthIndex, day);
                debugDate.setUTCHours(0, 0, 0, 0);
                var debugInfo = PanchangaNative.generateDebugInfo(debugDate);
                currentDebugInfo = debugInfo.debug || "Debug information not available";
            } else {
                currentDebugInfo = "Debug Information:
  Error: Could not parse date for debug information";
            }
        } catch (e) {
            console.error("Error generating debug info:", e);
            currentDebugInfo = "Debug Information:
  Error: " + e.message;
        }
    }

    function toDevanagari(num) {
        return PanchangaNative.toDevanagari(num);
    }
}
