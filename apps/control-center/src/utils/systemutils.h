#pragma once

#include <QObject>
#include <QString>
#include <QStringList>
#include <QMap>

class SystemUtils : public QObject
{
    Q_OBJECT

public:
    explicit SystemUtils(QObject *parent = nullptr);

    static SystemUtils *instance();

    struct SystemInfo {
        QString distroName;
        QString distroVersion;
        QString kernelVersion;
        QString plasmaVersion;
        QString qtVersion;
        QString cpuModel;
        int cpuCores;
        quint64 totalMemory;
        quint64 usedMemory;
        QString hostname;
        QString username;
    };

    static QString runCommand(const QString &command, int timeoutMs = 5000);
    static QStringList runCommandGetLines(const QString &command, int timeoutMs = 5000);
    static bool runCommandSuccess(const QString &command);

    SystemInfo getSystemInfo();
    QString getDistroName();
    QString getKernelVersion();
    QString getPlasmaVersion();
    QString getQtVersion();
    QString getCpuInfo();
    int getCpuCores();
    quint64 getTotalMemoryKB();
    quint64 getUsedMemoryKB();
    QString getHostname();
    QString getCurrentUser();
    QString getDiskUsage(const QString &path = QStringLiteral("/"));
    QString getGpuInfo();
    QString getGpuDriver();
    bool isPackageInstalled(const QString &packageName);
    bool isServiceActive(const QString &serviceName);
    bool enableService(const QString &serviceName);
    bool disableService(const QString &serviceName);
    bool startService(const QString &serviceName);
    bool stopService(const QString &serviceName);
    QStringList getConnectedDevices();
    QStringList getWifiNetworks();
    bool isBluetoothEnabled();
    bool toggleBluetooth(bool enabled);
    QStringList getAudioSinks();
    QStringList getAudioSources();
    QString getCurrentAudioSink();
    QString getCurrentAudioSource();
    bool setDefaultAudioSink(const QString &sink);
    bool setDefaultAudioSource(const QString &source);
    bool setVolume(int volume);
    int getVolume();
    bool isMuted();
    bool toggleMute();
    QStringList getDisplays();
    bool setResolution(const QString &display, const QString &resolution);
    QStringList getNetworkConnections();
    bool isNetworkConnected();
    QStringList getStartupApps();
    bool addStartupApp(const QString &desktopFile);
    bool removeStartupApp(const QString &desktopFile);
    QStringList getInstalledPackages();
    int getInstalledPackageCount();
    bool isFlatpakInstalled();
    bool isAURHelperInstalled();
    QStringList getWallpapers();
    QStringList getBluetoothDevices();
    bool startBluetoothScan();
    QStringList getPairedBluetoothDevices();
    bool pairBluetoothDevice(const QString &address);
    bool unpairBluetoothDevice(const QString &address);
    QStringList getPowerProfiles();
    QString getCurrentPowerProfile();
    bool setPowerProfile(const QString &profile);
    bool isGameModeInstalled();
    bool isMangoHudInstalled();
    QStringList getThemes();
    bool isTelemetryDisabled();
    bool isLocationServicesEnabled();
    bool toggleLocationServices(bool enabled);

private:
    static SystemUtils *s_instance;
};
