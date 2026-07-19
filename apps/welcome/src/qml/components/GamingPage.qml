import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import Backend 1.0

Page {
    id: gamingPage

    property Backend backend
    property color primaryColor: "#4CC9F0"
    property color secondaryColor: "#7B2CBF"
    property color surfaceColor: "#16213e"
    property color cardColor: "#0f3460"
    property color textPrimary: "#e0e0e0"
    property color textSecondary: "#a0a0b0"

    background: Rectangle { color: "transparent" }

    property bool isInstalling: false
    property var selectedPackages: []
    property var packageStatus: ({})

    property var gamingTools: [
        { id: "steam", name: "Steam", desc: "The largest PC gaming platform", icon: "🎮", defaultSelected: true },
        { id: "gamemode", name: "Gamemode", desc: "Optimize system performance for gaming", icon: "⚡", defaultSelected: true },
        { id: "mangohud", name: "MangoHud", desc: "GPU/CPU performance overlay", icon: "📊", defaultSelected: true },
        { id: "lib32-mesa", name: "32-bit Libraries", desc: "Compatibility for 32-bit games", icon: "🔧", defaultSelected: true },
        { id: "wine", name: "Wine", desc: "Run Windows applications", icon: "🍷", defaultSelected: false },
        { id: "lutris", name: "Lutris", desc: "Open gaming platform", icon: "🕹️", defaultSelected: false }
    ]

    Component.onCompleted: {
        var sel = [];
        for (var i = 0; i < gamingTools.length; i++) {
            if (gamingTools[i].defaultSelected) {
                sel.push(gamingTools[i].id);
            }
        }
        gamingPage.selectedPackages = sel;
    }

    Connections {
        target: backend
        function onPackageInstalled(packageName, success, message) {
            var newStatus = gamingPage.packageStatus;
            newStatus[packageName] = success ? "done" : "error";
            gamingPage.packageStatus = newStatus;
        }
    }

    PageCard {
        anchors.fill: parent
        title: "Gaming Setup"
        description: "Install tools and drivers for the best gaming experience"
        primaryColor: gamingPage.primaryColor
        surfaceColor: gamingPage.surfaceColor
        cardColor: gamingPage.cardColor
        textPrimary: gamingPage.textPrimary
        textSecondary: gamingPage.textSecondary

        ColumnLayout {
            anchors.fill: parent
            anchors.topMargin: 80
            spacing: 12

            ScrollView {
                Layout.fillWidth: true
                Layout.fillHeight: true
                clip: true

                ColumnLayout {
                    width: parent.width
                    spacing: 8

                    Repeater {
                        model: gamingPage.gamingTools

                        Rectangle {
                            Layout.fillWidth: true
                            Layout.preferredHeight: 64
                            radius: 10
                            color: checkbox.checked ?
                                   Qt.rgba(0.298, 0.788, 0.941, 0.08) :
                                   hoverHandler.hovered ? Qt.rgba(1,1,1,0.03) : "transparent"
                            border.color: checkbox.checked ? Qt.rgba(0.298, 0.788, 0.941, 0.25) : Qt.rgba(1,1,1,0.05)
                            border.width: 1

                            HoverHandler { id: hoverHandler }

                            RowLayout {
                                anchors.fill: parent
                                anchors.leftMargin: 16
                                anchors.rightMargin: 16
                                spacing: 14

                                CheckBox {
                                    id: checkbox
                                    checked: modelData.defaultSelected
                                    enabled: !gamingPage.isInstalling

                                    indicator: Rectangle {
                                        width: 20
                                        height: 20
                                        radius: 5
                                        color: checkbox.checked ? gamingPage.primaryColor : "transparent"
                                        border.color: checkbox.checked ? gamingPage.primaryColor : Qt.rgba(1,1,1,0.2)
                                        border.width: 1.5

                                        Text {
                                            anchors.centerIn: parent
                                            text: "✓"
                                            font.pixelSize: 12
                                            font.bold: true
                                            color: "#1a1a2e"
                                            visible: checkbox.checked
                                        }

                                        Behavior on color { ColorAnimation { duration: 150 } }
                                    }

                                    onCheckedChanged: {
                                        var sel = gamingPage.selectedPackages.slice();
                                        if (checked) {
                                            if (sel.indexOf(modelData.id) === -1) sel.push(modelData.id);
                                        } else {
                                            sel = sel.filter(function(p) { return p !== modelData.id; });
                                        }
                                        gamingPage.selectedPackages = sel;
                                    }
                                }

                                Text {
                                    text: modelData.icon
                                    font.pixelSize: 22
                                }

                                ColumnLayout {
                                    spacing: 2
                                    Layout.fillWidth: true

                                    Text {
                                        text: modelData.name
                                        font.pixelSize: 14
                                        font.bold: true
                                        color: gamingPage.textPrimary
                                    }

                                    Text {
                                        text: modelData.desc
                                        font.pixelSize: 11
                                        color: gamingPage.textSecondary
                                    }
                                }

                                // Status indicator
                                Rectangle {
                                    width: 24
                                    height: 24
                                    radius: 12
                                    visible: gamingPage.packageStatus[modelData.id] !== undefined
                                    color: gamingPage.packageStatus[modelData.id] === "done" ? Qt.rgba(0, 0.902, 0.463, 0.2) :
                                           Qt.rgba(1, 0.322, 0.322, 0.2)

                                    Text {
                                        anchors.centerIn: parent
                                        text: gamingPage.packageStatus[modelData.id] === "done" ? "✓" : "✗"
                                        font.pixelSize: 12
                                        color: gamingPage.packageStatus[modelData.id] === "done" ? "#00e676" : "#ff5252"
                                    }
                                }
                            }
                        }
                    }
                }
            }

            // Install button
            Button {
                id: installBtn
                Layout.alignment: Qt.AlignHCenter
                Layout.preferredWidth: 240
                Layout.preferredHeight: 44
                text: gamingPage.isInstalling ? "Installing..." : "Install Gaming Tools"
                font.pixelSize: 14
                font.bold: true
                enabled: !gamingPage.isInstalling && gamingPage.selectedPackages.length > 0

                background: Rectangle {
                    radius: 22
                    color: installBtn.down ? Qt.darker(gamingPage.secondaryColor, 1.2) :
                           installBtn.hovered ? Qt.lighter(gamingPage.secondaryColor, 1.1) :
                           gamingPage.secondaryColor
                    opacity: installBtn.enabled ? 1.0 : 0.5

                    Behavior on color { ColorAnimation { duration: 200 } }
                }

                contentItem: Text {
                    text: installBtn.text
                    font: installBtn.font
                    color: "#ffffff"
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }

                onClicked: {
                    gamingPage.isInstalling = true;
                    backend.installPackages(gamingPage.selectedPackages);
                }
            }

            BusyIndicator {
                Layout.alignment: Qt.AlignHCenter
                width: 20
                height: 20
                running: gamingPage.isInstalling
                visible: gamingPage.isInstalling
            }

            Text {
                Layout.alignment: Qt.AlignHCenter
                text: gamingPage.selectedPackages.length + " package" + (gamingPage.selectedPackages.length !== 1 ? "s" : "") + " selected"
                font.pixelSize: 12
                color: gamingPage.textSecondary
            }
        }
    }
}
