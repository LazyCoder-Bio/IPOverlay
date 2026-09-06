#include "widget.h"
#include "floatingipoverlaywidget.h"
#include "ipservice.h"
#include "traymanager.h"
#include "ConfigHelper.h"
#include <QTimer>
#include <QFile>
#include <QDebug>

/**
 * @brief 主控制器窗口
 * @param parent 父对象
 * @note 使用Tool+无边框，本控件自身不显示，作为业务逻辑调度中枢
 */
Widget::Widget(QWidget *parent)
    : QWidget(parent, Qt::Tool| Qt::FramelessWindowHint)
{
    qDebug() << "[Widget] application widget init start";
    // 设置无系统背景，透明鼠标事件，本窗口只做逻辑，不渲染界面
    setAttribute(Qt::WA_NoSystemBackground);
    setAttribute(Qt::WA_TransparentForMouseEvents);

    setupFloatWindow();   // 初始化悬浮IP显示窗口
    setupIpService();     // 初始化IP获取服务（内网/公网）
    setupTray();          // 初始化系统托盘管理器
    qDebug() << "[Widget] all component setup finished";
}

Widget::~Widget()
{

}

/**
 * @brief 初始化悬浮显示窗口
 */
void Widget::setupFloatWindow()
{
    // 创建悬浮IP弹窗，无父窗口，独立顶层窗口
    m_floatWin = new FloatingIpoverlayWidget(nullptr);

    // 放到事件循环末尾再显示，避免界面渲染时序问题
    QTimer::singleShot(0, this, [this](){
        if(m_floatWin)
            m_floatWin->safeShow();
    });

    // 获取配置文件完整路径
    QString iniPath = ConfigHelper::getConfigFilePath();
    QFile iniFile(iniPath);
    // 如果配置文件不存在，生成一份默认配置ini
    if (!iniFile.exists())
    {
        AppConfig defaultCfg = ConfigHelper::loadConfig();
        ConfigHelper::saveConfig(defaultCfg);
    }

    // IP初始占位文本，等待网络回调填充真实IP
    m_lastPubIp = "待获取";
    m_lastLanIp = "待获取";
}

/**
 * @brief 初始化IP网络服务，内网IP、公网IP、定时刷新
 */
void Widget::setupIpService()
{
    m_ipService = new IpService(this);
    AppConfig cfg = ConfigHelper::loadConfig();

    // -------- 内网IP逻辑 --------
    // 收到内网IP就绪信号，更新UI
    connect(m_ipService, &IpService::lanIpReady, this, &Widget::onLanIpReady);
    // 立即执行一次内网IP获取
    m_ipService->fetchLanIp();
    // 启动定时轮询刷新IP，使用配置里的刷新间隔
    m_ipService->startPeriodRefresh(cfg.refreshIntervalMs);

    // -------- 公网IP逻辑 --------
    // 加载公网相关配置（接口地址、超时等）
    m_ipService->setWanConfig(cfg);
    // 公网IP获取成功回调
    connect(m_ipService, &IpService::wanIpGot, this, &Widget::onWanIpGot);
    // 公网IP获取失败回调
    connect(m_ipService, &IpService::wanIpError, this, &Widget::onWanIpError);
}

/**
 * @brief 初始化系统托盘管理器，绑定悬浮窗与信号
 */
void Widget::setupTray()
{
    m_trayManager = new TrayManager(this);
    // 初始化托盘图标、菜单，传入悬浮窗对象供托盘菜单控制
    m_trayManager->initTray(m_floatWin);

    // 托盘菜单修改配置后，触发配置变更回调
    connect(m_trayManager, &TrayManager::configChanged, this, &Widget::onConfigChanged);
    // 托盘手动刷新IP菜单，触发立即刷新IP
    connect(m_trayManager, &TrayManager::triggerManualRefresh, this, &Widget::onManualRefreshIp);
}

/**
 * @brief 托盘修改配置回调：应用新配置到IP服务
 * @param cfg 更新后的应用配置
 */
void Widget::onConfigChanged(const AppConfig &cfg)
{
    // 将新配置下发给公网IP服务
    m_ipService->setWanConfig(cfg);
    // 更新IP定时刷新定时器间隔
    m_ipService->startPeriodRefresh(cfg.refreshIntervalMs);
}

/**
 * @brief 内网IP获取完成回调
 * @param lanIp 本机内网IP字符串
 */
void Widget::onLanIpReady(const QString &lanIp)
{
    m_lastLanIp = lanIp;
    // 更新悬浮窗显示（公网+内网IP）
    if(m_floatWin)
    {
        m_floatWin->setIpText(m_lastPubIp, m_lastLanIp);
    }
}

/**
 * @brief 公网IP获取成功回调
 * @param pubIp 公网出口IP字符串
 */
void Widget::onWanIpGot(const QString &pubIp)
{
    m_lastPubIp = pubIp;
    if(m_floatWin)
    {
        m_floatWin->setIpText(m_lastPubIp, m_lastLanIp);
    }
}

/**
 * @brief 公网IP获取失败回调
 * @param errMsg 错误描述信息
 */
void Widget::onWanIpError(const QString &errMsg)
{
    m_lastPubIp = QString("获取失败：%1").arg(errMsg);
    if(m_floatWin)
    {
        m_floatWin->setIpText(m_lastPubIp, m_lastLanIp);
    }
}

/**
 * @brief 手动刷新IP（托盘菜单触发）
 * 同时重新拉取内网IP + 公网IP
 */
void Widget::onManualRefreshIp()
{
    qDebug() << "[Widget] onManualRefreshIp triggered by tray menu";
    if(!m_ipService)
        return;
    m_ipService->fetchLanIp();     // 刷新内网IP
    m_ipService->fetchWanIpNow();  // 立即请求公网IP
}
