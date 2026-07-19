#include "configmanager.h"

#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QSettings>
#include <QStandardPaths>
#include <QTextStream>

ConfigManager *ConfigManager::s_instance = nullptr;

ConfigManager::ConfigManager(QObject *parent)
    : QObject(parent)
{
}

ConfigManager *ConfigManager::instance()
{
    if (!s_instance) {
        s_instance = new ConfigManager;
    }
    return s_instance;
}

QString ConfigManager::configPath(const QString &component) const
{
    QString basePath = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation);
    return basePath + QStringLiteral("/%1").arg(component);
}

QString ConfigManager::plasmaConfigPath(const QString &file) const
{
    QString basePath = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation);
    return basePath + QStringLiteral("/plasma-org.kde.plasma.desktop-appletsrc");
}

QVariant ConfigManager::readValue(const QString &group, const QString &key, const QVariant &defaultValue)
{
    QString path = configPath(QStringLiteral("yummy-control-center"));
    QDir().mkpath(path);
    QSettings settings(path + QStringLiteral("/yummycc.conf"), QSettings::IniFormat);
    settings.beginGroup(group);
    QVariant val = settings.value(key, defaultValue);
    settings.endGroup();
    return val;
}

void ConfigManager::writeValue(const QString &group, const QString &key, const QVariant &value)
{
    QString path = configPath(QStringLiteral("yummy-control-center"));
    QDir().mkpath(path);
    QSettings settings(path + QStringLiteral("/yummycc.conf"), QSettings::IniFormat);
    settings.beginGroup(group);
    settings.setValue(key, value);
    settings.endGroup();
}

QVariant ConfigManager::readKDEConfig(const QString &component, const QString &group, const QString &key, const QVariant &defaultValue)
{
    QString path = configPath(component);
    if (!QFile::exists(path)) {
        return defaultValue;
    }
    QSettings settings(path, QSettings::IniFormat);
    settings.beginGroup(group);
    QVariant val = settings.value(key, defaultValue);
    settings.endGroup();
    return val;
}

void ConfigManager::writeKDEConfig(const QString &component, const QString &group, const QString &key, const QVariant &value)
{
    QString path = configPath(component);
    QDir().mkpath(QFileInfo(path).absolutePath());
    QSettings settings(path, QSettings::IniFormat);
    settings.beginGroup(group);
    settings.setValue(key, value);
    settings.endGroup();
}

QVariant ConfigManager::readPlasmaConfig(const QString &group, const QString &key, const QVariant &defaultValue)
{
    QString path = plasmaConfigPath(QString());
    if (!QFile::exists(path)) {
        return defaultValue;
    }
    QSettings settings(path, QSettings::IniFormat);
    settings.beginGroup(group);
    QVariant val = settings.value(key, defaultValue);
    settings.endGroup();
    return val;
}

void ConfigManager::writePlasmaConfig(const QString &group, const QString &key, const QVariant &value)
{
    QString path = plasmaConfigPath(QString());
    QSettings settings(path, QSettings::IniFormat);
    settings.beginGroup(group);
    settings.setValue(key, value);
    settings.endGroup();
}

bool ConfigManager::readBoolConfig(const QString &file, const QString &group, const QString &key, bool defaultValue)
{
    return readValue(group, key, defaultValue).toBool();
}

void ConfigManager::writeBoolConfig(const QString &file, const QString &group, const QString &key, bool value)
{
    writeValue(group, key, value);
}

QString ConfigManager::readStringConfig(const QString &file, const QString &group, const QString &key, const QString &defaultValue)
{
    return readValue(group, key, defaultValue).toString();
}

void ConfigManager::writeStringConfig(const QString &file, const QString &group, const QString &key, const QString &value)
{
    writeValue(group, key, value);
}
