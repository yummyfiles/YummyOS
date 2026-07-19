#include "gamingpage.h"
#include "../utils/configmanager.h"
#include "../utils/systemutils.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QLabel>
#include <QCheckBox>
#include <QMessageBox>
#include <QProcess>
#include <QFrame>

GamingPage::GamingPage(QWidget *parent)
    : QWidget(parent)
{
    setupUi();
    loadCurrentSettings();
}

void GamingPage::setupUi()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(24, 24, 24, 24);
    mainLayout->setSpacing(16);

    QLabel *title = new QLabel(QStringLiteral("Gaming"), this);
    title->setStyleSheet(QStringLiteral("font-size: 22px; font-weight: bold; color: #e0e0ee; padding-bottom: 8px;"));
    mainLayout->addWidget(title);

    QLabel *subtitle = new QLabel(QStringLiteral("Optimize your system for gaming"), this);
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
        "  color: #7B2CBF;"
        "}"
        "QGroupBox::title { subcontrol-origin: margin; subcontrol-position: top left; padding: 2px 12px; }"
    );

    QString checkboxStyle = QStringLiteral(
        "QCheckBox { color: #d0d0ee; font-size: 13px; spacing: 8px; }"
        "QCheckBox::indicator {"
        "  width: 20px; height: 20px; border-radius: 4px; border: 2px solid #555577;"
        "  background-color: #1a1a2e;"
        "}"
        "QCheckBox::indicator:checked { background-color: #4CC9F0; border-color: #4CC9F0; }"
        "QCheckBox::indicator:hover { border-color: #4CC9F0; }"
    );

    QString statusStyle = QStringLiteral(
        "QLabel { color: #4CC9F0; font-size: 12px; font-weight: bold; padding: 2px 0; }"
    );

    QString buttonStyle = QStringLiteral(
        "QPushButton {"
        "  background-color: #7B2CBF; color: #ffffff; border: none; border-radius: 8px;"
        "  padding: 10px 24px; font-size: 13px; font-weight: bold;"
        "}"
        "QPushButton:hover { background-color: #9333d9; }"
        "QPushButton:pressed { background-color: #6a1faa; }"
    );

    QString sliderStyle = QStringLiteral(
        "QSlider::groove:horizontal {"
        "  border: none; height: 6px; background: #333355; border-radius: 3px;"
        "}"
        "QSlider::handle:horizontal {"
        "  background: #7B2CBF; width: 16px; height: 16px; margin: -5px 0;"
        "  border-radius: 8px;"
        "}"
        "QSlider::handle:horizontal:hover { background: #9333d9; }"
        "QSlider::sub-page:horizontal { background: #7B2CBF; border-radius: 3px; }"
    );

    QString infoLabelStyle = QStringLiteral(
        "QLabel { background-color: #1a1a2e; border: 1px solid #333355; border-radius: 8px; padding: 12px; color: #d0d0ee; font-size: 12px; }"
    );

    // GameMode
    QGroupBox *gameModeGroup = new QGroupBox(QStringLiteral("GameMode"), this);
    gameModeGroup->setStyleSheet(groupBoxStyle);
    QVBoxLayout *gmLayout = new QVBoxLayout(gameModeGroup);
    gmLayout->setSpacing(10);

    m_gameModeCheck = new QCheckBox(QStringLiteral("Enable GameMode (gamemoded)"), gameModeGroup);
    m_gameModeCheck->setStyleSheet(checkboxStyle);
    connect(m_gameModeCheck, &QCheckBox::toggled, this, &GamingPage::toggleGameMode);
    gmLayout->addWidget(m_gameModeCheck);

    m_gameModeStatus = new QLabel(QStringLiteral("Status: Checking..."), gameModeGroup);
    m_gameModeStatus->setStyleSheet(statusStyle);
    gmLayout->addWidget(m_gameModeStatus);

    QLabel *gmDesc = new QLabel(QStringLiteral(
        "GameMode automatically optimizes CPU governor, I/O priority, and scheduling "
        "when games are detected. Requires gamemoded to be installed."
    ), gameModeGroup);
    gmDesc->setWordWrap(true);
    gmDesc->setStyleSheet(QStringLiteral("color: #8888aa; font-size: 12px;"));
    gmLayout->addWidget(gmDesc);

    mainLayout->addWidget(gameModeGroup);

    // MangoHud
    QGroupBox *mangoGroup = new QGroupBox(QStringLiteral("MangoHud"), this);
    mangoGroup->setStyleSheet(groupBoxStyle);
    QVBoxLayout *mangoLayout = new QVBoxLayout(mangoGroup);
    mangoLayout->setSpacing(10);

    m_mangoHudCheck = new QCheckBox(QStringLiteral("Enable MangoHud overlay"), mangoGroup);
    m_mangoHudCheck->setStyleSheet(checkboxStyle);
    connect(m_mangoHudCheck, &QCheckBox::toggled, this, &GamingPage::toggleMangoHud);
    mangoLayout->addWidget(m_mangoHudCheck);

    m_mangoHudStatus = new QLabel(QStringLiteral("Status: Checking..."), mangoGroup);
    m_mangoHudStatus->setStyleSheet(statusStyle);
    mangoLayout->addWidget(m_mangoHudStatus);

    QLabel *mangoDesc = new QLabel(QStringLiteral(
        "MangoHud displays an FPS counter, CPU/GPU usage, temperatures, and other "
        "performance metrics as an overlay in games."
    ), mangoGroup);
    mangoDesc->setWordWrap(true);
    mangoDesc->setStyleSheet(QStringLiteral("color: #8888aa; font-size: 12px;"));
    mangoLayout->addWidget(mangoDesc);

    mainLayout->addWidget(mangoGroup);

    // Steam Integration & Shader Cache
    QGroupBox *steamGroup = new QGroupBox(QStringLiteral("Steam Integration"), this);
    steamGroup->setStyleSheet(groupBoxStyle);
    QVBoxLayout *steamLayout = new QVBoxLayout(steamGroup);
    steamLayout->setSpacing(10);

    m_steamCheck = new QCheckBox(QStringLiteral("Enable Steam Proton integration"), steamGroup);
    m_steamCheck->setStyleSheet(checkboxStyle);
    steamLayout->addWidget(m_steamCheck);

    m_acoCheck = new QCheckBox(QStringLiteral("Enable ACO shader compiler (better performance)"), steamGroup);
    m_acoCheck->setStyleSheet(checkboxStyle);
    steamLayout->addWidget(m_acoCheck);

    QLabel *shaderLabel = new QLabel(QStringLiteral("Shader Pre-caching Size (GB):"), steamGroup);
    shaderLabel->setStyleSheet(QStringLiteral("color: #b0b0cc; font-size: 13px; padding-top: 8px;"));
    steamLayout->addWidget(shaderLabel);

    QHBoxLayout *shaderLayout = new QHBoxLayout();
    m_shaderCacheSlider = new QSlider(Qt::Horizontal, steamGroup);
    m_shaderCacheSlider->setStyleSheet(sliderStyle);
    m_shaderCacheSlider->setRange(1, 50);
    m_shaderCacheSlider->setValue(10);
    shaderLayout->addWidget(m_shaderCacheSlider);

    m_shaderCacheLabel = new QLabel(QStringLiteral("10 GB"), steamGroup);
    m_shaderCacheLabel->setStyleSheet(QStringLiteral("color: #d0d0ee; font-size: 13px; min-width: 60px;"));
    shaderLayout->addWidget(m_shaderCacheLabel);

    connect(m_shaderCacheSlider, &QSlider::valueChanged, this, [this](int value) {
        m_shaderCacheLabel->setText(QStringLiteral("%1 GB").arg(value));
    });

    steamLayout->addLayout(shaderLayout);

    QLabel *steamDesc = new QLabel(QStringLiteral(
        "ACO is Valve's shader compiler for Vulkan that provides faster shader compilation "
        "and reduced stuttering in games. Recommended for AMD GPUs."
    ), steamGroup);
    steamDesc->setWordWrap(true);
    steamDesc->setStyleSheet(QStringLiteral("color: #8888aa; font-size: 12px;"));
    steamLayout->addWidget(steamDesc);

    mainLayout->addWidget(steamGroup);

    // Apply button
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();
    m_applyButton = new QPushButton(QStringLiteral("Save Settings"), this);
    m_applyButton->setStyleSheet(buttonStyle);
    connect(m_applyButton, &QPushButton::clicked, this, &GamingPage::applySettings);
    buttonLayout->addWidget(m_applyButton);
    mainLayout->addLayout(buttonLayout);

    // Info
    QFrame *infoFrame = new QFrame(this);
    infoFrame->setStyleSheet(infoLabelStyle);
    QVBoxLayout *infoLayout = new QVBoxLayout(infoFrame);
    QLabel *infoTitle = new QLabel(QStringLiteral("Gaming Optimizations"), infoFrame);
    infoTitle->setStyleSheet(QStringLiteral("color: #4CC9F0; font-weight: bold; font-size: 13px;"));
    infoLayout->addWidget(infoTitle);
    QLabel *infoText = new QLabel(QStringLiteral(
        "YummyOS includes several out-of-the-box gaming optimizations:\n"
        "• Kernel with preemption enabled for lower input latency\n"
        "• Mesa and Vulkan drivers pre-installed\n"
        "• Steam and Proton available in the package manager\n"
        "• GameMode and MangoHud available for installation"
    ), infoFrame);
    infoText->setWordWrap(true);
    infoText->setStyleSheet(QStringLiteral("color: #b0b0cc; font-size: 12px;"));
    infoLayout->addWidget(infoText);
    mainLayout->addWidget(infoFrame);

    mainLayout->addStretch();
}

