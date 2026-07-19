#pragma once

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QCheckBox>
#include <QComboBox>

class UpdatesPage : public QWidget
{
    Q_OBJECT

public:
    explicit UpdatesPage(QWidget *parent = nullptr);

private slots:
    void checkForUpdates();
    void toggleAutoUpdate();
    void changeUpdateChannel();

private:
    void setupUi();
    void loadUpdateSettings();

    QLabel *m_lastCheckedLabel;
    QLabel *m_updateStatus;
    QCheckBox *m_autoUpdateCheck;
    QComboBox *m_channelCombo;
    QPushButton *m_checkBtn;
    QPushButton *m_applyBtn;
};
