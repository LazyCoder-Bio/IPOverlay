#ifndef TRAYMANAGER_H
#define TRAYMANAGER_H
#include <QObject>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QAction>

// 前置声明，减少头文件相互包含
class FloatingIpoverlayWidget;
struct AppConfig;

/**
 * @brief 系统托盘管理器
 * @note 负责托盘图标、右键菜单的创建与交互；
 * 处理显示/隐藏悬浮窗、开机自启切换、打开配置对话框、关于、退出、手动刷新等菜单逻辑；
 * 内部不执行业务逻辑，通过信号向上层Widget传递事件。
 */
class TrayManager : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief 托盘管理器构造函数
     * @param parent 父QObject对象
     */
    explicit TrayManager(QObject *parent = nullptr);
    ~TrayManager() override;

    /**
     * @brief 初始化托盘图标与全部右键菜单
     * @param floatWin 悬浮窗对象指针，用于菜单控制悬浮窗显示隐藏
     */
    void initTray(FloatingIpoverlayWidget* floatWin);

signals:
    /**
     * @brief 用户在设置对话框保存配置完成后发出
     * @param newCfg 修改后的完整配置结构体
     * @note 由上层Widget接收该信号，更新IP服务的运行参数
     */
    void configChanged(const AppConfig& newCfg);

    /**
     * @brief 用户点击托盘菜单【手动刷新】触发
     * @note 通知上层Widget执行内网、公网IP重新拉取
     */
    void triggerManualRefresh();

private slots:
    /**
     * @brief 槽：显示悬浮窗并置顶
     */
    void slotShowFloatWindow();

    /**
     * @brief 槽：隐藏悬浮窗
     */
    void slotHideFloatWindow();

    /**
     * @brief 槽：切换开机自启勾选状态
     * @param checked true开启自启；false关闭自启
     */
    void slotAutoStartToggled(bool checked);

    /**
     * @brief 槽：退出整个应用程序
     */
    void slotQuitApp();

private:
    QSystemTrayIcon* m_trayIcon = nullptr;     ///< 系统托盘图标实例
    QMenu* m_trayMenu = nullptr;               ///< 托盘右键主菜单
    QAction* m_actShow = nullptr;              ///< 菜单Action：显示悬浮窗
    QAction* m_actHide = nullptr;              ///< 菜单Action：隐藏悬浮窗
    QAction* m_actAutoStart = nullptr;         ///< 菜单Action：开机自启（可勾选）
    QAction* m_actSettings = nullptr;          ///< 菜单Action：打开配置对话框
    QAction* m_actAbout=nullptr;               ///< 菜单Action：打开关于对话框
    QAction* m_actQuit = nullptr;              ///< 菜单Action：退出程序
    QAction* m_actRefresh = nullptr;           ///< 菜单Action：手动刷新IP

    FloatingIpoverlayWidget* m_floatWindow = nullptr; ///< 悬浮窗对象指针

    /**
     * @brief 注册表Run项键名（静态常量）
     * @note Windows开机自启注册表使用该名称作为程序标识
     */
    inline static const QString AppRegKey = "IPOverlay";
};

#endif // TRAYMANAGER_H
