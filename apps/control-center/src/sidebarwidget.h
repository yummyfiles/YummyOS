#pragma once

#include <QWidget>
#include <QListWidget>

class SidebarWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SidebarWidget(QWidget *parent = nullptr);

signals:
    void appearanceClicked();
    void performanceClicked();
    void gamingClicked();
    void driversClicked();
    void updatesClicked();
    void audioClicked();
    void bluetoothClicked();
    void networkClicked();
    void displayClicked();
    void powerClicked();
    void privacyClicked();
    void startupAppsClicked();
    void packagesClicked();
    void systemInfoClicked();
    void backupClicked();
    void themesClicked();
    void wallpapersClicked();
    void userAccountsClicked();

private:
    void setupUi();
    QListWidgetItem *addSidebarItem(const QString &iconName, const QString &text);
    void onItemClicked(int row);

    QListWidget *m_list;
};
