#include "bluetoothpage.h"
#include "../utils/systemutils.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QLabel>
#include <QMessageBox>

BluetoothPage::BluetoothPage(QWidget *parent)
    : QWidget(parent)
{
    setupUi();
    loadDevices();
}

void BluetoothPage::setupUi()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(24, 24, 24, 24);
    mainLayout->setSpacing(16);

    QLabel *title = new QLabel(QStringLiteral("Bluetooth"), this);
    title->setStyleSheet(QStringLiteral("font-size: 22px; font-weight: bold; color: #e0e0ee; padding-bottom: 8px;"));
    mainLayout->addWidget(title);

    QLabel *subtitle = new QLabel(QStringLiteral("Manage Bluetooth devices and connections"), this);
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

    QString dangerButtonStyle = QStringLiteral(
        "QPushButton {"
        "  background-color: #e74c3c; color: #ffffff; border: none; border-radius: 8px;"
        "  padding: 10px 24px; font-size: 13px; font-weight: bold;"
        "}"
        "QPushButton:hover { background-color: #ec6b5e; }"
        "QPushButton:pressed { background-color: #c0392b; }"
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
        "  color: #d0d0ee; font-size: 13px; padding: 4px; outline: none;"
        "}"
        "QListWidget::item { padding: 10px; border-radius: 4px; margin: 2px 4px; }"
        "QListWidget::item:selected { background-color: #4CC9F0; color: #1a1a2e; }"
        "QListWidget::item:hover { background-color: #1e1e36; }"
    );

    // Status
    QGroupBox *statusGroup = new QGroupBox(QStringLiteral("Bluetooth Status"), this);
    statusGroup->setStyleSheet(groupBoxStyle);
    QVBoxLayout *statusLayout = new QVBoxLayout(statusGroup);
    statusLayout->setSpacing(10);

    QHBoxLayout *toggleRow = new QHBoxLayout();
    m_bluetoothCheck = new QCheckBox(QStringLiteral("Bluetooth Enabled"), statusGroup);
    m_bluetoothCheck->setStyleSheet(checkboxStyle);
    connect(m_bluetoothCheck, &QCheckBox::toggled, this, &BluetoothPage::toggleBluetooth);
    toggleRow->addWidget(m_bluetoothCheck);

    toggleRow->addStretch();

    m_refreshBtn = new QPushButton(QStringLiteral("Refresh"), statusGroup);
    m_refreshBtn->setStyleSheet(buttonStyle);
    connect(m_refreshBtn, &QPushButton::clicked, this, &BluetoothPage::refreshDevices);
    toggleRow->addWidget(m_refreshBtn);

    m_scanBtn = new QPushButton(QStringLiteral("Scan"), statusGroup);
    m_scanBtn->setStyleSheet(buttonStyle);
    connect(m_scanBtn, &QPushButton::clicked, this, &BluetoothPage::scanForDevices);
    toggleRow->addWidget(m_scanBtn);

    statusLayout->addLayout(toggleRow);

    m_statusLabel = new QLabel(QStringLiteral("Checking Bluetooth status..."), statusGroup);
    m_statusLabel->setStyleSheet(QStringLiteral("color: #d0d0ee; font-size: 13px;"));
    statusLayout->addWidget(m_statusLabel);

    mainLayout->addWidget(statusGroup);

    // Paired Devices
    QGroupBox *pairedGroup = new QGroupBox(QStringLiteral("Paired Devices"), this);
    pairedGroup->setStyleSheet(groupBoxStyle);
    QVBoxLayout *pairedLayout = new QVBoxLayout(pairedGroup);

    m_pairedList = new QListWidget(pairedGroup);
    m_pairedList->setStyleSheet(listStyle);
    m_pairedList->setMinimumHeight(120);
    pairedLayout->addWidget(m_pairedList);

    QHBoxLayout *pairedBtnLayout = new QHBoxLayout();
    pairedBtnLayout->addStretch();
    m_unpairBtn = new QPushButton(QStringLiteral("Unpair"), pairedGroup);
    m_unpairBtn->setStyleSheet(dangerButtonStyle);
    connect(m_unpairBtn, &QPushButton::clicked, this, &BluetoothPage::unpairDevice);
    pairedBtnLayout->addWidget(m_unpairBtn);
    pairedLayout->addLayout(pairedBtnLayout);

    mainLayout->addWidget(pairedGroup);

    // Available Devices
    QGroupBox *availableGroup = new QGroupBox(QStringLiteral("Available Devices"), this);
    availableGroup->setStyleSheet(groupBoxStyle);
    QVBoxLayout *availLayout = new QVBoxLayout(availableGroup);

    m_availableList = new QListWidget(availableGroup);
    m_availableList->setStyleSheet(listStyle);
    m_availableList->setMinimumHeight(120);
    availLayout->addWidget(m_availableList);

    QHBoxLayout *availBtnLayout = new QHBoxLayout();
    availBtnLayout->addStretch();
    m_pairBtn = new QPushButton(QStringLiteral("Pair"), availableGroup);
    m_pairBtn->setStyleSheet(buttonStyle);
    connect(m_pairBtn, &QPushButton::clicked, this, &BluetoothPage::pairDevice);
    availBtnLayout->addWidget(m_pairBtn);
    availLayout->addLayout(availBtnLayout);

    mainLayout->addWidget(availableGroup);

    mainLayout->addStretch();
}

