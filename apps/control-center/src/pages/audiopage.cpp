#include "audiopage.h"
#include "../utils/systemutils.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QLabel>
#include <QProcess>
#include <QMessageBox>

AudioPage::AudioPage(QWidget *parent)
    : QWidget(parent)
{
    setupUi();
    loadCurrentSettings();
}

void AudioPage::setupUi()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(24, 24, 24, 24);
    mainLayout->setSpacing(16);

    QLabel *title = new QLabel(QStringLiteral("Audio"), this);
    title->setStyleSheet(QStringLiteral("font-size: 22px; font-weight: bold; color: #e0e0ee; padding-bottom: 8px;"));
    mainLayout->addWidget(title);

    QLabel *subtitle = new QLabel(QStringLiteral("Manage audio devices and volume"), this);
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
        "  padding: 8px 12px; color: #e0e0ee; font-size: 13px; min-width: 300px;"
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

    QString sliderStyle = QStringLiteral(
        "QSlider::groove:horizontal {"
        "  border: none; height: 8px; background: #333355; border-radius: 4px;"
        "}"
        "QSlider::handle:horizontal {"
        "  background: #4CC9F0; width: 20px; height: 20px; margin: -6px 0;"
        "  border-radius: 10px;"
        "}"
        "QSlider::handle:horizontal:hover { background: #6dd5f5; }"
        "QSlider::sub-page:horizontal { background: #4CC9F0; border-radius: 4px; }"
    );

    QString checkboxStyle = QStringLiteral(
        "QCheckBox { color: #d0d0ee; font-size: 13px; spacing: 8px; }"
        "QCheckBox::indicator {"
        "  width: 20px; height: 20px; border-radius: 4px; border: 2px solid #555577; background-color: #1a1a2e;"
        "}"
        "QCheckBox::indicator:checked { background-color: #4CC9F0; border-color: #4CC9F0; }"
        "QCheckBox::indicator:hover { border-color: #4CC9F0; }"
    );

    // Volume
    QGroupBox *volumeGroup = new QGroupBox(QStringLiteral("Volume"), this);
    volumeGroup->setStyleSheet(groupBoxStyle);
    QVBoxLayout *volLayout = new QVBoxLayout(volumeGroup);
    volLayout->setSpacing(12);

    QHBoxLayout *volSliderLayout = new QHBoxLayout();
    QLabel *volIconLabel = new QLabel(QStringLiteral("Vol:"), volumeGroup);
    volIconLabel->setStyleSheet(labelStyle);
    volSliderLayout->addWidget(volIconLabel);

    m_volumeSlider = new QSlider(Qt::Horizontal, volumeGroup);
    m_volumeSlider->setStyleSheet(sliderStyle);
    m_volumeSlider->setRange(0, 100);
    m_volumeSlider->setValue(75);
    connect(m_volumeSlider, &QSlider::valueChanged, this, &AudioPage::changeVolume);
    volSliderLayout->addWidget(m_volumeSlider, 1);

    m_volumeLabel = new QLabel(QStringLiteral("75%"), volumeGroup);
    m_volumeLabel->setStyleSheet(QStringLiteral("color: #e0e0ee; font-size: 14px; font-weight: bold; min-width: 40px;"));
    connect(m_volumeSlider, &QSlider::valueChanged, this, [this](int value) {
        m_volumeLabel->setText(QStringLiteral("%1%").arg(value));
    });
    volSliderLayout->addWidget(m_volumeLabel);

    volLayout->addLayout(volSliderLayout);

    m_muteCheck = new QCheckBox(QStringLiteral("Mute"), volumeGroup);
    m_muteCheck->setStyleSheet(checkboxStyle);
    connect(m_muteCheck, &QCheckBox::toggled, this, &AudioPage::toggleMute);
    volLayout->addWidget(m_muteCheck);

    mainLayout->addWidget(volumeGroup);

    // Output Device
    QGroupBox *outputGroup = new QGroupBox(QStringLiteral("Output Device"), this);
    outputGroup->setStyleSheet(groupBoxStyle);
    QVBoxLayout *outLayout = new QVBoxLayout(outputGroup);

    QHBoxLayout *outSelectLayout = new QHBoxLayout();
    QLabel *outLabel = new QLabel(QStringLiteral("Device:"), outputGroup);
    outLabel->setStyleSheet(labelStyle);
    outSelectLayout->addWidget(outLabel);

    m_outputDeviceCombo = new QComboBox(outputGroup);
    m_outputDeviceCombo->setStyleSheet(comboStyle);
    connect(m_outputDeviceCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &AudioPage::changeOutputDevice);
    outSelectLayout->addWidget(m_outputDeviceCombo, 1);

    outLayout->addLayout(outSelectLayout);
    mainLayout->addWidget(outputGroup);

    // Input Device
    QGroupBox *inputGroup = new QGroupBox(QStringLiteral("Input Device"), this);
    inputGroup->setStyleSheet(groupBoxStyle);
    QVBoxLayout *inLayout = new QVBoxLayout(inputGroup);

    QHBoxLayout *inSelectLayout = new QHBoxLayout();
    QLabel *inLabel = new QLabel(QStringLiteral("Device:"), inputGroup);
    inLabel->setStyleSheet(labelStyle);
    inSelectLayout->addWidget(inLabel);

    m_inputDeviceCombo = new QComboBox(inputGroup);
    m_inputDeviceCombo->setStyleSheet(comboStyle);
    connect(m_inputDeviceCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &AudioPage::changeInputDevice);
    inSelectLayout->addWidget(m_inputDeviceCombo, 1);

    inLayout->addLayout(inSelectLayout);
    mainLayout->addWidget(inputGroup);

    // Refresh
    QHBoxLayout *refreshLayout = new QHBoxLayout();
    refreshLayout->addStretch();
    m_refreshBtn = new QPushButton(QStringLiteral("Refresh Devices"), this);
    m_refreshBtn->setStyleSheet(buttonStyle);
    connect(m_refreshBtn, &QPushButton::clicked, this, &AudioPage::refreshDevices);
    refreshLayout->addWidget(m_refreshBtn);
    mainLayout->addLayout(refreshLayout);

    mainLayout->addStretch();
}

