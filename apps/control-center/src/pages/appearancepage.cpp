#include "appearancepage.h"
#include "../utils/configmanager.h"
#include "../utils/systemutils.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QLabel>
#include <QFrame>
#include <QSpinBox>
#include <QProcess>
#include <QMessageBox>
#include <QDir>
#include <QStandardPaths>

AppearancePage::AppearancePage(QWidget *parent)
    : QWidget(parent)
{
    setupUi();
    loadCurrentSettings();
}

void AppearancePage::setupUi()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(24, 24, 24, 24);
    mainLayout->setSpacing(16);

    QLabel *title = new QLabel(QStringLiteral("Appearance"), this);
    title->setStyleSheet(QStringLiteral("font-size: 22px; font-weight: bold; color: #e0e0ee; padding-bottom: 8px;"));
    mainLayout->addWidget(title);

    QLabel *subtitle = new QLabel(QStringLiteral("Customize the look and feel of your YummyOS desktop"), this);
    subtitle->setStyleSheet(QStringLiteral("color: #8888aa; font-size: 13px; padding-bottom: 16px;"));
    mainLayout->addWidget(subtitle);

    QString groupBoxStyle = QStringLiteral(
        "QGroupBox {"
        "  background-color: #222244;"
        "  border: 1px solid #333355;"
        "  border-radius: 12px;"
        "  margin-top: 12px;"
        "  padding: 20px 16px 16px 16px;"
        "  font-size: 14px;"
        "  font-weight: bold;"
        "  color: #4CC9F0;"
        "}"
        "QGroupBox::title {"
        "  subcontrol-origin: margin;"
        "  subcontrol-position: top left;"
        "  padding: 2px 12px;"
        "}"
    );

    QString labelStyle = QStringLiteral("color: #b0b0cc; font-size: 13px;");
    QString comboStyle = QStringLiteral(
        "QComboBox {"
        "  background-color: #1a1a2e;"
        "  border: 1px solid #333355;"
        "  border-radius: 6px;"
        "  padding: 8px 12px;"
        "  color: #e0e0ee;"
        "  font-size: 13px;"
        "  min-width: 200px;"
        "}"
        "QComboBox:hover { border-color: #4CC9F0; }"
        "QComboBox::drop-down {"
        "  subcontrol-origin: padding;"
        "  subcontrol-position: top right;"
        "  border-left: 1px solid #333355;"
        "  width: 28px;"
        "}"
        "QComboBox QAbstractItemView {"
        "  background-color: #222244;"
        "  border: 1px solid #333355;"
        "  color: #e0e0ee;"
        "  selection-background-color: #4CC9F0;"
        "  selection-color: #1a1a2e;"
        "  outline: none;"
        "}"
    );

    QString buttonStyle = QStringLiteral(
        "QPushButton {"
        "  background-color: #4CC9F0;"
        "  color: #1a1a2e;"
        "  border: none;"
        "  border-radius: 8px;"
        "  padding: 10px 24px;"
        "  font-size: 13px;"
        "  font-weight: bold;"
        "}"
        "QPushButton:hover { background-color: #6dd5f5; }"
        "QPushButton:pressed { background-color: #3ab8de; }"
    );

    QString spinBoxStyle = QStringLiteral(
        "QSpinBox {"
        "  background-color: #1a1a2e;"
        "  border: 1px solid #333355;"
        "  border-radius: 6px;"
        "  padding: 8px 12px;"
        "  color: #e0e0ee;"
        "  font-size: 13px;"
        "}"
        "QSpinBox:hover { border-color: #4CC9F0; }"
    );

    // Theme selector
    QGroupBox *themeGroup = new QGroupBox(QStringLiteral("Global Theme"), this);
    themeGroup->setStyleSheet(groupBoxStyle);
    QHBoxLayout *themeLayout = new QHBoxLayout(themeGroup);
    themeLayout->setSpacing(12);

    QLabel *themeLabel = new QLabel(QStringLiteral("Theme:"), themeGroup);
    themeLabel->setStyleSheet(labelStyle);
    themeLayout->addWidget(themeLabel);

    m_themeCombo = new QComboBox(themeGroup);
    m_themeCombo->setStyleSheet(comboStyle);
    m_themeCombo->addItem(QStringLiteral("Breeze Dark"));
    m_themeCombo->addItem(QStringLiteral("Breeze Light"));
    m_themeCombo->addItem(QStringLiteral("Breeze Classic"));
    m_themeCombo->addItem(QStringLiteral("YummyOS Dark"));
    m_themeCombo->addItem(QStringLiteral("YummyOS Light"));
    themeLayout->addWidget(m_themeCombo);

    themeLayout->addStretch();

    m_applyButton = new QPushButton(QStringLiteral("Apply Theme"), themeGroup);
    m_applyButton->setStyleSheet(buttonStyle);
    connect(m_applyButton, &QPushButton::clicked, this, &AppearancePage::applyTheme);
    themeLayout->addWidget(m_applyButton);

    mainLayout->addWidget(themeGroup);

    // Accent color
    QGroupBox *accentGroup = new QGroupBox(QStringLiteral("Accent Color"), this);
    accentGroup->setStyleSheet(groupBoxStyle);
    QHBoxLayout *accentLayout = new QHBoxLayout(accentGroup);
    accentLayout->setSpacing(12);

    QLabel *accentLabel = new QLabel(QStringLiteral("Color:"), accentGroup);
    accentLabel->setStyleSheet(labelStyle);
    accentLayout->addWidget(accentLabel);

    m_accentCombo = new QComboBox(accentGroup);
    m_accentCombo->setStyleSheet(comboStyle);
    m_accentCombo->addItem(QStringLiteral("YummyOS Cyan (#4CC9F0)"));
    m_accentCombo->addItem(QStringLiteral("Purple (#7B2CBF)"));
    m_accentCombo->addItem(QStringLiteral("Green (#2ecc71)"));
    m_accentCombo->addItem(QStringLiteral("Red (#e74c3c)"));
    m_accentCombo->addItem(QStringLiteral("Orange (#f39c12)"));
    m_accentCombo->addItem(QStringLiteral("Teal (#1abc9c)"));
    accentLayout->addWidget(m_accentCombo);

    m_accentPreview = new QLabel(accentGroup);
    m_accentPreview->setFixedSize(32, 32);
    m_accentPreview->setStyleSheet(QStringLiteral("background-color: #4CC9F0; border-radius: 16px; border: 2px solid #333355;"));
    accentLayout->addWidget(m_accentPreview);

    connect(m_accentCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [this](int index) {
        QStringList colors = {
            QStringLiteral("#4CC9F0"), QStringLiteral("#7B2CBF"),
            QStringLiteral("#2ecc71"), QStringLiteral("#e74c3c"),
            QStringLiteral("#f39c12"), QStringLiteral("#1abc9c")
        };
        if (index >= 0 && index < colors.size()) {
            m_accentPreview->setStyleSheet(
                QStringLiteral("background-color: %1; border-radius: 16px; border: 2px solid #333355;").arg(colors[index]));
        }
    });

    accentLayout->addStretch();

    m_applyAccentButton = new QPushButton(QStringLiteral("Apply Color"), accentGroup);
    m_applyAccentButton->setStyleSheet(buttonStyle);
    connect(m_applyAccentButton, &QPushButton::clicked, this, &AppearancePage::applyAccentColor);
    accentLayout->addWidget(m_applyAccentButton);

    mainLayout->addWidget(accentGroup);

    // Font settings
    QGroupBox *fontGroup = new QGroupBox(QStringLiteral("Font"), this);
    fontGroup->setStyleSheet(groupBoxStyle);
    QHBoxLayout *fontLayout = new QHBoxLayout(fontGroup);
    fontLayout->setSpacing(12);

    QLabel *fontLabel = new QLabel(QStringLiteral("Font:"), fontGroup);
    fontLabel->setStyleSheet(labelStyle);
    fontLayout->addWidget(fontLabel);

    m_fontCombo = new QFontComboBox(fontGroup);
    m_fontCombo->setStyleSheet(comboStyle);
    m_fontCombo->setCurrentFont(QFont(QStringLiteral("Noto Sans")));
    fontLayout->addWidget(m_fontCombo);

    QLabel *sizeLabel = new QLabel(QStringLiteral("Size:"), fontGroup);
    sizeLabel->setStyleSheet(labelStyle);
    fontLayout->addWidget(sizeLabel);

    m_fontSizeSpin = new QSpinBox(fontGroup);
    m_fontSizeSpin->setStyleSheet(spinBoxStyle);
    m_fontSizeSpin->setRange(8, 24);
    m_fontSizeSpin->setValue(10);
    fontLayout->addWidget(m_fontSizeSpin);

    fontLayout->addStretch();

    m_applyFontButton = new QPushButton(QStringLiteral("Apply Font"), fontGroup);
    m_applyFontButton->setStyleSheet(buttonStyle);
    connect(m_applyFontButton, &QPushButton::clicked, this, &AppearancePage::applyFont);
    fontLayout->addWidget(m_applyFontButton);

    mainLayout->addWidget(fontGroup);

    // Icon theme
    QGroupBox *iconGroup = new QGroupBox(QStringLiteral("Icons & Cursor"), this);
    iconGroup->setStyleSheet(groupBoxStyle);
    QHBoxLayout *iconLayout = new QHBoxLayout(iconGroup);
    iconLayout->setSpacing(12);

    QLabel *iconLabel = new QLabel(QStringLiteral("Icons:"), iconGroup);
    iconLabel->setStyleSheet(labelStyle);
    iconLayout->addWidget(iconLabel);

    m_iconThemeCombo = new QComboBox(iconGroup);
    m_iconThemeCombo->setStyleSheet(comboStyle);
    m_iconThemeCombo->addItem(QStringLiteral("Breeze"));
    m_iconThemeCombo->addItem(QStringLiteral("Breeze Dark"));
    m_iconThemeCombo->addItem(QStringLiteral("Papirus"));
    m_iconThemeCombo->addItem(QStringLiteral("Tela"));
    m_iconThemeCombo->addItem(QStringLiteral("Candy"));
    iconLayout->addWidget(m_iconThemeCombo);

    QLabel *cursorLabel = new QLabel(QStringLiteral("Cursor:"), iconGroup);
    cursorLabel->setStyleSheet(labelStyle);
    iconLayout->addWidget(cursorLabel);

    m_cursorThemeCombo = new QComboBox(iconGroup);
    m_cursorThemeCombo->setStyleSheet(comboStyle);
    m_cursorThemeCombo->addItem(QStringLiteral("Breeze"));
    m_cursorThemeCombo->addItem(QStringLiteral("Breeze Light"));
    m_cursorThemeCombo->addItem(QStringLiteral("Yaru"));
    m_cursorThemeCombo->addItem(QStringLiteral("Adwaita"));
    m_cursorThemeCombo->addItem(QStringLiteral("Capitaine"));
    iconLayout->addWidget(m_cursorThemeCombo);

    iconLayout->addStretch();

    mainLayout->addWidget(iconGroup);

    mainLayout->addStretch();
}

