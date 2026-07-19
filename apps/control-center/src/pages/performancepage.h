#pragma once

#include <QWidget>
#include <QComboBox>
#include <QPushButton>
#include <QLabel>
#include <QSlider>

class PerformancePage : public QWidget
{
    Q_OBJECT

public:
    explicit PerformancePage(QWidget *parent = nullptr);

private slots:
    void applyProfile();
    void refreshInfo();

private:
    void setupUi();
    void loadCurrentSettings();

    QComboBox *m_profileCombo;
    QLabel *m_profileDesc;
    QLabel *m_cpuGovernor;
    QLabel *m_swapInfo;
    QPushButton *m_applyButton;
    QPushButton *m_refreshButton;
};
