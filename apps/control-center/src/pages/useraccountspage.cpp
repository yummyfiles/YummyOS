#include "useraccountspage.h"
#include "../utils/systemutils.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QLabel>
#include <QProcess>
#include <QMessageBox>
#include <QPixmap>

UserAccountsPage::UserAccountsPage(QWidget *parent)
    : QWidget(parent)
{
    setupUi();
    loadUserInfo();
}

void UserAccountsPage::setupUi()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(24, 24, 24, 24);
    mainLayout->setSpacing(16);

    QLabel *title = new QLabel(QStringLiteral("User Accounts"), this);
    title->setStyleSheet(QStringLiteral("font-size: 22px; font-weight: bold; color: #e0e0ee; padding-bottom: 8px;"));
    mainLayout->addWidget(title);

    QLabel *subtitle = new QLabel(QStringLiteral("Manage your user account settings"), this);
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

    QString lineEditStyle = QStringLiteral(
        "QLineEdit {"
        "  background-color: #1a1a2e; border: 1px solid #333355; border-radius: 6px;"
        "  padding: 8px 12px; color: #e0e0ee; font-size: 13px;"
        "}"
        "QLineEdit:hover { border-color: #4CC9F0; }"
    );

    // User Info
    QGroupBox *userGroup = new QGroupBox(QStringLiteral("User Information"), this);
    userGroup->setStyleSheet(groupBoxStyle);
    QVBoxLayout *userLayout = new QVBoxLayout(userGroup);
    userLayout->setSpacing(16);

    // Avatar
    QHBoxLayout *avatarLayout = new QHBoxLayout();

    m_avatarLabel = new QLabel(userGroup);
    m_avatarLabel->setFixedSize(100, 100);
    m_avatarLabel->setStyleSheet(QStringLiteral(
        "background-color: #4CC9F0; border-radius: 50px; font-size: 36px; font-weight: bold; color: #1a1a2e;"
    ));
    m_avatarLabel->setAlignment(Qt::AlignCenter);
    avatarLayout->addWidget(m_avatarLabel);

    QVBoxLayout *infoLayout = new QVBoxLayout();

    QHBoxLayout *usernameRow = new QHBoxLayout();
    QLabel *usernameKey = new QLabel(QStringLiteral("Username:"), userGroup);
    usernameKey->setStyleSheet(QStringLiteral("color: #b0b0cc; font-size: 13px; font-weight: bold;"));
    usernameRow->addWidget(usernameKey);
    m_usernameLabel = new QLabel(QStringLiteral("Loading..."), userGroup);
    m_usernameLabel->setStyleSheet(QStringLiteral("color: #e0e0ee; font-size: 13px;"));
    usernameRow->addWidget(m_usernameLabel);
    usernameRow->addStretch();
    infoLayout->addLayout(usernameRow);

    QHBoxLayout *hostnameRow = new QHBoxLayout();
    QLabel *hostnameKey = new QLabel(QStringLiteral("Hostname:"), userGroup);
    hostnameKey->setStyleSheet(QStringLiteral("color: #b0b0cc; font-size: 13px; font-weight: bold;"));
    hostnameRow->addWidget(hostnameKey);
    m_hostnameLabel = new QLabel(QStringLiteral("Loading..."), userGroup);
    m_hostnameLabel->setStyleSheet(QStringLiteral("color: #e0e0ee; font-size: 13px;"));
    hostnameRow->addWidget(m_hostnameLabel);
    hostnameRow->addStretch();
    infoLayout->addLayout(hostnameRow);

    avatarLayout->addLayout(infoLayout);
    avatarLayout->addStretch();
    userLayout->addLayout(avatarLayout);

    m_changeAvatarBtn = new QPushButton(QStringLiteral("Change Avatar"), userGroup);
    m_changeAvatarBtn->setStyleSheet(buttonStyle);
    connect(m_changeAvatarBtn, &QPushButton::clicked, this, &UserAccountsPage::changeAvatar);
    userLayout->addWidget(m_changeAvatarBtn);

    mainLayout->addWidget(userGroup);

    // Hostname
    QGroupBox *hostnameGroup = new QGroupBox(QStringLiteral("Hostname"), this);
    hostnameGroup->setStyleSheet(groupBoxStyle);
    QVBoxLayout *hostLayout = new QVBoxLayout(hostnameGroup);
    hostLayout->setSpacing(10);

    QLabel *hostDesc = new QLabel(QStringLiteral("Change the hostname of this computer. This requires administrator privileges."), hostnameGroup);
    hostDesc->setWordWrap(true);
    hostDesc->setStyleSheet(QStringLiteral("color: #b0b0cc; font-size: 13px;"));
    hostLayout->addWidget(hostDesc);

    QHBoxLayout *hostEditLayout = new QHBoxLayout();
    m_hostnameEdit = new QLineEdit(hostnameGroup);
    m_hostnameEdit->setStyleSheet(lineEditStyle);
    m_hostnameEdit->setPlaceholderText(QStringLiteral("Enter new hostname..."));
    hostEditLayout->addWidget(m_hostnameEdit, 1);

    m_changeHostnameBtn = new QPushButton(QStringLiteral("Change Hostname"), hostnameGroup);
    m_changeHostnameBtn->setStyleSheet(buttonStyle);
    connect(m_changeHostnameBtn, &QPushButton::clicked, this, &UserAccountsPage::changeHostname);
    hostEditLayout->addWidget(m_changeHostnameBtn);

    hostLayout->addLayout(hostEditLayout);

    mainLayout->addWidget(hostnameGroup);

    mainLayout->addStretch();
}

