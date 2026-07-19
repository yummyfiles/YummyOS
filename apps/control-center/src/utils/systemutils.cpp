#include "systemutils.h"

#include <QProcess>
#include <QFile>
#include <QDir>
#include <QTextStream>
#include <QRegularExpression>
#include <QStandardPaths>

SystemUtils *SystemUtils::s_instance = nullptr;

SystemUtils::SystemUtils(QObject *parent)
    : QObject(parent)
{
}

SystemUtils *SystemUtils::instance()
{
    if (!s_instance) {
        s_instance = new SystemUtils;
    }
    return s_instance;
}

QString SystemUtils::runCommand(const QString &command, int timeoutMs)
{
    QProcess process;
    process.start(QStringLiteral("sh"), QStringList() << QStringLiteral("-c") << command);
    if (!process.waitForStarted(timeoutMs)) {
        return QString();
    }
    if (!process.waitForFinished(timeoutMs)) {
        process.kill();
        return QString();
    }
    return QString::fromLocal8Bit(process.readAllStandardOutput()).trimmed();
}

QStringList SystemUtils::runCommandGetLines(const QString &command, int timeoutMs)
{
    QString output = runCommand(command, timeoutMs);
    return output.split(QLatin1Char('\n'), Qt::SkipEmptyParts);
}

bool SystemUtils::runCommandSuccess(const QString &command)
{
    QProcess process;
    process.start(QStringLiteral("sh"), QStringList() << QStringLiteral("-c") << command);
    if (!process.waitForStarted(5000)) {
        return false;
    }
    process.waitForFinished(5000);
    return process.exitCode() == 0;
}

SystemUtils::SystemInfo SystemUtils::getSystemInfo()
{
    SystemInfo info;
    info.distroName = getDistroName();
    info.kernelVersion = getKernelVersion();
    info.plasmaVersion = getPlasmaVersion();
    info.qtVersion = getQtVersion();
    info.cpuModel = getCpuInfo();
    info.cpuCores = getCpuCores();
    info.totalMemoryKB = getTotalMemoryKB();
    info.usedMemoryKB = getUsedMemoryKB();
    info.hostname = getHostname();
    info.username = getCurrentUser();
    return info;
}

QString SystemUtils::getDistroName()
{
    QFile file(QStringLiteral("/etc/os-release"));
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream stream(&file);
        while (!stream.atEnd()) {
            QString line = stream.readLine();
            if (line.startsWith(QStringLiteral("PRETTY_NAME="))) {
                return line.section(QLatin1Char('='), 1).remove(QLatin1Char('"'));
            }
        }
    }
    return runCommand(QStringLiteral("lsb_release -d 2>/dev/null | cut -f2"));
}

QString SystemUtils::getKernelVersion()
{
    return runCommand(QStringLiteral("uname -r"));
}

QString SystemUtils::getPlasmaVersion()
{
    return runCommand(QStringLiteral("plasmashell --version 2>/dev/null | head -1 | awk '{print $2}'"));
}

QString SystemUtils::getQtVersion()
{
    return runCommand(QStringLiteral("qmake6 -query QT_VERSION 2>/dev/null || qmake -query QT_VERSION 2>/dev/null"));
}

QString SystemUtils::getCpuInfo()
{
    QFile file(QStringLiteral("/proc/cpuinfo"));
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream stream(&file);
        while (!stream.atEnd()) {
            QString line = stream.readLine();
            if (line.startsWith(QStringLiteral("model name"))) {
                return line.section(QLatin1Char(':'), 1).trimmed();
            }
        }
    }
    return QStringLiteral("Unknown CPU");
}

int SystemUtils::getCpuCores()
{
    QFile file(QStringLiteral("/proc/cpuinfo"));
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        int count = 0;
        QTextStream stream(&file);
        while (!stream.atEnd()) {
            QString line = stream.readLine();
            if (line.startsWith(QStringLiteral("processor"))) {
                count++;
            }
        }
        return count > 0 ? count : 1;
    }
    return QThread::idealThreadCount();
}

quint64 SystemUtils::getTotalMemoryKB()
{
    QFile file(QStringLiteral("/proc/meminfo"));
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream stream(&file);
        while (!stream.atEnd()) {
            QString line = stream.readLine();
            if (line.startsWith(QStringLiteral("MemTotal"))) {
                QRegularExpression re(QStringLiteral("(\\d+)"));
                QRegularExpressionMatch match = re.match(line);
                if (match.hasMatch()) {
                    return match.captured(1).toULongLong();
                }
            }
        }
    }
    return 0;
}

