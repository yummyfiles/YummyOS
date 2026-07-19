#pragma once

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QTableWidget>
#include <QCheckBox>
#include <QGroupBox>

class PackagesPage : public QWidget
{
    Q_OBJECT

public:
    explicit PackagesPage(QWidget *parent = nullptr);

private slots:
    void openDiscover();
    void openTerminal();
    void refreshInfo();

private:
    void setupUi();
    void loadPackageInfo();

    QLabel *m_packageCountLabel;
    QLabel *m_flatpakStatus;
    QLabel *m_aurHelperStatus;
    QPushButton *m_discoverBtn;
    QPushButton *m_terminalBtn;
    QPushButton *m_refreshBtn;
};
