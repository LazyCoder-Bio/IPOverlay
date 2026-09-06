#include "ipservice.h"
#include <QNetworkInterface>
#include <QAbstractSocket>
#include <QNetworkReply>
#include <QHostAddress>
#include <QRegularExpression>
#include <QDebug>

/**
 * @brief IP获取服务类构造函数
 * @param parent 父对象
 * @note 负责内网IP扫描、公网IP网络请求、定时刷新；内部封装定时器与网络管理器
 */
IpService::IpService(QObject *parent)
    : QObject(parent)
{
    // 创建定时刷新定时器，父对象this自动内存回收
    m_periodTimer = new QTimer(this);
    connect(m_periodTimer, &QTimer::timeout, this, &IpService::onPeriodTick);

    // 创建网络访问管理器，用于请求公网IP接口
    m_netMgr = new QNetworkAccessManager(this);
    connect(m_netMgr, &QNetworkAccessManager::finished, this, &IpService::onWanReplyFinished);
}

// ====================== 内网IP相关逻辑 ======================

/**
 * @brief 扫描本机网卡，获取内网IPv4地址
 * @note 跳过回环网卡、虚拟虚拟机网卡(Vmware/VirtualBox)，取第一个有效IPv4；扫描完成发射lanIpReady信号
 */
void IpService::fetchLanIp()
{
    QString lanIp = "无";
    // 获取本机全部网络接口
    const auto& interfaces = QNetworkInterface::allInterfaces();
    for(const auto& iface : interfaces)
    {
        if(!iface.isValid()) continue;                     // 跳过无效网卡
        if(!iface.flags().testFlag(QNetworkInterface::IsUp)) continue; // 跳过未启用网卡
        if(iface.flags().testFlag(QNetworkInterface::IsLoopBack)) continue; // 跳过回环网卡127.0.0.1

        QString ifaceName = iface.humanReadableName().toLower();
        // 过滤虚拟机虚拟网卡，不读取虚拟网卡IP
        if(ifaceName.contains("vmware") || ifaceName.contains("virtualbox"))
        {
            continue;
        }

        // 遍历网卡的地址条目
        const auto& addrList = iface.addressEntries();
        for(const auto& entry : addrList)
        {
            QHostAddress ip = entry.ip();
            // 筛选IPv4，排除本地回环地址
            if(ip.protocol() == QAbstractSocket::IPv4Protocol && ip != QHostAddress::LocalHost)
            {
                lanIp = ip.toString();
                goto endScan; //拿到第一个有效内网IP直接跳出双层循环
            }
        }
    }
endScan:
    qDebug() << "[IpService] fetchLanIp result:" << lanIp;
    emit lanIpReady(lanIp); //抛出内网IP就绪信号
}

/**
 * @brief 启动定时刷新定时器
 * @param ms 刷新间隔，单位毫秒
 */
void IpService::startPeriodRefresh(int ms)
{
    m_periodTimer->setInterval(ms);
    m_periodTimer->start();
}

/**
 * @brief 停止IP定时刷新
 */
void IpService::stopPeriodRefresh()
{
    m_periodTimer->stop();
}

/**
 * @brief 定时周期触发回调
 * @note 每到定时时刻，同时刷新内网IP、公网IP
 */
void IpService::onPeriodTick()
{
    fetchLanIp();
    fetchWanIpNow();
}

// ====================== 公网IP相关逻辑 ======================

/**
 * @brief 设置公网IP请求配置，更新接口列表并立刻发起一次公网IP获取
 * @param cfg 应用配置对象，携带wanUrlList公网接口地址列表
 */
void IpService::setWanConfig(const AppConfig &cfg)
{
    m_wanCfg = cfg;
    fetchWanIpNow();
}

/**
 * @brief 立即开始公网IP获取，重置接口重试索引，开始轮询各个url
 */
void IpService::fetchWanIpNow()
{
    m_tryUrlList = m_wanCfg.wanUrlList;
    m_curTryIndex = 0;
    tryNextWanUrl();
}

/**
 * @brief 尝试下一个公网IP接口地址
 * @note 逐个遍历配置内url，全部失败则抛出wanIpError错误信号
 */
void IpService::tryNextWanUrl()
{
    // 所有接口全部尝试完毕，返回失败
    if(m_curTryIndex >= m_tryUrlList.size())
    {
        emit wanIpError("全部公网URL请求失败");
        return;
    }
    QString url = m_tryUrlList.at(m_curTryIndex);
    qDebug() << "尝试公网URL:" << url;

    // 构造网络请求，模拟浏览器UA，部分接口会拦截默认Qt UA
    QNetworkRequest req(url);
    req.setRawHeader("User-Agent", "Mozilla/5.0 (Windows NT 10.0; Win64; x64)");
    m_netMgr->get(req);
}

/**
 * @brief 公网接口请求完成回调
 * @param reply QNetworkReply网络应答对象
 * @note 请求失败、返回无IP文本、IP格式非法，都会自动切换下一个接口重试；解析成功抛出wanIpGot
 */
void IpService::onWanReplyFinished(QNetworkReply *reply)
{
    reply->deleteLater(); //应答对象延后自动释放

    // 网络层面错误：超时、连接失败、404等，切换下一个接口
    if(reply->error() != QNetworkReply::NoError)
    {
        qDebug() << "公网请求失败:" << reply->errorString();
        m_curTryIndex++;
        tryNextWanUrl();
        return;
    }

    QByteArray raw = reply->readAll();
    QString fullText = QString::fromUtf8(raw);

    //正则提取响应内容中第一个IPv4格式字符串
    static const QRegularExpression ipv4Rx(R"((\d{1,3}\.\d{1,3}\.\d{1,3}\.\d{1,3}))");
    QRegularExpressionMatch match = ipv4Rx.match(fullText);
    if(!match.hasMatch())
    {
        qDebug() << "响应文本未匹配到IPv4，跳过该接口";
        m_curTryIndex++;
        tryNextWanUrl();
        return;
    }

    QString ipText = match.captured(1).trimmed();
    QHostAddress addr(ipText);
    // 严格校验IP合法性，过滤类似999.999.999.999这种非法数值
    if (addr.isNull() || addr.protocol() != QAbstractSocket::IPv4Protocol)
    {
        qDebug() << "提取到的不是有效IPv4:" << ipText;
        m_curTryIndex++;
        tryNextWanUrl();
        return;
    }

    QString realIp = addr.toString();
    qDebug() << "公网IPv4获取成功:" << realIp;
    emit wanIpGot(realIp); //公网IP获取成功，向上抛出信号
}
