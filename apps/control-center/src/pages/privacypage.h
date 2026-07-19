#pragma once

#include <QWidget>
#include <QLabel>
#include <QCheckBox>
#include <QPushButton>
#include <QGroupBox>

class PrivacyPage : public QWidget
{
    Q_OBJECT

public:
    explicit PrivacyPage(QWidget *parent = nullptr);

private slots:
    void toggleLocationServices();
    void clearHistory();
    void clearCache();

private:
    void setupUi();
    void loadCurrentSettings();

    QCheckBox *m_locationCheck;
    QLabel *m_telemetryStatus;
    QLabel *m_micStatus;
    QLabel *m_cameraStatus;
    QPushButton *m_clearHistoryBtn;
    QPushButton *m_clearCacheBtn;
};
