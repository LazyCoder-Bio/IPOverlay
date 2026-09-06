#ifndef WIDGET_H
#define WIDGET_H
#include <QWidget>

//前置声明，减少头文件依赖
struct AppConfig;
class FloatingIpoverlayWidget;
class IpService;
class TrayManager;

/**
 * @brief 主业务调度控制器
 * @note 本窗口本身不做界面渲染，作为逻辑中枢：调度悬浮窗、IP网络服务、系统托盘，处理各个模块信号回调
 * 窗口属性：Qt::Tool + Qt::FramelessWindowHint，透明无背景，仅用于承载对象与信号槽
 */
class Widget : public QWidget
{
    Q_OBJECT
public:
    /**
     * @brief 构造函数
     * @param parent 父控件指针
     */
    explicit Widget(QWidget *parent = nullptr);
    ~Widget() override;

private:
    /**
     * @brief 初始化悬浮IP显示窗口
     */
    void setupFloatWindow();
    /**
     * @brief 初始化IP获取服务（内网、公网、定时刷新）
     */
    void setupIpService();
    /**
     * @brief 初始化系统托盘管理器
     */
    void setupTray();

private slots:
    /**
     * @brief 托盘配置变更回调，下发新配置到IP服务
     * @param cfg 更新后的应用配置结构体
     */
    void onConfigChanged(const AppConfig& cfg);

    /**
     * @brief 内网IP获取完成回调
     * @param lanIp 内网IPv4字符串
     */
    void onLanIpReady(const QString& lanIp);

    /**
     * @brief 公网IP获取成功回调
     * @param pubIp 公网出口IPv4字符串
     */
    void onWanIpGot(const QString& pubIp);

    /**
     * @brief 公网IP获取失败回调
     * @param errMsg 错误信息文本
     */
    void onWanIpError(const QString& errMsg);

    /**
     * @brief 手动刷新IP（托盘菜单触发），同时刷新内网、公网IP
     */
    void onManualRefreshIp();

private:
    FloatingIpoverlayWidget* m_floatWin = nullptr;  ///< IP悬浮显示窗口指针
    IpService* m_ipService = nullptr;                ///< IP网络服务实例
    TrayManager* m_trayManager = nullptr;            ///< 系统托盘管理器实例

    /**
     * @brief 缓存上一次拿到的公网IP
     * @note 内网IP信号与公网IP信号到达时机不同，缓存两份数据，收到任意一方更新都可以组合刷新UI
     */
    QString m_lastPubIp;
    /**
     * @brief 缓存上一次拿到的内网IP
     */
    QString m_lastLanIp;
};

#endif // WIDGET_H
