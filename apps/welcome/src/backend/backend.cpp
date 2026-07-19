#include "backend.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QDir>
#include <QStandardPaths>

Backend::Backend(QObject *parent)
    : QObject(parent)
    , m_process(new QProcess(this))
{
    connect(m_process, &QProcess::finished, this, &Backend::onProcessFinished);
    connect(m_process, &QProcess::errorOccurred, this, &Backend::onProcessError);
    connect(m_process, &QProcess::readyReadStandardOutput, this, &Backend::onProcessReadyRead);
}

Backend::~Backend()
{
    if (m_process->state() != QProcess::NotRunning) {
        m_process->terminate();
        m_process->waitForFinished(3000);
    }
}

void Backend::runAsync(const QString &command, const QString &tag)
{
    if (m_commandRunning) {
        m_process->terminate();
        m_process->waitForFinished(2000);
    }

    m_currentTag = tag;
    m_lastOutput.clear();
    m_lastError.clear();
    setCommandRunning(true);

    m_process->start("sh", {"-c", command});
}

void Backend::setCommandRunning(bool running)
{
    if (m_commandRunning != running) {
        m_commandRunning = running;
        emit commandRunningChanged();
    }
}

void Backend::onProcessFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    setCommandRunning(false);

    QString output = QString::fromUtf8(m_process->readAllStandardOutput());
    QString error = QString::fromUtf8(m_process->readAllStandardError());

    if (!output.isEmpty()) {
        m_lastOutput = output;
        emit lastOutputChanged();
    }
    if (!error.isEmpty()) {
        m_lastError = error;
        emit lastErrorChanged();
    }

    if (m_currentTag == "wifi_scan") {
        QVariantList networks;
        QStringList lines = output.trimmed().split('\n');
        for (const QString &line : lines) {
            QStringList parts = line.trimmed().split(QRegularExpression("\\s{2,}"));
            if (parts.size() >= 3) {
                QJsonObject net;
                net["ssid"] = parts[0].trimmed();
                net["signal"] = parts[1].trimmed().toInt();
                net["security"] = parts[2].trimmed();
                networks.append(net);
            }
        }
        emit wifiNetworksFound(networks);
    } else if (m_currentTag == "wifi_connect") {
        if (exitCode == 0) {
            QStringList lines = output.trimmed().split('\n');
            for (const QString &line : lines) {
                if (line.contains("connected to")) {
                    m_currentConnection = line.section("connected to ", 1).trimmed();
                    emit currentConnectionChanged();
                }
            }
        }
    } else if (m_currentTag == "gpu_detect") {
        QString gpuType = "unknown";
        QString driver = "modesetting";

        if (output.contains("NVIDIA", Qt::CaseInsensitive)) {
            gpuType = "nvidia";
            driver = "nvidia";
        } else if (output.contains("AMD", Qt::CaseInsensitive) || output.contains("ATI", Qt::CaseInsensitive)) {
            gpuType = "amd";
            driver = "amdgpu";
        } else if (output.contains("Intel", Qt::CaseInsensitive)) {
            gpuType = "intel";
            driver = "intel";
        }

        emit gpuDetected(gpuType, driver);
    } else if (m_currentTag == "update_check") {
        QVariantList updates;
        int count = 0;
        QStringList lines = output.trimmed().split('\n');
        for (const QString &line : lines) {
            if (line.startsWith("warning:") || line.startsWith("error:") || line.trimmed().isEmpty()) {
                continue;
            }
            QStringList parts = line.trimmed().split(QRegularExpression("\\s+"));
            if (parts.size() >= 3) {
                QJsonObject update;
                update["name"] = parts[0];
                update["currentVersion"] = parts[1];
                update["newVersion"] = parts[2];
                updates.append(update);
                count++;
            }
        }
        emit updateCheckComplete(count, updates);
    } else if (m_currentTag.startsWith("install_")) {
        QString pkgName = m_currentTag.section("install_", 1);
        bool success = (exitCode == 0);
        emit packageInstalled(pkgName, success, success ? "Installed successfully" : error);
    } else if (m_currentTag == "import_settings") {
        bool success = (exitCode == 0);
        emit settingsImported(success, success ? "Settings imported successfully" : "Failed to import settings: " + error);
    }

    m_currentTag.clear();
}

void Backend::onProcessError(QProcess::ProcessError error)
{
    setCommandRunning(false);

    QString errorMsg;
    switch (error) {
    case QProcess::FailedToStart:
        errorMsg = "Command failed to start";
        break;
    case QProcess::Crashed:
        errorMsg = "Process crashed";
        break;
    case QProcess::Timedout:
        errorMsg = "Process timed out";
        break;
    case QProcess::WriteError:
        errorMsg = "Write error";
        break;
    case QProcess::ReadError:
        errorMsg = "Read error";
        break;
    default:
        errorMsg = "Unknown error";
        break;
    }

    m_lastError = errorMsg;
    emit lastErrorChanged();
    m_currentTag.clear();
}

void Backend::onProcessReadyRead()
{
    // Additional real-time output handling if needed
}

// --- Q_INVOKABLE Methods ---

void Backend::scanWifiNetworks()
{
    runAsync("nmcli -t -f SSID,SIGNAL,SECURITY dev wifi list --rescan yes 2>/dev/null | sort -u", "wifi_scan");
}

void Backend::connectToWifi(const QString &ssid, const QString &password)
{
    QString escapedSsid = ssid;
    escapedSsid.replace("'", "'\\''");

    QString cmd;
    if (password.isEmpty()) {
        cmd = QString("nmcli dev wifi connect '%1' 2>&1").arg(escapedSsid);
    } else {
        QString escapedPass = password;
        escapedPass.replace("'", "'\\''");
        cmd = QString("nmcli dev wifi connect '%1' password '%2' 2>&1").arg(escapedSsid, escapedPass);
    }
    runAsync(cmd, "wifi_connect");
}

