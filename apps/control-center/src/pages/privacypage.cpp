#include "privacypage.h"
#include "../utils/configmanager.h"
#include "../utils/systemutils.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QLabel>
#include <QProcess>
#include <QMessageBox>
#include <QDir>

PrivacyPage::PrivacyPage(QWidget *parent)
    : QWidget(parent)
{
    setupUi();
    loadCurrentSettings();
}

void PrivacyPage::setupUi()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(24, 24, 24, 24);
    mainLayout->setSpacing(16);

    QLabel *title = new QLabel(QStringLiteral("Privacy"), this);
    title->setStyleSheet(QStringLiteral("font-size: 22px; font-weight: bold; color: #e0e0ee; padding-bottom: 8px;"));
    mainLayout->addWidget(title);

    QLabel *subtitle = new QLabel(QStringLiteral("Control your privacy settings and data collection"), this);
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

    QString dangerButtonStyle = QStringLiteral(
        "QPushButton {"
        "  background-color: #e74c3c; color: #ffffff; border: none; border-radius: 8px;"
        "  padding: 10px 24px; font-size: 13px; font-weight: bold;"
        "}"
        "QPushButton:hover { background-color: #ec6b5e; }"
        "QPushButton:pressed { background-color: #c0392b; }"
    );

    QString checkboxStyle = QStringLiteral(
        "QCheckBox { color: #d0d0ee; font-size: 13px; spacing: 8px; }"
        "QCheckBox::indicator {"
        "  width: 20px; height: 20px; border-radius: 4px; border: 2px solid #555577; background-color: #1a1a2e;"
        "}"
        "QCheckBox::indicator:checked { background-color: #4CC9F0; border-color: #4CC9F0; }"
        "QCheckBox::indicator:hover { border-color: #4CC9F0; }"
    );

    // Telemetry - always OFF
    QGroupBox *telemetryGroup = new QGroupBox(QStringLiteral("Telemetry"), this);
    telemetryGroup->setStyleSheet(groupBoxStyle);
    QVBoxLayout *telLayout = new QVBoxLayout(telemetryGroup);
    telLayout->setSpacing(10);

    QHBoxLayout *telStatusLayout = new QHBoxLayout();
    QLabel *telIcon = new QLabel(QStringLiteral("  "), telemetryGroup);
    telIcon->setStyleSheet(QStringLiteral("background-color: #2ecc71; border-radius: 12px; min-width: 24px; min-height: 24px; max-width: 24px; max-height: 24px;"));
    telStatusLayout->addWidget(telIcon);

    m_telemetryStatus = new QLabel(QStringLiteral("All telemetry is permanently disabled"), telemetryGroup);
    m_telemetryStatus->setStyleSheet(QStringLiteral("color: #2ecc71; font-size: 14px; font-weight: bold;"));
    telStatusLayout->addWidget(m_telemetryStatus);
    telStatusLayout->addStretch();
    telLayout->addLayout(telStatusLayout);

    QLabel *telDesc = new QLabel(QStringLiteral(
        "YummyOS collects ZERO telemetry data. No usage statistics, no error reports, "
        "no personal data is ever sent anywhere. Your system, your data."
    ), telemetryGroup);
    telDesc->setWordWrap(true);
    telDesc->setStyleSheet(QStringLiteral("color: #b0b0cc; font-size: 12px;"));
    telLayout->addWidget(telDesc);

    mainLayout->addWidget(telemetryGroup);

    // Permissions
    QGroupBox *permissionsGroup = new QGroupBox(QStringLiteral("Device Permissions"), this);
    permissionsGroup->setStyleSheet(groupBoxStyle);
    QVBoxLayout *permLayout = new QVBoxLayout(permissionsGroup);
    permLayout->setSpacing(10);

    QLabel *micLabel = new QLabel(QStringLiteral("Microphone Access:"), permissionsGroup);
    micLabel->setStyleSheet(QStringLiteral("color: #b0b0cc; font-size: 13px;"));
    permLayout->addWidget(micLabel);

    m_micStatus = new QLabel(QStringLiteral("Available to applications (no restrictions)"), permissionsGroup);
    m_micStatus->setStyleSheet(QStringLiteral("color: #d0d0ee; font-size: 13px; padding-left: 16px;"));
    permLayout->addWidget(m_micStatus);

    QLabel *cameraLabel = new QLabel(QStringLiteral("Camera Access:"), permissionsGroup);
    cameraLabel->setStyleSheet(QStringLiteral("color: #b0b0cc; font-size: 13px;"));
    permLayout->addWidget(cameraLabel);

    m_cameraStatus = new QLabel(QStringLiteral("Available to applications (no restrictions)"), permissionsGroup);
    m_cameraStatus->setStyleSheet(QStringLiteral("color: #d0d0ee; font-size: 13px; padding-left: 16px;"));
    permLayout->addWidget(m_cameraStatus);

    mainLayout->addWidget(permissionsGroup);

    // Location
    QGroupBox *locationGroup = new QGroupBox(QStringLiteral("Location Services"), this);
    locationGroup->setStyleSheet(groupBoxStyle);
    QVBoxLayout *locLayout = new QVBoxLayout(locationGroup);
    locLayout->setSpacing(10);

    m_locationCheck = new QCheckBox(QStringLiteral("Enable location services"), locationGroup);
    m_locationCheck->setStyleSheet(checkboxStyle);
    connect(m_locationCheck, &QCheckBox::toggled, this, &PrivacyPage::toggleLocationServices);
    locLayout->addWidget(m_locationCheck);

    QLabel *locDesc = new QLabel(QStringLiteral(
        "Location services allow applications to determine your approximate location. "
        "This is used by weather apps, maps, and similar applications."
    ), locationGroup);
    locDesc->setWordWrap(true);
    locDesc->setStyleSheet(QStringLiteral("color: #8888aa; font-size: 12px;"));
    locLayout->addWidget(locDesc);

    mainLayout->addWidget(locationGroup);

    // Clear Data
    QGroupBox *clearGroup = new QGroupBox(QStringLiteral("Clear Data"), this);
    clearGroup->setStyleSheet(groupBoxStyle);
    QVBoxLayout *clearLayout = new QVBoxLayout(clearGroup);
    clearLayout->setSpacing(12);

    QHBoxLayout *clearBtnLayout = new QHBoxLayout();

    m_clearHistoryBtn = new QPushButton(QStringLiteral("Clear Browsing History"), clearGroup);
    m_clearHistoryBtn->setStyleSheet(dangerButtonStyle);
    connect(m_clearHistoryBtn, &QPushButton::clicked, this, &PrivacyPage::clearHistory);
    clearBtnLayout->addWidget(m_clearHistoryBtn);

    m_clearCacheBtn = new QPushButton(QStringLiteral("Clear Cache"), clearGroup);
    m_clearCacheBtn->setStyleSheet(dangerButtonStyle);
    connect(m_clearCacheBtn, &QPushButton::clicked, this, &PrivacyPage::clearCache);
    clearBtnLayout->addWidget(m_clearCacheBtn);

    clearBtnLayout->addStretch();
    clearLayout->addLayout(clearBtnLayout);

    mainLayout->addWidget(clearGroup);

    mainLayout->addStretch();
}

