#ifndef WIDGET_H
#define WIDGET_H
#include <QWidget>

struct AppConfig;
class FloatingIpoverlayWidget;
class IpService;
class TrayManager;

class Widget : public QWidget
{
    Q_OBJECT
public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget() override;

private:
    void setupFloatWindow();
    void setupIpService();
    void setupTray();

private slots:
    void onConfigChanged(const AppConfig& cfg);
    void onLanIpReady(const QString& lanIp);
    void onWanIpGot(const QString& pubIp);
    void onWanIpError(const QString& errMsg);
    void onManualRefreshIp(); //✅新增

private:
    FloatingIpoverlayWidget* m_floatWin = nullptr;
    IpService* m_ipService = nullptr;
    TrayManager* m_trayManager = nullptr;

    //保存当前显示的IP，两个变量缓存，方便信号到达时组合调用setIpText
    QString m_lastPubIp;
    QString m_lastLanIp;
};

#endif // WIDGET_H
