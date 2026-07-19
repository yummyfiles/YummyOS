#pragma once

#include <QMainWindow>
#include <QStackedWidget>

class SidebarWidget;
class AppearancePage;
class PerformancePage;
class GamingPage;
class PrivacyPage;
class SystemInfoPage;
class AudioPage;
class DisplayPage;
class PowerPage;
class StartupAppsPage;
class PackagesPage;
class NetworkPage;
class UpdatesPage;
class ThemesPage;
class WallpapersPage;
class BackupPage;
class BluetoothPage;
class DriversPage;
class UserAccountsPage;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private slots:
    void onAppearanceClicked();
    void onPerformanceClicked();
    void onGamingClicked();
    void onDriversClicked();
    void onUpdatesClicked();
    void onAudioClicked();
    void onBluetoothClicked();
    void onNetworkClicked();
    void onDisplayClicked();
    void onPowerClicked();
    void onPrivacyClicked();
    void onStartupAppsClicked();
    void onPackagesClicked();
    void onSystemInfoClicked();
    void onBackupClicked();
    void onThemesClicked();
    void onWallpapersClicked();
    void onUserAccountsClicked();

private:
    void setupUi();
    void setupConnections();

    SidebarWidget *m_sidebar;
    QStackedWidget *m_stack;

    AppearancePage *m_appearancePage;
    PerformancePage *m_performancePage;
    GamingPage *m_gamingPage;
    DriversPage *m_driversPage;
    UpdatesPage *m_updatesPage;
    AudioPage *m_audioPage;
    BluetoothPage *m_bluetoothPage;
    NetworkPage *m_networkPage;
    DisplayPage *m_displayPage;
    PowerPage *m_powerPage;
    PrivacyPage *m_privacyPage;
    StartupAppsPage *m_startupAppsPage;
    PackagesPage *m_packagesPage;
    SystemInfoPage *m_systemInfoPage;
    BackupPage *m_backupPage;
    ThemesPage *m_themesPage;
    WallpapersPage *m_wallpapersPage;
    UserAccountsPage *m_userAccountsPage;
};