void GamingPage::loadCurrentSettings()
{
    bool gameModeEnabled = SystemUtils::instance()->isServiceActive(QStringLiteral("gamemedaemon"));
    m_gameModeCheck->setChecked(gameModeEnabled);
    m_gameModeStatus->setText(gameModeEnabled ? QStringLiteral("Status: Active") : QStringLiteral("Status: Inactive"));
    m_gameModeStatus->setStyleSheet(gameModeEnabled
        ? QStringLiteral("color: #2ecc71; font-size: 12px; font-weight: bold;")
        : QStringLiteral("color: #e74c3c; font-size: 12px; font-weight: bold;"));

    bool mangoInstalled = SystemUtils::instance()->isMangoHudInstalled();
    m_mangoHudStatus->setText(mangoInstalled ? QStringLiteral("Status: Installed") : QStringLiteral("Status: Not installed"));
    m_mangoHudStatus->setStyleSheet(mangoInstalled
        ? QStringLiteral("color: #2ecc71; font-size: 12px; font-weight: bold;")
        : QStringLiteral("color: #f39c12; font-size: 12px; font-weight: bold;"));

    ConfigManager *cm = ConfigManager::instance();
    m_steamCheck->setChecked(cm->readValue(QStringLiteral("Gaming"), QStringLiteral("SteamIntegration"), true).toBool());
    m_acoCheck->setChecked(cm->readValue(QStringLiteral("Gaming"), QStringLiteral("ACOCompiler"), true).toBool());
    m_shaderCacheSlider->setValue(cm->readValue(QStringLiteral("Gaming"), QStringLiteral("ShaderCacheSize"), 10).toInt());
}