void AppearancePage::loadCurrentSettings()
{
    ConfigManager *cm = ConfigManager::instance();
    QString theme = cm->readKDEConfig(QStringLiteral("kdeglobals"), QStringLiteral("KDE"), QStringLiteral("widgetStyle"), QStringLiteral("Breeze")).toString();
    int themeIndex = m_themeCombo->findText(theme);
    if (themeIndex >= 0) {
        m_themeCombo->setCurrentIndex(themeIndex);
    }

    QString accent = cm->readValue(QStringLiteral("Appearance"), QStringLiteral("AccentColor"), QStringLiteral("#4CC9F0")).toString();
    QStringList colorNames = {
        QStringLiteral("YummyOS Cyan (#4CC9F0)"), QStringLiteral("Purple (#7B2CBF)"),
        QStringLiteral("Green (#2ecc71)"), QStringLiteral("Red (#e74c3c)"),
        QStringLiteral("Orange (#f39c12)"), QStringLiteral("Teal (#1abc9c)")
    };
    for (int i = 0; i < colorNames.size(); i++) {
        if (accent.contains(colorNames[i].section(QLatin1Char('('), 1).chopped(1))) {
            m_accentCombo->setCurrentIndex(i);
            break;
        }
    }

    QString fontFamily = cm->readKDEConfig(QStringLiteral("kdeglobals"), QStringLiteral("General"), QStringLiteral("font"), QStringLiteral("Noto Sans,10,-1,5,50,0,0,0,0,0")).toString();
    QString fontName = fontFamily.section(QLatin1Char(','), 0, 0);
    QFont font(fontName);
    m_fontCombo->setCurrentFont(font);

    int fontSize = fontFamily.section(QLatin1Char(','), 1, 1).toInt();
    if (fontSize > 0) {
        m_fontSizeSpin->setValue(fontSize);
    }

    QString iconTheme = cm->readKDEConfig(QStringLiteral("kdeglobals"), QStringLiteral("Icons"), QStringLiteral("Theme"), QStringLiteral("Breeze")).toString();
    int iconIndex = m_iconThemeCombo->findText(iconTheme);
    if (iconIndex >= 0) {
        m_iconThemeCombo->setCurrentIndex(iconIndex);
    }

    QString cursorTheme = cm->readKDEConfig(QStringLiteral("kcminputrc"), QStringLiteral("Mouse"), QStringLiteral("cursorTheme"), QStringLiteral("Breeze")).toString();
    int cursorIndex = m_cursorThemeCombo->findText(cursorTheme);
    if (cursorIndex >= 0) {
        m_cursorThemeCombo->setCurrentIndex(cursorIndex);
    }
}

