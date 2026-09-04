#ifndef IPSERVICE_H
#define IPSERVICE_H

#include <QObject>
#include <QTimer>
#include <QNetworkAccessManager>
#include "ConfigHelper.h"

class IpService : public QObject
{
    Q_OBJECT
public:
    explicit IpService(QObject *parent = nullptr);

    // ========== 内网部分 ==========
    void fetchLanIp();
    void startPeriodRefresh(int ms);
    void stopPeriodRefresh();

    // ========== 公网部分 ==========
    void setWanConfig(const AppConfig& cfg);
    void fetchWanIpNow();

signals:
    void lanIpReady(const QString& ip);
    void wanIpGot(const QString& ip);
    void wanIpError(const QString& errMsg);

private slots:
    void onPeriodTick();
    void onWanReplyFinished(QNetworkReply* reply);

private:
    void tryNextWanUrl();

    QTimer* m_periodTimer = nullptr;
    QNetworkAccessManager* m_netMgr = nullptr;

    AppConfig m_wanCfg;
    QStringList m_tryUrlList;
    int m_curTryIndex = 0;
};

#endif // IPSERVICE_H
