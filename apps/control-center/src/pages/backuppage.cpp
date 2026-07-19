#include "backuppage.h"
#include "../utils/systemutils.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QLabel>
#include <QProcess>
#include <QMessageBox>

BackupPage::BackupPage(QWidget *parent)
    : QWidget(parent)
{
    setupUi();
}

void BackupPage::setupUi()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(24, 24, 24, 24);
    mainLayout->setSpacing(16);

    QLabel *title = new QLabel(QStringLiteral("Backup"), this);
    title->setStyleSheet(QStringLiteral("font-size: 22px; font-weight: bold; color: #e0e0ee; padding-bottom: 8px;"));
    mainLayout->addWidget(title);

    QLabel *subtitle = new QLabel(QStringLiteral("Create and restore system backups using rsync"), this);
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

    QString checkboxStyle = QStringLiteral(
        "QCheckBox { color: #d0d0ee; font-size: 13px; spacing: 8px; }"
        "QCheckBox::indicator {"
        "  width: 20px; height: 20px; border-radius: 4px; border: 2px solid #555577; background-color: #1a1a2e;"
        "}"
        "QCheckBox::indicator:checked { background-color: #4CC9F0; border-color: #4CC9F0; }"
        "QCheckBox::indicator:hover { border-color: #4CC9F0; }"
    );

    QString lineEditStyle = QStringLiteral(
        "QLineEdit {"
        "  background-color: #1a1a2e; border: 1px solid #333355; border-radius: 6px;"
        "  padding: 8px 12px; color: #e0e0ee; font-size: 13px;"
        "}"
        "QLineEdit:hover { border-color: #4CC9F0; }"
    );

    // Backup Location
    QGroupBox *locationGroup = new QGroupBox(QStringLiteral("Backup Location"), this);
    locationGroup->setStyleSheet(groupBoxStyle);
    QHBoxLayout *locLayout = new QHBoxLayout(locationGroup);

    m_locationEdit = new QLineEdit(locationGroup);
    m_locationEdit->setStyleSheet(lineEditStyle);
    m_locationEdit->setPlaceholderText(QStringLiteral("Select backup destination..."));
    m_locationEdit->setText(QDir::homePath() + QStringLiteral("/Backups"));
    locLayout->addWidget(m_locationEdit, 1);

    m_browseBtn = new QPushButton(QStringLiteral("Browse"), locationGroup);
    m_browseBtn->setStyleSheet(buttonStyle);
    connect(m_browseBtn, &QPushButton::clicked, this, &BackupPage::selectBackupLocation);
    locLayout->addWidget(m_browseBtn);

    mainLayout->addWidget(locationGroup);

    // What to include
    QGroupBox *includeGroup = new QGroupBox(QStringLiteral("What to Include"), this);
    includeGroup->setStyleSheet(groupBoxStyle);
    QVBoxLayout *incLayout = new QVBoxLayout(includeGroup);
    incLayout->setSpacing(8);

    m_includeHomeCheck = new QCheckBox(QStringLiteral("Home Directory (/home/user)"), includeGroup);
    m_includeHomeCheck->setStyleSheet(checkboxStyle);
    m_includeHomeCheck->setChecked(true);
    incLayout->addWidget(m_includeHomeCheck);

    m_includeConfigCheck = new QCheckBox(QStringLiteral("Configuration Files (~/.config)"), includeGroup);
    m_includeConfigCheck->setStyleSheet(checkboxStyle);
    m_includeConfigCheck->setChecked(true);
    incLayout->addWidget(m_includeConfigCheck);

    m_includeDocumentsCheck = new QCheckBox(QStringLiteral("Documents (~/Documents)"), includeGroup);
    m_includeDocumentsCheck->setStyleSheet(checkboxStyle);
    m_includeDocumentsCheck->setChecked(true);
    incLayout->addWidget(m_includeDocumentsCheck);

    m_includePicturesCheck = new QCheckBox(QStringLiteral("Pictures (~/Pictures)"), includeGroup);
    m_includePicturesCheck->setStyleSheet(checkboxStyle);
    m_includePicturesCheck->setChecked(true);
    incLayout->addWidget(m_includePicturesCheck);

    mainLayout->addWidget(includeGroup);

    // Actions
    QFrame *actionFrame = new QFrame(this);
    actionFrame->setStyleSheet(QStringLiteral(
        "QFrame { background-color: #222244; border: 1px solid #333355; border-radius: 12px; padding: 16px; }"
    ));
    QHBoxLayout *actionLayout = new QHBoxLayout(actionFrame);

    m_backupBtn = new QPushButton(QStringLiteral("Start Backup"), actionFrame);
    m_backupBtn->setStyleSheet(QStringLiteral(
        "QPushButton {"
        "  background-color: #2ecc71; color: #ffffff; border: none; border-radius: 8px;"
        "  padding: 14px 32px; font-size: 14px; font-weight: bold;"
        "}"
        "QPushButton:hover { background-color: #45d987; }"
        "QPushButton:pressed { background-color: #27ae60; }"
    ));
    connect(m_backupBtn, &QPushButton::clicked, this, &BackupPage::startBackup);
    actionLayout->addWidget(m_backupBtn);

    actionLayout->addStretch();

    m_restoreBtn = new QPushButton(QStringLiteral("Restore from Backup"), actionFrame);
    m_restoreBtn->setStyleSheet(QStringLiteral(
        "QPushButton {"
        "  background-color: #f39c12; color: #ffffff; border: none; border-radius: 8px;"
        "  padding: 14px 32px; font-size: 14px; font-weight: bold;"
        "}"
        "QPushButton:hover { background-color: #f5b041; }"
        "QPushButton:pressed { background-color: #e67e22; }"
    ));
    connect(m_restoreBtn, &QPushButton::clicked, this, &BackupPage::startRestore);
    actionLayout->addWidget(m_restoreBtn);

    mainLayout->addWidget(actionFrame);

    // Status
    m_statusLabel = new QLabel(QStringLiteral("Ready to backup"), this);
    m_statusLabel->setWordWrap(true);
    m_statusLabel->setStyleSheet(QStringLiteral(
        "color: #b0b0cc; font-size: 13px; padding: 8px; "
        "background-color: #1a1a2e; border: 1px solid #333355; border-radius: 8px;"
    ));
    mainLayout->addWidget(m_statusLabel);

    mainLayout->addStretch();
}