quint64 SystemUtils::getUsedMemoryKB()
{
    quint64 total = getTotalMemoryKB();
    quint64 available = 0;
    QFile file(QStringLiteral("/proc/meminfo"));
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream stream(&file);
        while (!stream.atEnd()) {
            QString line = stream.readLine();
            if (line.startsWith(QStringLiteral("MemAvailable"))) {
                QRegularExpression re(QStringLiteral("(\\d+)"));
                QRegularExpressionMatch match = re.match(line);
                if (match.hasMatch()) {
                    available = match.captured(1).toULongLong();
                }
                break;
            }
        }
    }
    return total - available;
}

QString SystemUtils::getHostname()
{
    return runCommand(QStringLiteral("hostname"));
}

QString SystemUtils::getCurrentUser()
{
    return runCommand(QStringLiteral("whoami"));
}

QString SystemUtils::getDiskUsage(const QString &path)
{
    return runCommand(QStringLiteral("df -h %1 | tail -1 | awk '{print $3 \"/\" $2 \" (\" $5 \")\"}'").arg(path));
}

QString SystemUtils::getGpuInfo()
{
    QString gpu = runCommand(QStringLiteral("lspci 2>/dev/null | grep -i 'vga\\|3d\\|display' | head -1 | sed 's/.*: //'"));
    if (gpu.isEmpty()) {
        gpu = runCommand(QStringLiteral("glxinfo 2>/dev/null | grep 'OpenGL renderer' | sed 's/.*: //'"));
    }
    return gpu.isEmpty() ? QStringLiteral("Unknown GPU") : gpu;
}

QString SystemUtils::getGpuDriver()
{
    QString gpu = runCommand(QStringLiteral("lspci -k 2>/dev/null | grep -A2 -i 'vga\\|3d\\|display' | grep 'Kernel driver' | head -1 | sed 's/.*: //'"));
    return gpu.isEmpty() ? QStringLiteral("Unknown") : gpu;
}

bool SystemUtils::isPackageInstalled(const QString &packageName)
{
    return runCommandSuccess(QStringLiteral("pacman -Qi %1 >/dev/null 2>&1 || dpkg -s %1 >/dev/null 2>&1 || rpm -q %1 >/dev/null 2>&1 || flatpak list --app 2>/dev/null | grep -i %1 >/dev/null 2>&1").arg(packageName));
}

bool SystemUtils::isServiceActive(const QString &serviceName)
{
    return runCommandSuccess(QStringLiteral("systemctl is-active --quiet %1 2>/dev/null").arg(serviceName));
}

bool SystemUtils::enableService(const QString &serviceName)
{
    return runCommandSuccess(QStringLiteral("sudo systemctl enable --now %1 2>/dev/null").arg(serviceName));
}

bool SystemUtils::disableService(const QString &serviceName)
{
    return runCommandSuccess(QStringLiteral("sudo systemctl disable --now %1 2>/dev/null").arg(serviceName));
}

bool SystemUtils::startService(const QString &serviceName)
{
    return runCommandSuccess(QStringLiteral("sudo systemctl start %1 2>/dev/null").arg(serviceName));
}

bool SystemUtils::stopService(const QString &serviceName)
{
    return runCommandSuccess(QStringLiteral("sudo systemctl stop %1 2>/dev/null").arg(serviceName));
}

QStringList SystemUtils::getConnectedDevices()
{
    return runCommandGetLines(QStringLiteral("ip -br link show 2>/dev/null | awk '{print $1, $2}' | grep -v 'lo'"));
}

QStringList SystemUtils::getWifiNetworks()
{
    return runCommandGetLines(QStringLiteral("nmcli -t -f SSID,SIGNAL dev wifi list 2>/dev/null || iwctl station wlan0 scan 2>/dev/null"));
}

bool SystemUtils::isBluetoothEnabled()
{
    return runCommandSuccess(QStringLiteral("bluetoothctl show 2>/dev/null | grep 'Powered: yes'"));
}

bool SystemUtils::toggleBluetooth(bool enabled)
{
    QString state = enabled ? QStringLiteral("on") : QStringLiteral("off");
    return runCommandSuccess(QStringLiteral("bluetoothctl power %1 2>/dev/null").arg(state));
}

