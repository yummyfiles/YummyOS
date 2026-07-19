#include "powerpage.h"
#include "../utils/configmanager.h"
#include "../utils/systemutils.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QLabel>
#include <QProcess>
#include <QMessageBox>

PowerPage::PowerPage(QWidget *parent)
    : QWidget(parent)
{
    setupUi();
    loadCurrentSettings();
}

void PowerPage::setupUi()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(24, 24, 24, 24);
    mainLayout->setSpacing(16);

    QLabel *title = new QLabel(QStringLiteral("Power"), this);
    title->setStyleSheet(QStringLiteral("font-size: 22px; font-weight: bold; color: #e0e0ee; padding-bottom: 8px;"));
    mainLayout->addWidget(title);

    QLabel *subtitle = new QLabel(QStringLiteral("Manage power profiles and energy settings"), this);
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

    QString labelStyle = QStringLiteral("color: #b0b0cc; font-size: 13px;");
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

    QString buttonStyle = QStringLiteral(
        "QPushButton {"
        "  background-color: #4CC9F0; color: #1a1a2e; border: none; border-radius: 8px;"
        "  padding: 10px 24px; font-size: 13px; font-weight: bold;"
        "}"
        "QPushButton:hover { background-color: #6dd5f5; }"
        "QPushButton:pressed { background-color: #3ab8de; }"
    );

    QString spinBoxStyle = QStringLiteral(
        "QSpinBox {"
        "  background-color: #1a1a2e; border: 1px solid #333355; border-radius: 6px;"
        "  padding: 8px 12px; color: #e0e0ee; font-size: 13px;"
        "}"
        "QSpinBox:hover { border-color: #4CC9F0; }"
    );

    // Power Profile
    QGroupBox *profileGroup = new QGroupBox(QStringLiteral("Power Profile"), this);
    profileGroup->setStyleSheet(groupBoxStyle);
    QVBoxLayout *profileLayout = new QVBoxLayout(profileGroup);
    profileLayout->setSpacing(12);

    QHBoxLayout *profileRow = new QHBoxLayout();
    QLabel *profileLabel = new QLabel(QStringLiteral("Profile:"), profileGroup);
    profileLabel->setStyleSheet(labelStyle);
    profileRow->addWidget(profileLabel);

    m_powerProfileCombo = new QComboBox(profileGroup);
    m_powerProfileCombo->setStyleSheet(comboStyle);
    m_powerProfileCombo->addItem(QStringLiteral("Balanced"));
    m_powerProfileCombo->addItem(QStringLiteral("Performance"));
    m_powerProfileCombo->addItem(QStringLiteral("Power Saver"));
    profileRow->addWidget(m_powerProfileCombo);

    profileRow->addStretch();

    m_applyButton = new QPushButton(QStringLiteral("Apply"), profileGroup);
    m_applyButton->setStyleSheet(buttonStyle);
    connect(m_applyButton, &QPushButton::clicked, this, &PowerPage::applyPowerProfile);
    profileRow->addWidget(m_applyButton);

    profileLayout->addLayout(profileRow);

    m_currentProfileLabel = new QLabel(QStringLiteral("Current: Checking..."), profileGroup);
    m_currentProfileLabel->setStyleSheet(QStringLiteral("color: #4CC9F0; font-size: 13px; font-weight: bold;"));
    profileLayout->addWidget(m_currentProfileLabel);

    mainLayout->addWidget(profileGroup);

    // Auto Suspend
    QGroupBox *suspendGroup = new QGroupBox(QStringLiteral("Suspend & Sleep"), this);
    suspendGroup->setStyleSheet(groupBoxStyle);
    QVBoxLayout *suspendLayout = new QVBoxLayout(suspendGroup);
    suspendLayout->setSpacing(12);

    QHBoxLayout *autoSuspendRow = new QHBoxLayout();
    QLabel *autoSuspendLabel = new QLabel(QStringLiteral("Auto-suspend after (minutes, 0=never):"), suspendGroup);
    autoSuspendLabel->setStyleSheet(labelStyle);
    autoSuspendRow->addWidget(autoSuspendLabel);

    m_autoSuspendSpin = new QSpinBox(suspendGroup);
    m_autoSuspendSpin->setStyleSheet(spinBoxStyle);
    m_autoSuspendSpin->setRange(0, 180);
    m_autoSuspendSpin->setValue(30);
    autoSuspendRow->addWidget(m_autoSuspendSpin);
    autoSuspendRow->addStretch();
    suspendLayout->addLayout(autoSuspendRow);

    QHBoxLayout *screenLockRow = new QHBoxLayout();
    QLabel *screenLockLabel = new QLabel(QStringLiteral("Screen lock timeout (minutes, 0=never):"), suspendGroup);
    screenLockLabel->setStyleSheet(labelStyle);
    screenLockRow->addWidget(screenLockLabel);

    m_screenLockSpin = new QSpinBox(suspendGroup);
    m_screenLockSpin->setStyleSheet(spinBoxStyle);
    m_screenLockSpin->setRange(0, 180);
    m_screenLockSpin->setValue(10);
    screenLockRow->addWidget(m_screenLockSpin);
    screenLockRow->addStretch();
    suspendLayout->addLayout(screenLockRow);

    mainLayout->addWidget(suspendGroup);

    // Lid & Battery
    QGroupBox *actionGroup = new QGroupBox(QStringLiteral("Actions"), this);
    actionGroup->setStyleSheet(groupBoxStyle);
    QVBoxLayout *actionLayout = new QVBoxLayout(actionGroup);
    actionLayout->setSpacing(12);

    QHBoxLayout *lidRow = new QHBoxLayout();
    QLabel *lidLabel = new QLabel(QStringLiteral("Lid close action:"), actionGroup);
    lidLabel->setStyleSheet(labelStyle);
    lidRow->addWidget(lidLabel);

    m_lidCloseCombo = new QComboBox(actionGroup);
    m_lidCloseCombo->setStyleSheet(comboStyle);
    m_lidCloseCombo->addItems({
        QStringLiteral("Suspend"), QStringLiteral("Hibernate"),
        QStringLiteral("Turn Off Screen"), QStringLiteral("Do Nothing")
    });
    lidRow->addWidget(m_lidCloseCombo);
    lidRow->addStretch();
    actionLayout->addLayout(lidRow);

    QHBoxLayout *batteryRow = new QHBoxLayout();
    QLabel *batteryLabel = new QLabel(QStringLiteral("Low battery action:"), actionGroup);
    batteryLabel->setStyleSheet(labelStyle);
    batteryRow->addWidget(batteryLabel);

    m_lowBatteryCombo = new QComboBox(actionGroup);
    m_lowBatteryCombo->setStyleSheet(comboStyle);
    m_lowBatteryCombo->addItems({
        QStringLiteral("Suspend"), QStringLiteral("Hibernate"),
        QStringLiteral("Show Notification"), QStringLiteral("Shutdown")
    });
    batteryRow->addWidget(m_lowBatteryCombo);
    batteryRow->addStretch();
    actionLayout->addLayout(batteryRow);

    mainLayout->addWidget(actionGroup);

    // Apply All
    QHBoxLayout *applyAllLayout = new QHBoxLayout();
    applyAllLayout->addStretch();
    QPushButton *applyAllBtn = new QPushButton(QStringLiteral("Save All Settings"), this);
    applyAllBtn->setStyleSheet(buttonStyle);
    connect(applyAllBtn, &QPushButton::clicked, this, &PowerPage::applySettings);
    applyAllLayout->addWidget(applyAllBtn);
    mainLayout->addLayout(applyAllLayout);

    mainLayout->addStretch();
}

