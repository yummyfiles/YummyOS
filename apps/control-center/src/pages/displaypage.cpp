#include "displaypage.h"
#include "../utils/configmanager.h"
#include "../utils/systemutils.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QLabel>
#include <QProcess>
#include <QMessageBox>

DisplayPage::DisplayPage(QWidget *parent)
    : QWidget(parent)
{
    setupUi();
    loadCurrentSettings();
}

void DisplayPage::setupUi()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(24, 24, 24, 24);
    mainLayout->setSpacing(16);

    QLabel *title = new QLabel(QStringLiteral("Display"), this);
    title->setStyleSheet(QStringLiteral("font-size: 22px; font-weight: bold; color: #e0e0ee; padding-bottom: 8px;"));
    mainLayout->addWidget(title);

    QLabel *subtitle = new QLabel(QStringLiteral("Configure display resolution, refresh rate, and scaling"), this);
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

    QString checkboxStyle = QStringLiteral(
        "QCheckBox { color: #d0d0ee; font-size: 13px; spacing: 8px; }"
        "QCheckBox::indicator {"
        "  width: 20px; height: 20px; border-radius: 4px; border: 2px solid #555577; background-color: #1a1a2e;"
        "}"
        "QCheckBox::indicator:checked { background-color: #4CC9F0; border-color: #4CC9F0; }"
        "QCheckBox::indicator:hover { border-color: #4CC9F0; }"
    );

    QString spinBoxStyle = QStringLiteral(
        "QSpinBox {"
        "  background-color: #1a1a2e; border: 1px solid #333355; border-radius: 6px;"
        "  padding: 8px 12px; color: #e0e0ee; font-size: 13px;"
        "}"
        "QSpinBox:hover { border-color: #4CC9F0; }"
    );

    QString timeEditStyle = QStringLiteral(
        "QTimeEdit {"
        "  background-color: #1a1a2e; border: 1px solid #333355; border-radius: 6px;"
        "  padding: 8px 12px; color: #e0e0ee; font-size: 13px;"
        "}"
        "QTimeEdit:hover { border-color: #4CC9F0; }"
    );

    // Resolution
    QGroupBox *resolutionGroup = new QGroupBox(QStringLiteral("Resolution & Refresh Rate"), this);
    resolutionGroup->setStyleSheet(groupBoxStyle);
    QVBoxLayout *resLayout = new QVBoxLayout(resolutionGroup);
    resLayout->setSpacing(12);

    QHBoxLayout *resRow1 = new QHBoxLayout();
    QLabel *resLabel = new QLabel(QStringLiteral("Resolution:"), resolutionGroup);
    resLabel->setStyleSheet(labelStyle);
    resRow1->addWidget(resLabel);

    m_resolutionCombo = new QComboBox(resolutionGroup);
    m_resolutionCombo->setStyleSheet(comboStyle);
    m_resolutionCombo->addItems({
        QStringLiteral("3840x2160"), QStringLiteral("2560x1440"),
        QStringLiteral("1920x1080"), QStringLiteral("1680x1050"),
        QStringLiteral("1600x900"), QStringLiteral("1366x768"),
        QStringLiteral("1280x720")
    });
    resRow1->addWidget(m_resolutionCombo);

    QLabel *refreshLabel = new QLabel(QStringLiteral("Refresh Rate:"), resolutionGroup);
    refreshLabel->setStyleSheet(labelStyle);
    resRow1->addWidget(refreshLabel);

    m_refreshRateCombo = new QComboBox(resolutionGroup);
    m_refreshRateCombo->setStyleSheet(comboStyle);
    m_refreshRateCombo->addItems({
        QStringLiteral("60 Hz"), QStringLiteral("75 Hz"),
        QStringLiteral("120 Hz"), QStringLiteral("144 Hz"),
        QStringLiteral("165 Hz"), QStringLiteral("240 Hz")
    });
    resRow1->addWidget(m_refreshRateCombo);

    resRow1->addStretch();

    m_applyResolutionBtn = new QPushButton(QStringLiteral("Apply"), resolutionGroup);
    m_applyResolutionBtn->setStyleSheet(buttonStyle);
    connect(m_applyResolutionBtn, &QPushButton::clicked, this, &DisplayPage::applyResolution);
    resRow1->addWidget(m_applyResolutionBtn);

    resLayout->addLayout(resRow1);
    mainLayout->addWidget(resolutionGroup);

    // Scaling
    QGroupBox *scalingGroup = new QGroupBox(QStringLiteral("Display Scaling"), this);
    scalingGroup->setStyleSheet(groupBoxStyle);
    QHBoxLayout *scaleLayout = new QHBoxLayout(scalingGroup);
    scaleLayout->setSpacing(12);

    QLabel *scaleLabel = new QLabel(QStringLiteral("Scale Factor:"), scalingGroup);
    scaleLabel->setStyleSheet(labelStyle);
    scaleLayout->addWidget(scaleLabel);

    m_scalingSpin = new QSpinBox(scalingGroup);
    m_scalingSpin->setStyleSheet(spinBoxStyle);
    m_scalingSpin->setRange(100, 300);
    m_scalingSpin->setSingleStep(25);
    m_scalingSpin->setValue(100);
    m_scalingSpin->setSuffix(QStringLiteral("%"));
    scaleLayout->addWidget(m_scalingSpin);

    scaleLayout->addStretch();

    m_applyScalingBtn = new QPushButton(QStringLiteral("Apply Scaling"), scalingGroup);
    m_applyScalingBtn->setStyleSheet(buttonStyle);
    connect(m_applyScalingBtn, &QPushButton::clicked, this, &DisplayPage::applyScaling);
    scaleLayout->addWidget(m_applyScalingBtn);

    mainLayout->addWidget(scalingGroup);

    // Night Light
    QGroupBox *nightLightGroup = new QGroupBox(QStringLiteral("Night Light"), this);
    nightLightGroup->setStyleSheet(groupBoxStyle);
    QVBoxLayout *nlLayout = new QVBoxLayout(nightLightGroup);
    nlLayout->setSpacing(12);

    m_nightLightCheck = new QCheckBox(QStringLiteral("Enable Night Light (reduce blue light)"), nightLightGroup);
    m_nightLightCheck->setStyleSheet(checkboxStyle);
    connect(m_nightLightCheck, &QCheckBox::toggled, this, &DisplayPage::toggleNightLight);
    nlLayout->addWidget(m_nightLightCheck);

    QHBoxLayout *scheduleLayout = new QHBoxLayout();
    QLabel *fromLabel = new QLabel(QStringLiteral("From:"), nightLightGroup);
    fromLabel->setStyleSheet(labelStyle);
    scheduleLayout->addWidget(fromLabel);

    m_nightLightStart = new QTimeEdit(nightLightGroup);
    m_nightLightStart->setStyleSheet(timeEditStyle);
    m_nightLightStart->setTime(QTime(20, 0));
    scheduleLayout->addWidget(m_nightLightStart);

    QLabel *toLabel = new QLabel(QStringLiteral("To:"), nightLightGroup);
    toLabel->setStyleSheet(labelStyle);
    scheduleLayout->addWidget(toLabel);

    m_nightLightEnd = new QTimeEdit(nightLightGroup);
    m_nightLightEnd->setStyleSheet(timeEditStyle);
    m_nightLightEnd->setTime(QTime(7, 0));
    scheduleLayout->addWidget(m_nightLightEnd);

    scheduleLayout->addStretch();
    nlLayout->addLayout(scheduleLayout);

    QLabel *nlDesc = new QLabel(QStringLiteral(
        "Night Light gradually reduces blue light output to reduce eye strain "
        "and improve sleep quality during nighttime usage."
    ), nightLightGroup);
    nlDesc->setWordWrap(true);
    nlDesc->setStyleSheet(QStringLiteral("color: #8888aa; font-size: 12px;"));
    nlLayout->addWidget(nlDesc);

    mainLayout->addWidget(nightLightGroup);

    mainLayout->addStretch();
}

