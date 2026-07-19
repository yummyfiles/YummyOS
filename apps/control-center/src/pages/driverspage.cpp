#include "driverspage.h"
#include "../utils/systemutils.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QLabel>
#include <QHeaderView>
#include <QProcess>
#include <QMessageBox>

DriversPage::DriversPage(QWidget *parent)
    : QWidget(parent)
{
    setupUi();
    loadDriverInfo();
}

void DriversPage::setupUi()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(24, 24, 24, 24);
    mainLayout->setSpacing(16);

    QLabel *title = new QLabel(QStringLiteral("Drivers"), this);
    title->setStyleSheet(QStringLiteral("font-size: 22px; font-weight: bold; color: #e0e0ee; padding-bottom: 8px;"));
    mainLayout->addWidget(title);

    QLabel *subtitle = new QLabel(QStringLiteral("View and manage hardware drivers"), this);
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

    QString infoLabelStyle = QStringLiteral(
        "QLabel { background-color: #1a1a2e; border: 1px solid #333355; border-radius: 8px; padding: 12px; color: #d0d0ee; font-size: 13px; }"
    );

    // GPU Info
    QGroupBox *gpuGroup = new QGroupBox(QStringLiteral("GPU Information"), this);
    gpuGroup->setStyleSheet(groupBoxStyle);
    QVBoxLayout *gpuLayout = new QVBoxLayout(gpuGroup);
    gpuLayout->setSpacing(10);

    QLabel *gpuTitle = new QLabel(QStringLiteral("Detected GPU:"), gpuGroup);
    gpuTitle->setStyleSheet(QStringLiteral("color: #b0b0cc; font-size: 13px;"));
    gpuLayout->addWidget(gpuTitle);

    m_gpuInfoLabel = new QLabel(QStringLiteral("Loading..."), gpuGroup);
    m_gpuInfoLabel->setWordWrap(true);
    m_gpuInfoLabel->setStyleSheet(infoLabelStyle);
    m_gpuInfoLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);
    gpuLayout->addWidget(m_gpuInfoLabel);

    QLabel *driverTitle = new QLabel(QStringLiteral("Current Driver:"), gpuGroup);
    driverTitle->setStyleSheet(QStringLiteral("color: #b0b0cc; font-size: 13px;"));
    gpuLayout->addWidget(driverTitle);

    m_currentDriverLabel = new QLabel(QStringLiteral("Loading..."), gpuGroup);
    m_currentDriverLabel->setStyleSheet(infoLabelStyle);
    m_currentDriverLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);
    gpuLayout->addWidget(m_currentDriverLabel);

    QLabel *recTitle = new QLabel(QStringLiteral("Recommended Driver:"), gpuGroup);
    recTitle->setStyleSheet(QStringLiteral("color: #b0b0cc; font-size: 13px;"));
    gpuLayout->addWidget(recTitle);

    m_recommendedDriverLabel = new QLabel(QStringLiteral("Loading..."), gpuGroup);
    m_recommendedDriverLabel->setStyleSheet(infoLabelStyle);
    gpuLayout->addWidget(m_recommendedDriverLabel);

    mainLayout->addWidget(gpuGroup);

    // Driver Table
    QGroupBox *tableGroup = new QGroupBox(QStringLiteral("Installed Drivers"), this);
    tableGroup->setStyleSheet(groupBoxStyle);
    QVBoxLayout *tableLayout = new QVBoxLayout(tableGroup);

    m_driverTable = new QTableWidget(tableGroup);
    m_driverTable->setColumnCount(3);
    m_driverTable->setHorizontalHeaderLabels({QStringLiteral("Module"), QStringLiteral("Type"), QStringLiteral("Status")});
    m_driverTable->horizontalHeader()->setStretchLastSection(true);
    m_driverTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    m_driverTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    m_driverTable->verticalHeader()->hide();
    m_driverTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_driverTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_driverTable->setAlternatingRowColors(true);
    m_driverTable->setStyleSheet(QStringLiteral(
        "QTableWidget {"
        "  background-color: #1a1a2e; border: 1px solid #333355; border-radius: 8px;"
        "  color: #d0d0ee; gridline-color: #333355; font-size: 13px;"
        "  selection-background-color: #4CC9F0; selection-color: #1a1a2e;"
        "}"
        "QTableWidget::item { padding: 6px; }"
        "QTableWidget::item:alternate { background-color: #1e1e36; }"
        "QHeaderView::section {"
        "  background-color: #222244; color: #4CC9F0; border: 1px solid #333355;"
        "  padding: 8px; font-weight: bold; font-size: 13px;"
        "}"
    ));
    tableLayout->addWidget(m_driverTable);

    QHBoxLayout *btnLayout = new QHBoxLayout();
    btnLayout->addStretch();
    m_refreshBtn = new QPushButton(QStringLiteral("Refresh"), tableGroup);
    m_refreshBtn->setStyleSheet(buttonStyle);
    connect(m_refreshBtn, &QPushButton::clicked, this, &DriversPage::refreshDrivers);
    btnLayout->addWidget(m_refreshBtn);

    m_driverManagerBtn = new QPushButton(QStringLiteral("Open Driver Manager"), tableGroup);
    m_driverManagerBtn->setStyleSheet(buttonStyle);
    connect(m_driverManagerBtn, &QPushButton::clicked, this, &DriversPage::openDriverManager);
    btnLayout->addWidget(m_driverManagerBtn);

    tableLayout->addLayout(btnLayout);

    mainLayout->addWidget(tableGroup, 1);
}