void PowerPage::loadCurrentSettings()
{
    QString profile = SystemUtils::instance()->getCurrentPowerProfile();
    m_currentProfileLabel->setText(QStringLiteral("Current: %1").arg(profile.isEmpty() ? QStringLiteral("Unknown") : profile));

    int index = m_powerProfileCombo->findText(profile, Qt::MatchInsensitive);
    if (index >= 0) {
        m_powerProfileCombo->setCurrentIndex(index);
    }

    ConfigManager *cm = ConfigManager::instance();
    m_autoSuspendSpin->setValue(cm->readValue(QStringLiteral("Power"), QStringLiteral("AutoSuspend"), 30).toInt());
    m_screenLockSpin->setValue(cm->readValue(QStringLiteral("Power"), QStringLiteral("ScreenLock"), 10).toInt());

    QString lidAction = cm->readValue(QStringLiteral("Power"), QStringLiteral("LidClose"), QStringLiteral("Suspend")).toString();
    int lidIndex = m_lidCloseCombo->findText(lidAction);
    if (lidIndex >= 0) m_lidCloseCombo->setCurrentIndex(lidIndex);

    QString batteryAction = cm->readValue(QStringLiteral("Power"), QStringLiteral("LowBattery"), QStringLiteral("Suspend")).toString();
    int batteryIndex = m_lowBatteryCombo->findText(batteryAction);
    if (batteryIndex >= 0) m_lowBatteryCombo->setCurrentIndex(batteryIndex);
}

void PowerPage::applyPowerProfile()
{
    QString profile = m_powerProfileCombo->currentText().toLower().replace(QLatin1Char(' '), QLatin1Char('-'));
    SystemUtils::instance()->setPowerProfile(profile);
    m_currentProfileLabel->setText(QStringLiteral("Current: %1").arg(m_powerProfileCombo->currentText()));
}

void PowerPage::applySettings()
{
    applyPowerProfile();

    ConfigManager *cm = ConfigManager::instance();
    cm->writeValue(QStringLiteral("Power"), QStringLiteral("AutoSuspend"), m_autoSuspendSpin->value());
    cm->writeValue(QStringLiteral("Power"), QStringLiteral("ScreenLock"), m_screenLockSpin->value());
    cm->writeValue(QStringLiteral("Power"), QStringLiteral("LidClose"), m_lidCloseCombo->currentText());
    cm->writeValue(QStringLiteral("Power"), QStringLiteral("LowBattery"), m_lowBatteryCombo->currentText());

    int suspendMin = m_autoSuspendSpin->value();
    if (suspendMin > 0) {
        SystemUtils::runCommand(QStringLiteral("systemctl set-idle-delay %1000 2>/dev/null").arg(suspendMin));
    }

    QMessageBox::information(this, QStringLiteral("Settings Saved"),
        QStringLiteral("Power settings have been saved."));
}
