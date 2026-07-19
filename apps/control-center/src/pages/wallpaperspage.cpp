#include "wallpaperspage.h"
#include "../utils/systemutils.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QMessageBox>
#include <QProcess>
#include <QPixmap>
#include <QIcon>

WallpapersPage::WallpapersPage(QWidget *parent)
    : QWidget(parent)
{
    setupUi();
    loadWallpapers();
}

void WallpapersPage::setupUi()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(24, 24, 24, 24);
    mainLayout->setSpacing(16);

    QLabel *title = new QLabel(QStringLiteral("Wallpapers"), this);
    title->setStyleSheet(QStringLiteral("font-size: 22px; font-weight: bold; color: #e0e0ee; padding-bottom: 8px;"));
    mainLayout->addWidget(title);

    QLabel *subtitle = new QLabel(QStringLiteral("Choose a wallpaper for your desktop"), this);
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

    // Toolbar
    QHBoxLayout *toolbarLayout = new QHBoxLayout();
    toolbarLayout->addStretch();

    m_countLabel = new QLabel(this);
    m_countLabel->setStyleSheet(QStringLiteral("color: #8888aa; font-size: 12px;"));
    toolbarLayout->addWidget(m_countLabel);

    m_refreshBtn = new QPushButton(QStringLiteral("Refresh"), this);
    m_refreshBtn->setStyleSheet(buttonStyle);
    connect(m_refreshBtn, &QPushButton::clicked, this, &WallpapersPage::refreshWallpapers);
    toolbarLayout->addWidget(m_refreshBtn);

    mainLayout->addLayout(toolbarLayout);

    // Grid scroll area
    m_scrollArea = new QScrollArea(this);
    m_scrollArea->setWidgetResizable(true);
    m_scrollArea->setFrameShape(QFrame::NoFrame);
    m_scrollArea->setStyleSheet(QStringLiteral("QScrollArea { background-color: #1a1a2e; border: none; }"));

    m_gridWidget = new QWidget();
    m_gridLayout = new QGridLayout(m_gridWidget);
    m_gridLayout->setSpacing(12);
    m_gridLayout->setContentsMargins(4, 4, 4, 4);

    m_scrollArea->setWidget(m_gridWidget);
    mainLayout->addWidget(m_scrollArea, 1);

    // Set wallpaper button
    QHBoxLayout *btnLayout = new QHBoxLayout();
    btnLayout->addStretch();

    QPushButton *setWallpaperBtn = new QPushButton(QStringLiteral("Set as Wallpaper"), this);
    setWallpaperBtn->setStyleSheet(QStringLiteral(
        "QPushButton {"
        "  background-color: #7B2CBF; color: #ffffff; border: none; border-radius: 8px;"
        "  padding: 12px 32px; font-size: 14px; font-weight: bold;"
        "}"
        "QPushButton:hover { background-color: #9333d9; }"
        "QPushButton:pressed { background-color: #6a1faa; }"
    ));
    connect(setWallpaperBtn, &QPushButton::clicked, this, &WallpapersPage::setWallpaper);
    btnLayout->addWidget(setWallpaperBtn);

    mainLayout->addLayout(btnLayout);
}

void WallpapersPage::loadWallpapers()
{
    QLayoutItem *item;
    while ((item = m_gridLayout->takeAt(0)) != nullptr) {
        if (item->widget()) {
            item->widget()->deleteLater();
        }
        delete item;
    }
    m_wallpaperButtons.clear();

    QStringList wallpapers = SystemUtils::instance()->getWallpapers();
    m_countLabel->setText(QStringLiteral("%1 wallpapers found").arg(wallpapers.size()));

    int cols = 4;
    int thumbSize = 220;

    for (int i = 0; i < wallpapers.size(); i++) {
        const QString &path = wallpapers[i];

        QPushButton *thumbBtn = new QPushButton(m_gridWidget);
        thumbBtn->setFixedSize(thumbSize, thumbSize + 24);
        thumbBtn->setIconSize(QSize(thumbSize - 10, thumbSize - 10));
        thumbBtn->setToolTip(QFileInfo(path).fileName());
        thumbBtn->setStyleSheet(QStringLiteral(
            "QPushButton {"
            "  background-color: #16162a; border: 2px solid #333355; border-radius: 10px;"
            "  padding: 4px;"
            "}"
            "QPushButton:hover { border-color: #4CC9F0; }"
        ));

        QPixmap pix(path);
        if (!pix.isNull()) {
            QPixmap scaled = pix.scaled(thumbSize - 20, thumbSize - 20, Qt::KeepAspectRatio, Qt::SmoothTransformation);
            thumbBtn->setIcon(QIcon(scaled));
        } else {
            thumbBtn->setIcon(QIcon::fromTheme(QStringLiteral("image-x-generic")));
        }

        connect(thumbBtn, &QPushButton::clicked, this, [this, path, thumbBtn]() {
            m_selectedWallpaper = path;
            for (QPushButton *btn : m_wallpaperButtons) {
                btn->setStyleSheet(QStringLiteral(
                    "QPushButton {"
                    "  background-color: #16162a; border: 2px solid #333355; border-radius: 10px;"
                    "  padding: 4px;"
                    "}"
                    "QPushButton:hover { border-color: #4CC9F0; }"
                ));
            }
            thumbBtn->setStyleSheet(QStringLiteral(
                "QPushButton {"
                "  background-color: #16162a; border: 2px solid #4CC9F0; border-radius: 10px;"
                "  padding: 4px;"
                "}"
            ));
        });

        int row = i / cols;
        int col = i % cols;
        m_gridLayout->addWidget(thumbBtn, row, col);
        m_wallpaperButtons.append(thumbBtn);
    }
}

void WallpapersPage::setWallpaper()
{
    if (m_selectedWallpaper.isEmpty()) {
        QMessageBox::warning(this, QStringLiteral("No Selection"), QStringLiteral("Please select a wallpaper first."));
        return;
    }

    SystemUtils::runCommand(QStringLiteral(
        "qdbus org.kde.plasmashell /PlasmaShell org.kde.PlasmaShell.evaluateScript \"var allDesktops = desktops(); for (var i=0; i<allDesktops.length; i++) { allDesktops[i].wallpaperPlugin = 'org.kde.image'; allDesktops[i].currentConfig.Group = 'Wallpaper'; allDesktops[i].currentConfig.Image = '%1'; allDesktops[i].reloadConfig(); }\" 2>/dev/null"
    ).arg(m_selectedWallpaper));

    SystemUtils::runCommand(QStringLiteral(
        "gsettings set org.gnome.desktop.background picture-uri 'file://%1' 2>/dev/null"
    ).arg(m_selectedWallpaper));

    QMessageBox::information(this, QStringLiteral("Wallpaper Set"),
        QStringLiteral("Wallpaper has been set to:\n%1").arg(QFileInfo(m_selectedWallpaper).fileName()));
}

void WallpapersPage::refreshWallpapers()
{
    loadWallpapers();
}
