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


TrayManager::TrayManager(QObject *parent)
    : QObject(parent)
{
}

TrayManager::~TrayManager()
{
    if(m_trayIcon)
    {
        m_trayIcon->hide();
        //❗千万不要 delete m_trayIcon
    }
}

void TrayManager::initTray(FloatingIpoverlayWidget *floatWin)
{
    m_floatWindow = floatWin;

    if (!QSystemTrayIcon::isSystemTrayAvailable())
    {
        QMessageBox::warning(nullptr, "提示", "当前系统不支持系统托盘");
        return;
    }

    m_trayIcon = new QSystemTrayIcon(this);
    //调试用系统内置图标，正式版替换为 :/res/icon.png
    m_trayIcon->setIcon(QIcon(":/IPOverlay.png"));
    m_trayIcon->setToolTip("IPOverlay");

    //构建右键菜单
    m_trayMenu = new QMenu();

    m_actShow = new QAction("显示悬浮窗", m_trayMenu);
    m_actHide = new QAction("隐藏悬浮窗", m_trayMenu);
    m_actAutoStart = new QAction("开机自启", m_trayMenu);
    m_actAutoStart->setCheckable(true);
    m_actRefresh = new QAction("手动刷新", m_trayMenu);
    m_actSettings = new QAction("配置", m_trayMenu); //新增设置
    m_actAbout=new QAction("关于",m_trayMenu);
    m_actQuit = new QAction("退出程序", m_trayMenu);


    m_trayMenu->addAction(m_actShow);
    m_trayMenu->addAction(m_actHide);
    m_trayMenu->addSeparator();
    m_trayMenu->addAction(m_actAutoStart);
    m_trayMenu->addSeparator();
    m_trayMenu->addAction(m_actRefresh); //加入手动刷新
     m_trayMenu->addSeparator();
    m_trayMenu->addAction(m_actSettings); //插入设置菜单项
     m_trayMenu->addSeparator();
    m_trayMenu->addAction(m_actAbout);
    m_trayMenu->addSeparator();
    m_trayMenu->addAction(m_actQuit);

    //读取开机自启状态
    //bool autoOn = AutoStartHelper::isAutoStart(AppRegKey);
    QString exePath = QCoreApplication::applicationFilePath();
    bool autoOn = AutoStartHelper::isAutoStart(AppRegKey, exePath);

    m_actAutoStart->setChecked(autoOn);

    //普通菜单绑定
    connect(m_actShow,&QAction::triggered,this,&TrayManager::slotShowFloatWindow);
    connect(m_actHide,&QAction::triggered,this,&TrayManager::slotHideFloatWindow);
    connect(m_actAutoStart,&QAction::toggled,this,&TrayManager::slotAutoStartToggled);
    connect(m_actQuit,&QAction::triggered,this,&TrayManager::slotQuitApp);
    // 绑定手动刷新
    connect(m_actRefresh, &QAction::triggered, this, [this](){
        emit triggerManualRefresh(); //仅向外发信号，不操作自启
    });

    //=====点击【设置】弹出SettingsDialog对话框=====
    connect(m_actSettings,&QAction::triggered,this,[this](){
        AppConfig currentCfg = ConfigHelper::loadConfig();

        SettingsDialog* dlg = new SettingsDialog();
        dlg->setAttribute(Qt::WA_DeleteOnClose);
        dlg->setConfig(currentCfg);

        dlg->open(); //open模态，不会阻塞主程序事件循环

        connect(dlg,&SettingsDialog::accepted,this,[this,dlg](){
            AppConfig newCfg = dlg->getConfig();
            ConfigHelper::saveConfig(newCfg); //写入config.ini
            emit configChanged(newCfg); //发出配置变更信号
        });
    });


    //====点击【关于】弹出AboutDialog对话框====
    connect(m_actAbout,&QAction::triggered,this,[this](){
        AboutDialog* dlg = new AboutDialog();
        dlg->setAttribute(Qt::WA_DeleteOnClose);
        dlg->open(); //open模态，不会阻塞主程序事件循环
    });


    //双击托盘图标切换悬浮窗显示隐藏
    connect(m_trayIcon,&QSystemTrayIcon::activated,this,[this](QSystemTrayIcon::ActivationReason reason){
        if(reason == QSystemTrayIcon::DoubleClick)
        {
            if(m_floatWindow && m_floatWindow->isVisible())
                slotHideFloatWindow();
            else
                slotShowFloatWindow();
        }
    });

    m_trayIcon->setContextMenu(m_trayMenu);
    QTimer::singleShot(500,this,[this](){
        m_trayIcon->show();
    });
}

void TrayManager::slotShowFloatWindow()
{
    if(m_floatWindow)
    {
        m_floatWindow->show();
        m_floatWindow->raise();
    }
}

void TrayManager::slotHideFloatWindow()
{
    if(m_floatWindow)
    {
        m_floatWindow->hide();
    }
}

void TrayManager::slotAutoStartToggled(bool checked)
{
    QString exePath = QCoreApplication::applicationFilePath();
    bool ok = AutoStartHelper::setAutoStart(checked, AppRegKey, exePath);
    if(!ok)
    {
        m_actAutoStart->setChecked(!checked);
    }
}

void TrayManager::slotQuitApp()
{
    qApp->quit();
}
