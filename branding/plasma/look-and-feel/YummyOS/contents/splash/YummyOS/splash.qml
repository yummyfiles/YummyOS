import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Layouts 1.15
import QtGraphicalEffects 1.15

Item {
    id: root

    property real stage: 0

    anchors.fill: parent

    // Dark background with gradient
    Rectangle {
        id: background
        anchors.fill: parent
        gradient: Gradient {
            GradientStop { position: 0.0; color: "#0a0a1a" }
            GradientStop { position: 1.0; color: "#1a1a2e" }
        }
    }

    // Subtle accent glow behind text
    RadialGradient {
        anchors.centerIn: parent
        width: 400
        height: 400
        gradient: Gradient {
            GradientStop { position: 0.0; color: "#4CC9F022" }
            GradientStop { position: 1.0; color: "#4CC9F000" }
        }
        opacity: label.opacity
    }

    // Logo and text container
    ColumnLayout {
        anchors.centerIn: parent
        spacing: 24

        // YummyOS text
        Text {
            id: label
            text: "YummyOS"
            color: "white"
            font.pixelSize: 72
            font.weight: Font.Bold
            font.family: "Noto Sans"
            Layout.alignment: Qt.AlignHCenter
            opacity: 0

            SequentialAnimation on opacity {
                id: labelAnimation
                running: false
                PauseAnimation { duration: 200 }
                NumberAnimation {
                    from: 0.0
                    to: 1.0
                    duration: 1200
                    easing.type: Easing.InOutCubic
                }
            }
        }

        // Loading indicator
        RowLayout {
            Layout.alignment: Qt.AlignHCenter
            spacing: 8
            opacity: 0

            Repeater {
                model: 3

                Rectangle {
                    width: 8
                    height: 8
                    radius: 4
                    color: "#4CC9F0"
                    opacity: 0.3

                    SequentialAnimation on opacity {
                        id: dotAnim_${index}
                        running: false
                        loops: Animation.Infinite
                        PauseAnimation { duration: index * 200 }
                        NumberAnimation {
                            from: 0.3
                            to: 1.0
                            duration: 600
                            easing.type: Easing.InOutSine
                        }
                        NumberAnimation {
                            from: 1.0
                            to: 0.3
                            duration: 600
                            easing.type: Easing.InOutSine
                        }
                        PauseAnimation { duration: (3 - index) * 200 }
                    }
                }
            }

            SequentialAnimation on opacity {
                id: dotsAnimation
                running: false
                PauseAnimation { duration: 800 }
                NumberAnimation {
                    from: 0.0
                    to: 1.0
                    duration: 800
                    easing.type: Easing.InOutCubic
                }
            }
        }
    }

    // Startup sequence
    Component.onCompleted: {
        labelAnimation.start();
        dotsAnimation.start();
        for (var i = 0; i < 3; i++) {
            dotAnim_${i}.start();
        }
    }

    // Fade out when ready
    SequentialAnimation {
        id: fadeOut
        running: false
        PauseAnimation { duration: 3000 }
        NumberAnimation {
            targets: [root]
            property: "opacity"
            from: 1.0
            to: 0.0
            duration: 600
            easing.type: Easing.InOutCubic
        }
        ScriptAction {
            script: Qt.quit()
        }
    }

    Timer {
        interval: 3600
        running: true
        onTriggered: fadeOut.start()
    }
}
