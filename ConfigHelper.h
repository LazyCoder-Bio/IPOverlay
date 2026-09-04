#ifndef CONFIGHELPER_H
#define CONFIGHELPER_H

#include <QString>
#include <QStringList>

struct AppConfig
{
    QStringList wanUrlList;
    int refreshIntervalMs;
};

class ConfigHelper
{
public:
    static AppConfig loadConfig();
    static bool saveConfig(const AppConfig& cfg);

    static QString getConfigFilePath();
};

#endif // CONFIGHELPER_H
