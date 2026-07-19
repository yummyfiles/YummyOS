import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import Backend 1.0

Page {
    id: driversPage

    property Backend backend
    property color primaryColor: "#4CC9F0"
    property color secondaryColor: "#7B2CBF"
    property color surfaceColor: "#16213e"
    property color cardColor: "#0f3460"
    property color textPrimary: "#e0e0e0"
    property color textSecondary: "#a0a0b0"

    background: Rectangle { color: "transparent" }

    property string detectedGpu: ""
    property string recommendedDriver: ""
    property bool isInstalling: false
    property bool installComplete: false

    Connections {
        target: backend
        function onGpuDetected(gpuType, recommendedDriver) {
            driversPage.detectedGpu = gpuType;
            driversPage.recommendedDriver = recommendedDriver;
            detectingLabel.visible = false;
            detectedCard.visible = true;
        }
        function onPackageInstalled(packageName, success, message) {
            if (packageName === driversPage.recommendedDriver) {
                driversPage.isInstalling = false;
                driversPage.installComplete = true;
                installStatus.text = success ? "Driver installed successfully!" : "Installation failed: " + message;
                installStatus.color = success ? "#00e676" : "#ff5252";
            }
        }
    }

    Component.onCompleted: {
        backend.detectGpu();
    }

    PageCard {
        anchors.fill: parent
        title: "Driver Installation"
        description: "Detect and install GPU drivers for optimal performance"
        primaryColor: driversPage.primaryColor
        surfaceColor: driversPage.surfaceColor
        cardColor: driversPage.cardColor
        textPrimary: driversPage.textPrimary
        textSecondary: driversPage.textSecondary

        ColumnLayout {
            anchors.fill: parent
            anchors.topMargin: 80
            spacing: 20

            // Detecting indicator
            RowLayout {
                id: detectingLabel
                spacing: 10
                Layout.alignment: Qt.AlignHCenter
                visible: true

                BusyIndicator {
                    width: 20
                    height: 20
                    running: detectingLabel.visible
                }

                Text {
                    text: "Detecting GPU..."
                    font.pixelSize: 14
                    color: driversPage.textSecondary
                }
            }

            // Detected GPU info
            Rectangle {
                id: detectedCard
                Layout.fillWidth: true
                Layout.preferredHeight: 120
                radius: 12
                color: Qt.rgba(0.298, 0.788, 0.941, 0.08)
                border.color: Qt.rgba(0.298, 0.788, 0.941, 0.2)
                border.width: 1
                visible: false

                RowLayout {
                    anchors.fill: parent
                    anchors.margins: 20
                    spacing: 20

                    // GPU icon
                    Rectangle {
                        width: 60
                        height: 60
                        radius: 16
                        color: driversPage.detectedGpu === "nvidia" ? Qt.rgba(0.486, 0.988, 0.0, 0.15) :
                               driversPage.detectedGpu === "amd" ? Qt.rgba(1, 0.333, 0.333, 0.15) :
                               Qt.rgba(0.298, 0.788, 0.941, 0.15)

                        Text {
                            anchors.centerIn: parent
                            text: driversPage.detectedGpu === "nvidia" ? "N" :
                                  driversPage.detectedGpu === "amd" ? "A" : "I"
                            font.pixelSize: 26
                            font.bold: true
                            color: driversPage.detectedGpu === "nvidia" ? "#76b900" :
                                   driversPage.detectedGpu === "amd" ? "#ff5555" :
                                   driversPage.primaryColor
                        }
                    }

                    ColumnLayout {
                        spacing: 6
                        Layout.fillWidth: true

                        Text {
                            text: "GPU Detected"
                            font.pixelSize: 11
                            color: driversPage.textSecondary
                            textTransform: Text.Capitalize
                        }

                        Text {
                            text: {
                                switch (driversPage.detectedGpu) {
                                case "nvidia": return "NVIDIA GPU";
                                case "amd": return "AMD / Radeon GPU";
                                case "intel": return "Intel Integrated Graphics";
                                default: return "Unknown GPU";
                                }
                            }
                            font.pixelSize: 20
                            font.bold: true
                            color: driversPage.textPrimary
                        }

                        Text {
                            text: "Recommended driver: " + driversPage.recommendedDriver
                            font.pixelSize: 13
                            color: driversPage.primaryColor
                        }
                    }
                }
            }

            // Open-source notice for Intel
            Rectangle {
                Layout.fillWidth: true
                Layout.preferredHeight: 60
                radius: 10
                color: Qt.rgba(0, 0.902, 0.463, 0.08)
                border.color: Qt.rgba(0, 0.902, 0.463, 0.2)
                border.width: 1
                visible: driversPage.detectedGpu === "intel"

                RowLayout {
                    anchors.fill: parent
                    anchors.margins: 16
                    spacing: 12

                    Text {
                        text: "ℹ"
                        font.pixelSize: 20
                        color: "#00e676"
                    }

                    Text {
                        text: "Intel graphics use open-source drivers included with YummyOS.\nNo proprietary drivers needed!"
                        font.pixelSize: 13
                        color: "#00e676"
                        lineHeight: 1.3
                        Layout.fillWidth: true
                    }
                }
            }

            // Install button
            Button {
                id: installDriverBtn
                Layout.alignment: Qt.AlignHCenter
                Layout.preferredWidth: 220
                Layout.preferredHeight: 44
                text: driversPage.isInstalling ? "Installing..." :
                      driversPage.installComplete ? "✓ Installed" :
                      "Install " + driversPage.recommendedDriver + " Driver"
                font.pixelSize: 14
                font.bold: true
                enabled: !driversPage.isInstalling && !driversPage.installComplete && driversPage.detectedGpu !== "intel"
                visible: driversPage.detectedGpu !== ""

                background: Rectangle {
                    radius: 22
                    color: parent.down ? Qt.darker(driversPage.primaryColor, 1.2) :
                           parent.hovered ? Qt.lighter(driversPage.primaryColor, 1.1) :
                           driversPage.installComplete ? "#00e676" :
                           driversPage.primaryColor
                    opacity: parent.enabled ? 1.0 : 0.5

                    Behavior on color { ColorAnimation { duration: 200 } }
                }

                contentItem: Text {
                    text: installDriverBtn.text
                    font: installDriverBtn.font
                    color: "#1a1a2e"
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }

                onClicked: {
                    driversPage.isInstalling = true;
                    backend.installGpuDriver(driversPage.recommendedDriver);
                }
            }

            // Progress indicator
            BusyIndicator {
                Layout.alignment: Qt.AlignHCenter
                width: 24
                height: 24
                running: driversPage.isInstalling
                visible: driversPage.isInstalling
            }

            // Install status
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
