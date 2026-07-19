import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import SddmComponents 2.0

Item {
    id: loginRoot
    width: 400
    height: 420

    property alias userName: userNameLabel.text
    property string errorMessage: ""
    property bool isWrongPassword: false

    Behavior on isWrongPassword {
        SequentialAnimation {
            NumberAnimation { target: passwordField; property: "x"; to: -10; duration: 50 }
            NumberAnimation { target: passwordField; property: "x"; to: 10; duration: 50 }
            NumberAnimation { target: passwordField; property: "x"; to: -6; duration: 50 }
            NumberAnimation { target: passwordField; property: "x"; to: 6; duration: 50 }
            NumberAnimation { target: passwordField; property: "x"; to: 0; duration: 50 }
        }
    }

    ColumnLayout {
        anchors.centerIn: parent
        spacing: 20
        width: parent.width

        // User Avatar
        Item {
            Layout.alignment: Qt.AlignHCenter
            width: 96
            height: 96

            Rectangle {
                id: avatarCircle
                width: 96
                height: 96
                radius: 48
                color: "#1a1a2e"
                border.color: "#4CC9F0"
                border.width: 2
                anchors.centerIn: parent

                Text {
                    anchors.centerIn: parent
                    text: userNameLabel.text.charAt(0).toUpperCase()
                    font.family: "Segoe UI, Noto Sans, sans-serif"
                    font.pixelSize: 40
                    font.weight: Font.Light
                    color: "#4CC9F0"
                }
            }
        }

        // User Name
        Text {
            id: userNameLabel
            Layout.alignment: Qt.AlignHCenter
            text: ""
            font.family: "Segoe UI, Noto Sans, sans-serif"
            font.pixelSize: 20
            font.weight: Font.Normal
            color: "#ffffff"
        }

        // Password Field
        Rectangle {
            id: passwordField
            Layout.alignment: Qt.AlignHCenter
            Layout.fillWidth: true
            Layout.preferredWidth: 320
            height: 48
            radius: 24
            color: "#1a1a2e"
            border.color: passwordInput.activeFocus ? "#4CC9F0" : "#333344"
            border.width: passwordInput.activeFocus ? 2 : 1

            Behavior on border.color {
                ColorAnimation { duration: 200; easing.type: Easing.InOutQuad }
            }

            TextInput {
                id: passwordInput
                anchors.fill: parent
                anchors.leftMargin: 20
                anchors.rightMargin: 50
                verticalAlignment: TextInput.AlignVCenter
                font.family: "Segoe UI, Noto Sans, sans-serif"
                font.pixelSize: 16
                color: "#ffffff"
                echoMode: TextInput.Password
                clip: true

                onAccepted: {
                    sddm.login(userNameLabel.text, passwordInput.text, sessionCombo.currentValue)
                }

                KeyNavigation.forward: loginButton

                Text {
                    anchors.verticalCenter: parent.verticalCenter
                    text: "Password..."
                    font.family: "Segoe UI, Noto Sans, sans-serif"
                    font.pixelSize: 16
                    color: "#666677"
                    visible: !passwordInput.activeFocus && passwordInput.text.length === 0
                }
            }

            // Eye toggle button
            Rectangle {
                id: eyeButton
                width: 36
                height: 36
                radius: 18
                anchors.right: parent.right
                anchors.rightMargin: 6
                anchors.verticalCenter: parent.verticalCenter
                color: eyeMouseArea.containsMouse ? "#2a2a3e" : "transparent"

                Text {
                    anchors.centerIn: parent
                    text: passwordInput.echoMode === TextInput.Password ? "\uD83D\uDD0D" : "\uD83D\uDD10"
                    font.pixelSize: 16
                    color: "#666677"
                }

                MouseArea {
                    id: eyeMouseArea
                    anchors.fill: parent
                    hoverEnabled: true
                    onClicked: {
                        passwordInput.echoMode = passwordInput.echoMode === TextInput.Password ? TextInput.Normal : TextInput.Password
                    }
                }
            }
        }

        // Error Message
        Text {
            id: errorLabel
            Layout.alignment: Qt.AlignHCenter
            text: loginRoot.errorMessage
            font.family: "Segoe UI, Noto Sans, sans-serif"
            font.pixelSize: 13
            color: "#ff6b6b"
            visible: loginRoot.errorMessage.length > 0
            opacity: visible ? 1 : 0

            Behavior on opacity {
                NumberAnimation { duration: 200 }
            }
        }

        // Login Button
        Rectangle {
            id: loginButton
            Layout.alignment: Qt.AlignHCenter
            Layout.fillWidth: true
            Layout.preferredWidth: 320
            height: 48
            radius: 24
            color: loginMouseArea.containsMouse ? "#5cd5ff" : "#4CC9F0"
            opacity: loginMouseArea.containsMouse ? 1 : 0.9

            Behavior on color {
                ColorAnimation { duration: 200; easing.type: Easing.InOutQuad }
            }

            Text {
                anchors.centerIn: parent
                text: "Login"
                font.family: "Segoe UI, Noto Sans, sans-serif"
                font.pixelSize: 16
                font.weight: Font.DemiBold
                color: "#0a0a1a"
            }

            MouseArea {
                id: loginMouseArea
                anchors.fill: parent
                hoverEnabled: true
                cursorShape: Qt.PointingHandCursor
                onClicked: {
                    sddm.login(userNameLabel.text, passwordInput.text, sessionCombo.currentValue)
                }
            }

            KeyNavigation.forward: sessionCombo
        }

        // Session Selector and Controls Row
        RowLayout {
            Layout.alignment: Qt.AlignHCenter
            Layout.fillWidth: true
            Layout.preferredWidth: 320
            spacing: 12

            ComboBox {
                id: sessionCombo
                Layout.fillWidth: true
                height: 36
                model: sessionModel
                textRole: "name"
                currentIndex: model.lastIndex

                background: Rectangle {
                    radius: 18
                    color: "#1a1a2e"
                    border.color: sessionCombo.activeFocus ? "#4CC9F0" : "#333344"
                    border.width: sessionCombo.activeFocus ? 2 : 1

                    Behavior on border.color {
                        ColorAnimation { duration: 200 }
                    }
                }

                contentItem: Text {
                    leftPadding: 16
                    text: sessionCombo.displayText
                    font.family: "Segoe UI, Noto Sans, sans-serif"
                    font.pixelSize: 13
                    color: "#aaaaaa"
                    verticalAlignment: Text.AlignVCenter
                    elide: Text.ElideRight
                }

                indicator: Text {
                    x: sessionCombo.width - width - 12
                    text: "\u25BC"
                    font.pixelSize: 10
                    color: "#666677"
                    anchors.verticalCenter: parent.verticalCenter
                }
            }

            // Shutdown
            Rectangle {
                width: 36
                height: 36
                radius: 18
                color: shutdownMouse.containsMouse ? "#ff4444" : "#1a1a2e"
                border.color: shutdownMouse.containsMouse ? "#ff4444" : "#333344"
                border.width: 1

                Behavior on color {
                    ColorAnimation { duration: 200 }
                }

                Text {
                    anchors.centerIn: parent
                    text: "\u23FB"
                    font.pixelSize: 16
                    color: shutdownMouse.containsMouse ? "#ffffff" : "#888888"
                }

                MouseArea {
                    id: shutdownMouse
                    anchors.fill: parent
                    hoverEnabled: true
                    cursorShape: Qt.PointingHandCursor
                    onClicked: sddm.powerOff()
                }
            }

            // Restart
            Rectangle {
                width: 36
                height: 36
                radius: 18
                color: restartMouse.containsMouse ? "#ff8800" : "#1a1a2e"
                border.color: restartMouse.containsMouse ? "#ff8800" : "#333344"
                border.width: 1

                Behavior on color {
                    ColorAnimation { duration: 200 }
                }

                Text {
                    anchors.centerIn: parent
                    text: "\u21BB"
                    font.pixelSize: 18
                    color: restartMouse.containsMouse ? "#ffffff" : "#888888"
                }

                MouseArea {
                    id: restartMouse
                    anchors.fill: parent
                    hoverEnabled: true
                    cursorShape: Qt.PointingHandCursor
                    onClicked: sddm.reboot()
                }
            }

            // Suspend
            Rectangle {
                width: 36
                height: 36
                radius: 18
                color: suspendMouse.containsMouse ? "#4CC9F0" : "#1a1a2e"
                border.color: suspendMouse.containsMouse ? "#4CC9F0" : "#333344"
                border.width: 1

                Behavior on color {
                    ColorAnimation { duration: 200 }
                }

                Text {
                    anchors.centerIn: parent
                    text: "\u23FE"
                    font.pixelSize: 16
                    color: suspendMouse.containsMouse ? "#0a0a1a" : "#888888"
                }

                MouseArea {
                    id: suspendMouse
                    anchors.fill: parent
                    hoverEnabled: true
                    cursorShape: Qt.PointingHandCursor
                    onClicked: sddm.suspend()
                }
            }
        }
    }

    Connections {
        target: sddm
        function onLoginFailed() {
            loginRoot.errorMessage = "Incorrect password, please try again."
            loginRoot.isWrongPassword = !loginRoot.isWrongPassword
            passwordInput.text = ""
            passwordInput.forceActiveFocus()
        }
    }
}
