#pragma once

#include <QWidget>
#include <QListWidget>
#include <QPushButton>
#include <QLabel>
#include <QCheckBox>
#include <QVBoxLayout>

class StartupAppsPage : public QWidget
{
    Q_OBJECT

public:
    explicit StartupAppsPage(QWidget *parent = nullptr);

private slots:
    void addStartupApp();
    void removeStartupApp();
    void toggleStartupApp();
    void refreshList();

private:
    void setupUi();
    void loadStartupApps();

    QListWidget *m_listWidget;
    QPushButton *m_addBtn;
    QPushButton *m_removeBtn;
    QPushButton *m_refreshBtn;
    QLabel *m_countLabel;
};