QStringList SystemUtils::getAudioSinks()
{
    return runCommandGetLines(QStringLiteral("pactl list sinks short 2>/dev/null || pw-cli ls Node 2>/dev/null"));
}

QStringList SystemUtils::getAudioSources()
{
    return runCommandGetLines(QStringLiteral("pactl list sources short 2>/dev/null || pw-cli ls Node 2>/dev/null"));
}

QString SystemUtils::getCurrentAudioSink()
{
    return runCommand(QStringLiteral("pactl get-default-sink 2>/dev/null"));
}

QString SystemUtils::getCurrentAudioSource()
{
    return runCommand(QStringLiteral("pactl get-default-source 2>/dev/null"));
}

bool SystemUtils::setDefaultAudioSink(const QString &sink)
{
    return runCommandSuccess(QStringLiteral("pactl set-default-sink \"%1\"").arg(sink));
}

bool SystemUtils::setDefaultAudioSource(const QString &source)
{
    return runCommandSuccess(QStringLiteral("pactl set-default-source \"%1\"").arg(source));
}

bool SystemUtils::setVolume(int volume)
{
    volume = qBound(0, volume, 100);
    return runCommandSuccess(QStringLiteral("pactl set-sink-volume @DEFAULT_SINK@ %1%%").arg(volume));
}

int SystemUtils::getVolume()
{
    QString vol = runCommand(QStringLiteral("pactl get-sink-volume @DEFAULT_SINK@ 2>/dev/null | head -1 | grep -oP '\\d+(?=%)'"));
    return vol.isEmpty() ? 0 : vol.toInt();
}

bool SystemUtils::isMuted()
{
    return runCommandSuccess(QStringLiteral("pactl get-sink-mute @DEFAULT_SINK@ 2>/dev/null | grep -q 'yes'"));
}

bool SystemUtils::toggleMute()
{
    return runCommandSuccess(QStringLiteral("pactl set-sink-mute @DEFAULT_SINK@ toggle"));
}

QStringList SystemUtils::getDisplays()
{
    return runCommandGetLines(QStringLiteral("xrandr --listmonitors 2>/dev/null | tail -n +2 | awk '{print $2}'"));
}

bool SystemUtils::setResolution(const QString &display, const QString &resolution)
{
    return runCommandSuccess(QStringLiteral("xrandr --output %1 --mode %2").arg(display, resolution));
}

QStringList SystemUtils::getNetworkConnections()
{
    return runCommandGetLines(QStringLiteral("nmcli -t -f NAME,TYPE,DEVICE connection show 2>/dev/null"));
}

bool SystemUtils::isNetworkConnected()
{
    return runCommandSuccess(QStringLiteral("nmcli networking connectivity check 2>/dev/null | grep -q full"));
}

QStringList SystemUtils::getStartupApps()
{
    QStringList apps;
    QString autostartPath = QDir::homePath() + QStringLiteral("/.config/autostart");
    QDir dir(autostartPath);
    if (dir.exists()) {
        QStringList desktopFiles = dir.entryList(QStringList() << QStringLiteral("*.desktop"), QDir::Files);
        for (const QString &file : desktopFiles) {
            apps.append(file);
        }
    }
    return apps;
}

bool SystemUtils::addStartupApp(const QString &desktopFile)
{
    QString src = desktopFile;
    QString dest = QDir::homePath() + QStringLiteral("/.config/autostart/") + QFileInfo(desktopFile).fileName();
    return QFile::copy(src, dest);
}

bool SystemUtils::removeStartupApp(const QString &desktopFile)
{
    QString path = QDir::homePath() + QStringLiteral("/.config/autostart/") + desktopFile;
    return QFile::remove(path);
}

QStringList SystemUtils::getInstalledPackages()
{
    return runCommandGetLines(QStringLiteral("pacman -Q 2>/dev/null | awk '{print $1}'"));
}

int SystemUtils::getInstalledPackageCount()
{
    QString count = runCommand(QStringLiteral("pacman -Q 2>/dev/null | wc -l"));
    return count.isEmpty() ? 0 : count.toInt();
}

bool SystemUtils::isFlatpakInstalled()
{
    return runCommandSuccess(QStringLiteral("which flatpak >/dev/null 2>&1"));
}

bool SystemUtils::isAURHelperInstalled()
{
    return runCommandSuccess(QStringLiteral("which yay >/dev/null 2>&1 || which paru >/dev/null 2>&1 || which trizen >/dev/null 2>&1"));
}

