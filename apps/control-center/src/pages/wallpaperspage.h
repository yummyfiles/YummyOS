#pragma once

#include <QWidget>
#include <QListWidget>
#include <QPushButton>
#include <QLabel>
#include <QGridLayout>
#include <QScrollArea>

class WallpapersPage : public QWidget
{
    Q_OBJECT

public:
    explicit WallpapersPage(QWidget *parent = nullptr);

private slots:
    void setWallpaper();
    void refreshWallpapers();

private:
    void setupUi();
    void loadWallpapers();

    QGridLayout *m_gridLayout;
    QWidget *m_gridWidget;
    QScrollArea *m_scrollArea;
    QPushButton *m_refreshBtn;
    QLabel *m_countLabel;
    QString m_selectedWallpaper;
    QList<QPushButton *> m_wallpaperButtons;
};