void AppearancePage::applyTheme()
{
    QString theme = m_themeCombo->currentText();
    ConfigManager *cm = ConfigManager::instance();
    cm->writeKDEConfig(QStringLiteral("kdeglobals"), QStringLiteral("KDE"), QStringLiteral("widgetStyle"), theme);

    SystemUtils::runCommand(QStringLiteral("plasma-apply-desktoptheme \"%1\" 2>/dev/null").arg(theme.toLower().remove(QLatin1Char(' '))));
    SystemUtils::runCommand(QStringLiteral("qdbus org.kde.plasmashell /PlasmaShell evaluateScript \"var allDesktops = desktops(); for (var i=0; i<allDesktops.length; i++) { allDesktops[i].wallpaperPlugin = allDesktops[i].wallpaperPlugin; }\" 2>/dev/null"));

    QMessageBox::information(this, QStringLiteral("Theme Applied"),
        QStringLiteral("Theme has been changed to '%1'. Some changes may require a logout to fully take effect.").arg(theme));
}

void AppearancePage::applyAccentColor()
{
    QStringList colors = {
        QStringLiteral("#4CC9F0"), QStringLiteral("#7B2CBF"),
        QStringLiteral("#2ecc71"), QStringLiteral("#e74c3c"),
        QStringLiteral("#f39c12"), QStringLiteral("#1abc9c")
    };
    int index = m_accentCombo->currentIndex();
    if (index < 0 || index >= colors.size()) return;

    QString color = colors[index];
    ConfigManager *cm = ConfigManager::instance();
    cm->writeValue(QStringLiteral("Appearance"), QStringLiteral("AccentColor"), color);

    QPalette palette = qApp->palette();
    palette.setColor(QPalette::Highlight, QColor(color));
    palette.setColor(QPalette::Link, QColor(color));

    QString r = QString::number(QColor(color).red());
    QString g = QString::number(QColor(color).green());
    QString b = QString::number(QColor(color).blue());

    SystemUtils::runCommand(QStringLiteral(
        "kwriteconfig5 --file kdeglobals --group Colors:Selection --key Background '%1,%2,%3'"
    ).arg(r, g, b));

    QMessageBox::information(this, QStringLiteral("Accent Color Applied"),
        QStringLiteral("Accent color changed to %1.").arg(color));
}

