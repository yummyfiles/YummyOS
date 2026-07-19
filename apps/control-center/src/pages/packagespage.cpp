#include "packagespage.h"
#include "../utils/systemutils.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QLabel>
#include <QProcess>
#include <QMessageBox>

PackagesPage::PackagesPage(QWidget *parent)
    : QWidget(parent)
{
    setupUi();
    loadPackageInfo();
}

void PackagesPage::setupUi()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(24, 24, 24, 24);
    mainLayout->setSpacing(16);

    QLabel *title = new QLabel(QStringLiteral("Packages"), this);
    title->setStyleSheet(QStringLiteral("font-size: 22px; font-weight: bold; color: #e0e0ee; padding-bottom: 8px;"));
    mainLayout->addWidget(title);

    QLabel *subtitle = new QLabel(QStringLiteral("Manage software packages and updates"), this);
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

    // Package Stats
    QGroupBox *statsGroup = new QGroupBox(QStringLiteral("Package Statistics"), this);
    statsGroup->setStyleSheet(groupBoxStyle);
    QVBoxLayout *statsLayout = new QVBoxLayout(statsGroup);
    statsLayout->setSpacing(12);

    QHBoxLayout *countRow = new QHBoxLayout();
    QLabel *countLabel = new QLabel(QStringLiteral("Installed Packages:"), statsGroup);
    countLabel->setStyleSheet(QStringLiteral("color: #b0b0cc; font-size: 13px;"));
    countRow->addWidget(countLabel);

    m_packageCountLabel = new QLabel(QStringLiteral("Loading..."), statsGroup);
    m_packageCountLabel->setStyleSheet(QStringLiteral("color: #4CC9F0; font-size: 18px; font-weight: bold;"));
    countRow->addWidget(m_packageCountLabel);
    countRow->addStretch();
    statsLayout->addLayout(countRow);

    mainLayout->addWidget(statsGroup);

    // Package Managers
    QGroupBox *managersGroup = new QGroupBox(QStringLiteral("Package Managers"), this);
    managersGroup->setStyleSheet(groupBoxStyle);
    QVBoxLayout *mgrLayout = new QVBoxLayout(managersGroup);
    mgrLayout->setSpacing(10);

    QLabel *flatpakLabel = new QLabel(QStringLiteral("Flatpak:"), managersGroup);
    flatpakLabel->setStyleSheet(QStringLiteral("color: #b0b0cc; font-size: 13px;"));
    mgrLayout->addWidget(flatpakLabel);

    m_flatpakStatus = new QLabel(QStringLiteral("Checking..."), managersGroup);
    m_flatpakStatus->setStyleSheet(infoLabelStyle);
    mgrLayout->addWidget(m_flatpakStatus);

    QLabel *aurLabel = new QLabel(QStringLiteral("AUR Helper:"), managersGroup);
    aurLabel->setStyleSheet(QStringLiteral("color: #b0b0cc; font-size: 13px;"));
    mgrLayout->addWidget(aurLabel);

    m_aurHelperStatus = new QLabel(QStringLiteral("Checking..."), managersGroup);
    m_aurHelperStatus->setStyleSheet(infoLabelStyle);
    mgrLayout->addWidget(m_aurHelperStatus);

    mainLayout->addWidget(managersGroup);

    // Quick Actions
    QGroupBox *actionsGroup = new QGroupBox(QStringLiteral("Quick Actions"), this);
    actionsGroup->setStyleSheet(groupBoxStyle);
    QHBoxLayout *actionsLayout = new QHBoxLayout(actionsGroup);

    m_discoverBtn = new QPushButton(QStringLiteral("Open Discover"), actionsGroup);
    m_discoverBtn->setStyleSheet(buttonStyle);
    connect(m_discoverBtn, &QPushButton::clicked, this, &PackagesPage::openDiscover);
    actionsLayout->addWidget(m_discoverBtn);

    m_terminalBtn = new QPushButton(QStringLiteral("Open Terminal"), actionsGroup);
    m_terminalBtn->setStyleSheet(buttonStyle);
    connect(m_terminalBtn, &QPushButton::clicked, this, &PackagesPage::openTerminal);
    actionsLayout->addWidget(m_terminalBtn);

    actionsLayout->addStretch();

    m_refreshBtn = new QPushButton(QStringLiteral("Refresh"), actionsGroup);
    m_refreshBtn->setStyleSheet(buttonStyle);
    connect(m_refreshBtn, &QPushButton::clicked, this, &PackagesPage::refreshInfo);
    actionsLayout->addWidget(m_refreshBtn);

    mainLayout->addWidget(actionsGroup);

    mainLayout->addStretch();
}

void PackagesPage::loadPackageInfo()
{
    int count = SystemUtils::instance()->getInstalledPackageCount();
    m_packageCountLabel->setText(QStringLiteral("%1 packages").arg(count));

    bool flatpak = SystemUtils::instance()->isFlatpakInstalled();
    if (flatpak) {
        QString flatpakCount = SystemUtils::runCommand(QStringLiteral("flatpak list --app 2>/dev/null | wc -l"));
        m_flatpakStatus->setText(QStringLiteral("Installed (%1 Flatpak apps)").arg(flatpakCount.trimmed()));
        m_flatpakStatus->setStyleSheet(QStringLiteral(
            "QLabel { background-color: #1a1a2e; border: 1px solid #333355; border-radius: 8px; padding: 12px; color: #2ecc71; font-size: 13px; }"));
    } else {
        m_flatpakStatus->setText(QStringLiteral("Not installed"));
        m_flatpakStatus->setStyleSheet(QStringLiteral(
            "QLabel { background-color: #1a1a2e; border: 1px solid #333355; border-radius: 8px; padding: 12px; color: #f39c12; font-size: 13px; }"));
    }

    bool aur = SystemUtils::instance()->isAURHelperInstalled();
    if (aur) {
        QString helper = SystemUtils::runCommand(QStringLiteral("which yay 2>/dev/null && echo yay || (which paru 2>/dev/null && echo paru || echo trizen)"));
        m_aurHelperStatus->setText(QStringLiteral("Installed (%1)").arg(helper.trimmed()));
        m_aurHelperStatus->setStyleSheet(QStringLiteral(
            "QLabel { background-color: #1a1a2e; border: 1px solid #333355; border-radius: 8px; padding: 12px; color: #2ecc71; font-size: 13px; }"));
    } else {
        m_aurHelperStatus->setText(QStringLiteral("Not installed (install yay or paru for AUR support)"));
        m_aurHelperStatus->setStyleSheet(QStringLiteral(
            "QLabel { background-color: #1a1a2e; border: 1px solid #333355; border-radius: 8px; padding: 12px; color: #f39c12; font-size: 13px; }"));
    }
}

void PackagesPage::openDiscover()
{
    SystemUtils::runCommand(QStringLiteral("plasma-discover &"));
}

void PackagesPage::openTerminal()
{
    SystemUtils::runCommand(QStringLiteral("konsole &"));
}

void PackagesPage::refreshInfo()
{
    loadPackageInfo();
}
