import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Item {
    id: loginForm
    width: 380
    height: 480

    property string primaryAccent: "#4CC9F0"
    property string secondaryAccent: "#7B2CBF"
    property string bgColor: "#0a0a1a"
    property string surfaceColor: "#12122a"
    property string textColor: "#FFFFFF"
    property string mutedText: "#8888aa"
    property string errorColor: "#E63946"

    property bool isError: false
    property bool isProcessing: false
    property int keyboardLayoutIndex: 0
    property var keyboardLayouts: ["US", "DE", "FR", "ES", "IT", "JP"]

    property string userName: "User"

    signal loginRequested(string userName, string password)
    signal shutdownRequested()
    signal restartRequested()
    signal suspendRequested()
    signal layoutSwitchRequested()

    // Background surface
    Rectangle {
        anchors.fill: parent
        radius: 16
        color: surfaceColor
        border {
            color: Qt.rgba(1, 1, 1, 0.06)
            width: 1
        }
    }

    ColumnLayout {
        anchors {
            fill: parent
            topMargin: 0
            bottomMargin: 24
        }
        spacing: 0

        // Avatar
        Item {
            Layout.fillWidth: true
            Layout.preferredHeight: 100
            Layout.topMargin: 36

            Rectangle {
                id: avatarCircle
                anchors.centerIn: parent
                width: 80
                height: 80
                radius: 40
                color: secondaryAccent

                gradient: Gradient {
                    orientation: Gradient.Horizontal
                    GradientStop { position: 0.0; color: secondaryAccent }
                    GradientStop { position: 1.0; color: primaryAccent }
                }

                Text {
                    anchors.centerIn: parent
                    font {
                        family: "Noto Sans, Segoe UI, sans-serif"
                        pixelSize: 36
                        weight: Font.Light
                    }
                    color: "#FFFFFF"
                    text: "U"
                }
            }
        }

        // Username
        Text {
            id: userNameText
            Layout.alignment: Qt.AlignHCenter
            Layout.topMargin: 16
            font {
                family: "Noto Sans, Segoe UI, sans-serif"
                pixelSize: 22
                weight: Font.Medium
            }
            color: textColor
            text: "User"
        }

        // Password field
        Item {
            Layout.fillWidth: true
            Layout.preferredHeight: 60
            Layout.topMargin: 24
            Layout.leftMargin: 32
            Layout.rightMargin: 32

            Rectangle {
                id: passwordBg
                anchors.fill: parent
                radius: 12
                color: Qt.rgba(0, 0, 0, 0.3)
                border {
                    color: loginForm.isError ? errorColor : (passwordInput.activeFocus ? primaryAccent : Qt.rgba(1, 1, 1, 0.08))
                    width: 2
                }

                Behavior on border.color {
                    ColorAnimation { duration: 200 }
                }

                TextField {
                    id: passwordInput
                    anchors {
                        fill: parent
                        leftMargin: 16
                        rightMargin: 16
                    }
                    placeholderText: "Password"
                    placeholderTextColor: mutedText
                    color: textColor
                    echoMode: TextInput.Password
                    passwordCharacter: "●"
                    passwordMaskDelay: 600
                    font {
                        family: "Noto Sans, Segoe UI, sans-serif"
                        pixelSize: 18
                    }
                    background: null
                    verticalAlignment: TextInput.AlignVCenter
                    focus: true

                    Keys.onReturnPressed: {
                        if (!loginForm.isProcessing) {
                            loginRequested(loginForm.userName, passwordInput.text);
                        }
                    }

                    onActiveFocusChanged: {
                        if (activeFocus) {
                            loginForm.isError = false;
                        }
                    }
                }
            }

            // Shake animation
            SequentialAnimation {
                id: shakeAnimation
                running: false

                NumberAnimation {
                    target: passwordBg
                    property: "x"
                    from: 0; to: -10
                    duration: 40
                }
                NumberAnimation {
                    target: passwordBg
                    property: "x"
                    from: -10; to: 10
                    duration: 40
                }
                NumberAnimation {
                    target: passwordBg
                    property: "x"
                    from: 10; to: -8
                    duration: 40
                }
                NumberAnimation {
                    target: passwordBg
                    property: "x"
                    from: -8; to: 8
                    duration: 40
                }
                NumberAnimation {
                    target: passwordBg
                    property: "x"
                    from: 8; to: 0
                    duration: 40
                }
            }
        }

        // Login button
        Rectangle {
            id: loginButton
            Layout.fillWidth: true
            Layout.preferredHeight: 52
            Layout.topMargin: 16
            Layout.leftMargin: 32
            Layout.rightMargin: 32
            radius: 12
            color: loginForm.isProcessing ? Qt.lighter(primaryAccent, 0.7) : primaryAccent
            enabled: !loginForm.isProcessing

            gradient: Gradient {
                orientation: Gradient.Horizontal
                GradientStop { position: 0.0; color: primaryAccent }
                GradientStop { position: 1.0; color: Qt.lighter(primaryAccent, 0.85) }
            }

            Behavior on color {
                ColorAnimation { duration: 150 }
            }

            Text {
                anchors.centerIn: parent
                font {
                    family: "Noto Sans, Segoe UI, sans-serif"
                    pixelSize: 18
                    weight: Font.Medium
                }
                color: "#FFFFFF"
                text: loginForm.isProcessing ? "Signing in..." : "Sign In"
            }

            MouseArea {
                anchors.fill: parent
                hoverEnabled: true
                cursorShape: Qt.PointingHandCursor
                onClicked: {
                    if (!loginForm.isProcessing) {
                        loginRequested(loginForm.userName, passwordInput.text);
                    }
                }
                onEntered: {
                    if (!loginForm.isProcessing) {
                        loginButton.color = Qt.lighter(primaryAccent, 1.1);
                    }
                }
                onExited: {
                    if (!loginForm.isProcessing) {
                        loginButton.color = primaryAccent;
                    }
                }
            }
        }

        // Session selector
        Item {
            Layout.fillWidth: true
            Layout.preferredHeight: 40
            Layout.topMargin: 12
            Layout.leftMargin: 32
            Layout.rightMargin: 32

            RowLayout {
                anchors.centerIn: parent
                spacing: 8

                Rectangle {
                    id: sessionCombo
                    width: 180
                    height: 36
                    radius: 8
                    color: Qt.rgba(0, 0, 0, 0.2)
                    border {
                        color: Qt.rgba(1, 1, 1, 0.06)
                        width: 1
                    }

                    Text {
                        id: sessionLabel
                        anchors {
                            left: parent.left
                            leftMargin: 12
                            verticalCenter: parent.verticalCenter
                        }
                        font {
                            family: "Noto Sans, Segoe UI, sans-serif"
                            pixelSize: 14
                        }
                        color: mutedText
                        text: "Session"
                        visible: sessionText.text === ""
                    }

                    Text {
                        id: sessionText
                        anchors {
                            left: parent.left
                            leftMargin: 12
                            verticalCenter: parent.verticalCenter
                        }
                        font {
                            family: "Noto Sans, Segoe UI, sans-serif"
                            pixelSize: 14
                        }
                        color: textColor
                        text: "Plasma (Wayland)"
                    }

                    MouseArea {
                        anchors.fill: parent
                        cursorShape: Qt.PointingHandCursor
                        onClicked: sessionMenu.open();

                        ComboBox {
                            id: sessionComboControl
                            anchors.fill: parent
                            visible: false
                            model: sessionModel
                            onActivated: {
                                sessionText.text = currentText;
                            }
                        }
                    }

                    ListModel {
                        id: sessionModel
                        ListElement { text: "Plasma (Wayland)" }
                        ListElement { text: "Plasma (X11)" }
                        ListElement { text: "GNOME" }
                        ListElement { text: "Sway" }
                        ListElement { text: "Hyprland" }
                    }

                    Menu {
                        id: sessionMenu
                        y: parent.height + 4
                        width: parent.width

                        background: Rectangle {
                            radius: 8
                            color: "#1a1a3a"
                            border {
                                color: Qt.rgba(1, 1, 1, 0.1)
                                width: 1
                            }
                        }

                        delegate: MenuItem {
                            text: model.text
                            font {
                                family: "Noto Sans, Segoe UI, sans-serif"
                                pixelSize: 14
                            }
                            onTriggered: {
                                sessionText.text = model.text;
                            }
                        }
                    }
                }

                // Keyboard layout
                Rectangle {
                    id: keyboardIndicator
                    width: 50
                    height: 36
                    radius: 8
                    color: Qt.rgba(0, 0, 0, 0.2)
                    border {
                        color: Qt.rgba(1, 1, 1, 0.06)
                        width: 1
                    }

                    Text {
                        anchors.centerIn: parent
                        font {
                            family: "Noto Sans, Segoe UI, sans-serif"
                            pixelSize: 14
                            weight: Font.Medium
                        }
                        color: primaryAccent
                        text: keyboardLayouts[keyboardLayoutIndex % keyboardLayouts.length]
                    }

                    MouseArea {
                        anchors.fill: parent
                        cursorShape: Qt.PointingHandCursor
                        onClicked: {
                            keyboardLayoutIndex = (keyboardLayoutIndex + 1) % keyboardLayouts.length;
                            layoutSwitchRequested();
                        }
                    }
                }
            }
        }

        // Spacer
        Item {
            Layout.fillHeight: true
        }

        // Power buttons
        Item {
            Layout.fillWidth: true
            Layout.preferredHeight: 48

            RowLayout {
                anchors.centerIn: parent
                spacing: 16

                // Suspend
                PowerButton {
                    icon: "⏾"
                    tooltip: "Suspend"
                    onClicked: suspendRequested()
                }

                // Restart
                PowerButton {
                    icon: "↻"
                    tooltip: "Restart"
                    onClicked: restartRequested()
                }

                // Shutdown
                PowerButton {
                    icon: "⏻"
                    tooltip: "Shutdown"
                    onClicked: shutdownRequested()
                }
            }
        }
    }

    // Public methods
    function shake() {
        isError = true;
        shakeAnimation.restart();
        passwordInput.selectAll();
        passwordInput.focus = true;
    }

    function setUsername(name) {
        userName = name;
        userNameText.text = name;
        if (name.length > 0) {
            avatarCircle.children[1].text = name.charAt(0).toUpperCase();
        }
    }

    function clearPassword() {
        passwordInput.text = "";
        isProcessing = false;
    }

    function setProcessing(state) {
        isProcessing = state;
    }
}
