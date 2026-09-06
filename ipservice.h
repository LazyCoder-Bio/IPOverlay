#ifndef IPSERVICE_H
#define IPSERVICE_H
#include <QObject>
#include <QTimer>
#include <QNetworkAccessManager>
#include "ConfigHelper.h"

class QNetworkReply;

/**
 * @brief IP获取服务类
 * @note 负责内网网卡扫描获取局域网IPv4，同时请求多个公网接口获取出口公网IP；
 * 自带周期定时器定时刷新IP；公网接口支持失败自动降级重试。
 */
class IpService : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief 构造函数
     * @param parent 父对象
     */
    explicit IpService(QObject *parent = nullptr);

    // ========== 内网部分 ==========
    /**
     * @brief 扫描本机网卡，获取有效内网IPv4地址
     */
    void fetchLanIp();

    /**
     * @brief 启动IP定时刷新定时器
     * @param ms 刷新间隔，单位毫秒
     */
    void startPeriodRefresh(int ms);

    /**
     * @brief 停止IP定时刷新
     */
    void stopPeriodRefresh();

    // ========== 公网部分 ==========
    /**
     * @brief 设置公网请求配置，更新接口列表并立刻发起一次公网IP查询
     * @param cfg 应用配置结构体，携带wanUrlList公网接口列表
     */
    void setWanConfig(const AppConfig& cfg);

    /**
     * @brief 立即发起公网IP获取，重置重试索引，开始逐个尝试接口
     */
    void fetchWanIpNow();

signals:
    /**
     * @brief 内网IP扫描完成信号
     * @param ip 扫描得到的内网IPv4字符串，扫描失败返回“无”
     */
    void lanIpReady(const QString& ip);

    /**
     * @brief 公网IP获取成功信号
     * @param ip 解析得到的公网IPv4
     */
    void wanIpGot(const QString& ip);

    /**
     * @brief 全部公网接口请求失败信号
     * @param errMsg 错误描述文本
     */
    void wanIpError(const QString& errMsg);

private slots:
    /**
     * @brief 定时周期回调，触发内网、公网IP同时刷新
     */
    void onPeriodTick();

    /**
     * @brief 公网网络请求完成回调，处理应答、解析IP、失败自动切换下一个接口
     * @param reply QNetworkReply网络应答对象
     */
    void onWanReplyFinished(QNetworkReply* reply);

private:
    /**
     * @brief 尝试下一个公网IP接口地址；全部失败则抛出wanIpError
     */
    void tryNextWanUrl();

    QTimer* m_periodTimer = nullptr;                ///< IP周期刷新定时器
    QNetworkAccessManager* m_netMgr = nullptr;      ///< 网络管理器，用于公网http请求
    AppConfig m_wanCfg;                             ///< 公网相关配置副本
    QStringList m_tryUrlList;                       ///< 当前一轮正在尝试的公网URL列表
    int m_curTryIndex = 0;                          ///< 当前正在尝试的URL下标
};

#endif // IPSERVICE_H
