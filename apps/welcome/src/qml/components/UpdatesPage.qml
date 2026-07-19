import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import Backend 1.0

Page {
    id: updatesPage

    property Backend backend
    property color primaryColor: "#4CC9F0"
    property color secondaryColor: "#7B2CBF"
    property color surfaceColor: "#16213e"
    property color cardColor: "#0f3460"
    property color textPrimary: "#e0e0e0"
    property color textSecondary: "#a0a0b0"

    background: Rectangle { color: "transparent" }

    property bool isChecking: false
    property bool isInstalling: false
    property int updateCount: 0
    property var updateList: []
    property bool checkComplete: false

    Connections {
        target: backend
        function onUpdateCheckComplete(count, updates) {
            updatesPage.updateCount = count;
            updatesPage.updateList = updates;
            updatesPage.isChecking = false;
            updatesPage.checkComplete = true;
        }
        function onPackageInstalled(packageName, success, message) {
            if (packageName === "updates") {
                updatesPage.isInstalling = false;
                installStatus.text = success ? "System updated successfully!" : "Update failed: " + message;
                installStatus.color = success ? "#00e676" : "#ff5252";
            }
        }
    }

    PageCard {
        anchors.fill: parent
        title: "System Updates"
        description: "Keep your system up to date with the latest packages"
        primaryColor: updatesPage.primaryColor
        surfaceColor: updatesPage.surfaceColor
        cardColor: updatesPage.cardColor
        textPrimary: updatesPage.textPrimary
        textSecondary: updatesPage.textSecondary

        ColumnLayout {
            anchors.fill: parent
            anchors.topMargin: 80
            spacing: 20

            // Update check result
            Rectangle {
                Layout.fillWidth: true
                Layout.preferredHeight: 100
                radius: 12
                color: updatesPage.checkComplete ?
                       (updatesPage.updateCount > 0 ? Qt.rgba(1, 0.757, 0.027, 0.08) : Qt.rgba(0, 0.902, 0.463, 0.08)) :
                       Qt.rgba(1,1,1,0.03)
                border.color: updatesPage.checkComplete ?
                              (updatesPage.updateCount > 0 ? Qt.rgba(1, 0.757, 0.027, 0.2) : Qt.rgba(0, 0.902, 0.463, 0.2)) :
                              Qt.rgba(1,1,1,0.05)
                border.width: 1
                visible: updatesPage.checkComplete

                RowLayout {
                    anchors.fill: parent
                    anchors.margins: 20
                    spacing: 16

                    Rectangle {
                        width: 50
                        height: 50
                        radius: 14
                        color: updatesPage.updateCount > 0 ? Qt.rgba(1, 0.757, 0.027, 0.15) : Qt.rgba(0, 0.902, 0.463, 0.15)

                        Text {
                            anchors.centerIn: parent
                            text: updatesPage.updateCount > 0 ? "⬆" : "✓"
                            font.pixelSize: 24
                            color: updatesPage.updateCount > 0 ? "#ffc107" : "#00e676"
                        }
                    }

                    ColumnLayout {
                        spacing: 4
                        Layout.fillWidth: true

                        Text {
                            text: updatesPage.updateCount > 0 ?
                                  updatesPage.updateCount + " update" + (updatesPage.updateCount !== 1 ? "s" : "") + " available" :
                                  "System is up to date!"
                            font.pixelSize: 20
                            font.bold: true
                            color: updatesPage.textPrimary
                        }

                        Text {
                            text: updatesPage.updateCount > 0 ?
                                  "Updated packages will improve security and performance" :
                                  "All packages are at their latest versions"
                            font.pixelSize: 13
                            color: updatesPage.textSecondary
                        }
                    }
                }
            }

            // Update list
            ScrollView {
                Layout.fillWidth: true
                Layout.fillHeight: updatesPage.updateCount === 0
                Layout.preferredHeight: Math.min(updatesPage.updateCount * 40 + 20, 250)
                visible: updatesPage.updateCount > 0 && updatesPage.checkComplete
                clip: true

                ListView {
                    model: updatesPage.updateList
                    spacing: 4

                    delegate: Rectangle {
                        width: updatesList.width
                        height: 36
                        radius: 6
                        color: index % 2 === 0 ? "transparent" : Qt.rgba(1,1,1,0.02)

                        RowLayout {
                            anchors.fill: parent
                            anchors.leftMargin: 12
                            anchors.rightMargin: 12

                            Text {
                                text: modelData.name || ""
                                font.pixelSize: 13
                                font.bold: true
                                color: updatesPage.textPrimary
                                Layout.preferredWidth: 150
                                elide: Text.ElideRight
                            }

                            Text {
                                text: (modelData.currentVersion || "") + " → " + (modelData.newVersion || "")
                                font.pixelSize: 12
                                color: updatesPage.primaryColor
                            }
                        }
                    }
                }
            }

            // Buttons row
            RowLayout {
                Layout.fillWidth: true
                spacing: 12

                Button {
                    id: checkBtn
                    Layout.preferredWidth: 180
                    Layout.preferredHeight: 44
                    text: updatesPage.isChecking ? "Checking..." : "Check for Updates"
                    font.pixelSize: 14
                    font.bold: true
                    enabled: !updatesPage.isChecking && !updatesPage.isInstalling

                    background: Rectangle {
                        radius: 22
                        color: checkBtn.down ? Qt.darker(updatesPage.primaryColor, 1.2) :
                               checkBtn.hovered ? Qt.lighter(updatesPage.primaryColor, 1.1) :
                               "transparent"
                        border.color: updatesPage.primaryColor
                        border.width: 2
                    }

                    contentItem: Text {
                        text: checkBtn.text
                        font: checkBtn.font
                        color: updatesPage.primaryColor
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }

                    onClicked: {
                        updatesPage.isChecking = true;
                        updatesPage.checkComplete = false;
                        backend.checkForUpdates();
                    }
                }

                Button {
                    id: updateNowBtn
                    Layout.preferredWidth: 160
                    Layout.preferredHeight: 44
                    text: updatesPage.isInstalling ? "Updating..." : "Update Now"
                    font.pixelSize: 14
                    font.bold: true
                    enabled: updatesPage.updateCount > 0 && !updatesPage.isInstalling && !updatesPage.isChecking

                    background: Rectangle {
                        radius: 22
                        color: parent.down ? Qt.darker(updatesPage.primaryColor, 1.2) :
                               parent.hovered ? Qt.lighter(updatesPage.primaryColor, 1.1) :
                               updatesPage.primaryColor
                        opacity: parent.enabled ? 1.0 : 0.5

                        Behavior on color { ColorAnimation { duration: 200 } }
                    }

                    contentItem: Text {
                        text: updateNowBtn.text
                        font: updateNowBtn.font
                        color: "#1a1a2e"
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }

                    onClicked: {
                        updatesPage.isInstalling = true;
                        backend.installUpdates();
                    }
                }

                Item { Layout.fillWidth: true }
            }

            BusyIndicator {
                Layout.alignment: Qt.AlignHCenter
                width: 24
                height: 24
                running: updatesPage.isChecking || updatesPage.isInstalling
                visible: running
            }

            Text {
                id: installStatus
                Layout.alignment: Qt.AlignHCenter
                text: ""
                font.pixelSize: 13
                visible: text !== ""
            }

            Item { Layout.fillHeight: true }
        }
    }
}
