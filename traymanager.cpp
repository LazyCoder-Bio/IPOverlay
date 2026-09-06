#include "traymanager.h"
#include "floatingipoverlaywidget.h"
#include "AutoStartHelper.h"
#include "ConfigHelper.h"
#include "SettingsDialog.h"
#include <QCoreApplication>
#include <QApplication>
#include <QMessageBox>
#include <QStyle>
#include <QTimer>
#include "aboutdialog.h"

/**
 * @brief 托盘管理器构造函数
 * @param parent 父QObject对象
 * @note 本类负责系统托盘图标、右键菜单、托盘交互逻辑
 */
TrayManager::TrayManager(QObject *parent)
    : QObject(parent)
{
}

/**
 * @brief 托盘管理器析构函数
 * @note QSystemTrayIcon由this(QObject父对象)自动回收，不要手动delete，只做隐藏
 */
TrayManager::~TrayManager()
{
    if(m_trayIcon)
    {
        m_trayIcon->hide();
        //❗千万不要 delete m_trayIcon，QObject父子托管自动释放
    }
}

/**
 * @brief 初始化系统托盘图标与全部右键菜单
 * @param floatWin 悬浮窗对象指针，用于菜单控制显示隐藏
 */
void TrayManager::initTray(FloatingIpoverlayWidget *floatWin)
{
    m_floatWindow = floatWin;

    // 判断当前操作系统是否支持系统托盘
    if (!QSystemTrayIcon::isSystemTrayAvailable())
    {
        QMessageBox::warning(nullptr, "提示", "当前系统不支持系统托盘");
        qWarning() << "[TrayManager] System tray is not available on this system";
        return;
    }

    // 创建托盘图标实例，父对象指定this自动内存管理
    m_trayIcon = new QSystemTrayIcon(this);
    // 设置托盘图标资源
    m_trayIcon->setIcon(QIcon(":/IPOverlay.png"));
    // 鼠标悬浮在托盘图标上的提示文字
    m_trayIcon->setToolTip("IPOverlay");

    // 构建托盘右键菜单
    m_trayMenu = new QMenu();

    // 各个菜单项Action
    m_actShow = new QAction("显示悬浮窗", m_trayMenu);
    m_actHide = new QAction("隐藏悬浮窗", m_trayMenu);
    m_actAutoStart = new QAction("开机自启", m_trayMenu);
    m_actAutoStart->setCheckable(true); // 可勾选的复选框菜单项
    m_actRefresh = new QAction("手动刷新", m_trayMenu);
    m_actSettings = new QAction("配置", m_trayMenu);
    m_actAbout=new QAction("关于",m_trayMenu);
    m_actQuit = new QAction("退出程序", m_trayMenu);

    // 将Action添加到菜单，加分隔线分组
    m_trayMenu->addAction(m_actShow);
    m_trayMenu->addAction(m_actHide);
    m_trayMenu->addSeparator();
    m_trayMenu->addAction(m_actAutoStart);
    m_trayMenu->addSeparator();
    m_trayMenu->addAction(m_actRefresh);
    m_trayMenu->addSeparator();
    m_trayMenu->addAction(m_actSettings);
    m_trayMenu->addSeparator();
    m_trayMenu->addAction(m_actAbout);
    m_trayMenu->addSeparator();
    m_trayMenu->addAction(m_actQuit);

    // 读取程序实际路径，查询注册表开机自启状态，设置菜单勾选状态
    QString exePath = QCoreApplication::applicationFilePath();
    bool autoOn = AutoStartHelper::isAutoStart(AppRegKey, exePath);
    m_actAutoStart->setChecked(autoOn);

    // ======菜单action信号绑定======
    // 显示悬浮窗
    connect(m_actShow,&QAction::triggered,this,&TrayManager::slotShowFloatWindow);
    // 隐藏悬浮窗
    connect(m_actHide,&QAction::triggered,this,&TrayManager::slotHideFloatWindow);
    // 切换开机自启勾选
    connect(m_actAutoStart,&QAction::toggled,this,&TrayManager::slotAutoStartToggled);
    // 退出程序
    connect(m_actQuit,&QAction::triggered,this,&TrayManager::slotQuitApp);

    // 手动刷新：仅向外发射信号，业务逻辑交给上层Widget处理
    connect(m_actRefresh, &QAction::triggered, this, [this](){
        emit triggerManualRefresh();
    });

    // =====打开设置对话框=====
    connect(m_actSettings,&QAction::triggered,this,[this](){
        AppConfig currentCfg = ConfigHelper::loadConfig();
        SettingsDialog* dlg = new SettingsDialog();
        dlg->setAttribute(Qt::WA_DeleteOnClose); //关闭自动释放对话框内存
        dlg->setConfig(currentCfg);
        dlg->open(); //open非阻塞模态，不卡住主程序事件循环

        // 用户点击确定保存配置
        connect(dlg,&SettingsDialog::accepted,this,[this,dlg](){
            AppConfig newCfg = dlg->getConfig();
            ConfigHelper::saveConfig(newCfg);      //写入本地ini配置文件
            emit configChanged(newCfg);            //发出配置变更信号通知主Widget
        });
    });

    // =====打开关于对话框=====
    connect(m_actAbout,&QAction::triggered,this,[this](){
        AboutDialog* dlg = new AboutDialog();
        dlg->setAttribute(Qt::WA_DeleteOnClose);
        dlg->open();
    });

    // 双击托盘图标：切换悬浮窗显示/隐藏
    connect(m_trayIcon,&QSystemTrayIcon::activated,this,[this](QSystemTrayIcon::ActivationReason reason){
        if(reason == QSystemTrayIcon::DoubleClick)
        {
            if(m_floatWindow && m_floatWindow->isVisible())
                slotHideFloatWindow();
            else
                slotShowFloatWindow();
        }
    });

    // 将构建好的菜单绑定给托盘图标
    m_trayIcon->setContextMenu(m_trayMenu);

    // 延迟500ms显示托盘图标，规避Windows启动早期托盘不渲染bug
    QTimer::singleShot(500,this,[this](){
        m_trayIcon->show();
    });
}

/**
 * @brief 槽函数：显示悬浮窗并置顶激活
 */
void TrayManager::slotShowFloatWindow()
{
    if(m_floatWindow)
    {
        m_floatWindow->show();
        m_floatWindow->raise();
    }
}

/**
 * @brief 槽函数：隐藏悬浮窗
 */
void TrayManager::slotHideFloatWindow()
{
    if(m_floatWindow)
    {
        m_floatWindow->hide();
    }
}

/**
 * @brief 槽函数：开机自启勾选状态切换
 * @param checked true勾选开启自启；false取消自启
 * @note 设置失败时回滚UI勾选状态，防止界面与注册表不一致
 */
void TrayManager::slotAutoStartToggled(bool checked)
{
    QString exePath = QCoreApplication::applicationFilePath();
    bool ok = AutoStartHelper::setAutoStart(checked, AppRegKey, exePath);
    if(!ok)
    {

        QMessageBox::warning(nullptr,"错误","设置开机自启失败！");
        qWarning() << "[TrayManager] slotAutoStartToggled setAutoStart failed, rollback UI";
        // 设置失败，回滚界面勾选
        m_actAutoStart->setChecked(!checked);
    }
}

/**
 * @brief 槽函数：退出整个应用程序
 */
void TrayManager::slotQuitApp()
{
    qDebug() << "[TrayManager] user trigger quit application";
    if(m_trayIcon)
        m_trayIcon->hide();
    qApp->quit();
}