void AudioPage::loadCurrentSettings()
{
    int vol = SystemUtils::instance()->getVolume();
    m_volumeSlider->setValue(vol);
    m_muteCheck->setChecked(SystemUtils::instance()->isMuted());
    refreshDevices();
}

void AudioPage::changeVolume(int value)
{
    SystemUtils::instance()->setVolume(value);
}

void AudioPage::changeOutputDevice(int index)
{
    if (index >= 0) {
        QString device = m_outputDeviceCombo->itemText(index);
        SystemUtils::instance()->setDefaultAudioSink(device);
    }
}

void AudioPage::changeInputDevice(int index)
{
    if (index >= 0) {
        QString device = m_inputDeviceCombo->itemText(index);
        SystemUtils::instance()->setDefaultAudioSource(device);
    }
}

void AudioPage::toggleMute()
{
    SystemUtils::instance()->toggleMute();
}

void AudioPage::refreshDevices()
{
    QString currentSink = SystemUtils::instance()->getCurrentAudioSink();
    QString currentSource = SystemUtils::instance()->getCurrentAudioSource();

    m_outputDeviceCombo->clear();
    m_inputDeviceCombo->clear();

    QStringList sinks = SystemUtils::instance()->getAudioSinks();
    int sinkIndex = 0;
    for (const QString &sink : sinks) {
        QString name = sink.section(QLatin1Char('\t'), 1, 1);
        if (name.isEmpty()) name = sink;
        m_outputDeviceCombo->addItem(name);
        if (sink.contains(currentSink)) {
            sinkIndex = m_outputDeviceCombo->count() - 1;
        }
    }
    if (m_outputDeviceCombo->count() > 0) {
        m_outputDeviceCombo->setCurrentIndex(sinkIndex);
    }

    QStringList sources = SystemUtils::instance()->getAudioSources();
    int sourceIndex = 0;
    for (const QString &source : sources) {
        QString name = source.section(QLatin1Char('\t'), 1, 1);
        if (name.isEmpty()) name = source;
        m_inputDeviceCombo->addItem(name);
        if (source.contains(currentSource)) {
            sourceIndex = m_inputDeviceCombo->count() - 1;
        }
    }
    if (m_inputDeviceCombo->count() > 0) {
        m_inputDeviceCombo->setCurrentIndex(sourceIndex);
    }
}
