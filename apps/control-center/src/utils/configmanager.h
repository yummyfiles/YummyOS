#pragma once

#include <QObject>
#include <QString>
#include <QStringList>
#include <QVariant>
#include <QMap>

class ConfigManager : public QObject
{
    Q_OBJECT

public:
    explicit ConfigManager(QObject *parent = nullptr);

    static ConfigManager *instance();

    QVariant readValue(const QString &group, const QString &key, const QVariant &defaultValue = QVariant());
    void writeValue(const QString &group, const QString &key, const QVariant &value);

    QVariant readKDEConfig(const QString &component, const QString &group, const QString &key, const QVariant &defaultValue = QVariant());
    void writeKDEConfig(const QString &component, const QString &group, const QString &key, const QVariant &value);

    QVariant readPlasmaConfig(const QString &group, const QString &key, const QVariant &defaultValue = QVariant());
    void writePlasmaConfig(const QString &group, const QString &key, const QVariant &value);

    bool readBoolConfig(const QString &file, const QString &group, const QString &key, bool defaultValue = false);
    void writeBoolConfig(const QString &file, const QString &group, const QString &key, bool value);

    QString readStringConfig(const QString &file, const QString &group, const QString &key, const QString &defaultValue = QString());
    void writeStringConfig(const QString &file, const QString &group, const QString &key, const QString &value);

private:
    QString configPath(const QString &component) const;
    QString plasmaConfigPath(const QString &file) const;

    static ConfigManager *s_instance;
};
