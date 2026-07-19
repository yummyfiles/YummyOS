#include "themespage.h"
#include "../utils/systemutils.h"
#include "../utils/configmanager.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QLabel>
#include <QMessageBox>
#include <QDir>

ThemesPage::ThemesPage(QWidget *parent)
    : QWidget(parent)
{
    setupUi();
    loadThemes();
}

void ThemesPage::setupUi()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(24, 24, 24, 24);
    mainLayout->setSpacing(16);

    QLabel *title = new QLabel(QStringLiteral("Themes"), this);
    title->setStyleSheet(QStringLiteral("font-size: 22px; font-weight: bold; color: #e0e0ee; padding-bottom: 8px;"));
    mainLayout->addWidget(title);

    QLabel *subtitle = new QLabel(QStringLiteral("Manage desktop themes and visual styles"), this);
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

    QString comboStyle = QStringLiteral(
        "QComboBox {"
        "  background-color: #1a1a2e; border: 1px solid #333355; border-radius: 6px;"
        "  padding: 8px 12px; color: #e0e0ee; font-size: 13px; min-width: 200px;"
        "}"
        "QComboBox:hover { border-color: #4CC9F0; }"
        "QComboBox::drop-down {"
        "  subcontrol-origin: padding; subcontrol-position: top right;"
        "  border-left: 1px solid #333355; width: 28px;"
        "}"
        "QComboBox QAbstractItemView {"
        "  background-color: #222244; border: 1px solid #333355;"
        "  color: #e0e0ee; selection-background-color: #4CC9F0; selection-color: #1a1a2e; outline: none;"
        "}"
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

    // Toolbar
    QHBoxLayout *toolbarLayout = new QHBoxLayout();

    QLabel *typeLabel = new QLabel(QStringLiteral("Type:"), this);
    typeLabel->setStyleSheet(QStringLiteral("color: #b0b0cc; font-size: 13px;"));
    toolbarLayout->addWidget(typeLabel);

    m_typeCombo = new QComboBox(this);
    m_typeCombo->setStyleSheet(comboStyle);
    m_typeCombo->addItems({
        QStringLiteral("Desktop Theme"),
        QStringLiteral("Look and Feel"),
        QStringLiteral("Color Scheme")
    });
    connect(m_typeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [this](int) { loadThemes(); });
    toolbarLayout->addWidget(m_typeCombo);

    toolbarLayout->addStretch();

    m_installBtn = new QPushButton(QStringLiteral("Install from File"), this);
    m_installBtn->setStyleSheet(buttonStyle);
    connect(m_installBtn, &QPushButton::clicked, this, &ThemesPage::installTheme);
    toolbarLayout->addWidget(m_installBtn);

    m_getNewBtn = new QPushButton(QStringLiteral("Get New Themes"), this);
    m_getNewBtn->setStyleSheet(buttonStyle);
    connect(m_getNewBtn, &QPushButton::clicked, this, &ThemesPage::getNewThemes);
    toolbarLayout->addWidget(m_getNewBtn);

    m_refreshBtn = new QPushButton(QStringLiteral("Refresh"), this);
    m_refreshBtn->setStyleSheet(buttonStyle);
    connect(m_refreshBtn, &QPushButton::clicked, this, &ThemesPage::refreshList);
    toolbarLayout->addWidget(m_refreshBtn);

    mainLayout->addLayout(toolbarLayout);

    // Theme list
    m_listWidget = new QListWidget(this);
    m_listWidget->setStyleSheet(listStyle);
    m_listWidget->setMinimumHeight(300);
    mainLayout->addWidget(m_listWidget, 1);

    // Apply button
    QHBoxLayout *applyLayout = new QHBoxLayout();
    applyLayout->addStretch();
    m_applyBtn = new QPushButton(QStringLiteral("Apply Selected Theme"), this);
    m_applyBtn->setStyleSheet(buttonStyle);
    connect(m_applyBtn, &QPushButton::clicked, this, &ThemesPage::applyTheme);
    applyLayout->addWidget(m_applyBtn);
    mainLayout->addLayout(applyLayout);
}