void DisplayPage::loadCurrentSettings()
{
    ConfigManager *cm = ConfigManager::instance();
    bool nightLight = cm->readKDEConfig(QStringLiteral("kwinrc"), QStringLiteral("NightLight"), QStringLiteral("Enabled"), false).toBool();
    m_nightLightCheck->setChecked(nightLight);
}

void DisplayPage::applyResolution()
{
    QString resolution = m_resolutionCombo->currentText();
    QString refresh = m_refreshRateCombo->currentText().split(QLatin1Char(' ')).first();
    QStringList displays = SystemUtils::instance()->getDisplays();

    QString mode = QStringLiteral("%1@%2").arg(resolution, refresh);
    for (const QString &display : displays) {
        SystemUtils::runCommand(QStringLiteral("xrandr --output %1 --mode %2 2>/dev/null").arg(display.trimmed(), mode));
    }

    QMessageBox::information(this, QStringLiteral("Resolution Applied"),
        QStringLiteral("Resolution changed to %1@%2Hz.").arg(resolution, refresh));
}

void DisplayPage::toggleNightLight()
{
    bool enabled = m_nightLightCheck->isChecked();
    ConfigManager *cm = ConfigManager::instance();
    cm->writeKDEConfig(QStringLiteral("kwinrc"), QStringLiteral("NightLight"), QStringLiteral("Enabled"), enabled);
}

void DisplayPage::applyScaling()
{
    int scale = m_scalingSpin->value();
    double factor = scale / 100.0;

    ConfigManager *cm = ConfigManager::instance();
    cm->writeKDEConfig(QStringLiteral("kdeglobals"), QStringLiteral("KDE"), QStringLiteral("forceFontDPI"), scale);

    QStringList displays = SystemUtils::instance()->getDisplays();
    for (const QString &display : displays) {
        SystemUtils::runCommand(QStringLiteral("xrandr --output %1 --scale %2x%2 2>/dev/null").arg(display.trimmed()).arg(factor, 0, 'f', 1));
    }

    QMessageBox::information(this, QStringLiteral("Scaling Applied"),
        QStringLiteral("Display scaling set to %1%.").arg(scale));
}
