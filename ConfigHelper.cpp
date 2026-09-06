#include "ConfigHelper.h"
#include <QCoreApplication>
#include <QDir>
#include <QSettings>
#include <QDebug>

/**
 * @brief 获取配置ini文件完整路径
 * @return 程序运行目录下 config.ini 的绝对路径
 * @note 配置文件与exe放在同一个目录
 */
QString ConfigHelper::getConfigFilePath()
{
    return QDir(QCoreApplication::applicationDirPath()).filePath("config.ini");
}

/**
 * @brief 从ini文件加载应用配置
 * @return AppConfig 配置结构体实例
 * @note 文件不存在时，直接返回内置默认配置；wanUrlList采用QSettings数组读写
 */
AppConfig ConfigHelper::loadConfig()
{
    AppConfig cfg;
    QString filePath = getConfigFilePath();
    qDebug() << "[ConfigHelper] loadConfig, path:" << filePath;

    QSettings settings(filePath, QSettings::IniFormat);
    // 设置程序内置默认参数，ini缺失对应项时使用该值
    cfg.refreshIntervalMs = 10 * 60 * 1000;
    cfg.wanUrlList = {
        "https://ipinfo.io/ip",
        "https://api-ipv4.ip.sb/ip",
        "https://api.ipify.org"
    };
    // 读取Network/wanUrls数组，加载用户自定义公网IP接口列表
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
    // 如果ini读出来为空，回填内置默认接口
    if(cfg.wanUrlList.isEmpty())
    {
        qWarning() << "[ConfigHelper] wanUrlList empty, use default urls";
        cfg.wanUrlList = {
            "https://ipinfo.io/ip",
            "https://api-ipv4.ip.sb/ip",
            "https://api.ipify.org"
        };
    }
    // 读取刷新间隔，第二个参数为默认兜底值
    cfg.refreshIntervalMs = settings.value("General/refreshIntervalMs", cfg.refreshIntervalMs).toLongLong();

    if(settings.status() != QSettings::NoError)
    {
        qWarning() << "[ConfigHelper] loadConfig error, status:" << settings.status();
    }
    return cfg;
}

/**
 * @brief 将配置结构体保存写入本地config.ini
 * @param cfg 需要保存的配置对象
 * @return true写入成功；false写入失败（权限不足、目录不可写等）
 * @note 只删除本程序自己的分组，不调用全局clear，避免丢失外部注释
 */
bool ConfigHelper::saveConfig(const AppConfig &cfg)
{
    QString filePath = getConfigFilePath();
    qDebug() << "[ConfigHelper] saveConfig, path:" << filePath;

    QSettings settings(filePath, QSettings::IniFormat);
    // 只删除本程序自己的分组，不要调用settings.clear()
    settings.remove("General");
    settings.remove("Network");

    // 写入公网接口URL数组
    settings.beginWriteArray("Network/wanUrls");
    for(int i = 0; i < cfg.wanUrlList.size(); i++)
    {
        settings.setArrayIndex(i);
        settings.setValue("url", cfg.wanUrlList[i]);
    }
    settings.endArray();
    // 写入定时刷新间隔
    settings.setValue("General/refreshIntervalMs", cfg.refreshIntervalMs);
    settings.sync(); //强制刷写到磁盘文件

    if(settings.status() != QSettings::NoError)
    {
        qWarning() << "[ConfigHelper] saveConfig failed, status:" << settings.status();
        return false;
    }
    qDebug() << "[ConfigHelper] saveConfig success";
    return true;
}
