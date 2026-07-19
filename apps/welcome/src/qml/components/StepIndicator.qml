import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Item {
    id: stepIndicator

    property int currentStep: 0
    property int totalSteps: 9
    property color primaryColor: "#4CC9F0"
    property color completedColor: "#00e676"
    property color inactiveColor: "#3a3a5c"

    property var stepNames: ["Welcome", "Network", "Drivers", "Updates", "Gaming", "Dev", "Theme", "Import", "Done"]

    implicitHeight: 60

    RowLayout {
        anchors.centerIn: parent
        spacing: 0
        width: Math.min(parent.width - 40, 800)

        Repeater {
            model: stepIndicator.totalSteps

            RowLayout {
                spacing: 0
                Layout.fillWidth: index === stepIndicator.totalSteps - 1

                // Step circle and line container
                ColumnLayout {
                    spacing: 4
                    Layout.fillWidth: true

                    RowLayout {
                        spacing: 0
                        Layout.fillWidth: true

                        // Left connector line (not for first item)
                        Rectangle {
                            visible: index > 0
                            Layout.fillWidth: true
                            Layout.preferredHeight: 2
                            Layout.alignment: Qt.AlignVCenter
                            color: index <= stepIndicator.currentStep ? stepIndicator.primaryColor : stepIndicator.inactiveColor

                            Behavior on color { ColorAnimation { duration: 300 } }
                        }

                        // Circle indicator
                        Rectangle {
                            width: 28
                            height: 28
                            radius: 14
                            color: index < stepIndicator.currentStep ? stepIndicator.completedColor :
                                   index === stepIndicator.currentStep ? stepIndicator.primaryColor :
                                   stepIndicator.inactiveColor

                            Behavior on color { ColorAnimation { duration: 300 } }

                            // Pulse animation for current step
                            Rectangle {
                                anchors.centerIn: parent
                                width: parent.width + 8
                                height: parent.height + 8
                                radius: (parent.width + 8) / 2
                                color: "transparent"
                                border.color: stepIndicator.primaryColor
                                border.width: 2
                                opacity: index === stepIndicator.currentStep ? 0.4 : 0

                                SequentialAnimation on opacity {
                                    running: index === stepIndicator.currentStep
                                    loops: Animation.Infinite
                                    NumberAnimation { to: 0.0; duration: 1000; easing.type: Easing.InOutSine }
                                    NumberAnimation { to: 0.4; duration: 1000; easing.type: Easing.InOutSine }
                                }

                                SequentialAnimation on width {
                                    running: index === stepIndicator.currentStep
                                    loops: Animation.Infinite
                                    NumberAnimation { to: parent.width + 16; duration: 1000; easing.type: Easing.InOutSine }
                                    NumberAnimation { to: parent.width + 8; duration: 1000; easing.type: Easing.InOutSine }
                                }

                                SequentialAnimation on height {
                                    running: index === stepIndicator.currentStep
                                    loops: Animation.Infinite
                                    NumberAnimation { to: parent.height + 16; duration: 1000; easing.type: Easing.InOutSine }
                                    NumberAnimation { to: parent.height + 8; duration: 1000; easing.type: Easing.InOutSine }
                                }

                                SequentialAnimation on radius {
                                    running: index === stepIndicator.currentStep
                                    loops: Animation.Infinite
                                    NumberAnimation { to: (parent.width + 16) / 2; duration: 1000; easing.type: Easing.InOutSine }
                                    NumberAnimation { to: (parent.width + 8) / 2; duration: 1000; easing.type: Easing.InOutSine }
                                }
                            }

                            Text {
                                anchors.centerIn: parent
                                text: index < stepIndicator.currentStep ? "✓" : (index + 1).toString()
                                font.pixelSize: 12
                                font.bold: true
                                color: index <= stepIndicator.currentStep ? "#1a1a2e" : "#888"
                            }
                        }

                        // Right connector line (not for last item)
                        Rectangle {
                            visible: index < stepIndicator.totalSteps - 1
                            Layout.fillWidth: true
                            Layout.preferredHeight: 2
                            Layout.alignment: Qt.AlignVCenter
                            color: index < stepIndicator.currentStep ? stepIndicator.primaryColor : stepIndicator.inactiveColor

                            Behavior on color { ColorAnimation { duration: 300 } }
                        }
                    }

                    // Step label
                    Text {
                        Layout.alignment: Qt.AlignHCenter
                        text: stepIndicator.stepNames[index] || ""
                        font.pixelSize: 9
                        color: index <= stepIndicator.currentStep ? stepIndicator.primaryColor : "#666"
                        elide: Text.ElideRight
                        horizontalAlignment: Text.AlignHCenter

                        Behavior on color { ColorAnimation { duration: 300 } }
                    }
                }
            }
        }
    }
}
