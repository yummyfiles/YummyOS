import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import org.kde.plasma.core 2.0 as PlasmaCore

Rectangle {
    id: root
    width: Screen.width
    height: Screen.height
    color: "#0a0a1a"

    // Background gradient overlay
    Rectangle {
        anchors.fill: parent
        color: "#0a0a1a"

        // Subtle radial glow
        Rectangle {
            x: parent.width * 0.5 - 400
            y: parent.height * 0.4 - 400
            width: 800
            height: 800
            radius: 400
            gradient: Gradient {
                orientation: Gradient.Horizontal
                GradientStop { position: 0.0; color: Qt.rgba(0.3, 0.75, 0.94, 0.08) }
                GradientStop { position: 0.5; color: Qt.rgba(0.48, 0.17, 0.75, 0.04) }
                GradientStop { position: 1.0; color: "transparent" }
            }
            opacity: 0.0

            NumberAnimation on opacity {
                id: glowFadeIn
                from: 0.0; to: 1.0
                duration: 2000
                easing.type: Easing.InOutCubic
                running: true
            }
        }
    }

    // Main content container with fade-in
    Item {
        id: mainContent
        anchors.fill: parent
        opacity: 0.0

        Behavior on opacity {
            NumberAnimation { duration: 800; easing.type: Easing.InOutCubic }
        }

        Component.onCompleted: {
            opacity = 1.0;
        }

        // Clock at the top
        Clock {
            id: clockWidget
            anchors {
                top: parent.top
                topMargin: parent.height * 0.08
                horizontalCenter: parent.horizontalCenter
            }
            textColor: "#FFFFFF"
        }

        // Login form centered
        LoginForm {
            id: loginForm
            anchors.centerIn: parent

            primaryAccent: "#4CC9F0"
            secondaryAccent: "#7B2CBF"

            onLoginRequested: function(userName, password) {
                loginForm.setProcessing(true);
                sddm.login(userName, password, -1);
            }

            onShutdownRequested: {
                sddm.powerOff();
            }

            onRestartRequested: {
                sddm.reboot();
            }

            onSuspendRequested: {
                sddm.suspend();
            }

            onLayoutSwitchRequested: {
                // SDDM keyboard layout switching handled externally
            }
        }

        // Bottom text
        Text {
            anchors {
                bottom: parent.bottom
                bottomMargin: 24
                horizontalCenter: parent.horizontalCenter
            }
            font {
                family: "Noto Sans, Segoe UI, sans-serif"
                pixelSize: 13
            }
            color: Qt.rgba(1, 1, 1, 0.3)
            text: "YummyOS"
        }
    }

    // Load users from SDDM's user list model
    Component.onCompleted: {
        if (typeof userListModel !== "undefined" && userListModel.count > 0) {
            var defaultUser = userListModel.get(0);
            loginForm.setUsername(defaultUser.name || defaultUser.realName || defaultUser.user);
        }
    }

    // SDDM connections
    Connections {
        target: sddm

        function onLoginFailed() {
            loginForm.shake();
            loginForm.clearPassword();
        }

        function onLoginSucceeded() {
            loginForm.setProcessing(false);
        }

        function onUserChanged(user) {
            loginForm.setUsername(user);
        }
    }

    // Keyboard shortcuts
    Item {
        focus: true
        Keys.onPressed: {
            if (event.key === Qt.Key_Escape) {
                loginForm.clearPassword();
            }
        }
    }
}
