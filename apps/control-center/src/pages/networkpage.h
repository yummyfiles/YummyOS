#pragma once

#include <QWidget>
#include <QLabel>
#include <QListWidget>
#include <QPushButton>
#include <QCheckBox>
#include <QComboBox>

class NetworkPage : public QWidget
{
    Q_OBJECT

public:
    explicit NetworkPage(QWidget *parent = nullptr);

private slots:
    void toggleWifi();
    void refreshConnections();
    void openVpnSettings();

private:
    void setupUi();
    void loadNetworkInfo();

    QListWidget *m_connectionsList;
    QCheckBox *m_wifiCheck;
    QLabel *m_statusLabel;
    QPushButton *m_refreshBtn;
    QPushButton *m_vpnBtn;
};
