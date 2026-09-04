#include "ConfigHelper.h"
#include <QCoreApplication>
#include <QDir>
#include <QSettings>

QString ConfigHelper::getConfigFilePath()
{
    return QDir(QCoreApplication::applicationDirPath()).filePath("config.ini");
}

AppConfig ConfigHelper::loadConfig()
{
    AppConfig cfg;
    QSettings settings(getConfigFilePath(), QSettings::IniFormat);

    //内置默认值
    cfg.refreshIntervalMs = 10 * 60 * 1000;
    cfg.wanUrlList = {
        "https://ipinfo.io/ip",
        "https://api-ipv4.ip.sb/ip",
        "https://api.ipify.org"
    };


    int size = settings.beginReadArray("Network/wanUrls");
    for(int i = 0; i < size; i++)
    {
        settings.setArrayIndex(i);
        QString url = settings.value("url").toString().trimmed();
        if(!url.isEmpty())
        {
            cfg.wanUrlList.append(url);
        }
    }
    settings.endArray();

    cfg.refreshIntervalMs = settings.value("General/refreshIntervalMs", cfg.refreshIntervalMs).toLongLong();
    return cfg;
}

bool ConfigHelper::saveConfig(const AppConfig &cfg)
{
    QSettings settings(getConfigFilePath(), QSettings::IniFormat);
    settings.clear();

    settings.beginWriteArray("Network/wanUrls");
    for(int i = 0; i < cfg.wanUrlList.size(); i++)
    {
        settings.setArrayIndex(i);
        settings.setValue("url", cfg.wanUrlList[i]);
    }
    settings.endArray();

    settings.setValue("General/refreshIntervalMs", cfg.refreshIntervalMs);
    settings.sync();
    return settings.status() == QSettings::NoError;
}
