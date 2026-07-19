#include "networkpage.h"
#include "../utils/systemutils.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QLabel>
#include <QProcess>
#include <QMessageBox>

NetworkPage::NetworkPage(QWidget *parent)
    : QWidget(parent)
{
    setupUi();
    loadNetworkInfo();
}

void NetworkPage::setupUi()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(24, 24, 24, 24);
    mainLayout->setSpacing(16);

    QLabel *title = new QLabel(QStringLiteral("Network"), this);
    title->setStyleSheet(QStringLiteral("font-size: 22px; font-weight: bold; color: #e0e0ee; padding-bottom: 8px;"));
    mainLayout->addWidget(title);

    QLabel *subtitle = new QLabel(QStringLiteral("Manage network connections and settings"), this);
    subtitle->setStyleSheet(QStringLiteral("color: #8888aa; font-size: 13px; padding-bottom: 16px;"));
    mainLayout->addWidget(subtitle);

    QString groupBoxStyle = QStringLiteral(
        "QGroupBox {"
        "  background-color: #222244; border: 1px solid #333355; border-radius: 12px;"
        "  margin-top: 12px; padding: 20px 16px 16px 16px;"
        "  font-size: 14px; font-weight: bold; color: #4CC9F0;"
        "}"
        "QGroupBox::title { subcontrol-origin: margin; subcontrol-position: top left; padding: 2px 12px; }"
    );

    QString buttonStyle = QStringLiteral(
        "QPushButton {"
        "  background-color: #4CC9F0; color: #1a1a2e; border: none; border-radius: 8px;"
        "  padding: 10px 24px; font-size: 13px; font-weight: bold;"
        "}"
        "QPushButton:hover { background-color: #6dd5f5; }"
        "QPushButton:pressed { background-color: #3ab8de; }"
    );

    QString checkboxStyle = QStringLiteral(
        "QCheckBox { color: #d0d0ee; font-size: 13px; spacing: 8px; }"
        "QCheckBox::indicator {"
        "  width: 20px; height: 20px; border-radius: 4px; border: 2px solid #555577; background-color: #1a1a2e;"
        "}"
        "QCheckBox::indicator:checked { background-color: #4CC9F0; border-color: #4CC9F0; }"
        "QCheckBox::indicator:hover { border-color: #4CC9F0; }"
    );

    QString listStyle = QStringLiteral(
        "QListWidget {"
        "  background-color: #1a1a2e; border: 1px solid #333355; border-radius: 8px;"
        "  color: #d0d0ee; font-size: 13px; padding: 4px;"
        "  outline: none;"
        "}"
        "QListWidget::item { padding: 8px; border-radius: 4px; }"
        "QListWidget::item:selected { background-color: #4CC9F0; color: #1a1a2e; }"
        "QListWidget::item:hover { background-color: #1e1e36; }"
    );

    // Connection Status
    QGroupBox *statusGroup = new QGroupBox(QStringLiteral("Connection Status"), this);
    statusGroup->setStyleSheet(groupBoxStyle);
    QVBoxLayout *statusLayout = new QVBoxLayout(statusGroup);
    statusLayout->setSpacing(10);

    m_statusLabel = new QLabel(QStringLiteral("Checking..."), statusGroup);
    m_statusLabel->setWordWrap(true);
    m_statusLabel->setStyleSheet(QStringLiteral("color: #d0d0ee; font-size: 13px; padding: 4px;"));
    statusLayout->addWidget(m_statusLabel);

    QHBoxLayout *statusBtnLayout = new QHBoxLayout();
    m_wifiCheck = new QCheckBox(QStringLiteral("Wi-Fi Enabled"), statusGroup);
    m_wifiCheck->setStyleSheet(checkboxStyle);
    connect(m_wifiCheck, &QCheckBox::toggled, this, &NetworkPage::toggleWifi);
    statusBtnLayout->addWidget(m_wifiCheck);

    statusBtnLayout->addStretch();

    m_refreshBtn = new QPushButton(QStringLiteral("Refresh"), statusGroup);
    m_refreshBtn->setStyleSheet(buttonStyle);
    connect(m_refreshBtn, &QPushButton::clicked, this, &NetworkPage::refreshConnections);
    statusBtnLayout->addWidget(m_refreshBtn);

    statusLayout->addLayout(statusBtnLayout);
    mainLayout->addWidget(statusGroup);

    // Active Connections
    QGroupBox *connectionsGroup = new QGroupBox(QStringLiteral("Active Connections"), this);
    connectionsGroup->setStyleSheet(groupBoxStyle);
    QVBoxLayout *connLayout = new QVBoxLayout(connectionsGroup);

    m_connectionsList = new QListWidget(connectionsGroup);
    m_connectionsList->setStyleSheet(listStyle);
    m_connectionsList->setMinimumHeight(200);
    connLayout->addWidget(m_connectionsList);

    mainLayout->addWidget(connectionsGroup);

    // VPN
    QGroupBox *vpnGroup = new QGroupBox(QStringLiteral("VPN"), this);
    vpnGroup->setStyleSheet(groupBoxStyle);
    QHBoxLayout *vpnLayout = new QHBoxLayout(vpnGroup);

    QLabel *vpnLabel = new QLabel(QStringLiteral("Configure VPN connections through NetworkManager"), vpnGroup);
    vpnLabel->setStyleSheet(QStringLiteral("color: #b0b0cc; font-size: 13px;"));
    vpnLayout->addWidget(vpnLabel);

    vpnLayout->addStretch();

    m_vpnBtn = new QPushButton(QStringLiteral("Open VPN Settings"), vpnGroup);
    m_vpnBtn->setStyleSheet(buttonStyle);
    connect(m_vpnBtn, &QPushButton::clicked, this, &NetworkPage::openVpnSettings);
    vpnLayout->addWidget(m_vpnBtn);

    mainLayout->addWidget(vpnGroup);

    mainLayout->addStretch();
}

