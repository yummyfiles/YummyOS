import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import Backend 1.0

Page {
    id: appearancePage

    property Backend backend
    property color primaryColor: "#4CC9F0"
    property color secondaryColor: "#7B2CBF"
    property color surfaceColor: "#16213e"
    property color cardColor: "#0f3460"
    property color textPrimary: "#e0e0e0"
    property color textSecondary: "#a0a0b0"

    background: Rectangle { color: "transparent" }

    property string selectedTheme: "dark"
    property string selectedAccent: "#4CC9F0"

    property var accentColors: [
        { color: "#4CC9F0", name: "Sky Blue" },
        { color: "#7B2CBF", name: "Purple" },
        { color: "#f72585", name: "Pink" },
        { color: "#ff5252", name: "Red" },
        { color: "#ff9100", name: "Orange" },
        { color: "#ffc107", name: "Amber" },
        { color: "#00e676", name: "Green" },
        { color: "#00bcd4", name: "Teal" },
        { color: "#2196f3", name: "Blue" },
        { color: "#e91e63", name: "Rose" },
        { color: "#9c27b0", name: "Violet" },
        { color: "#607d8b", name: "Slate" }
    ]

    PageCard {
        anchors.fill: parent
        title: "Appearance"
        description: "Customize the look and feel of YummyOS"
        primaryColor: appearancePage.primaryColor
        surfaceColor: appearancePage.surfaceColor
        cardColor: appearancePage.cardColor
        textPrimary: appearancePage.textPrimary
        textSecondary: appearancePage.textSecondary

        ColumnLayout {
            anchors.fill: parent
            anchors.topMargin: 80
            spacing: 24

            // Theme selector
            ColumnLayout {
                Layout.fillWidth: true
                spacing: 12

                Text {
                    text: "Theme"
                    font.pixelSize: 15
                    font.bold: true
                    color: appearancePage.textPrimary
                }

                RowLayout {
                    spacing: 12

                    Repeater {
                        model: [
                            { id: "dark", name: "Dark", preview: "#1a1a2e", textCol: "#e0e0e0" },
                            { id: "light", name: "Light", preview: "#f5f5f5", textCol: "#333333" },
                            { id: "system", name: "System", preview: "linear", textCol: "#e0e0e0" }
                        ]

                        Rectangle {
                            Layout.preferredWidth: 120
                            Layout.preferredHeight: 80
                            radius: 12
                            color: modelData.preview === "linear" ?
                                   Qt.rgba(0.102, 0.102, 0.18, 1) : modelData.preview
                            border.color: appearancePage.selectedTheme === modelData.id ?
                                          appearancePage.primaryColor : Qt.rgba(1,1,1,0.1)
                            border.width: appearancePage.selectedTheme === modelData.id ? 2 : 1

                            Behavior on border.color { ColorAnimation { duration: 200 } }

                            ColumnLayout {
                                anchors.centerIn: parent
                                spacing: 6

                                // Mini preview
                                Rectangle {
                                    Layout.alignment: Qt.AlignHCenter
                                    width: 60
                                    height: 36
                                    radius: 6
                                    color: modelData.preview === "linear" ? "#2a2a4e" : Qt.lighter(modelData.preview, 1.1)

                                    Rectangle {
                                        anchors.top: parent.top
                                        anchors.left: parent.left
                                        anchors.margins: 4
                                        width: 16
                                        height: 3
                                        radius: 1.5
                                        color: modelData.textCol
                                        opacity: 0.5
                                    }
                                    Rectangle {
                                        anchors.top: parent.top
                                        anchors.left: parent.left
                                        anchors.margins: 4
                                        anchors.topMargin: 10
                                        width: 30
                                        height: 2
                                        radius: 1
                                        color: modelData.textCol
                                        opacity: 0.3
                                    }
                                    Rectangle {
                                        anchors.bottom: parent.bottom
                                        anchors.left: parent.left
                                        anchors.margins: 4
                                        width: 40
                                        height: 6
                                        radius: 3
                                        color: appearancePage.primaryColor
                                        opacity: 0.6
                                    }
                                }

                                Text {
                                    text: modelData.name
                                    font.pixelSize: 12
                                    font.bold: appearancePage.selectedTheme === modelData.id
                                    color: appearancePage.selectedTheme === modelData.id ?
                                           appearancePage.primaryColor : appearancePage.textPrimary
                                    Layout.alignment: Qt.AlignHCenter
                                }
                            }

                            MouseArea {
                                anchors.fill: parent
                                cursorShape: Qt.PointingHandCursor
                                onClicked: {
                                    appearancePage.selectedTheme = modelData.id;
                                    backend.applyTheme(modelData.id);
                                }
                            }
                        }
                    }
                }
            }

            // Accent color
            ColumnLayout {
                Layout.fillWidth: true
                spacing: 12

                Text {
                    text: "Accent Color"
                    font.pixelSize: 15
                    font.bold: true
                    color: appearancePage.textPrimary
                }

                Flow {
                    Layout.fillWidth: true
                    spacing: 10

                    Repeater {
                        model: appearancePage.accentColors

                        Rectangle {
                            width: 48
                            height: 48
                            radius: 24
                            color: modelData.color
                            border.color: appearancePage.selectedAccent === modelData.color ?
                                          "#ffffff" : "transparent"
                            border.width: appearancePage.selectedAccent === modelData.color ? 3 : 0

                            Behavior on border.width { NumberAnimation { duration: 200 } }

                            scale: appearancePage.selectedAccent === modelData.color ? 1.15 : 1.0
                            Behavior on scale { NumberAnimation { duration: 200; easing.type: Easing.OutBack } }

                            Text {
                                anchors.centerIn: parent
                                text: "✓"
                                font.pixelSize: 16
                                font.bold: true
                                color: "#ffffff"
                                visible: appearancePage.selectedAccent === modelData.color
                                opacity: 0
                                Behavior on opacity { NumberAnimation { duration: 200 } }
                                opacity: appearancePage.selectedAccent === modelData.color ? 1.0 : 0.0
                            }

                            MouseArea {
                                anchors.fill: parent
                                cursorShape: Qt.PointingHandCursor
                                onClicked: {
                                    appearancePage.selectedAccent = modelData.color;
                                    backend.setAccentColor(modelData.color);
                                }
                            }

                            ToolTip {
                                text: modelData.name
                                visible: parentHover.hovered
                            }

                            HoverHandler { id: parentHover }
                        }
                    }
                }
            }

            // Wallpaper preview area
            ColumnLayout {
                Layout.fillWidth: true
                spacing: 12

                Text {
                    text: "Wallpaper"
                    font.pixelSize: 15
                    font.bold: true
                    color: appearancePage.textPrimary
                }

                Rectangle {
                    Layout.fillWidth: true
                    Layout.preferredHeight: 120
                    radius: 12
                    color: Qt.rgba(1,1,1,0.03)
                    border.color: Qt.rgba(1,1,1,0.08)
                    border.width: 1

                    RowLayout {
                        anchors.fill: parent
                        anchors.margins: 16
                        spacing: 12

                        // Preview thumbnails
                        Repeater {
                            model: [
                                { name: "YummyOS Default", color: "#1a1a2e" },
                                { name: "Gradient Blue", color: "#0f3460" },
                                { name: "Gradient Purple", color: "#7B2CBF" },
                                { name: "Midnight", color: "#0d0d1a" }
                            ]

                            Rectangle {
                                Layout.preferredWidth: 80
                                Layout.preferredHeight: 60
                                radius: 8
                                color: modelData.color

                                gradient: Gradient {
                                    GradientStop { position: 0.0; color: Qt.lighter(modelData.color, 1.3) }
                                    GradientStop { position: 1.0; color: Qt.darker(modelData.color, 1.2) }
                                }

                                border.color: index === 0 ? appearancePage.primaryColor : "transparent"
                                border.width: 1

                                ColumnLayout {
                                    anchors.fill: parent
                                    anchors.margins: 6

                                    Text {
                                        text: modelData.name
                                        font.pixelSize: 8
                                        color: "#ffffff"
                                        opacity: 0.8
                                        Layout.fillWidth: true
                                        elide: Text.ElideRight
                                    }
                                }

                                MouseArea {
                                    anchors.fill: parent
                                    cursorShape: Qt.PointingHandCursor
                                    onClicked: {
                                        backend.setWallpaper("/usr/share/wallpapers/" + modelData.name.replace(" ", "_").toLowerCase());
                                    }
                                }
                            }
                        }

                        Item { Layout.fillWidth: true }
                    }
                }
            }

            Item { Layout.fillHeight: true }
        }
    }
}
