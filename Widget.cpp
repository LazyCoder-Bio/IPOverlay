#include "widget.h"
#include "floatingipoverlaywidget.h"
#include "ipservice.h"
#include "traymanager.h"
#include "ConfigHelper.h"

#include <QTimer>
#include <QFile>
#include <QDebug>

Widget::Widget(QWidget *parent)
    : QWidget(parent, Qt::Tool| Qt::FramelessWindowHint)
{

    setWindowIcon(QIcon(":/IPOverlay.png"));
    setAttribute(Qt::WA_NoSystemBackground);
    setAttribute(Qt::WA_TransparentForMouseEvents);


    setupFloatWindow();
    setupIpService();
    setupTray();
}

Widget::~Widget()
{
}

void Widget::setupFloatWindow()
{
    m_floatWin = new FloatingIpoverlayWidget(nullptr);
    QTimer::singleShot(0, this, [this](){
        if(m_floatWin)
            m_floatWin->safeShow();
    });

    QString iniPath = ConfigHelper::getConfigFilePath();
    QFile iniFile(iniPath);
    if (!iniFile.exists())
    {
        AppConfig defaultCfg = ConfigHelper::loadConfig();
        ConfigHelper::saveConfig(defaultCfg);
    }

    //初始占位
    m_lastPubIp = "待获取";
    m_lastLanIp = "待获取";
}

void Widget::setupIpService()
{
    m_ipService = new IpService(this);
    AppConfig cfg = ConfigHelper::loadConfig();

    //====内网====
    connect(m_ipService, &IpService::lanIpReady, this, &Widget::onLanIpReady);
    m_ipService->fetchLanIp();
    m_ipService->startPeriodRefresh(cfg.refreshIntervalMs);

    //====公网====
    m_ipService->setWanConfig(cfg);
    connect(m_ipService, &IpService::wanIpGot, this, &Widget::onWanIpGot);
    connect(m_ipService, &IpService::wanIpError, this, &Widget::onWanIpError);
}

void Widget::setupTray()
{
    m_trayManager = new TrayManager(this);
    m_trayManager->initTray(m_floatWin);

    //托盘配置变更信号
    connect(m_trayManager, &TrayManager::configChanged, this, &Widget::onConfigChanged);
}

//托盘配置变更
void Widget::onConfigChanged(const AppConfig &cfg)
{
    //更新公网配置，立刻重新请求公网IP
    m_ipService->setWanConfig(cfg);
    //更新定时器刷新间隔
    m_ipService->startPeriodRefresh(cfg.refreshIntervalMs);
}

//内网IP更新
void Widget::onLanIpReady(const QString &lanIp)
{
    m_lastLanIp = lanIp;
    if(m_floatWin)
    {
        m_floatWin->setIpText(m_lastPubIp, m_lastLanIp);
    }
}

//公网IP成功
void Widget::onWanIpGot(const QString &pubIp)
{
    m_lastPubIp = pubIp;
    if(m_floatWin)
    {
        m_floatWin->setIpText(m_lastPubIp, m_lastLanIp);
    }
}

//公网IP失败
void Widget::onWanIpError(const QString &errMsg)
{
    m_lastPubIp = QString("获取失败：%1").arg(errMsg);
    if(m_floatWin)
    {
        m_floatWin->setIpText(m_lastPubIp, m_lastLanIp);
    }
}