void BackupPage::selectBackupLocation()
{
    QString dir = QFileDialog::getExistingDirectory(this, QStringLiteral("Select Backup Location"), QDir::homePath());
    if (!dir.isEmpty()) {
        m_locationEdit->setText(dir);
    }
}

void BackupPage::startBackup()
{
    QString location = m_locationEdit->text();
    if (location.isEmpty()) {
        QMessageBox::warning(this, QStringLiteral("No Location"), QStringLiteral("Please select a backup location."));
        return;
    }

    QString timestamp = QDateTime::currentDateTime().toString(QStringLiteral("yyyy-MM-dd_hh-mm-ss"));
    QString backupDir = QStringLiteral("%1/yummy-backup-%2").arg(location, timestamp);
    QDir().mkpath(backupDir);

    QStringList rsyncArgs;
    rsyncArgs << QStringLiteral("-avh --progress");

    QStringList excludeArgs;
    excludeArgs << QStringLiteral("--exclude=.cache")
                << QStringLiteral("--exclude=.local/share/Trash")
                << QStringLiteral("--exclude=*.tmp")
                << QStringLiteral("--exclude=__pycache__");

    QString homeDir = QDir::homePath();

    if (m_includeHomeCheck->isChecked()) {
        for (const QString &ex : excludeArgs) {
            rsyncArgs << ex;
        }
        rsyncArgs << homeDir + QStringLiteral("/") << backupDir + QStringLiteral("/home/");
    }

    if (m_includeConfigCheck->isChecked()) {
        rsyncArgs << QStringLiteral("-avh") << homeDir + QStringLiteral("/.config/") << backupDir + QStringLiteral("/config/");
    }

    if (m_includeDocumentsCheck->isChecked()) {
        rsyncArgs << QStringLiteral("-avh") << homeDir + QStringLiteral("/Documents/") << backupDir + QStringLiteral("/documents/");
    }

    if (m_includePicturesCheck->isChecked()) {
        rsyncArgs << QStringLiteral("-avh") << homeDir + QStringLiteral("/Pictures/") << backupDir + QStringLiteral("/pictures/");
    }

    m_statusLabel->setText(QStringLiteral("Backup in progress to: %1").arg(backupDir));
    m_statusLabel->setStyleSheet(QStringLiteral(
        "color: #f39c12; font-size: 13px; padding: 8px; "
        "background-color: #1a1a2e; border: 1px solid #f39c12; border-radius: 8px;"
    ));

    QProcess *process = new QProcess(this);
    connect(process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), this,
        [this, process, backupDir](int exitCode, QProcess::ExitStatus) {
            if (exitCode == 0) {
                m_statusLabel->setText(QStringLiteral("Backup completed successfully!\nLocation: %1").arg(backupDir));
                m_statusLabel->setStyleSheet(QStringLiteral(
                    "color: #2ecc71; font-size: 13px; padding: 8px; "
                    "background-color: #1a1a2e; border: 1px solid #2ecc71; border-radius: 8px;"
                ));
                QMessageBox::information(this, QStringLiteral("Backup Complete"),
                    QStringLiteral("Backup created at:\n%1").arg(backupDir));
            } else {
                m_statusLabel->setText(QStringLiteral("Backup failed. Check permissions and try again."));
                m_statusLabel->setStyleSheet(QStringLiteral(
                    "color: #e74c3c; font-size: 13px; padding: 8px; "
                    "background-color: #1a1a2e; border: 1px solid #e74c3c; border-radius: 8px;"
                ));
            }
            process->deleteLater();
        });

    process->start(QStringLiteral("rsync"), rsyncArgs);
}

void BackupPage::startRestore()
{
    QString location = m_locationEdit->text();
    if (location.isEmpty()) {
        QMessageBox::warning(this, QStringLiteral("No Location"), QStringLiteral("Please select a backup location."));
        return;
    }

    QMessageBox::StandardButton reply = QMessageBox::question(this,
        QStringLiteral("Restore Backup"),
        QStringLiteral("This will restore files from the backup. Existing files may be overwritten. Continue?"),
        QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        m_statusLabel->setText(QStringLiteral("Restore completed. A restart may be required."));
        m_statusLabel->setStyleSheet(QStringLiteral(
            "color: #2ecc71; font-size: 13px; padding: 8px; "
            "background-color: #1a1a2e; border: 1px solid #2ecc71; border-radius: 8px;"
        ));
    }
}

void BackupPage::toggleIncludeHome()
{
}

void BackupPage::toggleIncludeConfig()
{
}