void UserAccountsPage::loadUserInfo()
{
    QString username = SystemUtils::instance()->getCurrentUser();
    QString hostname = SystemUtils::instance()->getHostname();

    m_usernameLabel->setText(username);
    m_hostnameLabel->setText(hostname);
    m_hostnameEdit->setText(hostname);

    QString initial = username.isEmpty() ? QStringLiteral("?") : username.left(1).toUpper();
    m_avatarLabel->setText(initial);
}

void UserAccountsPage::changeAvatar()
{
    QString file = QFileDialog::getOpenFileName(this,
        QStringLiteral("Select Avatar Image"),
        QDir::homePath() + QStringLiteral("/Pictures"),
        QStringLiteral("Images (*.png *.jpg *.jpeg *.bmp *.svg)"));

    if (!file.isEmpty()) {
        QPixmap pixmap(file);
        if (!pixmap.isNull()) {
            QPixmap scaled = pixmap.scaled(100, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation);
            QPixmap rounded(100, 100);
            rounded.fill(Qt::transparent);
            QPainter painter(&rounded);
            painter.setRenderHint(QPainter::Antialiasing);
            QPainterPath path;
            path.addRoundedRect(0, 0, 100, 100, 50, 50);
            painter.setClipPath(path);
            painter.drawPixmap(0, 0, scaled);
            m_avatarLabel->setPixmap(rounded);

            QString avatarDir = QDir::homePath() + QStringLiteral("/.local/share/avatars");
            QDir().mkpath(avatarDir);
            QString dest = avatarDir + QStringLiteral("/user-avatar.png");
            pixmap.save(dest);

            SystemUtils::runCommand(QStringLiteral("sudo cp \"%1\" /var/lib/AccountsService/icons/\"%2\" 2>/dev/null")
                .arg(dest, SystemUtils::instance()->getCurrentUser()));

            QMessageBox::information(this, QStringLiteral("Avatar Changed"), QStringLiteral("Your avatar has been updated."));
        }
    }
}

void UserAccountsPage::changeHostname()
{
    QString newHostname = m_hostnameEdit->text().trimmed();
    if (newHostname.isEmpty()) {
        QMessageBox::warning(this, QStringLiteral("Invalid Hostname"), QStringLiteral("Hostname cannot be empty."));
        return;
    }

    QRegularExpression re(QStringLiteral("^[a-zA-Z0-9]([a-zA-Z0-9\\-]*[a-zA-Z0-9])?$"));
    if (!re.match(newHostname).hasMatch()) {
        QMessageBox::warning(this, QStringLiteral("Invalid Hostname"),
            QStringLiteral("Hostname must contain only alphanumeric characters and hyphens."));
        return;
    }

    QMessageBox::StandardButton reply = QMessageBox::question(this,
        QStringLiteral("Change Hostname"),
        QStringLiteral("Change hostname to '%1'?\nThis requires administrator privileges and a reboot.").arg(newHostname),
        QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        bool success = SystemUtils::runCommandSuccess(
            QStringLiteral("sudo hostnamectl set-hostname \"%1\" 2>/dev/null").arg(newHostname));

        if (success) {
            m_hostnameLabel->setText(newHostname);
            QMessageBox::information(this, QStringLiteral("Hostname Changed"),
                QStringLiteral("Hostname changed to '%1'. A reboot is required for changes to take full effect.").arg(newHostname));
        } else {
            QMessageBox::warning(this, QStringLiteral("Error"),
                QStringLiteral("Failed to change hostname. You may need to run this application as administrator."));
        }
    }
}
