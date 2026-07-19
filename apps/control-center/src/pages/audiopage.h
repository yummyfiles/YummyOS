#pragma once

#include <QWidget>
#include <QComboBox>
#include <QPushButton>
#include <QLabel>
#include <QSlider>
#include <QCheckBox>

class AudioPage : public QWidget
{
    Q_OBJECT

public:
    explicit AudioPage(QWidget *parent = nullptr);

private slots:
    void changeVolume(int value);
    void changeOutputDevice(int index);
    void changeInputDevice(int index);
    void toggleMute();
    void refreshDevices();

private:
    void setupUi();
    void loadCurrentSettings();

    QSlider *m_volumeSlider;
    QLabel *m_volumeLabel;
    QComboBox *m_outputDeviceCombo;
    QComboBox *m_inputDeviceCombo;
    QCheckBox *m_muteCheck;
    QPushButton *m_refreshBtn;
};
