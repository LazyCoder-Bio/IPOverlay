#include "floatingipoverlaywidget.h"
#include "./ui_floatingipoverlaywidget.h"
#include <QMouseEvent>
#include <QApplication>
#include <QScreen>

/**
 * @brief IP悬浮显示窗口构造函数
 * @param parent 父对象
 * @note 独立顶层无边框置顶窗口，支持鼠标拖拽移动，透明背景，不抢占窗口焦点
 */
FloatingIpoverlayWidget::FloatingIpoverlayWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::FloatingIpoverlayWidget)
{
    ui->setupUi(this);

    // 设置窗口标志位：独立顶层窗口、无边框、始终置顶、不获取输入焦点
    setWindowFlags(
        Qt::Window                // 标记为独立顶层窗口，不可省略
        | Qt::FramelessWindowHint // 去除系统标题栏与边框
        | Qt::WindowStaysOnTopHint // 窗口置顶，始终浮于其他窗口上层
        | Qt::WindowDoesNotAcceptFocus // 窗口不接收键盘输入焦点
        );

    // 开启窗口透明背景，支持圆角、半透明样式渲染
    setAttribute(Qt::WA_TranslucentBackground);
    // 显示窗口的时候不会抢夺系统输入焦点
    setAttribute(Qt::WA_ShowWithoutActivating);
}

/**
 * @brief 安全显示悬浮窗口，对外暴露接口
 */
void FloatingIpoverlayWidget::safeShow(){
    show();
}

/**
 * @brief 设置显示的公网IP、内网IP文本
 * @param pubStr 公网IP字符串
 * @param lanStr 内网IP字符串
 */
void FloatingIpoverlayWidget::setIpText(const QString &pubStr, const QString &lanStr)
{
    ui->m_PublicIp->setText(pubStr);
    ui->m_LocalIp->setText(lanStr);
}

/**
 * @brief 鼠标按下事件，拖拽窗口起始，记录鼠标相对窗口左上角偏移
 * @param event 鼠标事件对象
 */
void FloatingIpoverlayWidget::mousePressEvent(QMouseEvent *event)
{
    // 仅响应鼠标左键按下
    if (event->button() == Qt::LeftButton)
    {
        // 计算：屏幕鼠标坐标 − 窗口左上角屏幕坐标 = 鼠标在窗口内偏移量
        m_dragPos = event->globalPosition().toPoint() - frameGeometry().topLeft();
        event->accept(); //消费事件，不再向上传递
    }
}

/**
 * @brief 鼠标移动事件，实现窗口拖拽，同时做屏幕边界限制，防止拖出屏幕外
 * @param event 鼠标事件对象
 */
void FloatingIpoverlayWidget::mouseMoveEvent(QMouseEvent *event)
{
    // 判断左键处于按住状态
    if (event->buttons() & Qt::LeftButton)
    {
        // 根据鼠标全局位置和记录的偏移，计算窗口新位置
        QPoint newWinPos = event->globalPosition().toPoint() - m_dragPos;
        // 获取主屏幕可用几何区域（自动排除任务栏区域）
        QRect screenRect = QApplication::primaryScreen()->availableGeometry();
        // 构造新窗口的完整包围矩形
        QRect winRect(newWinPos, size());

        // 边界校验：只有窗口完整落在屏幕可用区域内，才执行移动
        if (screenRect.contains(winRect))
        {
            move(newWinPos);
        }
        event->accept();
    }
}

/**
 * @brief 析构，释放UI资源
 */
FloatingIpoverlayWidget::~FloatingIpoverlayWidget()
{
    delete ui;
}
