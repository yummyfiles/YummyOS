#include <QApplication>
#include <QCommandLineParser>
#include <QFont>
#include <QIcon>
#include <QPalette>
#include <QQmlApplicationEngine>
#include <QQuickStyle>
#include <QTranslator>

#include <KAboutData>
#include <KLocalizedString>

#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    KAboutData aboutData(QStringLiteral("yummy-control-center"),
                         QStringLiteral("Yummy Control Center"),
                         QStringLiteral("1.0.0"),
                         QStringLiteral("System Settings for YummyOS"),
                         KAboutLicense::GPL_V3,
                         QStringLiteral("Copyright 2026 YummyOS Team"));
    KAboutData::setApplicationData(aboutData);

    app.setApplicationName(QStringLiteral("yummy-control-center"));
    app.setApplicationDisplayName(QStringLiteral("Yummy Control Center"));
    app.setApplicationVersion(QStringLiteral("1.0.0"));
    app.setOrganizationDomain(QStringLiteral("yummyos.org"));
    app.setWindowIcon(QIcon::fromTheme(QStringLiteral("preferences-system")));

    QTranslator translator;
    const QString locale = QLocale::system().name();
    if (translator.load(QLatin1String("yummy-control-center_") + locale)) {
        app.installTranslator(&translator);
    }
    KLocalizedString::setApplicationDomain("yummy-control-center");

    QQuickStyle::setStyle(QStringLiteral("Breeze"));

    QPalette darkPalette;
    darkPalette.setColor(QPalette::Window, QColor(0x1a, 0x1a, 0x2e));
    darkPalette.setColor(QPalette::WindowText, QColor(0xe0, 0xe0, 0xe0));
    darkPalette.setColor(QPalette::Base, QColor(0x16, 0x16, 0x2a));
    darkPalette.setColor(QPalette::AlternateBase, QColor(0x1e, 0x1e, 0x36));
    darkPalette.setColor(QPalette::ToolTipBase, QColor(0x2a, 0x2a, 0x44));
    darkPalette.setColor(QPalette::ToolTipText, QColor(0xe0, 0xe0, 0xe0));
    darkPalette.setColor(QPalette::Text, QColor(0xe0, 0xe0, 0xe0));
    darkPalette.setColor(QPalette::Button, QColor(0x22, 0x22, 0x3a));
    darkPalette.setColor(QPalette::ButtonText, QColor(0xe0, 0xe0, 0xe0));
    darkPalette.setColor(QPalette::BrightText, QColor(0xff, 0xff, 0xff));
    darkPalette.setColor(QPalette::Link, QColor(0x4C, 0xC9, 0xF0));
    darkPalette.setColor(QPalette::Highlight, QColor(0x4C, 0xC9, 0xF0));
    darkPalette.setColor(QPalette::HighlightedText, QColor(0x1a, 0x1a, 0x2e));
    darkPalette.setColor(QPalette::Disabled, QPalette::Text, QColor(0x66, 0x66, 0x80));
    darkPalette.setColor(QPalette::Disabled, QPalette::ButtonText, QColor(0x66, 0x66, 0x80));
    app.setPalette(darkPalette);

    QFont defaultFont(QStringLiteral("Noto Sans"), 10);
    defaultFont.setStyleStrategy(QFont::PreferAntialias);
    app.setFont(defaultFont);

    MainWindow window;
    window.resize(1100, 700);
    window.show();

    return app.exec();
}
