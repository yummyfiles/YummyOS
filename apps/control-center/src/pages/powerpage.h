#pragma once

#include <QWidget>
#include <QComboBox>
#include <QPushButton>
#include <QLabel>
#include <QSpinBox>
#include <QGroupBox>

class PowerPage : public QWidget
{
    Q_OBJECT

public:
    explicit PowerPage(QWidget *parent = nullptr);

private slots:
    void applyPowerProfile();
    void applySettings();

private:
    void setupUi();
    void loadCurrentSettings();

    QComboBox *m_powerProfileCombo;
    QSpinBox *m_autoSuspendSpin;
    QSpinBox *m_screenLockSpin;
    QComboBox *m_lidCloseCombo;
    QComboBox *m_lowBatteryCombo;
    QPushButton *m_applyButton;
    QLabel *m_currentProfileLabel;
};