QStringList SystemUtils::getWallpapers()
{
    QStringList wallpapers;
    QStringList paths = {
        QStringLiteral("/usr/share/wallpapers/"),
        QDir::homePath() + QStringLiteral("/.local/share/wallpapers/"),
        QDir::homePath() + QStringLiteral("/Pictures/Wallpapers/")
    };

    QStringList nameFilters;
    nameFilters << QStringLiteral("*.jpg") << QStringLiteral("*.jpeg") << QStringLiteral("*.png")
                << QStringLiteral("*.bmp") << QStringLiteral("*.webp");

    for (const QString &path : paths) {
        QDir dir(path);
        if (dir.exists()) {
            QStringList files = dir.entryList(nameFilters, QDir::Files);
            for (const QString &file : files) {
                wallpapers.append(dir.absoluteFilePath(file));
            }
            QStringList subdirs = dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
            for (const QString &subdir : subdirs) {
                QDir subDir(dir.absoluteFilePath(subdir));
                QStringList subFiles = subDir.entryList(nameFilters, QDir::Files);
                for (const QString &file : subFiles) {
                    wallpapers.append(subDir.absoluteFilePath(file));
                }
            }
        }
    }
    return wallpapers;
}

QStringList SystemUtils::getBluetoothDevices()
{
    return runCommandGetLines(QStringLiteral("bluetoothctl devices 2>/dev/null"));
}

bool SystemUtils::startBluetoothScan()
{
    runCommand(QStringLiteral("bluetoothctl scan on &"));
    return true;
}

QStringList SystemUtils::getPairedBluetoothDevices()
{
    return runCommandGetLines(QStringLiteral("bluetoothctl paired-devices 2>/dev/null"));
}

bool SystemUtils::pairBluetoothDevice(const QString &address)
{
    return runCommandSuccess(QStringLiteral("echo 'pair %1\ntrust %1\nconnect %1' | bluetoothctl 2>/dev/null").arg(address));
}

bool SystemUtils::unpairBluetoothDevice(const QString &address)
{
    return runCommandSuccess(QStringLiteral("bluetoothctl remove %1 2>/dev/null").arg(address));
}

QStringList SystemUtils::getPowerProfiles()
{
    return runCommandGetLines(QStringLiteral("powerprofilesctl list 2>/dev/null | grep '.*:' | sed 's/[:* ]//g'"));
}

QString SystemUtils::getCurrentPowerProfile()
{
    return runCommand(QStringLiteral("powerprofilesctl get 2>/dev/null"));
}

bool SystemUtils::setPowerProfile(const QString &profile)
{
    return runCommandSuccess(QStringLiteral("powerprofilesctl set %1 2>/dev/null").arg(profile));
}

bool SystemUtils::isGameModeInstalled()
{
    return runCommandSuccess(QStringLiteral("which gamemoded >/dev/null 2>&1"));
}

bool SystemUtils::isMangoHudInstalled()
{
    return runCommandSuccess(QStringLiteral("which mangohud >/dev/null 2>&1 || pacman -Qi mangohud >/dev/null 2>&1"));
}

QStringList SystemUtils::getThemes()
{
    QStringList themes;
    QStringList paths = {
        QDir::homePath() + QStringLiteral("/.local/share/plasma/desktoptheme/"),
        QStringLiteral("/usr/share/plasma/desktoptheme/"),
        QDir::homePath() + QStringLiteral("/.local/share/plasma/look-and-feel/"),
        QStringLiteral("/usr/share/plasma/look-and-feel/")
    };

    for (const QString &path : paths) {
        QDir dir(path);
        if (dir.exists()) {
            QStringList dirs = dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
            for (const QString &d : dirs) {
                if (!themes.contains(d)) {
                    themes.append(d);
                }
            }
        }
    }
    return themes;
}

bool SystemUtils::isTelemetryDisabled()
{
    return true;
}

bool SystemUtils::isLocationServicesEnabled()
{
    QString val = runCommand(QStringLiteral("kreadconfig5 --group 'GeoLocation' --key 'Enable' 2>/dev/null"));
    return val == QStringLiteral("true");
}

bool SystemUtils::toggleLocationServices(bool enabled)
{
    QString val = enabled ? QStringLiteral("true") : QStringLiteral("false");
    return runCommandSuccess(QStringLiteral("kwriteconfig5 --file kdeglobals --group GeoLocation --key Enable %1").arg(val));
}