void AppearancePage::applyFont()
{
    QString fontFamily = m_fontCombo->currentFont().family();
    int fontSize = m_fontSizeSpin->value();
    QString fontStr = QStringLiteral("%1,%2,-1,5,50,0,0,0,0,0").arg(fontFamily).arg(fontSize);

    ConfigManager *cm = ConfigManager::instance();
    cm->writeKDEConfig(QStringLiteral("kdeglobals"), QStringLiteral("General"), QStringLiteral("font"), fontStr);
    cm->writeKDEConfig(QStringLiteral("kdeglobals"), QStringLiteral("General"), QStringLiteral("menuFont"), fontStr);
    cm->writeKDEConfig(QStringLiteral("kdeglobals"), QStringLiteral("General"), QStringLiteral("toolBarFont"), fontStr);
    cm->writeKDEConfig(QStringLiteral("kdeglobals"), QStringLiteral("General"), QStringLiteral("smallFont"), fontStr);

    QFont newFont(fontFamily, fontSize);
    qApp->setFont(newFont);

    SystemUtils::runCommand(QStringLiteral(
        "kwriteconfig5 --file kdeglobals --group General --key font \"%1\""
    ).arg(fontStr));

    QMessageBox::information(this, QStringLiteral("Font Applied"),
        QStringLiteral("Font changed to %1 %2pt.").arg(fontFamily).arg(fontSize));
}
