#pragma once

#include <QWidget>
#include <QListWidget>
#include <QPushButton>
#include <QLabel>
#include <QComboBox>

class ThemesPage : public QWidget
{
    Q_OBJECT

public:
    explicit ThemesPage(QWidget *parent = nullptr);

private slots:
    void installTheme();
    void getNewThemes();
    void applyTheme();
    void refreshList();

private:
    void setupUi();
    void loadThemes();

    QListWidget *m_listWidget;
    QComboBox *m_typeCombo;
    QPushButton *m_installBtn;
    QPushButton *m_getNewBtn;
    QPushButton *m_applyBtn;
    QPushButton *m_refreshBtn;
};
