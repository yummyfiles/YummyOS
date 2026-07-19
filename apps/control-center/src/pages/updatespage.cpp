#include "updatespage.h"
#include "../utils/configmanager.h"
#include "../utils/systemutils.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QLabel>
#include <QProcess>
#include <QMessageBox>

UpdatesPage::UpdatesPage(QWidget *parent)
    : QWidget(parent)
{
    setupUi();
    loadUpdateSettings();
}

void UpdatesPage::setupUi()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(24, 24, 24, 24);
    mainLayout->setSpacing(16);

    QLabel *title = new QLabel(QStringLiteral("Updates"), this);
    title->setStyleSheet(QStringLiteral("font-size: 22px; font-weight: bold; color: #e0e0ee; padding-bottom: 8px;"));
    mainLayout->addWidget(title);

    QLabel *subtitle = new QLabel(QStringLiteral("Manage system updates and update settings"), this);
    subtitle->setStyleSheet(QStringLiteral("color: #8888aa; font-size: 13px; padding-bottom: 16px;"));
    mainLayout->addWidget(subtitle);

    QString groupBoxStyle = QStringLiteral(
        "QGroupBox {"
        "  background-color: #222244; border: 1px solid #333355; border-radius: 12px;"
        "  margin-top: 12px; padding: 20px 16px 16px 16px;"
        "  font-size: 14px; font-weight: bold; color: #4CC9F0;"
        "}"
        "QGroupBox::title { subcontrol-origin: margin; subcontrol-position: top left; padding: 2px 12px; }"
    );

    QString buttonStyle = QStringLiteral(
        "QPushButton {"
        "  background-color: #4CC9F0; color: #1a1a2e; border: none; border-radius: 8px;"
        "  padding: 10px 24px; font-size: 13px; font-weight: bold;"
        "}"
        "QPushButton:hover { background-color: #6dd5f5; }"
        "QPushButton:pressed { background-color: #3ab8de; }"
    );

    QString checkboxStyle = QStringLiteral(
        "QCheckBox { color: #d0d0ee; font-size: 13px; spacing: 8px; }"
        "QCheckBox::indicator {"
        "  width: 20px; height: 20px; border-radius: 4px; border: 2px solid #555577; background-color: #1a1a2e;"
        "}"
        "QCheckBox::indicator:checked { background-color: #4CC9F0; border-color: #4CC9F0; }"
        "QCheckBox::indicator:hover { border-color: #4CC9F0; }"
    );

    QString comboStyle = QStringLiteral(
        "QComboBox {"
        "  background-color: #1a1a2e; border: 1px solid #333355; border-radius: 6px;"
        "  padding: 8px 12px; color: #e0e0ee; font-size: 13px; min-width: 200px;"
        "}"
        "QComboBox:hover { border-color: #4CC9F0; }"
        "QComboBox::drop-down {"
        "  subcontrol-origin: padding; subcontrol-position: top right;"
        "  border-left: 1px solid #333355; width: 28px;"
        "}"
        "QComboBox QAbstractItemView {"
        "  background-color: #222244; border: 1px solid #333355;"
        "  color: #e0e0ee; selection-background-color: #4CC9F0; selection-color: #1a1a2e; outline: none;"
        "}"
    );

    QString infoLabelStyle = QStringLiteral(
        "QLabel { background-color: #1a1a2e; border: 1px solid #333355; border-radius: 8px; padding: 12px; color: #d0d0ee; font-size: 13px; }"
    );

    // Check for Updates
    QGroupBox *checkGroup = new QGroupBox(QStringLiteral("Check for Updates"), this);
    checkGroup->setStyleSheet(groupBoxStyle);
    QVBoxLayout *checkLayout = new QVBoxLayout(checkGroup);
    checkLayout->setSpacing(12);

    QHBoxLayout *checkRow = new QHBoxLayout();
    m_checkBtn = new QPushButton(QStringLiteral("Check Now"), checkGroup);
    m_checkBtn->setStyleSheet(buttonStyle);
    connect(m_checkBtn, &QPushButton::clicked, this, &UpdatesPage::checkForUpdates);
    checkRow->addWidget(m_checkBtn);

    checkRow->addStretch();

    QLabel *lastCheckedLabel = new QLabel(QStringLiteral("Last checked:"), checkGroup);
    lastCheckedLabel->setStyleSheet(QStringLiteral("color: #b0b0cc; font-size: 13px;"));
    checkRow->addWidget(lastCheckedLabel);

    m_lastCheckedLabel = new QLabel(QStringLiteral("Never"), checkGroup);
    m_lastCheckedLabel->setStyleSheet(QStringLiteral("color: #d0d0ee; font-size: 13px;"));
    checkRow->addWidget(m_lastCheckedLabel);

    checkLayout->addLayout(checkRow);

    m_updateStatus = new QLabel(QStringLiteral("Click 'Check Now' to look for available updates"), checkGroup);
    m_updateStatus->setWordWrap(true);
    m_updateStatus->setStyleSheet(infoLabelStyle);
    checkLayout->addWidget(m_updateStatus);

    mainLayout->addWidget(checkGroup);

    // Update Settings
    QGroupBox *settingsGroup = new QGroupBox(QStringLiteral("Update Settings"), this);
    settingsGroup->setStyleSheet(groupBoxStyle);
    QVBoxLayout *settingsLayout = new QVBoxLayout(settingsGroup);
    settingsLayout->setSpacing(12);

    m_autoUpdateCheck = new QCheckBox(QStringLiteral("Automatically check for updates"), settingsGroup);
    m_autoUpdateCheck->setStyleSheet(checkboxStyle);
    settingsLayout->addWidget(m_autoUpdateCheck);

    QHBoxLayout *channelRow = new QHBoxLayout();
    QLabel *channelLabel = new QLabel(QStringLiteral("Update Channel:"), settingsGroup);
    channelLabel->setStyleSheet(QStringLiteral("color: #b0b0cc; font-size: 13px;"));
    channelRow->addWidget(channelLabel);

    m_channelCombo = new QComboBox(settingsGroup);
    m_channelCombo->setStyleSheet(comboStyle);
    m_channelCombo->addItems({
        QStringLiteral("Stable"), QStringLiteral("Testing"),
        QStringLiteral("Unstable (Development)")
    });
    channelRow->addWidget(m_channelCombo);
    channelRow->addStretch();
    settingsLayout->addLayout(channelRow);

    m_applyBtn = new QPushButton(QStringLiteral("Save Settings"), settingsGroup);
    m_applyBtn->setStyleSheet(buttonStyle);
    connect(m_applyBtn, &QPushButton::clicked, this, &UpdatesPage::changeUpdateChannel);
    settingsLayout->addWidget(m_applyBtn);

    mainLayout->addWidget(settingsGroup);

    mainLayout->addStretch();
}

