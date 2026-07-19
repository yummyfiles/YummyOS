#pragma once

#include <QWidget>
#include <QComboBox>
#include <QPushButton>
#include <QLabel>
#include <QCheckBox>
#include <QSpinBox>
#include <QTimeEdit>

class DisplayPage : public QWidget
{
    Q_OBJECT

public:
    explicit DisplayPage(QWidget *parent = nullptr);

private slots:
    void applyResolution();
    void toggleNightLight();
    void applyScaling();

private:
    void setupUi();
    void loadCurrentSettings();

    QComboBox *m_resolutionCombo;
    QComboBox *m_refreshRateCombo;
    QCheckBox *m_nightLightCheck;
    QTimeEdit *m_nightLightStart;
    QTimeEdit *m_nightLightEnd;
    QSpinBox *m_scalingSpin;
    QPushButton *m_applyResolutionBtn;
    QPushButton *m_applyScalingBtn;
};
