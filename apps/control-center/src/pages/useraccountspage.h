#pragma once

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QFileDialog>

class UserAccountsPage : public QWidget
{
    Q_OBJECT

public:
    explicit UserAccountsPage(QWidget *parent = nullptr);

private slots:
    void changeAvatar();
    void changeHostname();

private:
    void setupUi();
    void loadUserInfo();

    QLabel *m_usernameLabel;
    QLabel *m_hostnameLabel;
    QLabel *m_avatarLabel;
    QPushButton *m_changeAvatarBtn;
    QPushButton *m_changeHostnameBtn;
    QLineEdit *m_hostnameEdit;
};