void Backend::disconnectWifi()
{
    runAsync("nmcli networking off && sleep 1 && nmcli networking on", "wifi_restart");
}

void Backend::detectGpu()
{
    runAsync("lspci 2>/dev/null | grep -i 'vga\\|3d\\|display'", "gpu_detect");
}

void Backend::installGpuDriver(const QString &driver)
{
    if (driver == "nvidia") {
        runAsync("pkexec pacman -S --noconfirm nvidia nvidia-utils nvidia-settings lib32-nvidia-utils 2>&1", "install_nvidia");
    } else if (driver == "amdgpu") {
        runAsync("pkexec pacman -S --noconfirm mesa lib32-mesa vulkan-radeon lib32-vulkan-radeon xf86-video-amdgpu 2>&1", "install_amdgpu");
    } else if (driver == "intel") {
        runAsync("pkexec pacman -S --noconfirm mesa lib32-mesa vulkan-intel lib32-vulkan-intel xf86-video-intel 2>&1", "install_intel");
    }
}

void Backend::checkForUpdates()
{
    runAsync("checkupdates 2>/dev/null || pacman -Sy --print-format '%n %v' 2>/dev/null", "update_check");
}

void Backend::installUpdates()
{
    runAsync("pkexec pacman -Syu --noconfirm 2>&1", "install_updates");
}

void Backend::installPackage(const QString &package)
{
    QString escaped = package;
    escaped.replace("'", "'\\''");
    runAsync(QString("pkexec pacman -S --noconfirm '%1' 2>&1").arg(escaped), "install_" + package);
}

void Backend::installPackages(const QStringList &packages)
{
    QStringList escaped;
    for (const QString &pkg : packages) {
        QString e = pkg;
        e.replace("'", "'\\''");
        escaped.append("'" + e + "'");
    }
    runAsync(QString("pkexec pacman -S --noconfirm %1 2>&1").arg(escaped.join(' ')), "install_pkgs");
}

void Backend::applyTheme(const QString &theme)
{
    QString gtkTheme, iconTheme, colorScheme;

    if (theme == "dark") {
        gtkTheme = "YummyOS-Dark";
        iconTheme = "Papirus-Dark";
        colorScheme = "prefer-dark";
    } else if (theme == "light") {
        gtkTheme = "YummyOS-Light";
        iconTheme = "Papirus-Light";
        colorScheme = "prefer-light";
    } else {
        gtkTheme = "YummyOS-Dark";
        iconTheme = "Papirus";
        colorScheme = "default";
    }

    QString cmd = QString(
        "gsettings set org.gnome.desktop.interface gtk-theme '%1' && "
        "gsettings set org.gnome.desktop.interface icon-theme '%2' && "
        "gsettings set org.gnome.desktop.interface color-scheme '%3' && "
        "kwriteconfig5 --file kdeglobals --group General --key Name '%1' 2>/dev/null; "
        "qdbus org.kde.KWin /KWin reconfigure 2>/dev/null"
    ).arg(gtkTheme, iconTheme, colorScheme);

    runAsync(cmd, "theme_" + theme);
}

void Backend::setAccentColor(const QString &color)
{
    QString cmd = QString(
        "kwriteconfig6 --file kdeglobals --group General --key AccentColor '%1' 2>/dev/null; "
        "gsettings set org.gnome.desktop.interface accent-color '%1' 2>/dev/null; "
        "qdbus org.kde.KWin /KWin reconfigure 2>/dev/null"
    ).arg(color);
    runAsync(cmd, "accent_color");
}

void Backend::setWallpaper(const QString &path)
{
    QString escaped = path;
    escaped.replace("'", "'\\''");

    QString cmd = QString(
        "gsettings set org.gnome.desktop.background picture-uri 'file://%1' 2>/dev/null; "
        "gsettings set org.gnome.desktop.background picture-uri-dark 'file://%1' 2>/dev/null; "
        "qdbus org.kde.plasmashell /PlasmaShell evaluateScript \"var allDesktops = desktops(); for (var i = 0; i < allDesktops.length; i++) { allDesktops[i].wallpaperPlugin = 'org.kde.image'; allDesktops[i].currentConfigGroup = ['Wallpaper', 'org.kde.image', 'General']; allDesktops[i].writeConfig('Image', 'file://%1'); }\" 2>/dev/null"
    ).arg(escaped);
    runAsync(cmd, "wallpaper");
}

void Backend::runCommand(const QString &command)
{
    QString escaped = command;
    escaped.replace("'", "'\\''");
    runAsync(QString("pkexec sh -c '%1' 2>&1").arg(escaped), "custom");
}

void Backend::importSettings(const QString &sourcePath)
{
    QString escaped = sourcePath;
    escaped.replace("'", "'\\''");
    QString dest = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
    QString cmd = QString("rsync -av --progress '%1/' '%2/' 2>&1").arg(escaped, dest);
    runAsync(cmd, "import_settings");
}

void Backend::launchDesktop()
{
    QProcess::startDetached("sh", {"-c", "startplasma-x11 2>/dev/null || startxfce4 2>/dev/null || start cinnamon 2>/dev/null"});
}

void Backend::openControlCenter()
{
    QProcess::startDetached("sh", {"-c", "yummy-control-center 2>/dev/null || gnome-control-center 2>/dev/null || systemsettings 2>/dev/null"});
}
