import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

pragma Singleton

QtObject {
    id: theme

    readonly property color accentColor: "#4CC9F0"
    readonly property color secondaryAccent: "#7B2CBF"
    readonly property color backgroundColor: "#1a1a2e"
    readonly property color surfaceColor: "#222244"
    readonly property color cardColor: "#16162a"
    readonly property color borderColor: "#333355"
    readonly property color textPrimary: "#e0e0ee"
    readonly property color textSecondary: "#b0b0cc"
    readonly property color textMuted: "#8888aa"
    readonly property color successColor: "#2ecc71"
    readonly property color warningColor: "#f39c12"
    readonly property color errorColor: "#e74c3c"
    readonly property color hoverColor: "#1e1e36"
    readonly property color selectedColor: "#4CC9F0"
    readonly property color selectedTextColor: "#1a1a2e"

    readonly property string fontFamily: "Noto Sans"
    readonly property int fontSizeSmall: 11
    readonly property int fontSizeNormal: 13
    readonly property int fontSizeMedium: 16
    readonly property int fontSizeLarge: 22
    readonly property int fontSizeTitle: 28

    readonly property int radiusSmall: 4
    readonly property int radiusMedium: 8
    readonly property int radiusLarge: 12

    readonly property int spacingSmall: 4
    readonly property int spacingNormal: 8
    readonly property int spacingMedium: 16
    readonly property int spacingLarge: 24

    readonly property int sidebarWidth: 240

    readonly property Component button: Button {
        font.family: theme.fontFamily
        font.pixelSize: theme.fontSizeNormal
        font.bold: true
        background: Rectangle {
            radius: theme.radiusMedium
            color: parent.down ? Qt.darker(theme.accentColor, 1.1) : (parent.hovered ? Qt.lighter(theme.accentColor, 1.1) : theme.accentColor)
        }
        contentItem: Text {
            text: parent.text
            font: parent.font
            color: theme.backgroundColor
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }
    }

    readonly property Component secondaryButton: Button {
        font.family: theme.fontFamily
        font.pixelSize: theme.fontSizeNormal
        font.bold: true
        background: Rectangle {
            radius: theme.radiusMedium
            color: parent.down ? Qt.darker(theme.secondaryAccent, 1.1) : (parent.hovered ? Qt.lighter(theme.secondaryAccent, 1.1) : theme.secondaryAccent)
        }
        contentItem: Text {
            text: parent.text
            font: parent.font
            color: "#ffffff"
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }
    }

    readonly property Component dangerButton: Button {
        font.family: theme.fontFamily
        font.pixelSize: theme.fontSizeNormal
        font.bold: true
        background: Rectangle {
            radius: theme.radiusMedium
            color: parent.down ? Qt.darker(theme.errorColor, 1.1) : (parent.hovered ? Qt.lighter(theme.errorColor, 1.1) : theme.errorColor)
        }
        contentItem: Text {
            text: parent.text
            font: parent.font
            color: "#ffffff"
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }
    }

    readonly property Component card: Rectangle {
        radius: theme.radiusLarge
        color: theme.surfaceColor
        border.color: theme.borderColor
        border.width: 1
    }

    readonly property Component textField: TextField {
        font.family: theme.fontFamily
        font.pixelSize: theme.fontSizeNormal
        color: theme.textPrimary
        background: Rectangle {
            radius: theme.radiusMedium
            color: theme.backgroundColor
            border.color: parent.activeFocus ? theme.accentColor : theme.borderColor
            border.width: 1
        }
        padding: 10
    }

    readonly property Component comboBox: ComboBox {
        font.family: theme.fontFamily
        font.pixelSize: theme.fontSizeNormal
        background: Rectangle {
            radius: theme.radiusMedium
            color: theme.backgroundColor
            border.color: theme.borderColor
            border.width: 1
        }
        contentItem: Text {
            text: parent.displayText
            font: parent.font
            color: theme.textPrimary
            verticalAlignment: Text.AlignVCenter
            leftPadding: 12
        }
    }
}