void DriversPage::loadDriverInfo()
{
    QString gpu = SystemUtils::instance()->getGpuInfo();
    m_gpuInfoLabel->setText(gpu);

    QString driver = SystemUtils::instance()->getGpuDriver();
    m_currentDriverLabel->setText(driver);

    QString recommended;
    if (gpu.contains(QStringLiteral("NVIDIA"), Qt::CaseInsensitive)) {
        recommended = QStringLiteral("nvidia (proprietary) or nouveau (open-source)");
    } else if (gpu.contains(QStringLiteral("AMD"), Qt::CaseInsensitive) || gpu.contains(QStringLiteral("Radeon"), Qt::CaseInsensitive)) {
        recommended = QStringLiteral("amdgpu (built-in) with Mesa");
    } else if (gpu.contains(QStringLiteral("Intel"), Qt::CaseInsensitive)) {
        recommended = QStringLiteral("i915 (built-in) with Mesa");
    } else {
        recommended = QStringLiteral("Unable to determine recommended driver");
    }
    m_recommendedDriverLabel->setText(recommended);

    m_driverTable->setRowCount(0);
    QStringList loadedModules = SystemUtils::runCommandGetLines(QStringLiteral("lsmod 2>/dev/null | awk 'NR>1 && $1 ~ /nvidia|amdgpu|i915|nouveau|radeon|vmw|virtio/ {print $1}'"));

    m_driverTable->setRowCount(loadedModules.size());
    for (int i = 0; i < loadedModules.size(); i++) {
        QString module = loadedModules[i].trimmed();
        QString type;
        if (module.contains(QStringLiteral("nvidia"))) type = QStringLiteral("Proprietary");
        else if (module.contains(QStringLiteral("amdgpu")) || module.contains(QStringLiteral("i915")) || module.contains(QStringLiteral("nouveau"))) type = QStringLiteral("Open-source");
        else type = QStringLiteral("Kernel");

        QTableWidgetItem *modItem = new QTableWidgetItem(module);
        modItem->setFont(QFont(QStringLiteral("Noto Sans"), 10, QFont::Bold));
        m_driverTable->setItem(i, 0, modItem);
        m_driverTable->setItem(i, 1, new QTableWidgetItem(type));
        m_driverTable->setItem(i, 2, new QTableWidgetItem(QStringLiteral("Loaded")));
    }
}

void DriversPage::refreshDrivers()
{
    loadDriverInfo();
}

void DriversPage::openDriverManager()
{
    SystemUtils::runCommand(QStringLiteral("ubuntu-drivers-gui 2>/dev/null || echo 'Install manjaro-settings-manager for driver management' &"));
    QMessageBox::information(this, QStringLiteral("Driver Manager"),
        QStringLiteral("Driver manager opened. If not available, use the terminal:\n\n"
                       "NVIDIA: sudo pacman -S nvidia nvidia-utils\n"
                       "AMD: Mesa is pre-installed (amdgpu kernel module)\n"
                       "Intel: Mesa is pre-installed (i915 kernel module)"));
}