void ThemesPage::loadThemes()
{
    m_listWidget->clear();

    int typeIndex = m_typeCombo->currentIndex();
    QStringList paths;

    if (typeIndex == 0) {
        paths << QDir::homePath() + QStringLiteral("/.local/share/plasma/desktoptheme/")
              << QStringLiteral("/usr/share/plasma/desktoptheme/");
    } else if (typeIndex == 1) {
        paths << QDir::homePath() + QStringLiteral("/.local/share/plasma/look-and-feel/")
              << QStringLiteral("/usr/share/plasma/look-and-feel/");
    } else {
        paths << QDir::homePath() + QStringLiteral("/.local/share/color-schemes/")
              << QStringLiteral("/usr/share/color-schemes/");
    }

    for (const QString &path : paths) {
        QDir dir(path);
        if (!dir.exists()) continue;

        QStringList nameFilters;
        if (typeIndex == 2) {
            nameFilters << QStringLiteral("*.colors");
        }

        QStringList entries = dir.entryList(typeIndex == 2 ? nameFilters : QStringList(),
                                            QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot);
        for (const QString &entry : entries) {
            QString display = entry;
            if (typeIndex == 2 && display.endsWith(QStringLiteral(".colors"))) {
                display.chop(7);
            }
            bool isSystem = path.startsWith(QStringLiteral("/usr/"));
            QString suffix = isSystem ? QStringLiteral(" [System]") : QStringLiteral(" [User]");
            QListWidgetItem *item = new QListWidgetItem(display + suffix, m_listWidget);
            item->setData(Qt::UserRole, dir.absoluteFilePath(entry));
            item->setIcon(QIcon::fromTheme(QStringLiteral("preferences-desktop-theme")));
        }
    }
}

void ThemesPage::installTheme()
{
    QString file = QFileDialog::getOpenFileName(this,
        QStringLiteral("Install Theme"),
        QDir::homePath(),
        QStringLiteral("Theme Files (*.tar.gz *.tar.xz *.zip *.desktoptheme *.colors)"));

    if (!file.isEmpty()) {
        QString dest;
        int typeIndex = m_typeCombo->currentIndex();
        if (typeIndex == 0) {
            dest = QDir::homePath() + QStringLiteral("/.local/share/plasma/desktoptheme/");
        } else if (typeIndex == 1) {
            dest = QDir::homePath() + QStringLiteral("/.local/share/plasma/look-and-feel/");
        } else {
            dest = QDir::homePath() + QStringLiteral("/.local/share/color-schemes/");
        }

        QDir().mkpath(dest);
        if (file.endsWith(QStringLiteral(".tar.gz")) || file.endsWith(QStringLiteral(".tar.xz"))) {
            SystemUtils::runCommand(QStringLiteral("tar xf \"%1\" -C \"%2\" 2>/dev/null").arg(file, dest));
        } else {
            QFile::copy(file, dest + QFileInfo(file).fileName());
        }

        loadThemes();
        QMessageBox::information(this, QStringLiteral("Theme Installed"), QStringLiteral("Theme has been installed successfully."));
    }
}

void ThemesPage::getNewThemes()
{
    SystemUtils::runCommand(QStringLiteral("kreadconfig5 --file kdeglobals --group KDE --key SingleClick 2>/dev/null"));
    QDesktopServices::openUrl(QUrl(QStringLiteral("https://store.kde.org/browse?cat=108&ord=rating")));
}

void ThemesPage::applyTheme()
{
    QListWidgetItem *item = m_listWidget->currentItem();
    if (!item) {
        QMessageBox::warning(this, QStringLiteral("No Selection"), QStringLiteral("Please select a theme to apply."));
        return;
    }

    QString themeName = item->text().split(QStringLiteral(" [")).first();
    int typeIndex = m_typeCombo->currentIndex();

    if (typeIndex == 0) {
        SystemUtils::runCommand(QStringLiteral("plasma-apply-desktoptheme \"%1\" 2>/dev/null").arg(themeName));
    } else if (typeIndex == 1) {
        SystemUtils::runCommand(QStringLiteral("plasma-apply-lookandfeel -a \"%1\" 2>/dev/null").arg(themeName));
    } else {
        ConfigManager::instance()->writeKDEConfig(QStringLiteral("kdeglobals"), QStringLiteral("General"), QStringLiteral("ColorScheme"), themeName);
    }

    QMessageBox::information(this, QStringLiteral("Theme Applied"), QStringLiteral("Theme '%1' has been applied.").arg(themeName));
}

void ThemesPage::refreshList()
{
    loadThemes();
}