void GamingPage::toggleGameMode()
{
    bool enabled = m_gameModeCheck->isChecked();
    if (enabled) {
        SystemUtils::enableService(QStringLiteral("gamemedaemon"));
    } else {
        SystemUtils::disableService(QStringLiteral("gamemedaemon"));
    }
    m_gameModeStatus->setText(enabled ? QStringLiteral("Status: Active") : QStringLiteral("Status: Inactive"));
    m_gameModeStatus->setStyleSheet(enabled
        ? QStringLiteral("color: #2ecc71; font-size: 12px; font-weight: bold;")
        : QStringLiteral("color: #e74c3c; font-size: 12px; font-weight: bold;"));
}

void GamingPage::toggleMangoHud()
{
    bool enabled = m_mangoHudCheck->isChecked();
    ConfigManager::instance()->writeValue(QStringLiteral("Gaming"), QStringLiteral("MangoHud"), enabled);

    if (enabled) {
        SystemUtils::runCommand(QStringLiteral("echo 'mangohud' > ~/.config/environment.d/mangohud.conf 2>/dev/null"));
    } else {
        QFile::remove(QDir::homePath() + QStringLiteral("/.config/environment.d/mangohud.conf"));
    }
}

void GamingPage::toggleSteamIntegration()
{
    ConfigManager::instance()->writeValue(QStringLiteral("Gaming"), QStringLiteral("SteamIntegration"), m_steamCheck->isChecked());
}

void GamingPage::toggleACOCompiler()
{
    bool enabled = m_acoCheck->isChecked();
    ConfigManager::instance()->writeValue(QStringLiteral("Gaming"), QStringLiteral("ACOCompiler"), enabled);

    if (enabled) {
        SystemUtils::runCommand(QStringLiteral("echo 'RADV_PERFTEST=aco' > ~/.config/environment.d/radv.conf 2>/dev/null"));
    } else {
        QFile::remove(QDir::homePath() + QStringLiteral("/.config/environment.d/radv.conf"));
    }
}

void GamingPage::applySettings()
{
    ConfigManager *cm = ConfigManager::instance();
    cm->writeValue(QStringLiteral("Gaming"), QStringLiteral("SteamIntegration"), m_steamCheck->isChecked());
    cm->writeValue(QStringLiteral("Gaming"), QStringLiteral("ACOCompiler"), m_acoCheck->isChecked());
    cm->writeValue(QStringLiteral("Gaming"), QStringLiteral("ShaderCacheSize"), m_shaderCacheSlider->value());

    QMessageBox::information(this, QStringLiteral("Settings Saved"),
        QStringLiteral("Gaming settings have been saved. Some changes may require a logout."));
}