void NetworkPage::loadNetworkInfo()
{
    bool connected = SystemUtils::instance()->isNetworkConnected();
    m_statusLabel->setText(connected
        ? QStringLiteral("Connected to the internet")
        : QStringLiteral("Not connected to the internet"));
    m_statusLabel->setStyleSheet(connected
        ? QStringLiteral("color: #2ecc71; font-size: 13px; padding: 4px; font-weight: bold;")
        : QStringLiteral("color: #e74c3c; font-size: 13px; padding: 4px; font-weight: bold;"));

    bool wifiEnabled = SystemUtils::runCommandSuccess(QStringLiteral("nmcli radio wifi 2>/dev/null | grep -q enabled"));
    m_wifiCheck->setChecked(wifiEnabled);

    refreshConnections();
}

void NetworkPage::toggleWifi()
{
    bool enabled = m_wifiCheck->isChecked();
    SystemUtils::runCommand(QStringLiteral("nmcli radio wifi %1 2>/dev/null").arg(enabled ? QStringLiteral("on") : QStringLiteral("off")));
}

void NetworkPage::refreshConnections()
{
    m_connectionsList->clear();
    QStringList connections = SystemUtils::instance()->getNetworkConnections();
    for (const QString &conn : connections) {
        QStringList parts = conn.split(QLatin1Char(':'));
        QString display = parts.isEmpty() ? conn : parts.first();
        if (parts.size() > 2) {
            display += QStringLiteral(" (%1)").arg(parts.last());
        }
        QListWidgetItem *item = new QListWidgetItem(display, m_connectionsList);
        item->setIcon(QIcon::fromTheme(QStringLiteral("network-wired")));
    }

    loadNetworkInfo();
}

void NetworkPage::openVpnSettings()
{
    SystemUtils::runCommand(QStringLiteral("nm-connection-editor --type vpn 2>/dev/null &"));
}
