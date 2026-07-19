#include "sidebarwidget.h"

#include <QVBoxLayout>
#include <QLabel>
#include <QScrollArea>

SidebarWidget::SidebarWidget(QWidget *parent)
    : QWidget(parent)
{
    setupUi();
}

void SidebarWidget::setupUi()
{
    setFixedWidth(240);
    setStyleSheet(QStringLiteral(
        "SidebarWidget { background-color: #16162a; }"
    ));

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    QLabel *logo = new QLabel(QStringLiteral("  YummyOS"), this);
    logo->setFixedHeight(56);
    logo->setStyleSheet(QStringLiteral(
        "QLabel { color: #4CC9F0; font-size: 18px; font-weight: bold; "
        "background-color: #16162a; padding: 14px 0; }"
    ));
    layout->addWidget(logo);

    QFrame *logoSep = new QFrame(this);
    logoSep->setFrameShape(QFrame::HLine);
    logoSep->setStyleSheet(QStringLiteral("color: #333355; max-height: 1px;"));
    layout->addWidget(logoSep);

    m_list = new QListWidget(this);
    m_list->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    m_list->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_list->setIconSize(QSize(20, 20));
    m_list->setSpacing(2);
    m_list->setStyleSheet(QStringLiteral(
        "QListWidget {"
        "  background-color: #16162a;"
        "  border: none;"
        "  outline: none;"
        "  padding: 6px 0;"
        "}"
        "QListWidget::item {"
        "  color: #b0b0cc;"
        "  padding: 10px 16px;"
        "  border-radius: 8px;"
        "  margin: 1px 8px;"
        "  font-size: 13px;"
        "}"
        "QListWidget::item:selected {"
        "  background-color: #222244;"
        "  color: #4CC9F0;"
        "  font-weight: bold;"
        "}"
        "QListWidget::item:hover:!selected {"
        "  background-color: #1e1e36;"
        "  color: #d0d0ee;"
        "}"
        "QScrollBar:vertical {"
        "  background: #16162a;"
        "  width: 6px;"
        "  border-radius: 3px;"
        "}"
        "QScrollBar::handle:vertical {"
        "  background: #333355;"
        "  border-radius: 3px;"
        "  min-height: 30px;"
        "}"
        "QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {"
        "  height: 0px;"
        "}"
    ));

    addSidebarItem(QStringLiteral("palette"), QStringLiteral("Appearance"));
    addSidebarItem(QStringLiteral("speed"), QStringLiteral("Performance"));
    addSidebarItem(QStringLiteral("gamepad"), QStringLiteral("Gaming"));
    addSidebarItem(QStringLiteral("cpu"), QStringLiteral("Drivers"));
    addSidebarItem(QStringLiteral("download"), QStringLiteral("Updates"));
    addSidebarItem(QStringLiteral("speaker"), QStringLiteral("Audio"));
    addSidebarItem(QStringLiteral("bluetooth"), QStringLiteral("Bluetooth"));
    addSidebarItem(QStringLiteral("network"), QStringLiteral("Network"));
    addSidebarItem(QStringLiteral("display"), QStringLiteral("Display"));
    addSidebarItem(QStringLiteral("battery"), QStringLiteral("Power"));
    addSidebarItem(QStringLiteral("shield"), QStringLiteral("Privacy"));
    addSidebarItem(QStringLiteral("rocket"), QStringLiteral("Startup Apps"));
    addSidebarItem(QStringLiteral("package"), QStringLiteral("Packages"));
    addSidebarItem(QStringLiteral("info"), QStringLiteral("System Info"));
    addSidebarItem(QStringLiteral("database"), QStringLiteral("Backup"));
    addSidebarItem(QStringLiteral("brush"), QStringLiteral("Themes"));
    addSidebarItem(QStringLiteral("image"), QStringLiteral("Wallpapers"));
    addSidebarItem(QStringLiteral("user"), QStringLiteral("User Accounts"));

    connect(m_list, &QListWidget::currentRowChanged, this, &SidebarWidget::onItemClicked);

    QScrollArea *scrollArea = new QScrollArea(this);
    scrollArea->setWidget(m_list);
    scrollArea->setWidgetResizable(true);
    scrollArea->setFrameShape(QFrame::NoFrame);
    scrollArea->setStyleSheet(QStringLiteral("QScrollArea { background-color: #16162a; border: none; }"));

    layout->addWidget(scrollArea, 1);

    m_list->setCurrentRow(0);
}

QListWidgetItem *SidebarWidget::addSidebarItem(const QString &iconName, const QString &text)
{
    QListWidgetItem *item = new QListWidgetItem(QIcon::fromTheme(iconName), text, m_list);
    item->setSizeHint(QSize(0, 42));
    item->setTextAlignment(Qt::AlignVCenter);
    return item;
}

void SidebarWidget::onItemClicked(int row)
{
    switch (row) {
    case 0:  emit appearanceClicked(); break;
    case 1:  emit performanceClicked(); break;
    case 2:  emit gamingClicked(); break;
    case 3:  emit driversClicked(); break;
    case 4:  emit updatesClicked(); break;
    case 5:  emit audioClicked(); break;
    case 6:  emit bluetoothClicked(); break;
    case 7:  emit networkClicked(); break;
    case 8:  emit displayClicked(); break;
    case 9:  emit powerClicked(); break;
    case 10: emit privacyClicked(); break;
    case 11: emit startupAppsClicked(); break;
    case 12: emit packagesClicked(); break;
    case 13: emit systemInfoClicked(); break;
    case 14: emit backupClicked(); break;
    case 15: emit themesClicked(); break;
    case 16: emit wallpapersClicked(); break;
    case 17: emit userAccountsClicked(); break;
    }
}
