#pragma once

#include <QWidget>
#include <QComboBox>
#include <QPushButton>
#include <QLabel>
#include <QCheckBox>
#include <QGroupBox>
#include <QSlider>

class GamingPage : public QWidget
{
    Q_OBJECT

public:
    explicit GamingPage(QWidget *parent = nullptr);

private slots:
    void toggleGameMode();
    void toggleMangoHud();
    void toggleSteamIntegration();
    void toggleACOCompiler();
    void applySettings();

private:
    void setupUi();
    void loadCurrentSettings();

    QCheckBox *m_gameModeCheck;
    QCheckBox *m_mangoHudCheck;
    QCheckBox *m_steamCheck;
    QCheckBox *m_acoCheck;
    QSlider *m_shaderCacheSlider;
    QLabel *m_shaderCacheLabel;
    QLabel *m_gameModeStatus;
    QLabel *m_mangoHudStatus;
    QPushButton *m_applyButton;
};
