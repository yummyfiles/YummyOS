import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import Backend 1.0

Page {
    id: networkPage

    property Backend backend
    property color primaryColor: "#4CC9F0"
    property color secondaryColor: "#7B2CBF"
    property color surfaceColor: "#16213e"
    property color cardColor: "#0f3460"
    property color textPrimary: "#e0e0e0"
    property color textSecondary: "#a0a0b0"

    background: Rectangle { color: "transparent" }

    property var networks: []
    property string selectedSsid: ""
    property bool isConnecting: false
    property string connectionStatus: ""

    Connections {
        target: backend
        function onWifiNetworksFound(networks) {
            networkPage.networks = networks;
            scanningLabel.visible = false;
        }
        function onCurrentConnectionChanged() {
            networkPage.connectionStatus = backend.currentConnection;
            networkPage.isConnecting = false;
            statusLabel.text = "Connected to: " + backend.currentConnection;
            statusLabel.color = "#00e676";
        }
        function onCommandRunningChanged() {
            if (!backend.commandRunning && networkPage.isConnecting) {
                if (networkPage.connectionStatus === "") {
                    statusLabel.text = "Connection failed. Check password and try again.";
                    statusLabel.color = "#ff5252";
                    networkPage.isConnecting = false;
                }
            }
        }
    }

    Component.onCompleted: {
        backend.scanWifiNetworks();
        scanningLabel.visible = true;
    }

    PageCard {
        anchors.fill: parent
        title: "Network Setup"
        description: "Connect to a Wi-Fi network to get online"
        primaryColor: networkPage.primaryColor
        surfaceColor: networkPage.surfaceColor
        cardColor: networkPage.cardColor
        textPrimary: networkPage.textPrimary
        textSecondary: networkPage.textSecondary

        ColumnLayout {
            anchors.fill: parent
            anchors.topMargin: 80
            spacing: 16

            // Connection status
            Rectangle {
                Layout.fillWidth: true
                Layout.preferredHeight: 44
                radius: 10
                color: networkPage.connectionStatus !== "" ? Qt.rgba(0, 0.902, 0.463, 0.1) : Qt.rgba(1, 0.757, 0.027, 0.1)
                border.color: networkPage.connectionStatus !== "" ? Qt.rgba(0, 0.902, 0.463, 0.3) : Qt.rgba(1, 0.757, 0.027, 0.3)
                border.width: 1

                RowLayout {
                    anchors.fill: parent
                    anchors.leftMargin: 16
                    anchors.rightMargin: 16

                    Rectangle {
                        width: 8
                        height: 8
                        radius: 4
                        color: networkPage.connectionStatus !== "" ? "#00e676" : "#ffc107"

                        SequentialAnimation on opacity {
                            loops: Animation.Infinite
                            NumberAnimation { to: 0.4; duration: 800 }
                            NumberAnimation { to: 1.0; duration: 800 }
                        }
                    }

                    Text {
                        id: statusLabel
                        text: networkPage.connectionStatus !== "" ? "Connected to: " + networkPage.connectionStatus : "No connection"
                        font.pixelSize: 13
                        color: networkPage.connectionStatus !== "" ? "#00e676" : "#ffc107"
                    }

                    Item { Layout.fillWidth: true }

                    Text {
                        text: "Wi-Fi"
                        font.pixelSize: 12
                        color: networkPage.textSecondary
                    }
                }
            }

            // Scanning indicator
            RowLayout {
                id: scanningLabel
                spacing: 8
                visible: false

                BusyIndicator {
                    width: 16
                    height: 16
                    running: scanningLabel.visible
                }

                Text {
                    text: "Scanning for networks..."
                    font.pixelSize: 13
                    color: networkPage.textSecondary
                }
            }

            // Network list
            ScrollView {
                Layout.fillWidth: true
                Layout.fillHeight: true
                clip: true

                ListView {
                    id: networkList
                    model: networkPage.networks
                    spacing: 6

                    delegate: Rectangle {
                        width: networkList.width
                        height: 56
                        radius: 10
                        color: index === networkPage.selectedIndex ?
                               Qt.rgba(0.298, 0.788, 0.941, 0.15) :
                               networkHover.hovered ? Qt.rgba(1,1,1,0.03) : "transparent"
                        border.color: index === networkPage.selectedIndex ?
                                      Qt.rgba(0.298, 0.788, 0.941, 0.4) : "transparent"
                        border.width: 1

                        property int selectedIndex: -1

                        HoverHandler {
                            id: networkHover
                        }

                        RowLayout {
                            anchors.fill: parent
                            anchors.leftMargin: 16
                            anchors.rightMargin: 16
                            spacing: 12

                            // Signal strength icon
                            Text {
                                text: modelData.signal > 75 ? "⦿" : modelData.signal > 40 ? "⦿" : "⦿"
                                font.pixelSize: 20
                                color: modelData.signal > 75 ? "#00e676" :
                                       modelData.signal > 40 ? "#ffc107" : "#ff5252"
                                opacity: modelData.signal > 75 ? 1.0 : modelData.signal > 40 ? 0.8 : 0.6
                            }

                            ColumnLayout {
                                spacing: 2
                                Layout.fillWidth: true

                                Text {
                                    text: modelData.ssid || "Hidden Network"
                                    font.pixelSize: 14
                                    font.bold: true
                                    color: networkPage.textPrimary
                                    elide: Text.ElideRight
                                }

                                Text {
                                    text: modelData.security + " • Signal: " + modelData.signal + "%"
                                    font.pixelSize: 11
                                    color: networkPage.textSecondary
                                }
                            }

                            // Signal bars
                            Row {
                                spacing: 2
                                Layout.alignment: Qt.AlignVCenter

                                Repeater {
                                    model: 4
                                    Rectangle {
                                        width: 3
                                        height: 6 + index * 4
                                        radius: 1
                                        color: index < Math.ceil(modelData.signal / 25) ?
                                               networkPage.primaryColor : Qt.rgba(1,1,1,0.1)
                                    }
                                }
                            }

                            // Connect button
                            Button {
                                text: "Connect"
                                font.pixelSize: 11
                                visible: modelData.ssid !== networkPage.connectionStatus
                                enabled: !networkPage.isConnecting

                                background: Rectangle {
                                    radius: 14
                                    color: parent.down ? Qt.darker(networkPage.primaryColor, 1.2) :
                                           parent.hovered ? Qt.lighter(networkPage.primaryColor, 1.1) :
                                           Qt.rgba(0.298, 0.788, 0.941, 0.2)
                                }

                                contentItem: Text {
                                    text: parent.text
                                    font: parent.font
                                    color: networkPage.primaryColor
                                    horizontalAlignment: Text.AlignHCenter
                                    verticalAlignment: Text.AlignVCenter
                                }

                                onClicked: {
                                    networkPage.selectedSsid = modelData.ssid;
                                    networkPage.selectedIndex = index;
                                    if (modelData.security === "" || modelData.security === "--") {
                                        networkPage.isConnecting = true;
                                        statusLabel.text = "Connecting to " + modelData.ssid + "...";
                                        statusLabel.color = networkPage.primaryColor;
                                        backend.connectToWifi(modelData.ssid, "");
                                    } else {
                                        passwordDialog.open();
                                    }
                                }
                            }

                            Text {
                                text: "✓ Connected"
                                font.pixelSize: 11
                                color: "#00e676"
                                visible: modelData.ssid === networkPage.connectionStatus
                            }
                        }
                    }
                }
            }

            // Refresh button
            RowLayout {
                Layout.fillWidth: true

                Item { Layout.fillWidth: true }

                Button {
                    text: "↻ Refresh"
                    font.pixelSize: 12

                    background: Rectangle {
                        radius: 16
                        color: parent.down ? Qt.rgba(1,1,1,0.1) :
                               parent.hovered ? Qt.rgba(1,1,1,0.05) : "transparent"
                        border.color: Qt.rgba(1,1,1,0.15)
                        border.width: 1
                    }

                    contentItem: Text {
                        text: parent.text
                        font: parent.font
                        color: networkPage.textSecondary
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }

                    onClicked: {
                        scanningLabel.visible = true;
                        backend.scanWifiNetworks();
                    }
                }
            }
        }
    }

    // Password dialog
    Dialog {
        id: passwordDialog
        parent: Overlay.overlay
        anchors.centerIn: parent
        title: "Connect to " + networkPage.selectedSsid
        modal: true
        width: 380
        height: 200
        closePolicy: Popup.CloseOnEscape

        background: Rectangle {
            color: networkPage.cardColor
            radius: 16
            border.color: Qt.rgba(0.298, 0.788, 0.941, 0.2)
            border.width: 1
        }

        contentItem: ColumnLayout {
            spacing: 16

            Text {
                text: "Enter the Wi-Fi password for \"" + networkPage.selectedSsid + "\""
                font.pixelSize: 13
                color: networkPage.textSecondary
                wrapMode: Text.WordWrap
            }

            TextField {
                id: passwordField
                Layout.fillWidth: true
                Layout.preferredHeight: 40
                placeholderText: "Password"
                echoMode: TextInput.Password
                color: networkPage.textPrimary
                placeholderTextColor: Qt.rgba(1,1,1,0.3)
                font.pixelSize: 13

                background: Rectangle {
                    radius: 8
                    color: "transparent"
                    border.color: passwordField.activeFocus ? networkPage.primaryColor : Qt.rgba(1,1,1,0.15)
                    border.width: 1

                    Behavior on border.color { ColorAnimation { duration: 200 } }
                }

                onAccepted: connectBtn.clicked()
            }

            RowLayout {
                Layout.fillWidth: true

                Button {
                    text: "Cancel"
                    font.pixelSize: 13

                    background: Rectangle {
                        radius: 16
                        color: parent.down ? Qt.rgba(1,1,1,0.1) : "transparent"
                        border.color: Qt.rgba(1,1,1,0.15)
                        border.width: 1
                    }

                    contentItem: Text {
                        text: parent.text
                        font: parent.font
                        color: networkPage.textSecondary
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }

                    onClicked: passwordDialog.close()
                }

                Item { Layout.fillWidth: true }

                Button {
                    id: connectBtn
                    text: "Connect"
                    font.pixelSize: 13
                    font.bold: true

                    background: Rectangle {
                        radius: 16
                        color: parent.down ? Qt.darker(networkPage.primaryColor, 1.2) : networkPage.primaryColor
                    }

                    contentItem: Text {
                        text: parent.text
                        font: parent.font
                        color: "#1a1a2e"
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }

                    onClicked: {
                        networkPage.isConnecting = true;
                        statusLabel.text = "Connecting to " + networkPage.selectedSsid + "...";
                        statusLabel.color = networkPage.primaryColor;
                        backend.connectToWifi(networkPage.selectedSsid, passwordField.text);
                        passwordDialog.close();
                        passwordField.text = "";
                    }
                }
            }
        }
    }
}
