#include "ipservice.h"
#include <QNetworkInterface>
#include <QAbstractSocket>
#include <QNetworkReply>
#include <QHostAddress>
#include <QRegularExpression>
#include <QDebug>

IpService::IpService(QObject *parent)
    : QObject(parent)
{
    // 内网定时器
    m_periodTimer = new QTimer(this);
    connect(m_periodTimer, &QTimer::timeout, this, &IpService::onPeriodTick);

    // 公网网络管理器
    m_netMgr = new QNetworkAccessManager(this);
    connect(m_netMgr, &QNetworkAccessManager::finished, this, &IpService::onWanReplyFinished);
}

// ========== 内网逻辑 ==========
void IpService::fetchLanIp()
{
    QString lanIp = "无";
    const auto& interfaces = QNetworkInterface::allInterfaces();
    for(const auto& iface : interfaces)
    {
        if(!iface.isValid()) continue;
        if(!iface.flags().testFlag(QNetworkInterface::IsUp)) continue;
        if(iface.flags().testFlag(QNetworkInterface::IsLoopBack)) continue;

        QString ifaceName = iface.humanReadableName().toLower();
        if(ifaceName.contains("vmware") || ifaceName.contains("virtualbox"))
        {
            continue;
        }

        const auto& addrList = iface.addressEntries();
        for(const auto& entry : addrList)
        {
            QHostAddress ip = entry.ip();
            if(ip.protocol() == QAbstractSocket::IPv4Protocol && ip != QHostAddress::LocalHost)
            {
                lanIp = ip.toString();
                goto endScan;
            }
        }
    }
endScan:
    emit lanIpReady(lanIp);
}

void IpService::startPeriodRefresh(int ms)
{
    m_periodTimer->setInterval(ms);
    m_periodTimer->start();
}

void IpService::stopPeriodRefresh()
{
    m_periodTimer->stop();
}

void IpService::onPeriodTick()
{
    fetchLanIp();
    fetchWanIpNow();
}

// ========== 公网逻辑 ==========
void IpService::setWanConfig(const AppConfig &cfg)
{
    m_wanCfg = cfg;
    fetchWanIpNow();
}

void IpService::fetchWanIpNow()
{
    m_tryUrlList = m_wanCfg.wanUrlList;
    m_curTryIndex = 0;
    tryNextWanUrl();
}

void IpService::tryNextWanUrl()
{
    if(m_curTryIndex >= m_tryUrlList.size())
    {
        emit wanIpError("全部公网URL请求失败");
        return;
    }
    QString url = m_tryUrlList.at(m_curTryIndex);
    qDebug() << "尝试公网URL:" << url;

    QNetworkRequest req(url);
    req.setRawHeader("User-Agent", "Mozilla/5.0 (Windows NT 10.0; Win64; x64)");
    m_netMgr->get(req);
}

void IpService::onWanReplyFinished(QNetworkReply *reply)
{
    reply->deleteLater();

    if(reply->error() != QNetworkReply::NoError)
    {
        qDebug() << "公网请求失败:" << reply->errorString();
        m_curTryIndex++;
        tryNextWanUrl();
        return;
    }

    QByteArray raw = reply->readAll();
    QString fullText = QString::fromUtf8(raw);

    // 正则提取文本中第一个IPv4
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

    // 严格校验是否为合法IPv4，过滤999.999.999.999这类非法格式
    if (addr.isNull() || addr.protocol() != QAbstractSocket::IPv4Protocol)
    {
        qDebug() << "提取到的不是有效IPv4:" << ipText;
        m_curTryIndex++;
        tryNextWanUrl();
        return;
    }

    QString realIp = addr.toString();
    qDebug() << "公网IPv4获取成功:" << realIp;
    emit wanIpGot(realIp);
}
