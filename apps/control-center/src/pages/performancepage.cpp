#include "performancepage.h"
#include "../utils/configmanager.h"
#include "../utils/systemutils.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QLabel>
#include <QProcess>
#include <QMessageBox>
#include <QFrame>

PerformancePage::PerformancePage(QWidget *parent)
    : QWidget(parent)
{
    setupUi();
    loadCurrentSettings();
}

void PerformancePage::setupUi()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(24, 24, 24, 24);
    mainLayout->setSpacing(16);

    QLabel *title = new QLabel(QStringLiteral("Performance"), this);
    title->setStyleSheet(QStringLiteral("font-size: 22px; font-weight: bold; color: #e0e0ee; padding-bottom: 8px;"));
    mainLayout->addWidget(title);

    QLabel *subtitle = new QLabel(QStringLiteral("Optimize your system performance for your workload"), this);
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
        "QGroupBox::title { subcontrol-origin: margin; subcontrol-position: top left; padding: 2px 12px; }"
    );

    QString labelStyle = QStringLiteral("color: #b0b0cc; font-size: 13px;");
    QString comboStyle = QStringLiteral(
        "QComboBox {"
        "  background-color: #1a1a2e; border: 1px solid #333355; border-radius: 6px;"
        "  padding: 8px 12px; color: #e0e0ee; font-size: 13px; min-width: 220px;"
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

    QString infoLabelStyle = QStringLiteral(
        "QLabel { background-color: #1a1a2e; border: 1px solid #333355; border-radius: 8px; padding: 10px; color: #d0d0ee; font-size: 13px; }"
    );

    // Performance Profile
    QGroupBox *profileGroup = new QGroupBox(QStringLiteral("Performance Profile"), this);
    profileGroup->setStyleSheet(groupBoxStyle);
    QVBoxLayout *profileLayout = new QVBoxLayout(profileGroup);
    profileLayout->setSpacing(12);

    QHBoxLayout *profileSelectLayout = new QHBoxLayout();
    QLabel *profileLabel = new QLabel(QStringLiteral("Profile:"), profileGroup);
    profileLabel->setStyleSheet(labelStyle);
    profileSelectLayout->addWidget(profileLabel);

    m_profileCombo = new QComboBox(profileGroup);
    m_profileCombo->setStyleSheet(comboStyle);
    m_profileCombo->addItem(QStringLiteral("Balanced"));
    m_profileCombo->addItem(QStringLiteral("Performance"));
    m_profileCombo->addItem(QStringLiteral("Battery Saver"));
    m_profileCombo->addItem(QStringLiteral("Gaming"));
    profileSelectLayout->addWidget(m_profileCombo);

    profileSelectLayout->addStretch();

    m_applyButton = new QPushButton(QStringLiteral("Apply Profile"), profileGroup);
    m_applyButton->setStyleSheet(buttonStyle);
    connect(m_applyButton, &QPushButton::clicked, this, &PerformancePage::applyProfile);
    profileSelectLayout->addWidget(m_applyButton);

    profileLayout->addLayout(profileSelectLayout);

    m_profileDesc = new QLabel(QStringLiteral("Balanced: Optimizes for a balance of performance and power efficiency."), profileGroup);
    m_profileDesc->setWordWrap(true);
    m_profileDesc->setStyleSheet(QStringLiteral("color: #9999bb; font-size: 12px; padding: 4px 0;"));
    profileLayout->addWidget(m_profileDesc);

    connect(m_profileCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [this](int index) {
        QStringList descs = {
            QStringLiteral("Balanced: Optimizes for a balance of performance and power efficiency."),
            QStringLiteral("Performance: Maximum CPU frequency and I/O performance. Higher power consumption."),
            QStringLiteral("Battery Saver: Reduces CPU frequency and disables background services to extend battery life."),
            QStringLiteral("Gaming: Enables GameMode, disables CPU throttling, and optimizes for low-latency gaming.")
        };
        if (index >= 0 && index < descs.size()) {
            m_profileDesc->setText(descs[index]);
        }
    });

    mainLayout->addWidget(profileGroup);

    // System Info
    QGroupBox *infoGroup = new QGroupBox(QStringLiteral("System Information"), this);
    infoGroup->setStyleSheet(groupBoxStyle);
    QVBoxLayout *infoLayout = new QVBoxLayout(infoGroup);
    infoLayout->setSpacing(10);

    QHBoxLayout *refreshLayout = new QHBoxLayout();
    refreshLayout->addStretch();

    m_refreshButton = new QPushButton(QStringLiteral("Refresh"), infoGroup);
    m_refreshButton->setStyleSheet(buttonStyle);
    connect(m_refreshButton, &QPushButton::clicked, this, &PerformancePage::refreshInfo);
    refreshLayout->addWidget(m_refreshButton);

    infoLayout->addLayout(refreshLayout);

    QLabel *cpuLabel = new QLabel(QStringLiteral("CPU Governor:"), infoGroup);
    cpuLabel->setStyleSheet(labelStyle);
    infoLayout->addWidget(cpuLabel);

    m_cpuGovernor = new QLabel(QStringLiteral("Loading..."), infoGroup);
    m_cpuGovernor->setStyleSheet(infoLabelStyle);
    m_cpuGovernor->setTextInteractionFlags(Qt::TextSelectableByMouse);
    infoLayout->addWidget(m_cpuGovernor);

    QLabel *swapLabel = new QLabel(QStringLiteral("Swap:"), infoGroup);
    swapLabel->setStyleSheet(labelStyle);
    infoLayout->addWidget(swapLabel);

    m_swapInfo = new QLabel(QStringLiteral("Loading..."), infoGroup);
    m_swapInfo->setStyleSheet(infoLabelStyle);
    m_swapInfo->setTextInteractionFlags(Qt::TextSelectableByMouse);
    infoLayout->addWidget(m_swapInfo);

    mainLayout->addWidget(infoGroup);

    mainLayout->addStretch();
}

