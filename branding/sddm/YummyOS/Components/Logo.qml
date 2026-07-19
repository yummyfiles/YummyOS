import QtQuick 2.15
import QtGraphicalEffects 1.15

Item {
    id: logoRoot
    width: parent ? parent.width : 400
    height: 140

    Text {
        id: logoShadow
        anchors.centerIn: parent
        text: "YummyOS"
        font.family: "Segoe UI, Noto Sans, sans-serif"
        font.pixelSize: 72
        font.weight: Font.Light
        color: "#4CC9F0"
        opacity: 0.15
        scale: 1.05

        SequentialAnimation on opacity {
            running: true
            NumberAnimation { from: 0; to: 0.15; duration: 1000; easing.type: Easing.OutCubic }
        }
    }

    Text {
        id: logoText
        anchors.centerIn: parent
        text: "YummyOS"
        font.family: "Segoe UI, Noto Sans, sans-serif"
        font.pixelSize: 72
        font.weight: Font.Light
        color: "#4CC9F0"

        SequentialAnimation on opacity {
            id: loadAnimation
            running: true
            NumberAnimation { from: 0; to: 1; duration: 800; easing.type: Easing.OutCubic }
        }
    }

    Glow {
        anchors.fill: logoText
        radius: 24
        samples: 48
        color: "#4CC9F0"
        source: logoText
        opacity: 0.25

        SequentialAnimation on opacity {
            running: true
            NumberAnimation { from: 0; to: 0.25; duration: 1000; easing.type: Easing.OutCubic }
        }
    }

    Text {
        id: tagline
        anchors.top: logoText.bottom
        anchors.topMargin: 8
        anchors.horizontalCenter: parent.horizontalCenter
        text: "LOGIN"
        font.family: "Segoe UI, Noto Sans, sans-serif"
        font.pixelSize: 13
        font.weight: Font.Light
        color: "#7B2CBF"
        opacity: 0

        SequentialAnimation on opacity {
            running: true
            PauseAnimation { duration: 400 }
            NumberAnimation { from: 0; to: 0.7; duration: 600; easing.type: Easing.OutCubic }
        }
    }
}