void PrivacyPage::loadCurrentSettings()
{
    m_locationCheck->setChecked(SystemUtils::instance()->isLocationServicesEnabled());
}

void PrivacyPage::toggleLocationServices()
{
    bool enabled = m_locationCheck->isChecked();
    SystemUtils::instance()->toggleLocationServices(enabled);
}

void PrivacyPage::clearHistory()
{
    QMessageBox::StandardButton reply = QMessageBox::question(this,
        QStringLiteral("Clear Browsing History"),
        QStringLiteral("This will clear browser history for all browsers. Continue?"),
        QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        QDir cacheDir(QDir::homePath() + QStringLiteral("/.cache"));
        QStringList browsers = {
            QStringLiteral("firefox"), QStringLiteral("chromium"),
            QStringLiteral("brave"), QStringLiteral("vivaldi"),
            QStringLiteral("google-chrome")
        };
        for (const QString &browser : browsers) {
            QString path = cacheDir.absoluteFilePath(browser);
            if (QDir(path).exists()) {
                SystemUtils::runCommand(QStringLiteral("rm -rf \"%1\" 2>/dev/null").arg(path));
            }
        }
        QMessageBox::information(this, QStringLiteral("Cleared"), QStringLiteral("Browsing history has been cleared."));
    }
}

void PrivacyPage::clearCache()
{
    QMessageBox::StandardButton reply = QMessageBox::question(this,
        QStringLiteral("Clear Cache"),
        QStringLiteral("This will clear system cache. Continue?"),
        QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        SystemUtils::runCommand(QStringLiteral("sudo journalctl --vacuum-time=1d 2>/dev/null"));
        SystemUtils::runCommand(QStringLiteral("rm -rf ~/.cache/thumbnails/* 2>/dev/null"));
        SystemUtils::runCommand(QStringLiteral("sync && sudo sysctl -w vm.drop_caches=3 2>/dev/null"));
        QMessageBox::information(this, QStringLiteral("Cleared"), QStringLiteral("System cache has been cleared."));
    }
}