void PerformancePage::loadCurrentSettings()
{
    refreshInfo();

    QString profile = SystemUtils::instance()->getCurrentPowerProfile();
    int index = m_profileCombo->findText(profile, Qt::MatchInsensitive);
    if (index >= 0) {
        m_profileCombo->setCurrentIndex(index);
    }
}

void PerformancePage::applyProfile()
{
    QString profile = m_profileCombo->currentText().toLower().replace(QLatin1Char(' '), QLatin1Char('-'));

    if (profile == QStringLiteral("balanced")) {
        SystemUtils::runCommand(QStringLiteral("sudo cpupower frequency-set -g schedutil 2>/dev/null"));
        SystemUtils::runCommand(QStringLiteral("echo 1500000 | sudo tee /sys/devices/system/cpu/cpu*/cpufreq/scaling_max_freq 2>/dev/null"));
        SystemUtils::runCommand(QStringLiteral("sudo systemctl stop earlyoom 2>/dev/null"));
    } else if (profile == QStringLiteral("performance")) {
        SystemUtils::runCommand(QStringLiteral("sudo cpupower frequency-set -g performance 2>/dev/null"));
        SystemUtils::runCommand(QStringLiteral("echo 0 | sudo tee /proc/sys/vm/swappiness 2>/dev/null"));
    } else if (profile == QStringLiteral("battery-saver")) {
        SystemUtils::runCommand(QStringLiteral("sudo cpupower frequency-set -g powersave 2>/dev/null"));
        SystemUtils::runCommand(QStringLiteral("echo 60 | sudo tee /proc/sys/vm/swappiness 2>/dev/null"));
        SystemUtils::runCommand(QStringLiteral("echo 1200000 | sudo tee /sys/devices/system/cpu/cpu*/cpufreq/scaling_max_freq 2>/dev/null"));
    } else if (profile == QStringLiteral("gaming")) {
        SystemUtils::runCommand(QStringLiteral("sudo cpupower frequency-set -g performance 2>/dev/null"));
        SystemUtils::runCommand(QStringLiteral("echo 10 | sudo tee /proc/sys/vm/swappiness 2>/dev/null"));
        SystemUtils::runCommand(QStringLiteral("gamemoded -r 2>/dev/null"));
    }

    ConfigManager::instance()->writeValue(QStringLiteral("Performance"), QStringLiteral("Profile"), m_profileCombo->currentText());
    refreshInfo();

    QMessageBox::information(this, QStringLiteral("Profile Applied"),
        QStringLiteral("Performance profile changed to '%1'.").arg(m_profileCombo->currentText()));
}

void PerformancePage::refreshInfo()
{
    QString governor = SystemUtils::runCommand(QStringLiteral("cat /sys/devices/system/cpu/cpu0/cpufreq/scaling_governor 2>/dev/null || echo 'N/A'"));
    if (governor.isEmpty() || governor == QStringLiteral("N/A")) {
        governor = SystemUtils::runCommand(QStringLiteral("cat /sys/devices/system/cpu/cpu*/cpufreq/scaling_governor 2>/dev/null | head -1 || echo 'Unknown'"));
    }
    m_cpuGovernor->setText(QStringLiteral("Governor: %1").arg(governor.isEmpty() ? QStringLiteral("Unknown") : governor));

    QString swapTotal = SystemUtils::runCommand(QStringLiteral("awk '/SwapTotal/ {printf \"%.1f GB\", $2/1048576}' /proc/meminfo"));
    QString swapUsed = SystemUtils::runCommand(QStringLiteral("awk '/SwapFree/ {free=$2} /SwapTotal/ {total=$2} END {printf \"%.1f GB\", (total-free)/1048576}' /proc/meminfo"));
    QString swapiness = SystemUtils::runCommand(QStringLiteral("cat /proc/sys/vm/swappiness 2>/dev/null || echo 'N/A'"));
    m_swapInfo->setText(QStringLiteral("Total: %1 | Used: %2 | Swappiness: %3").arg(swapTotal, swapUsed, swapiness));
}
