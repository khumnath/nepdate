import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import "qrc:/PanchangaCalculator.js" as Panchanga
import "qrc:/qml/"

// PanchangaDetailDialog.qml
Dialog {
    id: panchangaDetailDialogRoot // Changed id to avoid name collision
    width: Math.min(parent.width * 0.9, 600)
    height: Math.min(parent.height * 0.8, 550)
    modal: true
    closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside
    x: (parent.width - width) / 2
    y: (parent.height - height) / 2
    padding: 0

    property var theme
    property var panchangaData: null
    property bool debugVisible: false
    property string currentDebugInfo: ""

    background: Rectangle {
        color: theme ? theme.secondaryBg : "white"
        radius: 12
        border.color: theme ? theme.borderColor : "grey"
        border.width: 1
        clip: true
    }

    contentItem: Item {
        width: panchangaDetailDialogRoot.width
        height: panchangaDetailDialogRoot.height

        Rectangle {
            id: modalHeader
            width: parent.width
            height: 60
            color: theme ? theme.secondaryBg : "white"

            Label {
                id: modalTitle
                text: "दिनको विवरण"
                font.pixelSize: 20
                font.bold: true
                color: theme ? theme.modalHeaderText : "black"
                anchors.centerIn: parent
                z: 1
            }
        }

        Rectangle {
            id: modalFooter
            width: parent.width
            height: 65
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 12
            color: theme ? theme.secondaryBg : "white"

            Button {
                text: "बन्द गर्नुहोस्"
                anchors.centerIn: parent
                onClicked: panchangaDetailDialogRoot.close()

                background: Rectangle {
                    color: theme ? theme.tertiaryBg : "lightgrey"
                    border.color: parent.hovered && theme ? theme.accent : "gray"
                    border.width: parent.hovered ? 2 : 1
                    radius: 12
                }

                contentItem: Text {
                    text: parent.text
                    font: parent.font
                    color: theme ? theme.primaryText : "black"
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    padding: 5
                }
            }
        }

        Flickable {
            id: flickableArea
            anchors {
                left: parent.left
                right: parent.right
                top: modalHeader.bottom
                bottom: modalFooter.top
                leftMargin: 30
                rightMargin: 20
            }
            clip: false
            contentHeight: scrollContent.height

            ColumnLayout {
                id: scrollContent
                width: parent.width
                spacing: 15

                Button {
                    id: showDebugButton
                    visible: !window.debugVisible
                    text: "डिबग जानकारी देखाउनुहोस्"
                    Layout.fillWidth: true
                    height: 40
                    onClicked: debugVisible = true

                    background: Rectangle {
                        color: theme.tertiaryBg
                        border.color: parent.hovered ?  theme.accent : "gray"
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

                ColumnLayout {
                    id: panchangaDetails
                    visible: !debugVisible
                    spacing: 15
                    Layout.fillWidth: true
                }

                Rectangle {
                    id: debugInfoArea
                    visible: debugVisible
                    z: 99
                    Layout.fillWidth: true
                    implicitHeight: debugInfoText.paintedHeight + 10
                    color: "black"
                    border.color: theme ? theme.accent : "blue"
                    border.width: 1
                    radius: 8

                    Flickable {
                        anchors.fill: parent
                        contentWidth: debugInfoText.paintedWidth
                        contentHeight: debugInfoText.paintedHeight + 10
                        clip: true

                        TextEdit {
                            id: debugInfoText
                            text: currentDebugInfo
                            color: "white"
                            font.family: "monospace"
                            font.pointSize: 9
                            readOnly: true
                            wrapMode: TextEdit.NoWrap
                            selectByMouse: true
                            padding: 10
                        }
                    }
                }

                Button {
                    id: hideDebugButton
                    visible: debugVisible
                    text: "डिबग लुकाउनुहोस्"
                    Layout.fillWidth: true
                    height: 40
                    onClicked: debugVisible = false

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

    onOpened: {
        if (panchangaData) {
            clearPanchangaDetails();
            modalTitle.text = panchangaData.gregorianDate;
            var details = [
                        createDetailRow("बिक्रम संवत", toDevanagari(panchangaData.bsYear) + " " + panchangaData.monthName + " " + toDevanagari(panchangaData.bsDay)),
                        createDetailRow("वार", panchangaData.weekday),
                        createDetailRow("तिथि", panchangaData.tithi + " (" + panchangaData.paksha + ")"),
                        createDetailRow("नक्षत्र", panchangaData.nakshatra),
                        createDetailRow("योग", panchangaData.yoga),
                        createDetailRow("करण", panchangaData.karana),
                        createDetailRow("सूर्य राशि", panchangaData.sunRashi),
                        createDetailRow("चन्द्र राशि", panchangaData.moonRashi),
                        createDetailRow("उदयास्त", "सूर्योदय " + panchangaData.sunrise + " | सूर्यास्त " + panchangaData.sunset),
                        createDetailRow("अधिक/क्षय मास", panchangaData.adhikaMasa)
                    ];
            for (var i = 0; i < details.length; ++i) {
                details[i].parent = panchangaDetails;
            }
            generateDebugInfo();
        }
    }

    onClosed: {
        clearPanchangaDetails();
        Panchanga.clearCache();
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

        var label2 = Qt.createQmlObject('import QtQuick.Controls 2.15; import QtQuick 2.15; Label {}', rowItem);
        label2.text = value;
        label2.color = theme ? theme.primaryText : "black";
        label2.wrapMode = Text.WordWrap;
        label2.font.pixelSize = 14;
        label2.Layout.fillWidth = true;
        return rowItem;
    }

    function generateDebugInfo() {
        try {
            var dateParts = panchangaData.gregorianDate.split(" ");
            var monthNames = ["January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"];
            var monthIndex = monthNames.indexOf(dateParts[1]);
            var day = parseInt(dateParts[2]);
            var year = parseInt(dateParts[3]);

            if (monthIndex !== -1) {
                var debugDate = new Date(Date.UTC(year, monthIndex, day));
                var debugInfo = Panchanga.generateDebugInfo(debugDate);
                currentDebugInfo = debugInfo.debug || "Debug information not available";
            } else {
                currentDebugInfo = "Debug Information:\n  Error: Could not parse date for debug information";
            }
        } catch (e) {
            console.error("Error generating debug info:", e);
            currentDebugInfo = "Debug Information:\n  Error: " + e.message;
        }
    }

    function toDevanagari(num) {
        return Panchanga.toDevanagari(num);
    }
}
