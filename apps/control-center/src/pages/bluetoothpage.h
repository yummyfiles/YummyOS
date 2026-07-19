#pragma once

#include <QWidget>
#include <QListWidget>
#include <QPushButton>
#include <QLabel>
#include <QCheckBox>

class BluetoothPage : public QWidget
{
    Q_OBJECT

public:
    explicit BluetoothPage(QWidget *parent = nullptr);

private slots:
    void toggleBluetooth();
    void scanForDevices();
    void pairDevice();
    void unpairDevice();
    void refreshDevices();

private:
    void setupUi();
    void loadDevices();

    QListWidget *m_pairedList;
    QListWidget *m_availableList;
    QCheckBox *m_bluetoothCheck;
    QLabel *m_statusLabel;
    QPushButton *m_scanBtn;
    QPushButton *m_pairBtn;
    QPushButton *m_unpairBtn;
    QPushButton *m_refreshBtn;
};
