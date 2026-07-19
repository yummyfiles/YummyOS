import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Rectangle {
    id: pageCard

    property alias title: titleLabel.text
    property alias description: descLabel.text

    property color primaryColor: "#4CC9F0"
    property color surfaceColor: "#16213e"
    property color cardColor: "#0f3460"
    property color textPrimary: "#e0e0e0"
    property color textSecondary: "#a0a0b0"

    color: Qt.rgba(0.059, 0.204, 0.376, 0.4)
    radius: 16
    border.color: Qt.rgba(0.298, 0.788, 0.941, 0.1)
    border.width: 1

    layer.enabled: true
    layer.effect: Item {}

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 32
        spacing: 16

        // Title row
        RowLayout {
            Layout.fillWidth: true
            spacing: 12

            Rectangle {
                width: 40
                height: 40
                radius: 12
                color: Qt.rgba(0.298, 0.788, 0.941, 0.15)

                Text {
                    anchors.centerIn: parent
                    text: "◆"
                    font.pixelSize: 16
                    color: pageCard.primaryColor
                }
            }

            ColumnLayout {
                spacing: 2
                Layout.fillWidth: true

                Text {
                    id: titleLabel
                    text: ""
                    font.pixelSize: 22
                    font.bold: true
                    color: pageCard.textPrimary
                }

                Text {
                    id: descLabel
                    text: ""
                    font.pixelSize: 13
                    color: pageCard.textSecondary
                    visible: text !== ""
                }
            }
        }

        // Separator
        Rectangle {
            Layout.fillWidth: true
            Layout.preferredHeight: 1
            color: Qt.rgba(0.298, 0.788, 0.941, 0.1)
        }

        // Content area
        Item {
            id: contentArea
            Layout.fillWidth: true
            Layout.fillHeight: true
        }
    }

    // Subtle shadow effect using multiple rectangles
    Rectangle {
        anchors.fill: parent
        anchors.margins: -1
        radius: parent.radius + 1
        color: "transparent"
        border.color: Qt.rgba(0, 0, 0, 0.1)
        border.width: 1
        z: -1
    }
}
