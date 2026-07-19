import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import Backend 1.0

Page {
    id: developerPage

    property Backend backend
    property color primaryColor: "#4CC9F0"
    property color secondaryColor: "#7B2CBF"
    property color surfaceColor: "#16213e"
    property color cardColor: "#0f3460"
    property color textPrimary: "#e0e0e0"
    property color textSecondary: "#a0a0b0"

    background: Rectangle { color: "transparent" }

    property var packageStatus: ({})

    property var devTools: [
        { id: "git", name: "Git", desc: "Version control system", icon: "📦" },
        { id: "docker", name: "Docker", desc: "Container platform", icon: "🐳" },
        { id: "python", name: "Python", desc: "Python programming language", icon: "🐍" },
        { id: "nodejs", name: "Node.js", desc: "JavaScript runtime", icon: "🟢" },
        { id: "rust", name: "Rust", desc: "Systems programming language", icon: "🦀" },
        { id: "gcc", name: "GCC", desc: "GNU Compiler Collection", icon: "⚙️" },
        { id: "clang", name: "Clang", desc: "LLVM C/C++ compiler", icon: "🔧" },
        { id: "jdk", name: "Java JDK", desc: "Java Development Kit", icon: "☕" }
    ]

    Connections {
        target: backend
        function onPackageInstalled(packageName, success, message) {
            var newStatus = developerPage.packageStatus;
            newStatus[packageName] = success ? "done" : "error";
            developerPage.packageStatus = newStatus;
        }
    }

    PageCard {
        anchors.fill: parent
        title: "Developer Tools"
        description: "Set up your development environment"
        primaryColor: developerPage.primaryColor
        surfaceColor: developerPage.surfaceColor
        cardColor: developerPage.cardColor
        textPrimary: developerPage.textPrimary
        textSecondary: developerPage.textSecondary

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
                    spacing: 6

                    Repeater {
                        model: developerPage.devTools

                        Rectangle {
                            Layout.fillWidth: true
                            Layout.preferredHeight: 60
                            radius: 10
                            color: hoverHandler.hovered ? Qt.rgba(1,1,1,0.03) : "transparent"
                            border.color: Qt.rgba(1,1,1,0.05)
                            border.width: 1

                            HoverHandler { id: hoverHandler }

                            RowLayout {
                                anchors.fill: parent
                                anchors.leftMargin: 16
                                anchors.rightMargin: 16
                                spacing: 14

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
                                        color: developerPage.textPrimary
                                    }

                                    Text {
                                        text: modelData.desc
                                        font.pixelSize: 11
                                        color: developerPage.textSecondary
                                    }
                                }

                                // Status
                                Rectangle {
                                    width: 24
                                    height: 24
                                    radius: 12
                                    visible: developerPage.packageStatus[modelData.id] !== undefined
                                    color: developerPage.packageStatus[modelData.id] === "done" ? Qt.rgba(0, 0.902, 0.463, 0.2) :
                                           Qt.rgba(1, 0.322, 0.322, 0.2)

                                    Text {
                                        anchors.centerIn: parent
                                        text: developerPage.packageStatus[modelData.id] === "done" ? "✓" : "✗"
                                        font.pixelSize: 12
                                        color: developerPage.packageStatus[modelData.id] === "done" ? "#00e676" : "#ff5252"
                                    }
                                }

                                // Individual install button
                                Button {
                                    text: developerPage.packageStatus[modelData.id] === "done" ? "Installed" :
                                          developerPage.packageStatus[modelData.id] === "error" ? "Retry" : "Install"
                                    font.pixelSize: 11
                                    enabled: developerPage.packageStatus[modelData.id] !== "done"

                                    background: Rectangle {
                                        radius: 14
                                        color: parent.down ? Qt.darker(developerPage.primaryColor, 1.2) :
                                               parent.hovered ? Qt.lighter(developerPage.primaryColor, 1.1) :
                                               Qt.rgba(0.298, 0.788, 0.941, 0.15)
                                        border.color: developerPage.packageStatus[modelData.id] === "error" ? "#ff5252" : Qt.rgba(0.298, 0.788, 0.941, 0.3)
                                        border.width: 1
                                    }

                                    contentItem: Text {
                                        text: parent.text
                                        font: parent.font
                                        color: developerPage.primaryColor
                                        horizontalAlignment: Text.AlignHCenter
                                        verticalAlignment: Text.AlignVCenter
                                    }

                                    onClicked: {
                                        var pkgMap = {
                                            "git": "git",
                                            "docker": "docker",
                                            "python": "python",
                                            "nodejs": "nodejs",
                                            "rust": "rust",
                                            "gcc": "gcc",
                                            "clang": "clang",
                                            "jdk": "jdk-openjdk"
                                        };
                                        backend.installPackage(pkgMap[modelData.id] || modelData.id);
                                    }
                                }
                            }
                        }
                    }
                }
            }

            // Install All button
            Button {
                id: installAllBtn
                Layout.alignment: Qt.AlignHCenter
                Layout.preferredWidth: 220
                Layout.preferredHeight: 44
                text: "Install All Selected"
                font.pixelSize: 14
                font.bold: true

                background: Rectangle {
                    radius: 22
                    color: installAllBtn.down ? Qt.darker(developerPage.secondaryColor, 1.2) :
                           installAllBtn.hovered ? Qt.lighter(developerPage.secondaryColor, 1.1) :
                           developerPage.secondaryColor
                }

                contentItem: Text {
                    text: installAllBtn.text
                    font: installAllBtn.font
                    color: "#ffffff"
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }

                onClicked: {
                    var packages = [];
                    for (var i = 0; i < developerPage.devTools.length; i++) {
                        var tool = developerPage.devTools[i];
                        if (developerPage.packageStatus[tool.id] !== "done") {
                            var pkgMap = {
                                "git": "git", "docker": "docker", "python": "python",
                                "nodejs": "nodejs", "rust": "rust", "gcc": "gcc",
                                "clang": "clang", "jdk": "jdk-openjdk"
                            };
                            packages.push(pkgMap[tool.id] || tool.id);
                        }
                    }
                    if (packages.length > 0) {
                        backend.installPackages(packages);
                    }
                }
            }
        }
    }
}
