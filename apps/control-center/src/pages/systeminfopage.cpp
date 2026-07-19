#include "systeminfopage.h"
#include "../utils/systemutils.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QHeaderView>
#include <QMessageBox>

SystemInfoPage::SystemInfoPage(QWidget *parent)
    : QWidget(parent)
{
    setupUi();
    loadSystemInfo();
}

void SystemInfoPage::setupUi()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(24, 24, 24, 24);
    mainLayout->setSpacing(16);

    QLabel *title = new QLabel(QStringLiteral("System Information"), this);
    title->setStyleSheet(QStringLiteral("font-size: 22px; font-weight: bold; color: #e0e0ee; padding-bottom: 8px;"));
    mainLayout->addWidget(title);

    QLabel *subtitle = new QLabel(QStringLiteral("View detailed information about your system"), this);
    subtitle->setStyleSheet(QStringLiteral("color: #8888aa; font-size: 13px; padding-bottom: 16px;"));
    mainLayout->addWidget(subtitle);

    QString buttonStyle = QStringLiteral(
        "QPushButton {"
        "  background-color: #4CC9F0; color: #1a1a2e; border: none; border-radius: 8px;"
        "  padding: 10px 24px; font-size: 13px; font-weight: bold;"
        "}"
        "QPushButton:hover { background-color: #6dd5f5; }"
        "QPushButton:pressed { background-color: #3ab8de; }"
    );

    QHBoxLayout *refreshLayout = new QHBoxLayout();
    refreshLayout->addStretch();
    m_refreshButton = new QPushButton(QStringLiteral("Refresh"), this);
    m_refreshButton->setStyleSheet(buttonStyle);
    connect(m_refreshButton, &QPushButton::clicked, this, &SystemInfoPage::refreshInfo);
    refreshLayout->addWidget(m_refreshButton);
    mainLayout->addLayout(refreshLayout);

    m_infoTable = new QTableWidget(this);
    m_infoTable->setColumnCount(2);
    m_infoTable->setHorizontalHeaderLabels({QStringLiteral("Property"), QStringLiteral("Value")});
    m_infoTable->horizontalHeader()->setStretchLastSection(true);
    m_infoTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    m_infoTable->verticalHeader()->hide();
    m_infoTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_infoTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_infoTable->setAlternatingRowColors(true);
    m_infoTable->setStyleSheet(QStringLiteral(
        "QTableWidget {"
        "  background-color: #1a1a2e; border: 1px solid #333355; border-radius: 8px;"
        "  color: #d0d0ee; gridline-color: #333355; font-size: 13px;"
        "  selection-background-color: #4CC9F0; selection-color: #1a1a2e;"
        "}"
        "QTableWidget::item { padding: 8px; }"
        "QTableWidget::item:alternate { background-color: #1e1e36; }"
        "QHeaderView::section {"
        "  background-color: #222244; color: #4CC9F0; border: 1px solid #333355;"
        "  padding: 8px; font-weight: bold; font-size: 13px;"
        "}"
    ));
    mainLayout->addWidget(m_infoTable, 1);
}

void SystemInfoPage::loadSystemInfo()
{
    SystemUtils::SystemInfo info = SystemUtils::instance()->getSystemInfo();

    struct InfoRow {
        QString key;
        QString value;
    };

    QList<InfoRow> rows;
    rows.append({QStringLiteral("Distro Name"), info.distroName});
    rows.append({QStringLiteral("Kernel Version"), info.kernelVersion});
    rows.append({QStringLiteral("KDE Plasma Version"), info.plasmaVersion});
    rows.append({QStringLiteral("Qt Version"), info.qtVersion});
    rows.append({QStringLiteral("CPU Model"), info.cpuModel});
    rows.append({QStringLiteral("CPU Cores"), QString::number(info.cpuCores)});

    double totalGB = info.totalMemoryKB / 1048576.0;
    double usedGB = info.usedMemoryKB / 1048576.0;
    rows.append({QStringLiteral("Total Memory"), QStringLiteral("%1 GB").arg(totalGB, 0, 'f', 1)});
    rows.append({QStringLiteral("Used Memory"), QStringLiteral("%1 GB (%2%)").arg(usedGB, 0, 'f', 1)
        .arg(info.totalMemoryKB > 0 ? (info.usedMemoryKB * 100 / info.totalMemoryKB) : 0)});

    rows.append({QStringLiteral("Hostname"), info.hostname});
    rows.append({QStringLiteral("Username"), info.username});

    QString diskUsage = SystemUtils::instance()->getDiskUsage(QStringLiteral("/"));
    rows.append({QStringLiteral("Disk Usage (/)"), diskUsage});

    QString gpu = SystemUtils::instance()->getGpuInfo();
    rows.append({QStringLiteral("GPU"), gpu});

    QString gpuDriver = SystemUtils::instance()->getGpuDriver();
    rows.append({QStringLiteral("GPU Driver"), gpuDriver});

    QString arch = SystemUtils::runCommand(QStringLiteral("uname -m"));
    rows.append({QStringLiteral("Architecture"), arch]);

    QString shell = SystemUtils::runCommand(QStringLiteral("echo $SHELL"));
    rows.append({QStringLiteral("Shell"), shell});

    QString de = SystemUtils::runCommand(QStringLiteral("echo $XDG_CURRENT_DESKTOP"));
    rows.append({QStringLiteral("Desktop Environment"), de.isEmpty() ? QStringLiteral("KDE Plasma") : de});

    QString distroID;
    QFile file(QStringLiteral("/etc/os-release"));
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream stream(&file);
        while (!stream.atEnd()) {
            QString line = stream.readLine();
            if (line.startsWith(QStringLiteral("ID="))) {
                distroID = line.section(QLatin1Char('='), 1).remove(QLatin1Char('"'));
                break;
            }
        }
    }
    rows.append({QStringLiteral("Distribution ID"), distroID});

    QString uptime = SystemUtils::runCommand(QStringLiteral("uptime -p 2>/dev/null"));
    rows.append({QStringLiteral("Uptime"), uptime});

    QString displayServer = SystemUtils::runCommand(QStringLiteral("echo $XDG_SESSION_TYPE"));
    rows.append({QStringLiteral("Display Server"), displayServer.isEmpty() ? QStringLiteral("Wayland") : displayServer});

    m_infoTable->setRowCount(rows.size());
    for (int i = 0; i < rows.size(); i++) {
        QTableWidgetItem *keyItem = new QTableWidgetItem(rows[i].key);
        keyItem->setFont(QFont(QStringLiteral("Noto Sans"), 10, QFont::Bold));
        m_infoTable->setItem(i, 0, keyItem);
        QTableWidgetItem *valItem = new QTableWidgetItem(rows[i].value);
        m_infoTable->setItem(i, 1, valItem);
    }
}

void SystemInfoPage::refreshInfo()
{
    loadSystemInfo();
}
