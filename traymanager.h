#ifndef TRAYMANAGER_H
#define TRAYMANAGER_H

#include <QObject>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QAction>

class FloatingIpoverlayWidget;
struct AppConfig;

class TrayManager : public QObject
{
    Q_OBJECT
public:
    explicit TrayManager(QObject *parent = nullptr);
    ~TrayManager() override;

    //把悬浮窗传进来，初始化托盘
    void initTray(FloatingIpoverlayWidget* floatWin);

signals:
    //保存配置之后发出信号，交给Widget接收
    void configChanged(const AppConfig& newCfg);
    void triggerManualRefresh(); //新增：触发手动刷新IP

private slots:
    void slotShowFloatWindow();
    void slotHideFloatWindow();
    void slotAutoStartToggled(bool checked);
    void slotQuitApp();

private:
    QSystemTrayIcon* m_trayIcon = nullptr;
    QMenu* m_trayMenu = nullptr;
    QAction* m_actShow = nullptr;
    QAction* m_actHide = nullptr;
    QAction* m_actAutoStart = nullptr;
    QAction* m_actSettings = nullptr; //设置菜单项
    QAction* m_actAbout=nullptr;
    QAction* m_actQuit = nullptr;
    QAction* m_actRefresh = nullptr; //✅新增手动刷新action

    FloatingIpoverlayWidget* m_floatWindow = nullptr;
    inline static const QString AppRegKey = "IPOverlay";
};

#endif // TRAYMANAGER_H
