#pragma once

#include <QObject>
#include <QProcess>
#include <QQmlEngine>
#include <QJsonObject>
#include <QJsonArray>
#include <QTimer>

class Backend : public QObject
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(bool commandRunning READ commandRunning NOTIFY commandRunningChanged)
    Q_PROPERTY(QString lastOutput READ lastOutput NOTIFY lastOutputChanged)
    Q_PROPERTY(QString lastError READ lastError NOTIFY lastErrorChanged)
    Q_PROPERTY(QString currentConnection READ currentConnection NOTIFY currentConnectionChanged)

public:
    explicit Backend(QObject *parent = nullptr);
    ~Backend();

    bool commandRunning() const { return m_commandRunning; }
    QString lastOutput() const { return m_lastOutput; }
    QString lastError() const { return m_lastError; }
    QString currentConnection() const { return m_currentConnection; }

    Q_INVOKABLE void scanWifiNetworks();
    Q_INVOKABLE void connectToWifi(const QString &ssid, const QString &password);
    Q_INVOKABLE void disconnectWifi();
    Q_INVOKABLE void detectGpu();
    Q_INVOKABLE void installGpuDriver(const QString &driver);
    Q_INVOKABLE void checkForUpdates();
    Q_INVOKABLE void installUpdates();
    Q_INVOKABLE void installPackage(const QString &package);
    Q_INVOKABLE void installPackages(const QStringList &packages);
    Q_INVOKABLE void applyTheme(const QString &theme);
    Q_INVOKABLE void setAccentColor(const QString &color);
    Q_INVOKABLE void setWallpaper(const QString &path);
    Q_INVOKABLE void runCommand(const QString &command);
    Q_INVOKABLE void importSettings(const QString &sourcePath);
    Q_INVOKABLE void launchDesktop();
    Q_INVOKABLE void openControlCenter();

signals:
    void commandRunningChanged();
    void lastOutputChanged();
    void lastErrorChanged();
    void currentConnectionChanged();
    void wifiNetworksFound(const QVariantList &networks);
    void gpuDetected(const QString &gpuType, const QString &recommendedDriver);
    void updateCheckComplete(int updateCount, const QVariantList &updates);
    void packageInstalled(const QString &package, bool success, const QString &message);
    void installProgress(const QString &package, int progress);
    void settingsImported(bool success, const QString &message);

private slots:
    void onProcessFinished(int exitCode, QProcess::ExitStatus exitStatus);
    void onProcessError(QProcess::ProcessError error);
    void onProcessReadyRead();

private:
    void runAsync(const QString &command, const QString &tag = QString());
    void setCommandRunning(bool running);

    QProcess *m_process = nullptr;
    bool m_commandRunning = false;
    QString m_lastOutput;
    QString m_lastError;
    QString m_currentConnection;
    QString m_currentTag;
};
