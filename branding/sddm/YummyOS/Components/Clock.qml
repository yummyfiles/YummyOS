import QtQuick 2.15
import QtQuick.Controls 2.15

Item {
    id: clockWidget
    width: parent.width
    height: 120

    property color textColor: "#FFFFFF"

    Text {
        id: timeDisplay
        anchors {
            top: parent.top
            horizontalCenter: parent.horizontalCenter
        }
        font {
            family: "Noto Sans, Segoe UI, sans-serif"
            pixelSize: 72
            weight: Font.Light
        }
        color: clockWidget.textColor
        text: "00:00"
        horizontalAlignment: Text.AlignHCenter

        Timer {
            id: clockTimer
            interval: 1000
            repeat: true
            running: true
            onTriggered: {
                var now = new Date();
                var hours = now.getHours().toString().padStart(2, '0');
                var minutes = now.getMinutes().toString().padStart(2, '0');
                timeDisplay.text = hours + ":" + minutes;
                dateDisplay.text = Qt.formatDate(now, "dddd, MMMM d");
            }
        }
    }

    Text {
        id: dateDisplay
        anchors {
            top: timeDisplay.bottom
            topMargin: 8
            horizontalCenter: parent.horizontalCenter
        }
        font {
            family: "Noto Sans, Segoe UI, sans-serif"
            pixelSize: 20
            weight: Font.Light
        }
        color: clockWidget.textColor
        opacity: 0.8
        text: "Monday, January 1"
        horizontalAlignment: Text.AlignHCenter
    }
}
