import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import "qrc:/PanchangaCalculator.js" as Panchanga
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

        ColumnLayout {
            id: contentColumn
            width: parent.width

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

            // Footer section with the close button.
            Rectangle {
                id: modalFooter
                Layout.fillWidth: true
                height: 65
                color: "transparent"

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
        }
    }


    //  Handlers and Functions
    onOpened: {
        if (panchangaData) {
            clearPanchangaDetails();
            modalTitle.text = panchangaData.gregorianDate;

            var eventComponent = Qt.createComponent("EventDisplay.qml");
            if (eventComponent.status === Component.Ready) {
                var eventObj = eventComponent.createObject(panchangaDetails, {
                                                               "theme": theme,
                                                               "events": panchangaData.events
                                                           });
            } else {
                console.log("Error creating event component:", eventComponent.errorString());
            }

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
