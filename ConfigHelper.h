#ifndef CONFIGHELPER_H
#define CONFIGHELPER_H
#include <QString>
#include <QStringList>

/**
 * @brief 应用全局配置结构体
 * @note 保存公网IP接口地址列表、IP定时刷新间隔毫秒数，由ConfigHelper读写到本地ini配置文件
 */
struct AppConfig
{
    QStringList wanUrlList;    ///< 公网IP查询HTTP接口URL列表，失败自动轮询降级
    int refreshIntervalMs;    ///< IP自动刷新时间，单位毫秒
};

/**
 * @brief 配置文件读写工具类
 * @note 全部为静态方法；读写程序同目录下config.ini；
 * 负责配置加载、保存、获取配置文件路径；文件不存在返回程序内置默认配置。
 */
class ConfigHelper
{
public:
    /**
     * @brief 从config.ini加载配置
     * @return AppConfig配置结构体，读取失败/文件不存在使用代码内置默认参数
     */
    static AppConfig loadConfig();

    /**
     * @brief 将配置结构体写入本地config.ini磁盘文件
     * @param cfg 需要持久化保存的配置对象
     * @return true写入成功；false写入失败（目录无权限等）
     */
    static bool saveConfig(const AppConfig& cfg);

    /**
     * @brief 获取配置ini文件完整绝对路径
     * @return exe同级目录下config.ini完整路径
     */
    static QString getConfigFilePath();
};

#endif // CONFIGHELPER_H
