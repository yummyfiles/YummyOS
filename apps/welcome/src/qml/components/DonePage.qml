import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Effects
import Backend 1.0

Page {
    id: donePage

    property Backend backend
    property color primaryColor: "#4CC9F0"
    property color secondaryColor: "#7B2CBF"
    property color surfaceColor: "#16213e"
    property color cardColor: "#0f3460"
    property color textPrimary: "#e0e0e0"
    property color textSecondary: "#a0a0b0"

    background: Rectangle { color: "transparent" }

    PageCard {
        anchors.fill: parent
        title: ""
        description: ""
        primaryColor: donePage.primaryColor
        surfaceColor: donePage.surfaceColor
        cardColor: donePage.cardColor
        textPrimary: donePage.textPrimary
        textSecondary: donePage.textSecondary

        ColumnLayout {
            anchors.fill: parent
            anchors.topMargin: 40
            spacing: 0

            Item { Layout.fillHeight: true }

            // Success animation area
            Rectangle {
                Layout.alignment: Qt.AlignHCenter
                Layout.preferredWidth: 120
                Layout.preferredHeight: 120
                radius: 60
                color: Qt.rgba(0, 0.902, 0.463, 0.1)

                Rectangle {
                    anchors.centerIn: parent
                    width: 90
                    height: 90
                    radius: 45
                    color: Qt.rgba(0, 0.902, 0.463, 0.15)

                    Rectangle {
                        anchors.centerIn: parent
                        width: 60
                        height: 60
                        radius: 30
                        color: "#00e676"

                        SequentialAnimation on scale {
                            running: true
                            NumberAnimation { from: 0.5; to: 1.0; duration: 500; easing.type: Easing.OutBack }
                        }

                        Text {
                            anchors.centerIn: parent
                            text: "✓"
                            font.pixelSize: 30
                            font.bold: true
                            color: "#1a1a2e"

                            SequentialAnimation on opacity {
                                running: true
                                NumberAnimation { from: 0; to: 1; duration: 300; delay: 300 }
                            }
                        }
                    }
                }
            }

            Item { Layout.preferredHeight: 24 }

            Text {
                Layout.alignment: Qt.AlignHCenter
                text: "You're all set!"
                font.pixelSize: 32
                font.bold: true
                color: donePage.textPrimary

                SequentialAnimation on opacity {
                    running: true
                    NumberAnimation { from: 0; to: 1; duration: 400; delay: 200 }
                }
            }

            Item { Layout.preferredHeight: 8 }

            Text {
                Layout.alignment: Qt.AlignHCenter
                text: "YummyOS is ready for you.\nEnjoy your new system!"
                font.pixelSize: 15
                color: donePage.textSecondary
                horizontalAlignment: Text.AlignHCenter
                lineHeight: 1.4

                SequentialAnimation on opacity {
                    running: true
                    NumberAnimation { from: 0; to: 1; duration: 400; delay: 400 }
                }
            }

            Item { Layout.preferredHeight: 40 }

            // Action buttons
            ColumnLayout {
                Layout.alignment: Qt.AlignHCenter
                spacing: 12

                Button {
                    id: launchBtn
                    Layout.alignment: Qt.AlignHCenter
                    Layout.preferredWidth: 240
                    Layout.preferredHeight: 48
                    text: "Launch Desktop"
                    font.pixelSize: 15
                    font.bold: true

                    background: Rectangle {
                        radius: 24
                        color: launchBtn.down ? Qt.darker(donePage.primaryColor, 1.2) :
                               launchBtn.hovered ? Qt.lighter(donePage.primaryColor, 1.1) :
                               donePage.primaryColor

                        Behavior on color { ColorAnimation { duration: 200 } }
                    }

                    contentItem: Text {
                        text: launchBtn.text
                        font: launchBtn.font
                        color: "#1a1a2e"
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }

                    onClicked: {
                        backend.launchDesktop();
                        Qt.quit();
                    }

                    SequentialAnimation on opacity {
                        running: true
                        NumberAnimation { from: 0; to: 1; duration: 400; delay: 600 }
                    }
                }

                RowLayout {
                    Layout.alignment: Qt.AlignHCenter
                    spacing: 12

                    Button {
                        id: controlCenterBtn
                        Layout.preferredWidth: 180
                        Layout.preferredHeight: 40
                        text: "Open Control Center"
                        font.pixelSize: 13

                        background: Rectangle {
                            radius: 20
                            color: controlCenterBtn.down ? Qt.rgba(1,1,1,0.1) :
                                   controlCenterBtn.hovered ? Qt.rgba(1,1,1,0.05) : "transparent"
                            border.color: Qt.rgba(1,1,1,0.2)
                            border.width: 1
                        }

                        contentItem: Text {
                            text: controlCenterBtn.text
                            font: controlCenterBtn.font
                            color: donePage.textPrimary
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                        }

                        onClicked: {
                            backend.openControlCenter();
                        }

                        SequentialAnimation on opacity {
                            running: true
                            NumberAnimation { from: 0; to: 1; duration: 400; delay: 700 }
                        }
                    }

                    Button {
                        id: startFreshBtn
                        Layout.preferredWidth: 140
                        Layout.preferredHeight: 40
                        text: "Start Fresh"
                        font.pixelSize: 13

                        background: Rectangle {
                            radius: 20
                            color: startFreshBtn.down ? Qt.rgba(1,0.322,0.322,0.1) :
                                   startFreshBtn.hovered ? Qt.rgba(1,0.322,0.322,0.05) : "transparent"
                            border.color: Qt.rgba(1,0.322,0.322,0.2)
                            border.width: 1
                        }

                        contentItem: Text {
                            text: startFreshBtn.text
                            font: startFreshBtn.font
                            color: "#ff5252"
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                        }

                        onClicked: {
                            Qt.quit();
                        }

                        SequentialAnimation on opacity {
                            running: true
                            NumberAnimation { from: 0; to: 1; duration: 400; delay: 800 }
                        }
                    }
                }
            }

            Item { Layout.fillHeight: true }
        }
    }
}
