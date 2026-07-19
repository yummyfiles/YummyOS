#include "startupappspage.h"
#include "../utils/systemutils.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QLabel>
#include <QMessageBox>
#include <QDir>
#include <QFile>
#include <QTextStream>

StartupAppsPage::StartupAppsPage(QWidget *parent)
    : QWidget(parent)
{
    setupUi();
    loadStartupApps();
}

void StartupAppsPage::setupUi()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(24, 24, 24, 24);
    mainLayout->setSpacing(16);

    QLabel *title = new QLabel(QStringLiteral("Startup Apps"), this);
    title->setStyleSheet(QStringLiteral("font-size: 22px; font-weight: bold; color: #e0e0ee; padding-bottom: 8px;"));
    mainLayout->addWidget(title);

    QLabel *subtitle = new QLabel(QStringLiteral("Manage applications that start automatically on login"), this);
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

    QString dangerButtonStyle = QStringLiteral(
        "QPushButton {"
        "  background-color: #e74c3c; color: #ffffff; border: none; border-radius: 8px;"
        "  padding: 10px 24px; font-size: 13px; font-weight: bold;"
        "}"
        "QPushButton:hover { background-color: #ec6b5e; }"
        "QPushButton:pressed { background-color: #c0392b; }"
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

    // Button row
    QHBoxLayout *btnLayout = new QHBoxLayout();
    btnLayout->setSpacing(8);

    m_addBtn = new QPushButton(QStringLiteral("Add App"), this);
    m_addBtn->setStyleSheet(buttonStyle);
    connect(m_addBtn, &QPushButton::clicked, this, &StartupAppsPage::addStartupApp);
    btnLayout->addWidget(m_addBtn);

    m_removeBtn = new QPushButton(QStringLiteral("Remove"), this);
    m_removeBtn->setStyleSheet(dangerButtonStyle);
    connect(m_removeBtn, &QPushButton::clicked, this, &StartupAppsPage::removeStartupApp);
    btnLayout->addWidget(m_removeBtn);

    btnLayout->addStretch();

    m_refreshBtn = new QPushButton(QStringLiteral("Refresh"), this);
    m_refreshBtn->setStyleSheet(buttonStyle);
    connect(m_refreshBtn, &QPushButton::clicked, this, &StartupAppsPage::refreshList);
    btnLayout->addWidget(m_refreshBtn);

    mainLayout->addLayout(btnLayout);

    // List
    m_listWidget = new QListWidget(this);
    m_listWidget->setStyleSheet(listStyle);
    m_listWidget->setMinimumHeight(300);
    mainLayout->addWidget(m_listWidget, 1);

    // Count
    m_countLabel = new QLabel(this);
    m_countLabel->setStyleSheet(QStringLiteral("color: #8888aa; font-size: 12px; padding: 4px;"));
    mainLayout->addWidget(m_countLabel);
}

void StartupAppsPage::loadStartupApps()
{
    m_listWidget->clear();

    QString autostartPath = QDir::homePath() + QStringLiteral("/.config/autostart");
    QDir dir(autostartPath);
    if (!dir.exists()) {
        dir.mkpath(autostartPath);
    }

    QStringList desktopFiles = dir.entryList(QStringList() << QStringLiteral("*.desktop"), QDir::Files);
    for (const QString &file : desktopFiles) {
        QString filePath = dir.absoluteFilePath(file);
        QString name;
        QString exec;

        QFile f(filePath);
        if (f.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream stream(&f);
            while (!stream.atEnd()) {
                QString line = stream.readLine();
                if (line.startsWith(QStringLiteral("Name="))) {
                    name = line.section(QLatin1Char('='), 1);
                } else if (line.startsWith(QStringLiteral("Exec="))) {
                    exec = line.section(QLatin1Char('='), 1);
                }
            }
        }

        QString display = name.isEmpty() ? file : name;
        if (!exec.isEmpty()) {
            display += QStringLiteral(" (%1)").arg(exec);
        }

        QListWidgetItem *item = new QListWidgetItem(display, m_listWidget);
        item->setData(Qt::UserRole, file);
        item->setIcon(QIcon::fromTheme(QStringLiteral("application-x-desktop")));
    }

    m_countLabel->setText(QStringLiteral("%1 startup app(s) configured").arg(desktopFiles.size()));
}

void StartupAppsPage::addStartupApp()
{
    QString desktopFile = QFileDialog::getOpenFileName(this,
        QStringLiteral("Select .desktop file"),
        QStringLiteral("/usr/share/applications"),
        QStringLiteral("Desktop Files (*.desktop)"));

    if (!desktopFile.isEmpty()) {
        SystemUtils::instance()->addStartupApp(desktopFile);
        loadStartupApps();
    }
}

void StartupAppsPage::removeStartupApp()
{
    QListWidgetItem *item = m_listWidget->currentItem();
    if (!item) {
        QMessageBox::warning(this, QStringLiteral("No Selection"), QStringLiteral("Please select a startup app to remove."));
        return;
    }

    QString file = item->data(Qt::UserRole).toString();
    QMessageBox::StandardButton reply = QMessageBox::question(this,
        QStringLiteral("Remove Startup App"),
        QStringLiteral("Remove '%1' from startup?").arg(file),
        QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        SystemUtils::instance()->removeStartupApp(file);
        loadStartupApps();
    }
}

void StartupAppsPage::toggleStartupApp()
{
}

void StartupAppsPage::refreshList()
{
    loadStartupApps();
}
