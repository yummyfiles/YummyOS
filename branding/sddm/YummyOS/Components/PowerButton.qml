import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Rectangle {
    id: control
    width: 42
    height: 42
    radius: 21
    color: Qt.rgba(1, 1, 1, 0.05)
    border {
        color: Qt.rgba(1, 1, 1, 0.08)
        width: 1
    }

    property string icon: "⏻"
    property string tooltip: ""

    signal clicked()

    Text {
        anchors.centerIn: parent
        font {
            pixelSize: 18
        }
        color: "#8888aa"
        text: control.icon
    }

    Text {
        id: tooltipText
        anchors {
            bottom: parent.top
            bottomMargin: 6
            horizontalCenter: parent.horizontalCenter
        }
        font {
            family: "Noto Sans, Segoe UI, sans-serif"
            pixelSize: 12
        }
        color: "#CCCCDD"
        text: control.tooltip
        visible: tooltipArea.containsMouse
        opacity: 0

        Behavior on opacity {
            NumberAnimation { duration: 150 }
        }
    }

    MouseArea {
        id: tooltipArea
        anchors.fill: parent
        hoverEnabled: true
        cursorShape: Qt.PointingHandCursor

        onContainsMouseChanged: {
            tooltipText.opacity = containsMouse ? 1.0 : 0.0;
        }

        onClicked: {
            control.clicked();
        }
    }

    Behavior on color {
        ColorAnimation { duration: 150 }
    }

    states: [
        State {
            name: "hovered"
            when: tooltipArea.containsMouse
            PropertyChanges {
                target: control
                color: Qt.rgba(1, 1, 1, 0.1)
                border.color: Qt.rgba(1, 1, 1, 0.15)
            }
            PropertyChanges {
                target: control.children[0]
                color: "#FFFFFF"
            }
        }
    ]
}