void BluetoothPage::loadDevices()
{
    bool enabled = SystemUtils::instance()->isBluetoothEnabled();
    m_bluetoothCheck->setChecked(enabled);
    m_statusLabel->setText(enabled ? QStringLiteral("Bluetooth is enabled") : QStringLiteral("Bluetooth is disabled"));
    m_statusLabel->setStyleSheet(enabled
        ? QStringLiteral("color: #2ecc71; font-size: 13px; font-weight: bold;")
        : QStringLiteral("color: #e74c3c; font-size: 13px; font-weight: bold;"));

    m_pairedList->clear();
    QStringList paired = SystemUtils::instance()->getPairedBluetoothDevices();
    for (const QString &device : paired) {
        QStringList parts = device.split(QLatin1Char(' '));
        QString name = parts.size() > 1 ? parts.mid(1).join(QLatin1Char(' ')) : device;
        QString address = parts.isEmpty() ? QString() : parts.at(1);
        QListWidgetItem *item = new QListWidgetItem(name, m_pairedList);
        item->setData(Qt::UserRole, address);
        item->setIcon(QIcon::fromTheme(QStringLiteral("bluetooth-device")));
    }

    m_availableList->clear();
    QStringList available = SystemUtils::instance()->getBluetoothDevices();
    for (const QString &device : available) {
        QStringList parts = device.split(QLatin1Char(' '));
        QString name = parts.size() > 2 ? parts.mid(2).join(QLatin1Char(' ')) : device;
        QString address = parts.size() > 1 ? parts.at(1) : QString();
        QListWidgetItem *item = new QListWidgetItem(name, m_availableList);
        item->setData(Qt::UserRole, address);
        item->setIcon(QIcon::fromTheme(QStringLiteral("bluetooth-device")));
    }
}

void BluetoothPage::toggleBluetooth()
{
    bool enabled = m_bluetoothCheck->isChecked();
    SystemUtils::instance()->toggleBluetooth(enabled);
    m_statusLabel->setText(enabled ? QStringLiteral("Bluetooth is enabled") : QStringLiteral("Bluetooth is disabled"));
    m_statusLabel->setStyleSheet(enabled
        ? QStringLiteral("color: #2ecc71; font-size: 13px; font-weight: bold;")
        : QStringLiteral("color: #e74c3c; font-size: 13px; font-weight: bold;"));
}

void BluetoothPage::scanForDevices()
{
    m_statusLabel->setText(QStringLiteral("Scanning for devices..."));
    m_statusLabel->setStyleSheet(QStringLiteral("color: #f39c12; font-size: 13px; font-weight: bold;"));

    SystemUtils::instance()->startBluetoothScan();

    QTimer::singleShot(5000, this, [this]() {
        loadDevices();
        m_statusLabel->setText(QStringLiteral("Scan complete"));
        m_statusLabel->setStyleSheet(QStringLiteral("color: #2ecc71; font-size: 13px; font-weight: bold;"));
    });
}

void BluetoothPage::pairDevice()
{
    QListWidgetItem *item = m_availableList->currentItem();
    if (!item) {
        QMessageBox::warning(this, QStringLiteral("No Selection"), QStringLiteral("Please select a device to pair."));
        return;
    }

    QString address = item->data(Qt::UserRole).toString();
    m_statusLabel->setText(QStringLiteral("Pairing with %1...").arg(item->text()));
    m_statusLabel->setStyleSheet(QStringLiteral("color: #f39c12; font-size: 13px; font-weight: bold;"));

    bool success = SystemUtils::instance()->pairBluetoothDevice(address);
    if (success) {
        m_statusLabel->setText(QStringLiteral("Successfully paired with %1").arg(item->text()));
        m_statusLabel->setStyleSheet(QStringLiteral("color: #2ecc71; font-size: 13px; font-weight: bold;"));
    } else {
        m_statusLabel->setText(QStringLiteral("Failed to pair with %1").arg(item->text()));
        m_statusLabel->setStyleSheet(QStringLiteral("color: #e74c3c; font-size: 13px; font-weight: bold;"));
    }

    loadDevices();
}

void BluetoothPage::unpairDevice()
{
    QListWidgetItem *item = m_pairedList->currentItem();
    if (!item) {
        QMessageBox::warning(this, QStringLiteral("No Selection"), QStringLiteral("Please select a device to unpair."));
        return;
    }

    QMessageBox::StandardButton reply = QMessageBox::question(this,
        QStringLiteral("Unpair Device"),
        QStringLiteral("Remove '%1' from paired devices?").arg(item->text()),
        QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        QString address = item->data(Qt::UserRole).toString();
        SystemUtils::instance()->unpairBluetoothDevice(address);
        loadDevices();
    }
}

void BluetoothPage::refreshDevices()
{
    loadDevices();
}
