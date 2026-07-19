#pragma once

#include <QWidget>
#include <QLabel>
#include <QTableWidget>
#include <QPushButton>

class DriversPage : public QWidget
{
    Q_OBJECT

public:
    explicit DriversPage(QWidget *parent = nullptr);

private slots:
    void refreshDrivers();
    void openDriverManager();

private:
    void setupUi();
    void loadDriverInfo();

    QTableWidget *m_driverTable;
    QLabel *m_gpuInfoLabel;
    QLabel *m_currentDriverLabel;
    QLabel *m_recommendedDriverLabel;
    QPushButton *m_refreshBtn;
    QPushButton *m_driverManagerBtn;
};
