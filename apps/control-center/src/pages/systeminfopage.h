#pragma once

#include <QWidget>
#include <QLabel>
#include <QTableWidget>
#include <QPushButton>

class SystemInfoPage : public QWidget
{
    Q_OBJECT

public:
    explicit SystemInfoPage(QWidget *parent = nullptr);

private slots:
    void refreshInfo();

private:
    void setupUi();
    void loadSystemInfo();

    QTableWidget *m_infoTable;
    QPushButton *m_refreshButton;
};
