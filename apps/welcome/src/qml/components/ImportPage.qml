import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Dialogs
import Backend 1.0

Page {
    id: importPage

    property Backend backend
    property color primaryColor: "#4CC9F0"
    property color secondaryColor: "#7B2CBF"
    property color surfaceColor: "#16213e"
    property color cardColor: "#0f3460"
    property color textPrimary: "#e0e0e0"
    property color textSecondary: "#a0a0b0"

    background: Rectangle { color: "transparent" }

    property bool isImporting: false
    property string importSource: ""
    property bool importComplete: false

    Connections {
        target: backend
        function onSettingsImported(success, message) {
            importPage.isImporting = false;
            importPage.importComplete = true;
            importStatus.text = message;
            importStatus.color = success ? "#00e676" : "#ff5252";
        }
    }

    PageCard {
        anchors.fill: parent
        title: "Import Settings"
        description: "Restore settings from a previous installation or backup"
        primaryColor: importPage.primaryColor
        surfaceColor: importPage.surfaceColor
        cardColor: importPage.cardColor
        textPrimary: importPage.textPrimary
        textSecondary: importPage.textSecondary

        ColumnLayout {
            anchors.fill: parent
            anchors.topMargin: 80
            spacing: 24

            Item { Layout.fillHeight: true }

            // Import options
            Rectangle {
                Layout.fillWidth: true
                Layout.preferredHeight: 100
                radius: 12
                color: Qt.rgba(1,1,1,0.03)
                border.color: Qt.rgba(1,1,1,0.08)
                border.width: 1

                RowLayout {
                    anchors.fill: parent
                    anchors.margins: 20
                    spacing: 16

                    Rectangle {
                        width: 48
                        height: 48
                        radius: 12
                        color: Qt.rgba(0.298, 0.788, 0.941, 0.15)

                        Text {
                            anchors.centerIn: parent
                            text: "📁"
                            font.pixelSize: 22
                        }
                    }

                    ColumnLayout {
                        spacing: 4
                        Layout.fillWidth: true

                        Text {
                            text: "From Backup Location"
                            font.pixelSize: 15
                            font.bold: true
                            color: importPage.textPrimary
                        }

                        Text {
                            text: "Restore from a previous YummyOS backup or another user's home directory"
                            font.pixelSize: 12
                            color: importPage.textSecondary
                            wrapMode: Text.WordWrap
                            Layout.fillWidth: true
                        }
                    }
                }
            }

            // Source path input
            RowLayout {
                Layout.fillWidth: true
                spacing: 10

                TextField {
                    id: sourcePathField
                    Layout.fillWidth: true
                    Layout.preferredHeight: 44
                    placeholderText: "/path/to/backup or /home/username"
                    color: importPage.textPrimary
                    placeholderTextColor: Qt.rgba(1,1,1,0.3)
                    font.pixelSize: 13
                    enabled: !importPage.isImporting

                    background: Rectangle {
                        radius: 10
                        color: "transparent"
                        border.color: sourcePathField.activeFocus ? importPage.primaryColor : Qt.rgba(1,1,1,0.12)
                        border.width: 1

                        Behavior on border.color { ColorAnimation { duration: 200 } }
                    }

                    onTextChanged: importPage.importSource = text
                }

                Button {
                    text: "Browse"
                    font.pixelSize: 12
                    enabled: !importPage.isImporting

                    background: Rectangle {
                        radius: 10
                        color: parent.down ? Qt.rgba(1,1,1,0.1) :
                               parent.hovered ? Qt.rgba(1,1,1,0.05) : "transparent"
                        border.color: Qt.rgba(1,1,1,0.15)
                        border.width: 1
                    }

                    contentItem: Text {
                        text: parent.text
                        font: parent.font
                        color: importPage.textSecondary
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }

                    onClicked: folderDialog.open()
                }
            }

            // Import button
            Button {
                id: importBtn
                Layout.alignment: Qt.AlignHCenter
                Layout.preferredWidth: 200
                Layout.preferredHeight: 44
                text: importPage.isImporting ? "Importing..." :
                      importPage.importComplete ? "✓ Imported" :
                      "Import Settings"
                font.pixelSize: 14
                font.bold: true
                enabled: !importPage.isImporting && !importPage.importComplete && importPage.importSource.length > 0

                background: Rectangle {
                    radius: 22
                    color: importBtn.down ? Qt.darker(importPage.primaryColor, 1.2) :
                           importBtn.hovered ? Qt.lighter(importPage.primaryColor, 1.1) :
                           importBtn.enabled ? importPage.primaryColor : Qt.rgba(1,1,1,0.1)
                }

                contentItem: Text {
                    text: importBtn.text
                    font: importBtn.font
                    color: importBtn.enabled ? "#1a1a2e" : importPage.textSecondary
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }

                onClicked: {
                    importPage.isImporting = true;
                    backend.importSettings(importPage.importSource);
                }
            }

            BusyIndicator {
                Layout.alignment: Qt.AlignHCenter
                width: 20
                height: 20
                running: importPage.isImporting
                visible: importPage.isImporting
            }

            Text {
                id: importStatus
                Layout.alignment: Qt.AlignHCenter
                text: ""
                font.pixelSize: 13
                visible: text !== ""
            }

            Item { Layout.fillHeight: true }
        }
    }

    FolderDialog {
        id: folderDialog
        title: "Select backup folder"
        onAccepted: {
            sourcePathField.text = folderDialog.selectedFolder.toString().replace("file://", "");
        }
    }
}
