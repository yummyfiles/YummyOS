import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Effects
import Backend 1.0

ApplicationWindow {
    id: root

    visible: true
    width: 900
    height: 650
    minimumWidth: 800
    minimumHeight: 600
    title: "Welcome to YummyOS"
    color: "#1a1a2e"
    flags: Qt.Window

    property color primaryColor: "#4CC9F0"
    property color secondaryColor: "#7B2CBF"
    property color backgroundColor: "#1a1a2e"
    property color surfaceColor: "#16213e"
    property color cardColor: "#0f3460"
    property color textPrimary: "#e0e0e0"
    property color textSecondary: "#a0a0b0"
    property color successColor: "#00e676"
    property color warningColor: "#ffc107"
    property color errorColor: "#ff5252"

    property int currentStep: 0
    property int totalSteps: 9

    Backend {
        id: backend
    }

    FontLoader {
        id: loadedFont
        source: "qrc:///fonts/SegoeUI.ttf"
    }

    Rectangle {
        id: mainContainer
        anchors.fill: parent
        color: root.backgroundColor

        // Background gradient overlay
        Rectangle {
            anchors.fill: parent
            gradient: Gradient {
                GradientStop { position: 0.0; color: Qt.rgba(0.102, 0.102, 0.18, 1.0) }
                GradientStop { position: 0.5; color: Qt.rgba(0.059, 0.204, 0.376, 0.3) }
                GradientStop { position: 1.0; color: Qt.rgba(0.102, 0.102, 0.18, 1.0) }
            }
            opacity: 0.5
        }

        ColumnLayout {
            anchors.fill: parent
            spacing: 0

            // Header area with YummyOS branding
            Rectangle {
                Layout.fillWidth: true
                Layout.preferredHeight: 50
                color: "transparent"

                RowLayout {
                    anchors.fill: parent
                    anchors.leftMargin: 30
                    anchors.rightMargin: 30

                    // YummyOS Logo
                    Rectangle {
                        width: 32
                        height: 32
                        radius: 8
                        color: root.primaryColor
                        opacity: 0.9

                        Text {
                            anchors.centerIn: parent
                            text: "Y"
                            font.pixelSize: 18
                            font.bold: true
                            color: "#1a1a2e"
                        }
                    }

                    Text {
                        text: "YummyOS"
                        font.pixelSize: 18
                        font.bold: true
                        color: root.textPrimary
                    }

                    Item { Layout.fillWidth: true }

                    Text {
                        text: "Step " + (root.currentStep + 1) + " of " + root.totalSteps
                        font.pixelSize: 13
                        color: root.textSecondary
                    }
                }

                // Bottom separator
                Rectangle {
                    anchors.bottom: parent.bottom
                    anchors.left: parent.left
                    anchors.right: parent.right
                    height: 1
                    color: Qt.rgba(0.298, 0.788, 0.941, 0.15)
                }
            }

            // Step Indicator
            StepIndicator {
                id: stepIndicator
                Layout.fillWidth: true
                Layout.preferredHeight: 60
                Layout.leftMargin: 40
                Layout.rightMargin: 40
                currentStep: root.currentStep
                totalSteps: root.totalSteps
            }

            // Main content area
            Rectangle {
                Layout.fillWidth: true
                Layout.fillHeight: true
                color: "transparent"
                Layout.leftMargin: 30
                Layout.rightMargin: 30
                Layout.topMargin: 10
                Layout.bottomMargin: 10

                SwipeView {
                    id: swipeView
                    anchors.fill: parent
                    currentIndex: root.currentStep
                    interactive: false
                    clip: true

                    onCurrentIndexChanged: {
                        root.currentStep = currentIndex;
                    }

                    // Page 0: Welcome
                    Page {
                        id: welcomePage
                        background: Rectangle { color: "transparent" }

                        Rectangle {
                            anchors.centerIn: parent
                            width: parent.width * 0.8
                            height: parent.height * 0.8
                            color: "transparent"

                            ColumnLayout {
                                anchors.centerIn: parent
                                spacing: 20

                                Item { Layout.fillHeight: true }

                                // Logo
                                Rectangle {
                                    Layout.alignment: Qt.AlignHCenter
                                    width: 100
                                    height: 100
                                    radius: 24
                                    color: Qt.rgba(0.298, 0.788, 0.941, 0.15)

                                    Rectangle {
                                        anchors.centerIn: parent
                                        width: 80
                                        height: 80
                                        radius: 20
                                        color: root.primaryColor

                                        Text {
                                            anchors.centerIn: parent
                                            text: "Y"
                                            font.pixelSize: 42
                                            font.bold: true
                                            color: "#1a1a2e"
                                        }
                                    }
                                }

                                Text {
                                    Layout.alignment: Qt.AlignHCenter
                                    text: "Welcome to YummyOS!"
                                    font.pixelSize: 32
                                    font.bold: true
                                    color: root.textPrimary
                                }

                                Text {
                                    Layout.alignment: Qt.AlignHCenter
                                    text: "A modern operating system that adapts to you."
                                    font.pixelSize: 16
                                    color: root.textSecondary
                                    horizontalAlignment: Text.AlignHCenter
                                }

                                Text {
                                    Layout.alignment: Qt.AlignHCenter
                                    text: "Let's set up your new system in just a few steps.\nThis wizard will help you connect, update, and personalize YummyOS."
                                    font.pixelSize: 13
                                    color: root.textSecondary
                                    horizontalAlignment: Text.AlignHCenter
                                    lineHeight: 1.4
                                }

                                Item { Layout.preferredHeight: 20 }

                                Button {
                                    id: getStartedBtn
                                    Layout.alignment: Qt.AlignHCenter
                                    Layout.preferredWidth: 200
                                    Layout.preferredHeight: 48
                                    text: "Get Started"
                                    font.pixelSize: 15
                                    font.bold: true

                                    background: Rectangle {
                                        radius: 24
                                        color: getStartedBtn.down ? Qt.darker(root.primaryColor, 1.2) :
                                               getStartedBtn.hovered ? Qt.lighter(root.primaryColor, 1.1) : root.primaryColor

                                        Behavior on color { ColorAnimation { duration: 200 } }
                                    }

                                    contentItem: Text {
                                        text: getStartedBtn.text
                                        font: getStartedBtn.font
                                        color: "#1a1a2e"
                                        horizontalAlignment: Text.AlignHCenter
                                        verticalAlignment: Text.AlignVCenter
                                    }

                                    onClicked: {
                                        root.currentStep = 1;
                                        swipeView.currentIndex = 1;
                                    }
                                }

                                Item { Layout.fillHeight: true }
                            }
                        }
                    }

                    // Page 1: Network
                    NetworkPage {
                        id: networkPage
                        backend: backend
                        primaryColor: root.primaryColor
                        secondaryColor: root.secondaryColor
                        surfaceColor: root.surfaceColor
                        cardColor: root.cardColor
                        textPrimary: root.textPrimary
                        textSecondary: root.textSecondary
                    }

                    // Page 2: Drivers
                    DriversPage {
                        id: driversPage
                        backend: backend
                        primaryColor: root.primaryColor
                        secondaryColor: root.secondaryColor
                        surfaceColor: root.surfaceColor
                        cardColor: root.cardColor
                        textPrimary: root.textPrimary
                        textSecondary: root.textSecondary
                    }

                    // Page 3: Updates
                    UpdatesPage {
                        id: updatesPage
                        backend: backend
                        primaryColor: root.primaryColor
                        secondaryColor: root.secondaryColor
                        surfaceColor: root.surfaceColor
                        cardColor: root.cardColor
                        textPrimary: root.textPrimary
                        textSecondary: root.textSecondary
                    }

                    // Page 4: Gaming
                    GamingPage {
                        id: gamingPage
                        backend: backend
                        primaryColor: root.primaryColor
                        secondaryColor: root.secondaryColor
                        surfaceColor: root.surfaceColor
                        cardColor: root.cardColor
                        textPrimary: root.textPrimary
                        textSecondary: root.textSecondary
                    }

                    // Page 5: Developer
                    DeveloperPage {
                        id: developerPage
                        backend: backend
                        primaryColor: root.primaryColor
                        secondaryColor: root.secondaryColor
                        surfaceColor: root.surfaceColor
                        cardColor: root.cardColor
                        textPrimary: root.textPrimary
                        textSecondary: root.textSecondary
                    }

                    // Page 6: Appearance
                    AppearancePage {
                        id: appearancePage
                        backend: backend
                        primaryColor: root.primaryColor
                        secondaryColor: root.secondaryColor
                        surfaceColor: root.surfaceColor
                        cardColor: root.cardColor
                        textPrimary: root.textPrimary
                        textSecondary: root.textSecondary
                    }

                    // Page 7: Import
                    ImportPage {
                        id: importPage
                        backend: backend
                        primaryColor: root.primaryColor
                        secondaryColor: root.secondaryColor
                        surfaceColor: root.surfaceColor
                        cardColor: root.cardColor
                        textPrimary: root.textPrimary
                        textSecondary: root.textSecondary
                    }

                    // Page 8: Done
                    DonePage {
                        id: donePage
                        backend: backend
                        primaryColor: root.primaryColor
                        secondaryColor: root.secondaryColor
                        surfaceColor: root.surfaceColor
                        cardColor: root.cardColor
                        textPrimary: root.textPrimary
                        textSecondary: root.textSecondary
                    }
                }
            }

            // Footer navigation
            Rectangle {
                Layout.fillWidth: true
                Layout.preferredHeight: 70
                color: "transparent"

                // Top separator
                Rectangle {
                    anchors.top: parent.top
                    anchors.left: parent.left
                    anchors.right: parent.right
                    height: 1
                    color: Qt.rgba(0.298, 0.788, 0.941, 0.15)
                }

                RowLayout {
                    anchors.fill: parent
                    anchors.leftMargin: 30
                    anchors.rightMargin: 30

                    Button {
                        id: backBtn
                        Layout.preferredWidth: 120
                        Layout.preferredHeight: 40
                        text: "← Back"
                        font.pixelSize: 13
                        enabled: root.currentStep > 0
                        opacity: enabled ? 1.0 : 0.3

                        background: Rectangle {
                            radius: 20
                            color: backBtn.down ? Qt.rgba(1,1,1,0.1) :
                                   backBtn.hovered ? Qt.rgba(1,1,1,0.05) : "transparent"
                            border.color: Qt.rgba(1,1,1,0.2)
                            border.width: 1
                        }

                        contentItem: Text {
                            text: backBtn.text
                            font: backBtn.font
                            color: root.textPrimary
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                        }

                        onClicked: {
                            if (root.currentStep > 0) {
                                root.currentStep--;
                                swipeView.currentIndex = root.currentStep;
                            }
                        }
                    }

                    Item { Layout.fillWidth: true }

                    Button {
                        id: skipBtn
                        Layout.preferredWidth: 80
                        Layout.preferredHeight: 40
                        text: "Skip"
                        font.pixelSize: 13
                        visible: root.currentStep < root.totalSteps - 1

                        background: Rectangle {
                            radius: 20
                            color: skipBtn.down ? Qt.rgba(1,1,1,0.1) :
                                   skipBtn.hovered ? Qt.rgba(1,1,1,0.05) : "transparent"
                        }

                        contentItem: Text {
                            text: skipBtn.text
                            font: skipBtn.font
                            color: root.textSecondary
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                        }

                        onClicked: {
                            if (root.currentStep < root.totalSteps - 1) {
                                root.currentStep++;
                                swipeView.currentIndex = root.currentStep;
                            }
                        }
                    }

                    Button {
                        id: nextBtn
                        Layout.preferredWidth: 140
                        Layout.preferredHeight: 40
                        text: root.currentStep === root.totalSteps - 1 ? "Finish" : "Next →"
                        font.pixelSize: 13
                        font.bold: true

                        background: Rectangle {
                            radius: 20
                            color: nextBtn.down ? Qt.darker(root.primaryColor, 1.2) :
                                   nextBtn.hovered ? Qt.lighter(root.primaryColor, 1.1) : root.primaryColor

                            Behavior on color { ColorAnimation { duration: 200 } }
                        }

                        contentItem: Text {
                            text: nextBtn.text
                            font: nextBtn.font
                            color: "#1a1a2e"
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                        }

                        onClicked: {
                            if (root.currentStep < root.totalSteps - 1) {
                                root.currentStep++;
                                swipeView.currentIndex = root.currentStep;
                            } else {
                                Qt.quit();
                            }
                        }
                    }
                }
            }
        }
    }

    // Smooth page transition
    Behavior on currentStep {
        NumberAnimation {
            duration: 300
            easing.type: Easing.InOutCubic
        }
    }
}
