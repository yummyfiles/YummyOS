#pragma once

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QCheckBox>
#include <QListWidget>
#include <QComboBox>
#include <QFileDialog>
#include <QLineEdit>

class BackupPage : public QWidget
{
    Q_OBJECT

public:
    explicit BackupPage(QWidget *parent = nullptr);

private slots:
    void selectBackupLocation();
    void startBackup();
    void startRestore();
    void toggleIncludeHome();
    void toggleIncludeConfig();

private:
    void setupUi();

    QLineEdit *m_locationEdit;
    QPushButton *m_browseBtn;
    QCheckBox *m_includeHomeCheck;
    QCheckBox *m_includeConfigCheck;
    QCheckBox *m_includeDocumentsCheck;
    QCheckBox *m_includePicturesCheck;
    QPushButton *m_backupBtn;
    QPushButton *m_restoreBtn;
    QLabel *m_statusLabel;
};
