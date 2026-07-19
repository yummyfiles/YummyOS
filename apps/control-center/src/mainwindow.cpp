#include "mainwindow.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QFrame>

#include "sidebarwidget.h"
#include "pages/appearancepage.h"
#include "pages/performancepage.h"
#include "pages/gamingpage.h"
#include "pages/driverspage.h"
#include "pages/updatespage.h"
#include "pages/audiopage.h"
#include "pages/bluetoothpage.h"
#include "pages/networkpage.h"
#include "pages/displaypage.h"
#include "pages/powerpage.h"
#include "pages/privacypage.h"
#include "pages/startupappspage.h"
#include "pages/packagespage.h"
#include "pages/systeminfopage.h"
#include "pages/backuppage.h"
#include "pages/themespage.h"
#include "pages/wallpaperspage.h"
#include "pages/useraccountspage.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setupUi();
    setupConnections();
    setWindowTitle(QStringLiteral("Yummy Control Center"));
    setMinimumSize(1000, 600);
}

MainWindow::~MainWindow() = default;

void MainWindow::setupUi()
{
    QWidget *central = new QWidget(this);
    QHBoxLayout *mainLayout = new QHBoxLayout(central);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    m_sidebar = new SidebarWidget(this);

    QFrame *separator = new QFrame(this);
    separator->setFrameShape(QFrame::VLine);
    separator->setStyleSheet(QStringLiteral("color: #333355;"));

    m_stack = new QStackedWidget(this);
    m_stack->setStyleSheet(QStringLiteral("QStackedWidget { background-color: #1a1a2e; }"));

    m_appearancePage = new AppearancePage(this);
    m_performancePage = new PerformancePage(this);
    m_gamingPage = new GamingPage(this);
    m_driversPage = new DriversPage(this);
    m_updatesPage = new UpdatesPage(this);
    m_audioPage = new AudioPage(this);
    m_bluetoothPage = new BluetoothPage(this);
    m_networkPage = new NetworkPage(this);
    m_displayPage = new DisplayPage(this);
    m_powerPage = new PowerPage(this);
    m_privacyPage = new PrivacyPage(this);
    m_startupAppsPage = new StartupAppsPage(this);
    m_packagesPage = new PackagesPage(this);
    m_systemInfoPage = new SystemInfoPage(this);
    m_backupPage = new BackupPage(this);
    m_themesPage = new ThemesPage(this);
    m_wallpapersPage = new WallpapersPage(this);
    m_userAccountsPage = new UserAccountsPage(this);

    m_stack->addWidget(m_appearancePage);
    m_stack->addWidget(m_performancePage);
    m_stack->addWidget(m_gamingPage);
    m_stack->addWidget(m_driversPage);
    m_stack->addWidget(m_updatesPage);
    m_stack->addWidget(m_audioPage);
    m_stack->addWidget(m_bluetoothPage);
    m_stack->addWidget(m_networkPage);
    m_stack->addWidget(m_displayPage);
    m_stack->addWidget(m_powerPage);
    m_stack->addWidget(m_privacyPage);
    m_stack->addWidget(m_startupAppsPage);
    m_stack->addWidget(m_packagesPage);
    m_stack->addWidget(m_systemInfoPage);
    m_stack->addWidget(m_backupPage);
    m_stack->addWidget(m_themesPage);
    m_stack->addWidget(m_wallpapersPage);
    m_stack->addWidget(m_userAccountsPage);

    mainLayout->addWidget(m_sidebar);
    mainLayout->addWidget(separator);
    mainLayout->addWidget(m_stack, 1);

    setCentralWidget(central);
}

void MainWindow::setupConnections()
{
    connect(m_sidebar, &SidebarWidget::appearanceClicked, this, &MainWindow::onAppearanceClicked);
    connect(m_sidebar, &SidebarWidget::performanceClicked, this, &MainWindow::onPerformanceClicked);
    connect(m_sidebar, &SidebarWidget::gamingClicked, this, &MainWindow::onGamingClicked);
    connect(m_sidebar, &SidebarWidget::driversClicked, this, &MainWindow::onDriversClicked);
    connect(m_sidebar, &SidebarWidget::updatesClicked, this, &MainWindow::onUpdatesClicked);
    connect(m_sidebar, &SidebarWidget::audioClicked, this, &MainWindow::onAudioClicked);
    connect(m_sidebar, &SidebarWidget::bluetoothClicked, this, &MainWindow::onBluetoothClicked);
    connect(m_sidebar, &SidebarWidget::networkClicked, this, &MainWindow::onNetworkClicked);
    connect(m_sidebar, &SidebarWidget::displayClicked, this, &MainWindow::onDisplayClicked);
    connect(m_sidebar, &SidebarWidget::powerClicked, this, &MainWindow::onPowerClicked);
    connect(m_sidebar, &SidebarWidget::privacyClicked, this, &MainWindow::onPrivacyClicked);
    connect(m_sidebar, &SidebarWidget::startupAppsClicked, this, &MainWindow::onStartupAppsClicked);
    connect(m_sidebar, &SidebarWidget::packagesClicked, this, &MainWindow::onPackagesClicked);
    connect(m_sidebar, &SidebarWidget::systemInfoClicked, this, &MainWindow::onSystemInfoClicked);
    connect(m_sidebar, &SidebarWidget::backupClicked, this, &MainWindow::onBackupClicked);
    connect(m_sidebar, &SidebarWidget::themesClicked, this, &MainWindow::onThemesClicked);
    connect(m_sidebar, &SidebarWidget::wallpapersClicked, this, &MainWindow::onWallpapersClicked);
    connect(m_sidebar, &SidebarWidget::userAccountsClicked, this, &MainWindow::onUserAccountsClicked);
}

void MainWindow::onAppearanceClicked() { m_stack->setCurrentWidget(m_appearancePage); }
void MainWindow::onPerformanceClicked() { m_stack->setCurrentWidget(m_performancePage); }
void MainWindow::onGamingClicked() { m_stack->setCurrentWidget(m_gamingPage); }
void MainWindow::onDriversClicked() { m_stack->setCurrentWidget(m_driversPage); }
void MainWindow::onUpdatesClicked() { m_stack->setCurrentWidget(m_updatesPage); }
void MainWindow::onAudioClicked() { m_stack->setCurrentWidget(m_audioPage); }
void MainWindow::onBluetoothClicked() { m_stack->setCurrentWidget(m_bluetoothPage); }
void MainWindow::onNetworkClicked() { m_stack->setCurrentWidget(m_networkPage); }
void MainWindow::onDisplayClicked() { m_stack->setCurrentWidget(m_displayPage); }
void MainWindow::onPowerClicked() { m_stack->setCurrentWidget(m_powerPage); }
void MainWindow::onPrivacyClicked() { m_stack->setCurrentWidget(m_privacyPage); }
void MainWindow::onStartupAppsClicked() { m_stack->setCurrentWidget(m_startupAppsPage); }
void MainWindow::onPackagesClicked() { m_stack->setCurrentWidget(m_packagesPage); }
void MainWindow::onSystemInfoClicked() { m_stack->setCurrentWidget(m_systemInfoPage); }
void MainWindow::onBackupClicked() { m_stack->setCurrentWidget(m_backupPage); }
void MainWindow::onThemesClicked() { m_stack->setCurrentWidget(m_themesPage); }
void MainWindow::onWallpapersClicked() { m_stack->setCurrentWidget(m_wallpapersPage); }
void MainWindow::onUserAccountsClicked() { m_stack->setCurrentWidget(m_userAccountsPage); }
