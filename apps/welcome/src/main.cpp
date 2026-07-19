#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickView>
#include <QQuickStyle>
#include <QPalette>
#include <QColor>
#include <QFont>
#include <QIcon>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    app.setApplicationName("Yummy Welcome");
    app.setOrganizationName("YummyOS");
    app.setApplicationDisplayName("Yummy Welcome");
    app.setDesktopFileName("yummy-welcome");

    QQuickStyle::setStyle("Basic");

    QPalette darkPalette;
    darkPalette.setColor(QPalette::Window, QColor("#1a1a2e"));
    darkPalette.setColor(QPalette::WindowText, QColor("#e0e0e0"));
    darkPalette.setColor(QPalette::Base, QColor("#16213e"));
    darkPalette.setColor(QPalette::AlternateBase, QColor("#1a1a2e"));
    darkPalette.setColor(QPalette::ToolTipBase, QColor("#1a1a2e"));
    darkPalette.setColor(QPalette::ToolTipText, QColor("#e0e0e0"));
    darkPalette.setColor(QPalette::Text, QColor("#e0e0e0"));
    darkPalette.setColor(QPalette::Button, QColor("#0f3460"));
    darkPalette.setColor(QPalette::ButtonText, QColor("#e0e0e0"));
    darkPalette.setColor(QPalette::BrightText, QColor("#ff0000"));
    darkPalette.setColor(QPalette::Link, QColor("#4CC9F0"));
    darkPalette.setColor(QPalette::Highlight, QColor("#4CC9F0"));
    darkPalette.setColor(QPalette::HighlightedText, QColor("#1a1a2e"));
    darkPalette.setColor(QPalette::Disabled, QPalette::Text, QColor("#666666"));
    darkPalette.setColor(QPalette::Disabled, QPalette::ButtonText, QColor("#666666"));

    app.setPalette(darkPalette);

    QFont defaultFont("Segoe UI", 10);
    defaultFont.setHintingPreference(QFont::PreferFullHinting);
    app.setFont(defaultFont);

    QQmlApplicationEngine engine;

    engine.addImportPath("qrc:///");
    engine.addImportPath("/usr/lib/qt6/qml");

    const QUrl qmlUrl(QUrl::fromLocalFile(":/src/qml/main.qml"));

    QObject::connect(&engine, &QQmlApplicationEngine::objectCreationFailed,
        &app, []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);

    engine.load(qmlUrl);

    if (engine.rootObjects().isEmpty()) {
        return -1;
    }

    return app.exec();
}