void UpdatesPage::loadUpdateSettings()
{
    ConfigManager *cm = ConfigManager::instance();
    m_autoUpdateCheck->setChecked(cm->readValue(QStringLiteral("Updates"), QStringLiteral("AutoUpdate"), false).toBool());
    QString channel = cm->readValue(QStringLiteral("Updates"), QStringLiteral("Channel"), QStringLiteral("Stable")).toString();
    int index = m_channelCombo->findText(channel);
    if (index >= 0) {
        m_channelCombo->setCurrentIndex(index);
    }
}

void UpdatesPage::checkForUpdates()
{
    m_updateStatus->setText(QStringLiteral("Checking for updates..."));
    m_updateStatus->setStyleSheet(QStringLiteral(
        "QLabel { background-color: #1a1a2e; border: 1px solid #333355; border-radius: 8px; padding: 12px; color: #f39c12; font-size: 13px; }"));

    QString output = SystemUtils::runCommand(QStringLiteral("pacman -Sy 2>/dev/null && pacman -Qu 2>/dev/null | head -20"), 15000);
    QString count = SystemUtils::runCommand(QStringLiteral("pacman -Qu 2>/dev/null | wc -l"), 15000);

    if (count.trimmed().toInt() > 0) {
        m_updateStatus->setText(QStringLiteral("%1 update(s) available:\n%2").arg(count.trimmed(), output));
        m_updateStatus->setStyleSheet(QStringLiteral(
            "QLabel { background-color: #1a1a2e; border: 1px solid #333355; border-radius: 8px; padding: 12px; color: #4CC9F0; font-size: 13px; }"));
    } else {
        m_updateStatus->setText(QStringLiteral("System is up to date!"));
        m_updateStatus->setStyleSheet(QStringLiteral(
            "QLabel { background-color: #1a1a2e; border: 1px solid #2ecc71; border-radius: 8px; padding: 12px; color: #2ecc71; font-size: 13px; font-weight: bold; }"));
    }

    QDateTime now = QDateTime::currentDateTime();
    m_lastCheckedLabel->setText(now.toString(QStringLiteral("yyyy-MM-dd hh:mm")));
}

void UpdatesPage::toggleAutoUpdate()
{
    ConfigManager::instance()->writeValue(QStringLiteral("Updates"), QStringLiteral("AutoUpdate"), m_autoUpdateCheck->isChecked());
}

void UpdatesPage::changeUpdateChannel()
{
    ConfigManager *cm = ConfigManager::instance();
    cm->writeValue(QStringLiteral("Updates"), QStringLiteral("AutoUpdate"), m_autoUpdateCheck->isChecked());
    cm->writeValue(QStringLiteral("Updates"), QStringLiteral("Channel"), m_channelCombo->currentText());
    QMessageBox::information(this, QStringLiteral("Settings Saved"), QStringLiteral("Update settings have been saved."));
}
