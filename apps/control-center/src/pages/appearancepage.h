#pragma once

#include <QWidget>
#include <QComboBox>
#include <QPushButton>
#include <QLabel>
#include <QColorDialog>
#include <QFontComboBox>

class AppearancePage : public QWidget
{
    Q_OBJECT

public:
    explicit AppearancePage(QWidget *parent = nullptr);

private slots:
    void applyTheme();
    void applyAccentColor();
    void applyFont();

private:
    void setupUi();
    void loadCurrentSettings();

    QComboBox *m_themeCombo;
    QComboBox *m_accentCombo;
    QComboBox *m_iconThemeCombo;
    QComboBox *m_cursorThemeCombo;
    QFontComboBox *m_fontCombo;
    QSpinBox *m_fontSizeSpin;
    QLabel *m_accentPreview;
    QPushButton *m_applyButton;
    QPushButton *m_applyFontButton;
    QPushButton *m_applyAccentButton;
};
